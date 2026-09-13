/*
 * tauraro_rt.h — Tauraro Language Runtime
 *
 * Included by every compiled Tauraro program.
 * Provides: threading, channels, mutex, waitgroup,
 *           exceptions, string helpers, I/O, dict.
 *
 * Cross-platform: Windows (Win32 API) and POSIX (pthreads).
 */
#ifndef TAURARO_RT_H
#define TAURARO_RT_H

/* TR_EXPORT — symbol-visibility attribute for `export def` functions, so they
 * appear in the dynamic symbol table of a shared library (`tauraroc --lib`).
 * On Windows: __declspec(dllexport); on ELF/Mach-O: default visibility. */
#if defined(_WIN32) || defined(__CYGWIN__)
#  define TR_EXPORT __declspec(dllexport)
#else
#  define TR_EXPORT __attribute__((visibility("default")))
#endif

/* Must be defined before any system header to expose full POSIX/platform extensions:
 * pthread_rwlock_t, setenv, strdup, struct addrinfo, NI_NAMEREQD, clock_gettime, etc. */
#if defined(__linux__)
/* Guard against a command-line -D_GNU_SOURCE (CI sets it) to avoid a
 * "_GNU_SOURCE redefined" warning; same value, so the guard is harmless. */
#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#  endif
#elif defined(__APPLE__)
/* macOS: _DARWIN_C_SOURCE re-enables BSD extensions (strdup, etc.); _XOPEN_SOURCE
 * is ALSO required because recent SDKs (Xcode 16+) guard the ucontext routines
 * (getcontext/makecontext/swapcontext, used by the coroutine scheduler) behind
 * `#if !defined(_XOPEN_SOURCE) #error ...`. Both must be set before any include. */
#  define _XOPEN_SOURCE 700
#  define _DARWIN_C_SOURCE
#elif defined(__unix__)
#  define _POSIX_C_SOURCE 200809L
#endif

/* ── Tauraro platform detection ──────────────────────────────────────────── *
 * Set TAURARO_NO_OS before including this header to target bare-metal or     *
 * freestanding environments (no OS, filesystem, networking, or threads).     *
 * These macros drive conditional compilation of all platform-specific code.  */
#if defined(__wasm__) || defined(__wasm32__) || defined(__EMSCRIPTEN__)
#  define TAURARO_WASM 1
#endif
#if defined(__ANDROID__)
#  define TAURARO_ANDROID 1
#endif
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#  define TAURARO_IOS 1
#endif
/* BARE = no OS: bare WASM (no WASI) or explicit TAURARO_NO_OS */
#if defined(TAURARO_NO_OS) || (defined(TAURARO_WASM) && !defined(__wasi__))
#  define TAURARO_BARE 1
#endif
/* KERNEL = Linux kernel module / bare-metal with user-supplied allocator.
 * Implies BARE (no OS threads/sockets), disables setjmp exceptions. */
#if defined(TAURARO_KERNEL)
#  if !defined(TAURARO_BARE)
#    define TAURARO_BARE 1
#  endif
#  define TAURARO_NO_EXCEPTIONS 1
#endif
/* ── Orthogonal capability switches ───────────────────────────────────────
 * Historically TAURARO_BARE meant "no OS" and conflated THREE separate things:
 * no libc, no threads, and no networking. wasi breaks that assumption — it HAS a
 * libc (stdio/file/malloc via wasi-libc) but has NO pthreads, NO BSD sockets, and
 * NO subprocess. So split the concerns into independent switches:
 *   TAURARO_NO_LIBC        - no stdio/malloc/file (true bare metal)  == BARE
 *   TAURARO_NO_THREADS     - no OS threads; locks are no-ops, spawn runs inline
 *   TAURARO_NO_NET         - no sockets
 *   TAURARO_NO_SUBPROCESS  - no popen/system
 * BARE (bare-metal / no-OS) implies ALL of them, so bare-metal builds behave
 * EXACTLY as before. wasi opts into the last three but keeps its libc. Every
 * former `defined(TAURARO_BARE)` guard is reclassified below to the specific
 * concern it actually gates. NOTE: derived here — AFTER both the NO_OS/WASM and
 * the KERNEL paths have finished resolving TAURARO_BARE — so a KERNEL/freestanding
 * build (which defines BARE only in the block just above) correctly gets all three.  */
#if defined(TAURARO_BARE)
#  define TAURARO_NO_LIBC 1
#endif
#if defined(TAURARO_BARE) || defined(__wasi__)
#  define TAURARO_NO_THREADS    1
#  define TAURARO_NO_NET        1
#  define TAURARO_NO_SUBPROCESS 1
#endif

#ifndef TAURARO_KERNEL
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdbool.h>
#  include <stdint.h>
#  include <string.h>
#  include <stdarg.h>
#  include <time.h>
#  include <math.h>
#  include <stdatomic.h>
#  include <ctype.h>
#  if defined(__wasi__)
     /* wasi-libc ships <pthread.h> (types + single-threaded stub functions). The
      * runtime's async scheduler/timers/TLS use raw pthread_* directly, so pull the
      * real declarations in; the _Tr* thread/lock wrappers still take the single-
      * threaded (BARE) path below via TAURARO_NO_THREADS. */
#    include <pthread.h>
#  endif
#  if defined(__wasm__)
     /* WebAssembly has no setjmp/longjmp without the (unratified) EH proposal, so
      * <setjmp.h> (even wasi-libc's) fails to compile. The LLVM backend lowers
      * try/except to plain branches and never calls the runtime's setjmp, so a
      * no-unwind stub that merely COMPILES is sufficient for wasm targets. */
     typedef void* jmp_buf[8];
#    define setjmp(b)      (0)
#    define longjmp(b, v)  __builtin_trap()
#  else
#    include <setjmp.h>
#  endif
#else
/* Kernel / freestanding: caller supplies context headers.
 * At minimum: stddef.h, stdbool.h, stdint.h, stdatomic.h must exist. */
#  include <stddef.h>
#  include <stdbool.h>
#  include <stdint.h>
#  include <stdatomic.h>
#  ifdef __KERNEL__
#    include <linux/kernel.h>
#    include <linux/slab.h>
#    include <linux/string.h>
#  else
     /* Bare-metal (non-Linux-kernel). stdarg.h IS a freestanding header (libc-lite
      * vsnprintf). setjmp.h is NOT — it's hosted; arm-none-eabi/newlib bundles it, but
      * bare toolchains like riscv64-unknown-elf don't provide it under -ffreestanding.
      * Use it when present; otherwise fall back to a trivial no-unwind version (a
      * bare-metal panic has nowhere to return to, so longjmp just stops). */
#    include <stdarg.h>
#    if defined(__has_include)
#      if __has_include(<setjmp.h>)
#        include <setjmp.h>
#        define _TR_HAS_SETJMP 1
#      endif
#    endif
#    ifndef _TR_HAS_SETJMP
       typedef void* jmp_buf[8];
#      define setjmp(b)      (0)
#      define longjmp(b, v)  do { for(;;){} } while(0)
#    endif
#  endif
#endif

/* ── Pluggable allocator macros ──────────────────────────────────────────── *
 * Override before including this header to redirect all runtime allocations: *
 *   #define TAURARO_ALLOC(sz)      kmalloc(sz, GFP_KERNEL)                   *
 *   #define TAURARO_FREE(p)        kfree(p)                                   *
 *   #define TAURARO_REALLOC(p,sz)  krealloc(p, sz, GFP_KERNEL)               *
 *   #define TAURARO_CALLOC(n,sz)   kzalloc((n)*(sz), GFP_KERNEL)             *
 * TAURARO_KERNEL mode requires all four to be defined externally.             */
#if defined(TAURARO_KERNEL)
#  if !defined(TAURARO_ALLOC) || !defined(TAURARO_FREE) || \
      !defined(TAURARO_REALLOC) || !defined(TAURARO_CALLOC)
#    error "TAURARO_KERNEL requires TAURARO_ALLOC/FREE/REALLOC/CALLOC to be defined"
#  endif
#else
#  ifndef TAURARO_ALLOC
#    define TAURARO_ALLOC(sz)      malloc(sz)
#  endif
#  ifndef TAURARO_FREE
#    define TAURARO_FREE(p)        free(p)
#  endif
#  ifndef TAURARO_REALLOC
#    define TAURARO_REALLOC(p,sz)  realloc(p,sz)
#  endif
#  ifndef TAURARO_CALLOC
#    define TAURARO_CALLOC(n,sz)   calloc(n,sz)
#  endif
#endif

#ifdef TR_HEAPDBG
/* ── Redzone/guard debug allocator ───────────────────────────────────────────
 * Wrap malloc/calloc/realloc/free with a header (magic|size|id) + trailing magic.
 * A registry of live blocks is scanned on EVERY op so an overrun/underrun is caught
 * at the next allocation after the bad write — pinpointing the offending block+id. */
#define _TRDBG_FRONT 0xA11C0DEBEEF00DULL
#define _TRDBG_BACK  0xDEADBEEFCAFEBABEULL
#define _TRDBG_HDR   32
#define _TRDBG_MAX   400000
static unsigned char* _trdbg_reg[_TRDBG_MAX];
static long _trdbg_regn = 0;
static long _trdbg_id = 0;
static void _trdbg_die(const char* what, unsigned char* base){
    unsigned long long id = base ? *(unsigned long long*)(base+16) : 0;
    unsigned long long sz = base ? *(unsigned long long*)(base+8) : 0;
    fprintf(stderr, "TRDBG: %s block id=%llu size=%llu\n", what, id, sz); fflush(stderr); abort();
}
__declspec(dllimport) int __stdcall IsBadReadPtr(const void*, unsigned long long);
static void _trdbg_check(unsigned char* base){
    if (IsBadReadPtr(base, _TRDBG_HDR)) return;   /* stale/freed entry (bookkeeping gap) — skip */
    if (*(unsigned long long*)base != _TRDBG_FRONT) _trdbg_die("FRONT-guard clobbered", base);
    unsigned long long sz = *(unsigned long long*)(base+8);
    if (sz > 0x40000000ULL) return;               /* implausible size (corrupt hdr on a stale block) */
    if (IsBadReadPtr(base+_TRDBG_HDR+sz, 8)) return;
    if (*(unsigned long long*)(base+_TRDBG_HDR+sz) != _TRDBG_BACK) _trdbg_die("BACK-guard clobbered (overrun)", base);
}
static void _trdbg_scan(void){ long i; for(i=0;i<_trdbg_regn;i++) _trdbg_check(_trdbg_reg[i]); }
/* Verify a write of n bytes at dst stays within the guarded block that contains dst.
 * dst outside all registered blocks (raw-malloc list buffer, stack, .rodata) -> skip. */
static void _trdbg_bounds(void* dst, size_t n){
    unsigned char* d = (unsigned char*)dst; long i;
    for(i=0;i<_trdbg_regn;i++){
        unsigned char* base = _trdbg_reg[i];
        unsigned long long sz = *(unsigned long long*)(base+8);
        unsigned char* user = base+_TRDBG_HDR;
        if(d >= user && d < user+sz){
            if(d + n > user + sz){
                fprintf(stderr, "TRDBG: memcpy OVERRUN dst-block id=%llu size=%llu, write %llu bytes at +%lld\n",
                    *(unsigned long long*)(base+16), sz, (unsigned long long)n, (long long)(d-user));
                fflush(stderr); abort();
            }
            return;
        }
    }
}
static void* _trdbg_alloc(size_t sz, int zero){
    _trdbg_scan();
    unsigned char* base = (unsigned char*)malloc(_TRDBG_HDR + sz + 8);
    if(!base) return 0;
    *(unsigned long long*)base = _TRDBG_FRONT;
    *(unsigned long long*)(base+8) = sz;
    *(unsigned long long*)(base+16) = (unsigned long long)(++_trdbg_id);
    *(unsigned long long*)(base+_TRDBG_HDR+sz) = _TRDBG_BACK;
    if(zero) memset(base+_TRDBG_HDR, 0, sz);
    if(_trdbg_regn < _TRDBG_MAX) _trdbg_reg[_trdbg_regn++] = base;
    return base + _TRDBG_HDR;
}
/* Registry index of the guarded block whose USER pointer == p, or -1 if p is a raw
 * (non-guarded) allocation. Looking up by user-ptr (not p-HDR) tolerates a mixed heap
 * where some blocks come from raw malloc (list buffers) and reach free()/realloc() here. */
static long _trdbg_find(void* p){
    long i; for(i=0;i<_trdbg_regn;i++){ if(_trdbg_reg[i]+_TRDBG_HDR == (unsigned char*)p) return i; }
    return -1;
}
static void _trdbg_free(void* p){
    if(!p) return;
    long idx = _trdbg_find(p);
    if(idx < 0){ free(p); return; }                 /* raw (non-guarded) block */
    unsigned char* base = _trdbg_reg[idx];
    _trdbg_check(base);
    _trdbg_reg[idx] = _trdbg_reg[--_trdbg_regn];
    _trdbg_scan();
    *(unsigned long long*)base = 0;                  /* poison front so double-free is caught */
    free(base);
}
static void* _trdbg_realloc(void* p, size_t sz){
    if(!p) return _trdbg_alloc(sz, 0);
    long idx = _trdbg_find(p);
    if(idx < 0) return realloc(p, sz);               /* raw (non-guarded) block */
    unsigned char* base = _trdbg_reg[idx];
    _trdbg_check(base);
    _trdbg_reg[idx] = _trdbg_reg[--_trdbg_regn];
    _trdbg_scan();
    unsigned char* nb = (unsigned char*)realloc(base, _TRDBG_HDR + sz + 8);
    if(!nb) return 0;
    *(unsigned long long*)(nb+8) = sz;
    *(unsigned long long*)(nb+_TRDBG_HDR+sz) = _TRDBG_BACK;
    if(_trdbg_regn < _TRDBG_MAX) _trdbg_reg[_trdbg_regn++] = nb;
    return nb + _TRDBG_HDR;
}
#undef TAURARO_ALLOC
#undef TAURARO_CALLOC
#undef TAURARO_REALLOC
#undef TAURARO_FREE
#define TAURARO_ALLOC(sz)      _trdbg_alloc((sz), 0)
#define TAURARO_CALLOC(n,sz)   _trdbg_alloc((size_t)(n)*(size_t)(sz), 1)
#define TAURARO_REALLOC(p,sz)  _trdbg_realloc((p),(sz))
#define TAURARO_FREE(p)        _trdbg_free((p))
#  define _TR_HEAPCHK(w) ((void)0)
#else
#  define _TR_HEAPCHK(w) ((void)0)
#endif

/* Normalize the freestanding tier flags: TAURARO_KERNEL (no libc) and
 * TAURARO_NO_OS (bare-metal target) both imply TAURARO_BARE — the canonical
 * "no OS services" flag used to gate file I/O / env / process / stdin below. */
#if (defined(TAURARO_KERNEL) || defined(TAURARO_NO_OS)) && !defined(TAURARO_BARE)
#  define TAURARO_BARE
#endif

/* Output sink hook — default no-op; a bare target redefines it (UART/semihosting).
 * Hoisted here so the libc-lite printf below can route through it; the print
 * section's own `#ifndef _TR_WRITE` guard then no-ops. */
#ifndef _TR_WRITE
#  define _TR_WRITE(s) ((void)(s))
#endif

/* ââ Freestanding libc-lite ââ *
 * Bare-metal (TAURARO_KERNEL, non-Linux-kernel) has no <string.h>; provide the
 * minimal mem/str primitives the core runtime needs. Kernel mode already pulled
 * <linux/string.h>; hosted has libc. A target may predefine any of these. */
#if defined(TAURARO_KERNEL) && !defined(__KERNEL__)
#ifndef _TR_HAVE_STRING
static inline size_t strlen(const char* s){ size_t n=0; while(s[n]) n++; return n; }
static inline void*  memcpy(void* d,const void* s,size_t n){ unsigned char* a=(unsigned char*)d; const unsigned char* b=(const unsigned char*)s; for(size_t i=0;i<n;i++) a[i]=b[i]; return d; }
static inline void*  memmove(void* d,const void* s,size_t n){ unsigned char* a=(unsigned char*)d; const unsigned char* b=(const unsigned char*)s; if(a<b){for(size_t i=0;i<n;i++)a[i]=b[i];}else{for(size_t i=n;i>0;i--)a[i-1]=b[i-1];} return d; }
static inline void*  memset(void* d,int c,size_t n){ unsigned char* a=(unsigned char*)d; for(size_t i=0;i<n;i++) a[i]=(unsigned char)c; return d; }
static inline int    memcmp(const void* x,const void* y,size_t n){ const unsigned char* a=(const unsigned char*)x; const unsigned char* b=(const unsigned char*)y; for(size_t i=0;i<n;i++){ if(a[i]!=b[i]) return (int)a[i]-(int)b[i]; } return 0; }
static inline int    strcmp(const char* a,const char* b){ while(*a && *a==*b){a++;b++;} return (int)(unsigned char)*a-(int)(unsigned char)*b; }
static inline int    strncmp(const char* a,const char* b,size_t n){ for(size_t i=0;i<n;i++){ if(a[i]!=b[i]) return (int)(unsigned char)a[i]-(int)(unsigned char)b[i]; if(!a[i]) break; } return 0; }
static inline char*  strchr(const char* s,int c){ for(;*s;s++){ if(*s==(char)c) return (char*)s; } return c?0:(char*)s; }
static inline char*  strcpy(char* d,const char* s){ char* r=d; while((*d++=*s++)); return r; }
static inline char*  strncpy(char* d,const char* s,size_t n){ size_t i=0; for(;i<n&&s[i];i++) d[i]=s[i]; for(;i<n;i++) d[i]='\0'; return d; }
static inline char*  strcat(char* d,const char* s){ char* r=d; while(*d)d++; while((*d++=*s++)); return r; }
static inline char*  strstr(const char* h,const char* n){ if(!*n) return (char*)h; for(;*h;h++){ const char* a=h; const char* b=n; while(*a&&*b&&*a==*b){a++;b++;} if(!*b) return (char*)h; } return 0; }
static inline char*  strrchr(const char* s,int c){ const char* last=0; for(;;s++){ if(*s==(char)c) last=s; if(!*s) break; } return (char*)last; }
static inline char*  strtok(char* s,const char* delim){
    static char* save; if(s) save=s; if(!save) return 0;
    while(*save){ const char* d=delim; int isd=0; for(;*d;d++) if(*save==*d){isd=1;break;} if(!isd) break; save++; }
    if(!*save){ save=0; return 0; }
    char* tok=save;
    while(*save){ const char* d=delim; int isd=0; for(;*d;d++) if(*save==*d){isd=1;break;} if(isd){ *save++='\0'; return tok; } save++; }
    save=0; return tok;
}
#endif  /* _TR_HAVE_STRING */

/* ctype / stdlib / stdio libc-lite (bare-metal, no <ctype.h>/<stdlib.h>/<stdio.h>).
 * Correct-by-inspection ASCII/int impls; float in the formatter is basic (bare
 * logging), never the hosted path. A target may predefine _TR_HAVE_CTYPE. */
#ifndef _TR_HAVE_CTYPE
static inline int isspace(int c){ return c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\v'||c=='\f'; }
static inline int isdigit(int c){ return c>='0'&&c<='9'; }
static inline int isalpha(int c){ return (c>='a'&&c<='z')||(c>='A'&&c<='Z'); }
static inline int isalnum(int c){ return isalpha(c)||isdigit(c); }
static inline int islower(int c){ return c>='a'&&c<='z'; }
static inline int isupper(int c){ return c>='A'&&c<='Z'; }
static inline int isxdigit(int c){ return isdigit(c)||(c>='a'&&c<='f')||(c>='A'&&c<='F'); }
static inline int tolower(int c){ return isupper(c)? c+32 : c; }
static inline int toupper(int c){ return islower(c)? c-32 : c; }
static inline char* strdup(const char* s){ if(!s) return 0; size_t n=strlen(s)+1; char* r=(char*)TAURARO_ALLOC(n); if(r) memcpy(r,s,n); return r; }
static inline int   atoi(const char* s){ int r=0,neg=0; while(*s==' '||*s=='\t')s++; if(*s=='-'){neg=1;s++;}else if(*s=='+')s++; while(*s>='0'&&*s<='9'){ r=r*10+(*s-'0'); s++; } return neg?-r:r; }
static inline long long strtoll(const char* s,char** end,int base){ (void)base; long long r=0; int neg=0; while(*s==' '||*s=='\t')s++; if(*s=='-'){neg=1;s++;}else if(*s=='+')s++; while(*s>='0'&&*s<='9'){ r=r*10+(*s-'0'); s++; } if(end)*end=(char*)s; return neg?-r:r; }
static inline unsigned long long strtoull(const char* s,char** end,int base){ (void)base; unsigned long long r=0; while(*s==' '||*s=='\t')s++; if(*s=='+')s++; while(*s>='0'&&*s<='9'){ r=r*10+(unsigned)(*s-'0'); s++; } if(end)*end=(char*)s; return r; }
static inline double strtod(const char* s,char** end){ double r=0; int neg=0; while(*s==' '||*s=='\t')s++; if(*s=='-'){neg=1;s++;}else if(*s=='+')s++; while(*s>='0'&&*s<='9'){ r=r*10+(*s-'0'); s++; } if(*s=='.'){ s++; double f=0.1; while(*s>='0'&&*s<='9'){ r+=(*s-'0')*f; f*=0.1; s++; } } if(end)*end=(char*)s; return neg?-r:r; }
static unsigned long long _tr_rng=88172645463325252ULL;
static inline void srand(unsigned s){ _tr_rng = s? (unsigned long long)s : 1ULL; }
static inline int  rand(void){ _tr_rng^=_tr_rng<<13; _tr_rng^=_tr_rng>>7; _tr_rng^=_tr_rng<<17; return (int)(_tr_rng & 0x7fffffff); }
static inline void qsort(void* base,size_t n,size_t sz,int(*cmp)(const void*,const void*)){
    char* a=(char*)base;
    for(size_t i=1;i<n;i++) for(size_t j=i; j>0 && cmp(a+(j-1)*sz, a+j*sz)>0; j--)
        for(size_t k=0;k<sz;k++){ char t=a[(j-1)*sz+k]; a[(j-1)*sz+k]=a[j*sz+k]; a[j*sz+k]=t; }
}
__attribute__((noreturn)) static inline void exit(int code){ (void)code; __builtin_trap(); for(;;){} }
/* Route raw malloc/calloc/realloc/free (used by the platform concurrency
 * primitives) through the pluggable allocator. Under TAURARO_KERNEL the
 * TAURARO_ALLOC/... macros are always user-supplied (enforced by #error above),
 * so there is no self-recursion into a libc malloc. */
static inline void* malloc(size_t n)            { return TAURARO_ALLOC(n); }
static inline void* calloc(size_t a, size_t b)  { return TAURARO_CALLOC(a, b); }
static inline void* realloc(void* p, size_t n)  { return TAURARO_REALLOC(p, n); }
static inline void  free(void* p)               { TAURARO_FREE(p); }
/* Minimal vsnprintf — %d/%i/%u/%l/%ll(+d/u/x)/%zu/%x/%X/%p/%c/%s/%f/%g/%% with
 * optional width.precision. Integer/string are exact; float is a basic decimal
 * (bare-metal logging only — hosted uses real libc). */
static int _tr_fmt_u(char* o,unsigned long long v,int base,int up){ char t[24];int i=0;const char* d=up?"0123456789ABCDEF":"0123456789abcdef"; if(!v)t[i++]='0'; while(v){t[i++]=d[v%base];v/=base;} for(int j=0;j<i;j++)o[j]=t[i-1-j]; return i; }
static int vsnprintf(char* buf,size_t cap,const char* fmt,va_list ap){
    size_t o=0; char tmp[64];
    #define _TR_PUT(ch) do{ if(o+1<cap) buf[o]=(ch); o++; }while(0)
    for(const char* p=fmt; *p; p++){
        if(*p!='%'){ _TR_PUT(*p); continue; }
        p++; int lng=0; int prec=-1;
        while(*p=='-'||*p=='+'||*p==' '||*p=='0'||*p=='#'){ p++; }
        while(*p>='0'&&*p<='9') p++;                 /* width (ignored) */
        if(*p=='.'){ p++; prec=0; while(*p>='0'&&*p<='9'){ prec=prec*10+(*p-'0'); p++; } }
        while(*p=='l'){ lng++; p++; } if(*p=='z'||*p=='j'){ lng=2; p++; }
        char c=*p; int n=0;
        if(c=='d'||c=='i'){ long long v = lng>=2? va_arg(ap,long long) : (long long)va_arg(ap,int); if(v<0){ _TR_PUT('-'); v=-v; } n=_tr_fmt_u(tmp,(unsigned long long)v,10,0); for(int k=0;k<n;k++)_TR_PUT(tmp[k]); }
        else if(c=='u'){ unsigned long long v = lng>=2? va_arg(ap,unsigned long long):(unsigned long long)va_arg(ap,unsigned); n=_tr_fmt_u(tmp,v,10,0); for(int k=0;k<n;k++)_TR_PUT(tmp[k]); }
        else if(c=='x'||c=='X'){ unsigned long long v = lng>=2? va_arg(ap,unsigned long long):(unsigned long long)va_arg(ap,unsigned); n=_tr_fmt_u(tmp,v,16,c=='X'); for(int k=0;k<n;k++)_TR_PUT(tmp[k]); }
        else if(c=='p'){ unsigned long long v=(unsigned long long)(size_t)va_arg(ap,void*); _TR_PUT('0');_TR_PUT('x'); n=_tr_fmt_u(tmp,v,16,0); for(int k=0;k<n;k++)_TR_PUT(tmp[k]); }
        else if(c=='c'){ _TR_PUT((char)va_arg(ap,int)); }
        else if(c=='s'){ const char* s=va_arg(ap,const char*); if(!s)s="(null)"; while(*s)_TR_PUT(*s++); }
        else if(c=='f'||c=='g'||c=='e'||c=='G'){ double d=va_arg(ap,double); if(d<0){_TR_PUT('-');d=-d;} unsigned long long ip=(unsigned long long)d; n=_tr_fmt_u(tmp,ip,10,0); for(int k=0;k<n;k++)_TR_PUT(tmp[k]); int pr=prec<0?6:prec; if(pr>0){ _TR_PUT('.'); double fr=d-(double)ip; for(int k=0;k<pr;k++){ fr*=10; int dg=(int)fr; _TR_PUT((char)('0'+(dg%10))); fr-=dg; } } }
        else if(c=='%'){ _TR_PUT('%'); }
        else { _TR_PUT('%'); if(c) _TR_PUT(c); }
    }
    if(cap) buf[o<cap?o:cap-1]='\0';
    #undef _TR_PUT
    return (int)o;
}
static int snprintf(char* buf,size_t cap,const char* fmt,...){ va_list ap; va_start(ap,fmt); int r=vsnprintf(buf,cap,fmt,ap); va_end(ap); return r; }
/* SECURITY [V-001]: freestanding libc-compat shim. Like ISO C sprintf it is UNBOUNDED —
 * the caller must guarantee `buf` is large enough. The Tauraro runtime itself NEVER calls
 * this (all internal formatting uses the bounded snprintf above; codegen emits snprintf).
 * Prefer snprintf(buf, sizeof(buf), ...) in new code. Kept only so freestanding/ported C
 * that references the `sprintf` symbol still links. */
static int sprintf(char* buf,const char* fmt,...){ va_list ap; va_start(ap,fmt); int r=vsnprintf(buf,(size_t)-1,fmt,ap); va_end(ap); return r; }
static int printf(const char* fmt,...){ char b[1024]; va_list ap; va_start(ap,fmt); int r=vsnprintf(b,sizeof(b),fmt,ap); va_end(ap); _TR_WRITE(b); return r; }
#endif  /* _TR_HAVE_CTYPE */
#endif  /* TAURARO_KERNEL && !__KERNEL__ */


/* ── Thread stack size ───────────────────────────────────────────────────── *
 * Override before including this header: -DTAURARO_THREAD_STACK_SIZE=N       *
 * 0 = use OS default (POSIX: skips setstacksize; Win32: passes 0 to          *
 * CreateThread which uses the executable's PE default, typically 1 MiB).     *
 * Platform defaults are applied below inside their respective #ifdef blocks.  */
#ifndef TAURARO_THREAD_STACK_SIZE
#  ifdef _WIN32
     /* Windows default: 2 MiB — matches legacy behaviour */
#    define TAURARO_THREAD_STACK_SIZE (2 * 1024 * 1024)
#  else
     /* POSIX default: 0 — let pthread use the OS default (typically 8 MiB) */
#    define TAURARO_THREAD_STACK_SIZE 0
#  endif
#endif

/* ── Panic / OOM hooks ───────────────────────────────────────────────────── */
#if defined(TAURARO_KERNEL) && defined(__KERNEL__)
#  define _TR_OOM_ABORT()    do { pr_err("tauraro: out of memory\n"); BUG(); } while(0)
#  define _TR_PANIC(msg)     do { pr_err("tauraro panic: %s\n", (msg)); BUG(); } while(0)
#elif defined(TAURARO_KERNEL)
#  define _TR_OOM_ABORT()    do { while(1); } while(0)
#  define _TR_PANIC(msg)     do { (void)(msg); while(1); } while(0)
#else
#  define _TR_OOM_ABORT()    do { fprintf(stderr, "tauraro: out of memory\n"); abort(); } while(0)
#  define _TR_PANIC(msg)     do { fprintf(stderr, "tauraro panic: %s\n", (msg)); abort(); } while(0)
#endif

/* ── Assert macros ───────────────────────────────────────────────────────── */
#if defined(TAURARO_KERNEL) && defined(__KERNEL__)
#  define _TR_ASSERT(cond)          do { if (!(cond)) { pr_err("assertion failed: %s  at %s:%d\n", #cond, __FILE__, __LINE__); BUG(); } } while(0)
#  define _TR_ASSERT_MSG(cond, msg) do { if (!(cond)) { pr_err("assertion failed: %s  message: %s  at %s:%d\n", #cond, (msg), __FILE__, __LINE__); BUG(); } } while(0)
#elif defined(TAURARO_KERNEL)
#  define _TR_ASSERT(cond)          do { if (!(cond)) { while(1); } } while(0)
#  define _TR_ASSERT_MSG(cond, msg) do { if (!(cond)) { (void)(msg); while(1); } } while(0)
#else
#  define _TR_ASSERT(cond) \
    do { if (!(cond)) { fprintf(stderr, "assertion failed: %s\n  at %s:%d\n", #cond, __FILE__, __LINE__); abort(); } } while(0)
#  define _TR_ASSERT_MSG(cond, msg) \
    do { if (!(cond)) { fprintf(stderr, "assertion failed: %s\n  message: %s\n  at %s:%d\n", #cond, (msg), __FILE__, __LINE__); abort(); } } while(0)
#endif

/* ââ Diagnostic + trap hooks ââ *
 * All raw stderr diagnostics and hard traps route through these so a tier below
 * `std` (TAURARO_KERNEL / freestanding) has NO direct libc: hosted keeps the exact
 * fprintf/abort behaviour; kernel spins/BUG()s; a bare-metal target can redefine
 * _TR_DIAG to a UART/semihosting write before including this header. */
#if defined(TAURARO_KERNEL) && defined(__KERNEL__)
#  ifndef _TR_DIAG
#    define _TR_DIAG(...)   pr_err(__VA_ARGS__)
#  endif
#  ifndef _TR_TRAP
#    define _TR_TRAP()      BUG()
#  endif
#elif defined(TAURARO_KERNEL)
#  ifndef _TR_DIAG
#    define _TR_DIAG(...)   ((void)0)
#  endif
#  ifndef _TR_TRAP
#    define _TR_TRAP()      do { while(1); } while(0)
#  endif
#else
#  ifndef _TR_DIAG
#    define _TR_DIAG(...)   fprintf(stderr, __VA_ARGS__)
#  endif
#  ifndef _TR_TRAP
#    define _TR_TRAP()      abort()
#  endif
#endif

/* Opt-in live-allocation counter for leak bisection. Compile with
 * -DTAURARO_MEMCOUNT to enable; _tr_report_mem() then also prints the net
 * number of outstanding (alloc'd minus freed) heap blocks, so a steady
 * per-request delta localizes a leak by object count. Zero-cost otherwise. */
#ifdef TAURARO_MEMCOUNT
#ifdef _TR_MAIN
long _tr_live_allocs = 0;
long _tr_live_dicts = 0;
long _tr_live_lists = 0;
long _tr_live_strs = 0;
#else
extern long _tr_live_allocs;
extern long _tr_live_dicts;
extern long _tr_live_lists;
extern long _tr_live_strs;
#endif
#define _TR_MEMCOUNT_INC() (_tr_live_allocs++)
#define _TR_MEMCOUNT_DEC() (_tr_live_allocs--)
#define _TR_MEMCOUNT_DICT_INC() (_tr_live_dicts++)
#define _TR_MEMCOUNT_DICT_DEC() (_tr_live_dicts--)
#define _TR_MEMCOUNT_LIST_INC() (_tr_live_lists++)
#define _TR_MEMCOUNT_LIST_DEC() (_tr_live_lists--)
#define _TR_MEMCOUNT_STR_INC() (_tr_live_strs++)
#define _TR_MEMCOUNT_STR_DEC() (_tr_live_strs--)
#else
#define _TR_MEMCOUNT_INC() ((void)0)
#define _TR_MEMCOUNT_DEC() ((void)0)
#define _TR_MEMCOUNT_DICT_INC() ((void)0)
#define _TR_MEMCOUNT_DICT_DEC() ((void)0)
#define _TR_MEMCOUNT_LIST_INC() ((void)0)
#define _TR_MEMCOUNT_LIST_DEC() ((void)0)
#define _TR_MEMCOUNT_STR_INC() ((void)0)
#define _TR_MEMCOUNT_STR_DEC() ((void)0)
#endif

/* Net live heap allocations (alloc'd minus freed) under -DTAURARO_MEMCOUNT,
 * else 0. Exposed to Tauraro so leak-gate tests can assert a workload returns
 * to its starting allocation count. */
static inline long long _tr_mem_live(void) {
#ifdef TAURARO_MEMCOUNT
    return (long long)_tr_live_allocs;
#else
    return 0LL;
#endif
}

/* Net live TrStr string objects (alloc'd minus freed) under -DTAURARO_MEMCOUNT.
 * Zero-copy views (StrView) hold a borrowed pointer and create NO TrStr, so this
 * counter distinguishes data-copying (str.slice) from borrowing (StrView).      */
static inline long long _tr_mem_live_strs(void) {
#ifdef TAURARO_MEMCOUNT
    return (long long)_tr_live_strs;
#else
    return 0LL;
#endif
}

/* _TR_XLINK: `static inline` for the C backend; EXPORTED for the NATIVE/LLVM backend
 * (native_abi.c #defines _TR_EXPORT_RT). Defined HERE (early) so functions above line 400
 * — the memory + I/O helpers — can use it too. */
#ifdef _TR_EXPORT_RT
#define _TR_XLINK
#else
#define _TR_XLINK static inline
#endif

/* Forward decl: the rc-prefixed string constructor (defined in native_abi.c). The native/
 * LLVM backend's `str` is an rc'd char* released via _tr_rt_str_release, so any _h wrapper
 * that hands a string back to that backend MUST return an rc string (not a raw _tr_checked_
 * alloc/borrowed pointer, which would corrupt the heap on release). */
_TR_XLINK char* _tr_rt_str_new(const char* s);

// Wrappers for core library to avoid signature conflicts
_TR_XLINK void* _tr_c_malloc(size_t size) {
    _TR_HEAPCHK("c_malloc");
    void* p = TAURARO_ALLOC(size);
    if (p) _TR_MEMCOUNT_INC();
    return p;
}
_TR_XLINK void* _tr_c_calloc(size_t count, size_t size) {
    void* p = TAURARO_CALLOC(count, size);
    if (!p && count * size > 0) { _TR_OOM_ABORT(); }
    if (p) _TR_MEMCOUNT_INC();
    return p;
}
_TR_XLINK void _tr_free(void* p) {
    _TR_HEAPCHK("free");
    if (p) { _TR_MEMCOUNT_DEC(); TAURARO_FREE(p); }
}
/* Runtime memory helpers used by std/core (Vec/String) via `extern "C"` decls. `static
 * inline` for the C backend (#includes this header, inlines them); EXPORTED as real
 * symbols for the NATIVE/LLVM backend (native_abi.c #defines _TR_EXPORT_RT) so those std
 * collections link. */
_TR_XLINK void _tr_c_free(void* ptr) { _tr_free(ptr); }

#ifndef TAURARO_KERNEL
static inline void _tr_print(char* s) { printf("%s\n", s); }
_TR_XLINK void _tr_print_raw(char* s) { printf("%s", s); fflush(stdout); }
static inline void _tr_eprint(char* s) { _TR_DIAG("%s\n", s); fflush(stderr); }
#else
#ifndef _TR_WRITE
#  define _TR_WRITE(s) ((void)(s))   /* freestanding sink: redefine to UART/semihosting */
#endif
static inline void _tr_print(char* s) { _TR_WRITE(s); _TR_WRITE("\n"); }
_TR_XLINK void _tr_print_raw(char* s) { _TR_WRITE(s); }
static inline void _tr_eprint(char* s) { _TR_WRITE(s); _TR_WRITE("\n"); }
#endif

_TR_XLINK void* _tr_c_realloc(void* ptr, size_t size) {
    _TR_HEAPCHK("realloc");
    void* p = TAURARO_REALLOC(ptr, size);
    /* realloc(NULL, n) acts as malloc -> a new live block; realloc of an
     * existing block frees the old internally (no _tr_free) and keeps the
     * same logical block, so only the fresh-allocation case is counted. */
    if (!ptr && p) _TR_MEMCOUNT_INC();
    return p;
}
_TR_XLINK void* _tr_checked_alloc(size_t sz) {
    _TR_HEAPCHK("checked_alloc");
    void* p = TAURARO_CALLOC(1, sz);
    if (!p && sz > 0) { _TR_OOM_ABORT(); }
    if (p) _TR_MEMCOUNT_INC();
    return p;
}

/* ── Class-instance ARC ──────────────────────────────────────────────────────
 * A heap class instance carries a refcount as its FIRST member (`size_t __rc`,
 * so it sits at offset 0). Instances are shared by retain/released by scope-exit
 * and container teardown, and freed when the count reaches zero — sound under
 * aliasing (no ownership proof needed). Retain/release are elided by codegen
 * wherever the borrow checker proves a value is only borrowed (zero-cost). */
static inline void* _tr_obj_alloc(size_t sz) {
    _TR_HEAPCHK("obj_alloc");
    void* p = TAURARO_CALLOC(1, sz);
    if (!p && sz > 0) { _TR_OOM_ABORT(); }
    if (p) { *(size_t*)p = 1; _TR_MEMCOUNT_INC(); }   /* rc = 1 */
    return p;
}
static inline void* _tr_obj_retain(void* p) {
    if (p) (*(size_t*)p)++;
    return p;
}
/* `drop` releases the instance's owned fields (generated per class). NULL for a
 * class with no droppable fields — the struct is still freed. */
static inline void _tr_obj_release(void* p, void (*drop)(void*)) {
    if (p && --(*(size_t*)p) == 0) {
        if (drop) drop(p);
        _TR_MEMCOUNT_DEC();
        TAURARO_FREE(p);
    }
}
/* Heap-allocated empty C string. Used by char*-returning helpers that need
 * an "empty result" fallback - returning a static string literal (`""`)
 * here would later be `_tr_str_wrap`'d (rc=1) and `_tr_str_release`'d,
 * calling free() on a non-heap pointer (UB / -Wfree-nonheap-object). */
static inline char* _tr_empty_heap_str(void) {
    char* e = (char*)TAURARO_ALLOC(1);
    e[0] = '\0';
    _TR_MEMCOUNT_INC();
    return e;
}
/* Owned (heap) copy of a C string, for char*-returning helpers declared `-> str`
 * whose NORMAL result is a compile-time constant (e.g. _tr_platform/_tr_arch/
 * _tr_exe_dir). Codegen UNIFORMLY wraps every `-> str` extern result as owned
 * (rc=1, _tr_str_wrap) and frees it via auto-drop, so returning a string literal
 * is a free()-on-non-heap corruption; this returns heap so the free is valid.
 * Memcount-balanced (INC here via _tr_checked_alloc, DEC at _tr_free). */
static inline char* _tr_str_dup_owned(const char* s) {
    if (!s) return _tr_empty_heap_str();
    size_t n = strlen(s);
    char* r = (char*)_tr_checked_alloc(n + 1);
    memcpy(r, s, n + 1);
    return r;
}
/* ── Refcounted string (TrStr): fat-pointer str representation ──
 * `data` points at the NUL-terminated bytes. `rc` points at a heap
 * refcount, or is NULL for literal/immortal strings — in that case
 * retain/release are no-ops, so string literals never need freeing. */
typedef struct {
    char* data;
    long* rc;
} TrStr;

static inline TrStr _tr_str_lit_impl(const char* s) {
    TrStr t; t.data = (char*)s; t.rc = NULL; return t;
}
static inline TrStr _tr_str_lit_passthrough(TrStr s) { return s; }
/* `_tr_str_lit(x)`: wrap a borrowed `const char*` into a TrStr (rc=NULL).
 * Some codegen call sites pass an expression that is ALREADY a TrStr
 * (e.g. `_tr_str_lit(StringObj_as_str(...))` once StringObj_as_str was
 * migrated to return TrStr) - in that case this is a redundant no-op
 * wrap, so pass the TrStr through unchanged instead of erroring.
 * _Generic dispatches on a FUNCTION DESIGNATOR (not a pre-built call
 * expression) so the unselected branch is never type-checked against `x`. */
#define _tr_str_lit(x) (_Generic((x), TrStr: _tr_str_lit_passthrough, default: _tr_str_lit_impl)(x))

/* Allocates a new heap string of `len` bytes (plus NUL terminator)
 * with refcount 1. Caller fills t.data[0..len-1].
 *
 * `data` and `rc` are SEPARATE allocations (not one combined block):
 * `_tr_strz(t)` returns `t.data` directly, and many call sites do
 * `_tr_c_free(_tr_strz(x))` (the `unsafe: _tr_c_free(x as Pointer[char])`
 * idiom in std files) - that free() must see a real malloc base pointer.
 * A combined allocation with `data = block + sizeof(long)` would make
 * that free() corrupt the heap (freeing a pointer 8 bytes past the
 * block start). Two allocations cost one extra malloc per _tr_str_new
 * call (concat/repeat/join/etc.) but keep `.data` independently valid. */
static inline TrStr _tr_str_new(size_t len) {
    TrStr t;
    t.data = (char*)_tr_checked_alloc(len + 1);
    t.data[len] = '\0';
    t.rc = (long*)_tr_checked_alloc(sizeof(long));
    *t.rc = 1;
    _TR_MEMCOUNT_STR_INC();
    return t;
}

static inline TrStr _tr_str_retain(TrStr s) {
    if (s.rc) { (*s.rc)++; }
    return s;
}

static inline void _tr_str_release(TrStr s) {
    if (s.rc) {
        if (--(*s.rc) == 0) {
            _TR_MEMCOUNT_STR_DEC();
            _tr_free(s.data);
            _tr_free((void*)s.rc);
        }
    }
}

/* Wraps an EXISTING heap `char*` (e.g. the result of any legacy
 * _tr_str_* function, malloc'd separately from its refcount) into a
 * TrStr with refcount 1. Two allocations, but lets every existing
 * char*-returning string helper become TrStr-compatible with no
 * changes to its own body. */
static inline TrStr _tr_str_wrap_impl(char* owned_data) {
    TrStr t;
    t.data = owned_data;
    t.rc = (long*)_tr_checked_alloc(sizeof(long));
    *t.rc = 1;
    _TR_MEMCOUNT_STR_INC();
    return t;
}

#ifndef _TR_EXPORT_RT
/* C backend: the native `char*` rc-string constructor is defined in native_abi.c, which the
 * header-only C backend does not link. std/async `_h` wrappers (e.g. _tr_task_get_error_h)
 * call _tr_rt_str_new and their result is immediately `_tr_str_wrap`'d into a TrStr — so the
 * correct C-backend form is a plain OWNED heap copy (_tr_str_wrap adopts it, rc=1). */
static inline char* _tr_rt_str_new(const char* s) {
    size_t n = s ? strlen(s) : 0;
    char* r = (char*)_tr_checked_alloc(n + 1);
    if (!r) return (char*)0;
    if (s && n) memcpy(r, s, n);
    r[n] = '\0';
    return r;
}
#endif

/* Stage 2 — in-process LLVM object emitter. Prototype only; the definition is in
 * runtime/tauraro_llvm.c (ALWAYS linked into the compiler): a stub returning -1 by default,
 * or the real llvm-c emitter when that file is built with -DTAURARO_LLVM_INPROC + linked
 * against libLLVM. -1 ("not compiled in") makes the compiler fall back to the bundled-llc
 * subprocess (Stage 1). Kept out of the header as a plain def to avoid mingw's multiple-
 * definition of a weak body across the many TUs that include this header. */
int _tr_llvm_emit_object(const char* ll_path, const char* out_path, const char* triple);

static inline TrStr _tr_str_wrap_passthrough(TrStr s) { return s; }
/* `_tr_str_wrap(x)`: wrap an owned `char*` into a TrStr (rc=1). Some
 * codegen call sites double-wrap (e.g. `_tr_str_wrap(_tr_str_wrap(buffer))`
 * from an extern-helper call whose declared return type is `str`) - in
 * that case this is a redundant no-op wrap, so pass the TrStr through
 * unchanged instead of erroring. Same _Generic function-designator-dispatch
 * trick as `_tr_str_lit`. */
#define _tr_str_wrap(x) (_Generic((x), TrStr: _tr_str_wrap_passthrough, default: _tr_str_wrap_impl)(x))

/* Extracts the raw `char*` for passing to functions that take
 * `const char*`/`char*`. Does not affect the refcount. */
static inline char* _tr_strz(TrStr s) {
    return s.data;
}

/* Box/unbox a TrStr (16 bytes) into the generic `void* val` slot used by
 * Option[T]/Result[T,E]. A direct (void*)(TrStr) cast is a hard error in C
 * since TrStr is a struct, not a pointer-sized scalar. */
static inline void* _tr_str_box(TrStr s) {
    TrStr* p = (TrStr*)_tr_checked_alloc(sizeof(TrStr));
    *p = s;
    return (void*)p;
}
static inline TrStr _tr_str_unbox(void* p) {
    if (!p) return _tr_str_lit("");
    /* Non-destructive: callers (e.g. f-string codegen) may evaluate the
     * source expression more than once (size-then-format snprintf pattern),
     * so freeing `p` here would cause a double-free/use-after-free on the
     * second evaluation. The 16-byte box leaks; acceptable until codegen
     * single-evaluates these expressions via temps. */
    return *(TrStr*)p;
}
/* ── Shared ownership: reference-counted box (replaces Rc/Arc/Mutex in one keyword) ── */
/* Control block for Shared[T] (Rc) and Weak[T]. The payload is destroyed when the
 * STRONG count hits 0; the block itself lingers until the WEAK count is also 0, so
 * a Weak[T] can safely observe liveness / attempt upgrade without dangling. This is
 * what lets Weak[T] break an ownership CYCLE with no leak and no use-after-free. */
typedef struct _TrSharedBox {
    _Atomic(int) refcount;    /* strong */
    _Atomic(int) weakcount;   /* outstanding Weak[T] handles */
    void* data;
    void (*drop)(void*);      /* payload field-drop (_trdrop_T); NULL → plain free */
} _TrSharedBox;

static inline _TrSharedBox* _tr_shared_new(void* data, void (*drop)(void*)) {
    _TrSharedBox* b = (_TrSharedBox*)_tr_checked_alloc(sizeof(_TrSharedBox));
    atomic_store(&b->refcount, 1);
    atomic_store(&b->weakcount, 0);
    b->data = data;
    b->drop = drop;
    return b;
}
static inline _TrSharedBox* _tr_shared_clone(_TrSharedBox* b) {
    if (b) { atomic_fetch_add(&b->refcount, 1); }
    return b;
}
static inline void _tr_shared_drop(_TrSharedBox* b) {
    if (!b) return;
    if (atomic_fetch_sub(&b->refcount, 1) == 1) {
        /* Release the payload: a plain-ARC class (drop set) releases its owned
         * fields + struct via the refcount; otherwise a plain free. */
        if (b->drop) { _tr_obj_release(b->data, b->drop); } else { _tr_free(b->data); }
        b->data = NULL;
        /* Keep the (payload-less) block alive while any Weak[T] still points here,
         * so their is_alive()/upgrade() read a valid refcount==0 instead of freed memory. */
        if (atomic_load(&b->weakcount) == 0) { _tr_free(b); }
    }
}
/* ── Weak[T] — non-owning reference to a Shared[T] box (does NOT keep it alive) ── */
typedef struct _TrWeakBox {
    _TrSharedBox* box;
} _TrWeakBox;
static inline _TrWeakBox* _tr_weak_new(_TrSharedBox* b) {
    _TrWeakBox* w = (_TrWeakBox*)_tr_checked_alloc(sizeof(_TrWeakBox));
    if (b) atomic_fetch_add(&b->weakcount, 1);
    w->box = b;
    return w;
}
static inline bool _tr_weak_is_alive(_TrWeakBox* w) {
    if (!w || !w->box) return false;
    return atomic_load(&w->box->refcount) > 0;
}
static inline _TrSharedBox* _tr_weak_upgrade(_TrWeakBox* w) {
    if (!w || !w->box) return NULL;
    int old = atomic_load(&w->box->refcount);
    if (old <= 0) return NULL;
    atomic_fetch_add(&w->box->refcount, 1);
    return w->box;
}
/* Drop a Weak[T]: decrement the box's weak count; if the payload is already gone
 * (strong==0) and this was the last weak handle, reclaim the block. Also frees the
 * small weak handle itself. */
static inline void _tr_weak_drop(_TrWeakBox* w) {
    if (!w) return;
    _TrSharedBox* b = w->box;
    if (b) {
        if (atomic_fetch_sub(&b->weakcount, 1) == 1 && atomic_load(&b->refcount) <= 0) {
            _tr_free(b);
        }
    }
    _tr_free(w);
}

_TR_XLINK void* _tr_c_memcpy(void* dst, void* src, size_t n) {
#ifdef TR_HEAPDBG
    _trdbg_bounds(dst, n);
#endif
    return memcpy(dst, src, n); }
_TR_XLINK void* _tr_c_memset(void* ptr, int val, size_t n) { return memset(ptr, val, n); }
_TR_XLINK void* _tr_c_memmove(void* dst, void* src, size_t n) { return memmove(dst, src, n); }
/* File I/O + env: std-tier only (need <stdio.h>'s FILE / getenv). Gated so a
 * freestanding (TAURARO_BARE) build parses past here — leaving these ungated was
 * the 'FILE undeclared' early-header-failure that made everything after look
 * implicit on bare-metal. */
#ifndef TAURARO_BARE
_TR_XLINK void* _tr_c_fopen(const char* path, const char* mode) { return (void*)fopen(path, mode); }
_TR_XLINK int _tr_c_fclose(void* fp) { return fclose((FILE*)fp); }
_TR_XLINK size_t _tr_c_fread(void* ptr, size_t size, size_t nmemb, void* fp) { return fread(ptr, size, nmemb, (FILE*)fp); }
_TR_XLINK size_t _tr_c_fwrite(const void* ptr, size_t size, size_t nmemb, void* fp) { return fwrite(ptr, size, nmemb, (FILE*)fp); }
_TR_XLINK int _tr_c_fseek(void* fp, long offset, int whence) { return fseek((FILE*)fp, offset, whence); }
_TR_XLINK long _tr_c_ftell(void* fp) { return ftell((FILE*)fp); }
_TR_XLINK char* _tr_getenv(const char* name) { char* v = getenv(name); return v ? v : ""; }
#else
_TR_XLINK char* _tr_getenv(const char* name) { (void)name; return (char*)""; }
#endif
#ifdef _WIN32
static inline int _tr_setenv(const char* name, const char* value) { return _putenv_s(name, value) == 0 ? 0 : -1; }
static inline int _tr_unsetenv(const char* name) { return _putenv_s(name, "") == 0 ? 0 : -1; }
#elif defined(TAURARO_BARE)
static inline int _tr_setenv(const char* name, const char* value) { (void)name; (void)value; return -1; }
static inline int _tr_unsetenv(const char* name) { (void)name; return -1; }
#else
static inline int _tr_setenv(const char* name, const char* value) { return setenv(name, value, 1) == 0 ? 0 : -1; }
static inline int _tr_unsetenv(const char* name) { return unsetenv(name) == 0 ? 0 : -1; }
#endif
#if defined(TAURARO_BARE) || defined(TAURARO_NO_SUBPROCESS)
/* No subprocess on bare / wasi: reading a command's output is unsupported. */
static inline char* _tr_popen_read(const char* cmd) { (void)cmd; return _tr_empty_heap_str(); }
#else
static inline char* _tr_popen_read(const char* cmd) {
    if (!cmd) return _tr_empty_heap_str();
#  ifdef _WIN32
    FILE* fp = _popen(cmd, "r");
#  else
    FILE* fp = popen(cmd, "r");
#  endif
    if (!fp) return _tr_empty_heap_str();
    size_t cap = 4096, total = 0; char* buf = (char*)TAURARO_ALLOC(cap); char tmp[512];
    if (!buf) { fclose(fp); return _tr_empty_heap_str(); }
    while (fgets(tmp, sizeof(tmp), fp)) {
        size_t n = strlen(tmp);
        if (total + n + 1 > cap) { cap = cap * 2 + n + 1; buf = (char*)TAURARO_REALLOC(buf, cap); if (!buf) break; }
        memcpy(buf + total, tmp, n); total += n;
    }
    if (buf) buf[total] = '\0';
#ifdef _WIN32
    _pclose(fp);
#else
    pclose(fp);
#endif
    return buf ? buf : "";
}
#endif /* TAURARO_BARE popen guard */
#if !defined(_WIN32) && !defined(TAURARO_BARE)
static inline long long _tr_time_ns(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + (long long)ts.tv_nsec;
}
static inline char* _tr_path_canonicalize(const char* path) {
    char* r = realpath(path, NULL); return r ? r : (char*)path;
}
#elif defined(TAURARO_BARE)
static inline long long _tr_time_ns(void) {
#  ifdef __wasi__
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + (long long)ts.tv_nsec;
#  else
    return 0LL;
#  endif
}
static inline char* _tr_path_canonicalize(const char* path) { return (char*)path; }
#endif

/* ── Prelude: Option[T] ──────────────────────────────────────────────────── */
#ifndef _TR_ENUM_OPTION_DEFINED
#define _TR_ENUM_OPTION_DEFINED
typedef struct Option Option;
typedef enum { Option_Some, Option_None } Option_Tag;
struct Option {
    Option_Tag tag;
    union {
        struct { void* val; } Some;
        struct { int dummy; } None;
    } data;
};
#endif

/* ── Prelude: Result[T, E] ───────────────────────────────────────────────── */
#ifndef _TR_ENUM_RESULT_DEFINED
#define _TR_ENUM_RESULT_DEFINED
typedef struct Result Result;
typedef enum { Result_Ok, Result_Err } Result_Tag;
struct Result {
    Result_Tag tag;
    union {
        struct { void* val; } Ok;
        struct { void* err; } Err;
    } data;
};
#endif

/* Bit-reinterpret a double <-> void* so Option[float]/Result[float] can store
 * a float/double payload in the generic `void* val` slot. A direct
 * (void*)(double) or (double)(void*) cast is a hard error in C. */
static inline void* _tr_f64_to_ptr(double d) { union { double d; void* p; } u; u.p = 0; u.d = d; return u.p; }
static inline double _tr_ptr_to_f64(void* p) { union { double d; void* p; } u; u.d = 0; u.p = p; return u.d; }

/* ── Option[T] methods ───────────────────────────────────────────────── */
static inline bool Option_is_some(Option self) { return self.tag == Option_Some; }
static inline bool Option_is_none(Option self) { return self.tag == Option_None; }
static inline void* Option_unwrap(Option self) {
    if (self.tag != Option_Some) { _TR_DIAG("Option.unwrap() called on None\n"); _TR_TRAP(); }
    return self.data.Some.val;
}
static inline void* Option_unwrap_or(Option self, void* _default) {
    return self.tag == Option_Some ? self.data.Some.val : _default;
}
static inline void* Option_expect(Option self, char* msg) {
    if (self.tag != Option_Some) { _TR_DIAG("%s\n", msg); _TR_TRAP(); }
    return self.data.Some.val;
}
static inline Option Option_map(Option self, void* (*f)(void*)) {
    if (self.tag != Option_Some) return self;
    Option r; r.tag = Option_Some; r.data.Some.val = f(self.data.Some.val); return r;
}
static inline Option Option_and_then(Option self, Option (*f)(void*)) {
    return self.tag == Option_Some ? f(self.data.Some.val) : self;
}
static inline Option Option_or(Option self, Option other) {
    return self.tag == Option_Some ? self : other;
}
static inline Option Option_or_else(Option self, Option (*f)()) {
    return self.tag == Option_Some ? self : f();
}
static inline Result Option_ok_or(Option self, void* err);  /* defined after Result */

/* ── Result[T, E] methods ────────────────────────────────────────────── */
static inline bool Result_is_ok(Result self)  { return self.tag == Result_Ok;  }
static inline bool Result_is_err(Result self) { return self.tag == Result_Err; }
static inline void* Result_unwrap(Result self) {
    if (self.tag != Result_Ok) { _TR_DIAG("Result.unwrap() called on Err\n"); _TR_TRAP(); }
    return self.data.Ok.val;
}
static inline void* Result_unwrap_err(Result self) {
    if (self.tag != Result_Err) { _TR_DIAG("Result.unwrap_err() called on Ok\n"); _TR_TRAP(); }
    return self.data.Err.err;
}
static inline void* Result_unwrap_or(Result self, void* _default) {
    return self.tag == Result_Ok ? self.data.Ok.val : _default;
}
static inline void* Result_expect(Result self, char* msg) {
    if (self.tag != Result_Ok) { _TR_DIAG("%s\n", msg); _TR_TRAP(); }
    return self.data.Ok.val;
}
static inline Result Result_map(Result self, void* (*f)(void*)) {
    if (self.tag != Result_Ok) return self;
    Result r; r.tag = Result_Ok; r.data.Ok.val = f(self.data.Ok.val); return r;
}
static inline Result Result_map_err(Result self, void* (*f)(void*)) {
    if (self.tag != Result_Err) return self;
    Result r; r.tag = Result_Err; r.data.Err.err = f(self.data.Err.err); return r;
}
static inline Result Result_and_then(Result self, Result (*f)(void*)) {
    return self.tag == Result_Ok ? f(self.data.Ok.val) : self;
}
static inline Option Result_ok(Result self) {
    Option o; o.tag = self.tag == Result_Ok ? Option_Some : Option_None;
    if (self.tag == Result_Ok) o.data.Some.val = self.data.Ok.val; return o;
}
static inline Option Result_err(Result self) {
    Option o; o.tag = self.tag == Result_Err ? Option_Some : Option_None;
    if (self.tag == Result_Err) o.data.Some.val = self.data.Err.err; return o;
}
/* Resolve forward decl */
static inline Result Option_ok_or(Option self, void* err) {
    Result r;
    r.tag = self.tag == Option_Some ? Result_Ok : Result_Err;
    if (self.tag == Option_Some) r.data.Ok.val = self.data.Some.val;
    else r.data.Err.err = err;
    return r;
}

/* ── Threading (cross-platform) ──────────────────────────────────────── */

/* Thread panic state — forward-declared before platform blocks so trampolines
 * can reference them.  Actual storage definitions live in the _TR_GLOBAL section. */
#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL) || defined(TAURARO_NO_THREADS)
/* Single-threaded (bare / wasi): plain globals, no thread-local storage needed. */
static int     _tr_thread_has_panic_buf   = 0;
static jmp_buf _tr_thread_panic_jmpbuf;
static char*   _tr_thread_panic_message   = NULL;
#elif defined(__GNUC__) || defined(__clang__)
/* MinGW also defines _WIN32 but is GCC-based — must use __thread, not __declspec(thread) */
extern __thread int     _tr_thread_has_panic_buf;
extern __thread jmp_buf _tr_thread_panic_jmpbuf;
extern __thread char*   _tr_thread_panic_message;
#elif defined(_MSC_VER)
extern __declspec(thread) int     _tr_thread_has_panic_buf;
extern __declspec(thread) jmp_buf _tr_thread_panic_jmpbuf;
extern __declspec(thread) char*   _tr_thread_panic_message;
#else
extern _Thread_local int     _tr_thread_has_panic_buf;
extern _Thread_local jmp_buf _tr_thread_panic_jmpbuf;
extern _Thread_local char*   _tr_thread_panic_message;
#endif

/* Panic result: written by thread, read by joiner via _TrThreadObj */
typedef struct { int panicked; char* panic_msg; } _TrSpawnResult;


/* _WIN32 is a TARGET/compiler macro, not a "windows.h is actually available"
 * guarantee -- a cross build (TAURARO_KERNEL/TAURARO_BARE, e.g. Cortex-M via
 * `zig cc -target thumb-freestanding-eabi` on a Windows host) can still see
 * _WIN32 defined by the driver while having no OS, no windows.h, and no
 * psapi/bcrypt libs to link. Every "_WIN32 means hosted Windows" branch below
 * must also check `!defined(TAURARO_BARE)`, or it tries to pull in headers
 * that don't exist for the target and the freestanding build fails to compile
 * (or, worse, is worked around per-project by manually `-U`-ing the macros).
 */
#if !defined(_WIN32) || defined(TAURARO_BARE)
/* Debug helper: prints current process memory usage to stderr, tagged with
 * `label`. Used to bisect memory growth across checkpoints during
 * leak-hunting; not called by normal runtime code. No-op on non-Windows
 * (and on any bare-metal/kernel target, Windows-triple macros notwithstanding). */
_TR_XLINK void _tr_report_mem(const char* label) { (void)label; }
#endif

#if defined(_WIN32) && !defined(TAURARO_BARE)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")   /* MSVC; MinGW links via -lbcrypt */

/* Debug helper: prints current process working-set size to stderr, tagged
 * with `label`. Used to bisect memory growth across checkpoints during
 * leak-hunting; not called by normal runtime code. */
_TR_XLINK void _tr_report_mem(const char* label) {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
#ifdef TAURARO_MEMCOUNT
    _TR_DIAG("%s: %zu bytes, %ld allocs, %ld dicts, %ld lists, %ld strs\n", label, (size_t)pmc.WorkingSetSize, _tr_live_allocs, _tr_live_dicts, _tr_live_lists, _tr_live_strs);
#else
    _TR_DIAG("%s: %zu bytes\n", label, (size_t)pmc.WorkingSetSize);
#endif
    fflush(stderr);
}

typedef HANDLE _TrThread;
/* Trampoline: routes void*(*)(void*) through DWORD WINAPI, installs
 * per-thread panic handler (setjmp), and enforces stack size.        */
typedef struct { void*(*fn)(void*); void* arg; _TrSpawnResult* result; } _TrWin32StartArg;
static DWORD WINAPI _tr_thread_start_trampoline(LPVOID raw) {
    _TrWin32StartArg* s = (_TrWin32StartArg*)raw;
    void*(*fn)(void*) = s->fn; void* arg = s->arg;
    _TrSpawnResult* result = s->result;
    free(s);
    /* Install per-thread panic handler */
    _tr_thread_has_panic_buf = 1;
    _tr_thread_panic_message = NULL;
    if (setjmp(_tr_thread_panic_jmpbuf) == 0) {
        fn(arg);
        if (result) { result->panicked = 0; result->panic_msg = NULL; }
    } else {
        if (result) { result->panicked = 1; result->panic_msg = _tr_thread_panic_message; }
        else { _TR_DIAG("thread panic (detached): %s\n",
               _tr_thread_panic_message ? _tr_thread_panic_message : "?"); }
    }
    _tr_thread_has_panic_buf = 0;
    return 0;
}
_TR_XLINK _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    _TrWin32StartArg* s = (_TrWin32StartArg*)malloc(sizeof(_TrWin32StartArg));
    s->fn = fn; s->arg = arg; s->result = NULL;
    SIZE_T ss = (SIZE_T)TAURARO_THREAD_STACK_SIZE;
    return CreateThread(NULL, ss, _tr_thread_start_trampoline, s, 0, NULL);
}
_TR_XLINK void _tr_thread_detach(_TrThread t) { CloseHandle(t); }
_TR_XLINK void _tr_thread_join_wait(_TrThread t) { WaitForSingleObject(t, INFINITE); CloseHandle(t); }

typedef CRITICAL_SECTION _TrMutex;
static void _tr_mutex_init(_TrMutex* m)   { InitializeCriticalSection(m); }
static void _tr_mutex_lock(_TrMutex* m)   { EnterCriticalSection(m); }
static void _tr_mutex_unlock(_TrMutex* m) { LeaveCriticalSection(m); }

typedef struct { CRITICAL_SECTION cs; CONDITION_VARIABLE cv; } _TrCondMutex;
static void _tr_condmutex_init(_TrCondMutex* cm)    { InitializeCriticalSection(&cm->cs); InitializeConditionVariable(&cm->cv); }
static void _tr_condmutex_lock(_TrCondMutex* cm)    { EnterCriticalSection(&cm->cs); }
static void _tr_condmutex_unlock(_TrCondMutex* cm)  { LeaveCriticalSection(&cm->cs); }
static void _tr_condmutex_wait(_TrCondMutex* cm)    { SleepConditionVariableCS(&cm->cv, &cm->cs, INFINITE); }
static void _tr_condmutex_signal(_TrCondMutex* cm)  { WakeConditionVariable(&cm->cv); }
_TR_XLINK void _tr_sleep_ms(long ms) { Sleep((DWORD)(ms < 0 ? 0 : ms)); }
static inline long long _tr_time_ns(void) {
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq); QueryPerformanceCounter(&cnt);
    return (long long)((double)cnt.QuadPart * 1000000000.0 / (double)freq.QuadPart);
}
static inline char* _tr_path_canonicalize(const char* path) {
    char* buf = (char*)malloc(MAX_PATH);
    if (!buf) return (char*)path;
    DWORD n = GetFullPathNameA(path, MAX_PATH, buf, NULL);
    if (n == 0) { free(buf); return (char*)path; }
    return buf;
}

#elif defined(TAURARO_BARE) || defined(TAURARO_NO_THREADS)
/* ── BARE/WASM/WASI: single-threaded primitive stubs (spawn runs inline) ── */
typedef int _TrThread;
_TR_XLINK _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) { fn(arg); return 0; }
_TR_XLINK void _tr_thread_detach(_TrThread t)      { (void)t; }
_TR_XLINK void _tr_thread_join_wait(_TrThread t)   { (void)t; }

typedef int _TrMutex;
static void _tr_mutex_init(_TrMutex* m)         { (void)m; }
static void _tr_mutex_lock(_TrMutex* m)         { (void)m; }
static void _tr_mutex_unlock(_TrMutex* m)       { (void)m; }

typedef struct { int dummy; } _TrCondMutex;
static void _tr_condmutex_init(_TrCondMutex* cm)    { (void)cm; }
static void _tr_condmutex_lock(_TrCondMutex* cm)    { (void)cm; }
static void _tr_condmutex_unlock(_TrCondMutex* cm)  { (void)cm; }
static void _tr_condmutex_wait(_TrCondMutex* cm)    { (void)cm; }
static void _tr_condmutex_signal(_TrCondMutex* cm)  { (void)cm; }
_TR_XLINK void _tr_sleep_ms(long ms) { (void)ms; }

#else
#include <pthread.h>
#include <time.h>

typedef pthread_t _TrThread;
/* POSIX panic trampoline — installs per-thread setjmp handler */
typedef struct { void*(*fn)(void*); void* arg; _TrSpawnResult* result; } _TrPosixStartArg;
static void* _tr_posix_thread_trampoline(void* raw) {
    _TrPosixStartArg* s = (_TrPosixStartArg*)raw;
    void*(*fn)(void*) = s->fn; void* arg = s->arg;
    _TrSpawnResult* result = s->result;
    free(s);
    _tr_thread_has_panic_buf = 1;
    _tr_thread_panic_message = NULL;
    if (setjmp(_tr_thread_panic_jmpbuf) == 0) {
        fn(arg);
        if (result) { result->panicked = 0; result->panic_msg = NULL; }
    } else {
        if (result) { result->panicked = 1; result->panic_msg = _tr_thread_panic_message; }
        else { _TR_DIAG("thread panic (detached): %s\n",
               _tr_thread_panic_message ? _tr_thread_panic_message : "?"); }
    }
    _tr_thread_has_panic_buf = 0;
    return NULL;
}
_TR_XLINK _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    _TrPosixStartArg* s = (_TrPosixStartArg*)malloc(sizeof(_TrPosixStartArg));
    s->fn = fn; s->arg = arg; s->result = NULL;
    pthread_attr_t attr; pthread_attr_init(&attr);
    if (TAURARO_THREAD_STACK_SIZE > 0)
        pthread_attr_setstacksize(&attr, (size_t)TAURARO_THREAD_STACK_SIZE);
    pthread_attr_setguardsize(&attr, 4096);  /* one-page guard against overflow */
    pthread_t t; pthread_create(&t, &attr, _tr_posix_thread_trampoline, s);
    pthread_attr_destroy(&attr); return t;
}
_TR_XLINK void _tr_thread_detach(_TrThread t) { pthread_detach(t); }
_TR_XLINK void _tr_thread_join_wait(_TrThread t) { pthread_join(t, NULL); }

typedef pthread_mutex_t _TrMutex;
static void _tr_mutex_init(_TrMutex* m)   { pthread_mutex_init(m, NULL); }
static void _tr_mutex_lock(_TrMutex* m)   { pthread_mutex_lock(m); }
static void _tr_mutex_unlock(_TrMutex* m) { pthread_mutex_unlock(m); }

typedef struct { pthread_mutex_t mu; pthread_cond_t cv; } _TrCondMutex;
static void _tr_condmutex_init(_TrCondMutex* cm)    { pthread_mutex_init(&cm->mu, NULL); pthread_cond_init(&cm->cv, NULL); }
static void _tr_condmutex_lock(_TrCondMutex* cm)    { pthread_mutex_lock(&cm->mu); }
static void _tr_condmutex_unlock(_TrCondMutex* cm)  { pthread_mutex_unlock(&cm->mu); }
static void _tr_condmutex_wait(_TrCondMutex* cm)    { pthread_cond_wait(&cm->cv, &cm->mu); }
static void _tr_condmutex_signal(_TrCondMutex* cm)  { pthread_cond_signal(&cm->cv); }
_TR_XLINK void _tr_sleep_ms(long ms) {
    struct timespec ts = {ms/1000, (ms%1000)*1000000LL}; nanosleep(&ts, NULL);
}
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * Production async primitives — Win32 and POSIX implementations.
 * Design invariants (100% deadlock / race free):
 *   • Single lock per primitive — never acquire two locks simultaneously.
 *   • All condvar waits use while-loops — handles spurious wakeups.
 *   • Broadcast (not signal) on close/cancel — unblocks all waiters.
 *   • All heap-allocated; Tauraro holds opaque char* (void*) handles.
 * ═══════════════════════════════════════════════════════════════════════════*/
#include <limits.h>

#ifdef _WIN32

/* ── Bounded MPMC channel ────────────────────────────────────────────── */
typedef struct {
    long long* buf; long long head, tail, count, cap; volatile int closed;
    CRITICAL_SECTION mu; CONDITION_VARIABLE not_empty, not_full;
} _TrChan;
_TR_XLINK _TrChan* _tr_chan_new(long long cap) {
    if (cap < 1) cap = 1;
    _TrChan* c = (_TrChan*)calloc(1, sizeof(_TrChan));
    c->buf = (long long*)TAURARO_CALLOC((size_t)cap, sizeof(long long)); c->cap = cap;
    InitializeCriticalSection(&c->mu);
    InitializeConditionVariable(&c->not_empty);
    InitializeConditionVariable(&c->not_full);
    return c;
}
_TR_XLINK void _tr_chan_send(_TrChan* c, long long val) {
    EnterCriticalSection(&c->mu);
    while (c->count >= c->cap && !c->closed)
        SleepConditionVariableCS(&c->not_full, &c->mu, INFINITE);
    if (!c->closed) {
        c->buf[c->tail] = val; c->tail = (c->tail+1)%c->cap; c->count++;
        WakeConditionVariable(&c->not_empty);
    }
    LeaveCriticalSection(&c->mu);
}
_TR_XLINK long long _tr_chan_recv(_TrChan* c) {
    EnterCriticalSection(&c->mu);
    while (c->count == 0 && !c->closed)
        SleepConditionVariableCS(&c->not_empty, &c->mu, INFINITE);
    long long v = 0;
    if (c->count > 0) {
        v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        WakeConditionVariable(&c->not_full);
    }
    LeaveCriticalSection(&c->mu); return v;
}
_TR_XLINK bool _tr_chan_try_send(_TrChan* c, long long val) {
    EnterCriticalSection(&c->mu);
    bool ok = !c->closed && c->count < c->cap;
    if (ok) { c->buf[c->tail]=val; c->tail=(c->tail+1)%c->cap; c->count++; WakeConditionVariable(&c->not_empty); }
    LeaveCriticalSection(&c->mu); return ok;
}
_TR_XLINK long long _tr_chan_try_recv_val(_TrChan* c) {
    EnterCriticalSection(&c->mu);
    long long v = LLONG_MIN;
    if (c->count > 0) { v=c->buf[c->head]; c->head=(c->head+1)%c->cap; c->count--; WakeConditionVariable(&c->not_full); }
    LeaveCriticalSection(&c->mu); return v;
}
_TR_XLINK bool _tr_chan_send_timeout(_TrChan* c, long long val, long long ms) {
    EnterCriticalSection(&c->mu);
    ULONGLONG dl = GetTickCount64()+(ULONGLONG)ms; bool ok=true;
    while (c->count>=c->cap && !c->closed) {
        ULONGLONG now=GetTickCount64();
        if (now>=dl||!SleepConditionVariableCS(&c->not_full,&c->mu,(DWORD)(dl-now))){ok=false;break;}
    }
    if (ok&&!c->closed&&c->count<c->cap){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;WakeConditionVariable(&c->not_empty);}else ok=false;
    LeaveCriticalSection(&c->mu); return ok;
}
_TR_XLINK long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms) {
    EnterCriticalSection(&c->mu);
    ULONGLONG dl=GetTickCount64()+(ULONGLONG)ms;
    while (c->count==0&&!c->closed){
        ULONGLONG now=GetTickCount64();
        if(now>=dl||!SleepConditionVariableCS(&c->not_empty,&c->mu,(DWORD)(dl-now))){LeaveCriticalSection(&c->mu);return LLONG_MIN;}
    }
    long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;WakeConditionVariable(&c->not_full);}
    LeaveCriticalSection(&c->mu); return v;
}
_TR_XLINK void _tr_chan_close(_TrChan* c) {
    EnterCriticalSection(&c->mu); c->closed=1;
    WakeAllConditionVariable(&c->not_empty); WakeAllConditionVariable(&c->not_full);
    LeaveCriticalSection(&c->mu);
}
_TR_XLINK bool _tr_chan_is_closed(_TrChan* c) { EnterCriticalSection(&c->mu); bool r=c->closed!=0; LeaveCriticalSection(&c->mu); return r; }
_TR_XLINK long long _tr_chan_len(_TrChan* c)    { EnterCriticalSection(&c->mu); long long n=c->count; LeaveCriticalSection(&c->mu); return n; }
static long long _tr_chan_cap(_TrChan* c)    { return c?c->cap:0; }
static void   _tr_chan_free(_TrChan* c)      { if(!c)return; DeleteCriticalSection(&c->mu); _tr_free(c->buf); _tr_free(c); }
_TR_XLINK long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
    EnterCriticalSection(&c->mu);
    while (c->count == 0 && !c->closed)
        SleepConditionVariableCS(&c->not_empty, &c->mu, INFINITE);
    long long v = 0; *ok = 0;
    if (c->count > 0) {
        v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        WakeConditionVariable(&c->not_full); *ok = 1;
    }
    LeaveCriticalSection(&c->mu); return v;
}

/* ── Blocking task completion state ─────────────────────────────────── */
/* refcount=2: one for caller (_tr_task_free), one for worker (_tr_task_complete).
 * This prevents use-after-free when await_timeout abandons a still-running task. */
typedef struct {
    volatile long long result; char* error;
    volatile int done, cancelled, refcount;
    CRITICAL_SECTION mu; CONDITION_VARIABLE cv;
} _TrTaskState;
static _TrTaskState* _tr_task_new(void) {
    _TrTaskState* t=(_TrTaskState*)calloc(1,sizeof(_TrTaskState));
    InitializeCriticalSection(&t->mu); InitializeConditionVariable(&t->cv); t->error=""; t->refcount=2; return t;
}
static void _tr_task_complete(_TrTaskState* t, long long r) {
    int sf; EnterCriticalSection(&t->mu); if(!t->done){t->result=r;t->done=1;} WakeAllConditionVariable(&t->cv); sf=(--t->refcount<=0); LeaveCriticalSection(&t->mu);
    if(sf){DeleteCriticalSection(&t->mu);free(t);}
}
static void _tr_task_complete_err(_TrTaskState* t, const char* msg) {
    int sf; EnterCriticalSection(&t->mu); if(!t->done){t->error=msg?(char*)msg:"error";t->done=1;} WakeAllConditionVariable(&t->cv); sf=(--t->refcount<=0); LeaveCriticalSection(&t->mu);
    if(sf){DeleteCriticalSection(&t->mu);free(t);}
}
static void _tr_task_cancel(_TrTaskState* t) {
    EnterCriticalSection(&t->mu); if(!t->done){t->cancelled=1;t->done=1;} WakeAllConditionVariable(&t->cv); LeaveCriticalSection(&t->mu);
}
static long long _tr_task_await(_TrTaskState* t) {
    EnterCriticalSection(&t->mu);
    while(!t->done) SleepConditionVariableCS(&t->cv,&t->mu,INFINITE);
    long long r=t->result; LeaveCriticalSection(&t->mu); return r;
}
static bool _tr_task_await_timeout(_TrTaskState* t, long long ms, long long* out) {
    EnterCriticalSection(&t->mu);
    ULONGLONG dl=GetTickCount64()+(ULONGLONG)ms;
    while(!t->done){ULONGLONG now=GetTickCount64();if(now>=dl||!SleepConditionVariableCS(&t->cv,&t->mu,(DWORD)(dl-now))){LeaveCriticalSection(&t->mu);return false;}}
    *out=t->result; LeaveCriticalSection(&t->mu); return true;
}
static bool  _tr_task_is_done(_TrTaskState* t)      { EnterCriticalSection(&t->mu); bool r=t->done!=0;      LeaveCriticalSection(&t->mu); return r; }
static bool  _tr_task_is_cancelled(_TrTaskState* t)  { EnterCriticalSection(&t->mu); bool r=t->cancelled!=0; LeaveCriticalSection(&t->mu); return r; }
static bool  _tr_task_has_error(_TrTaskState* t)     { EnterCriticalSection(&t->mu); bool r=t->error&&t->error[0]; LeaveCriticalSection(&t->mu); return r; }
static char* _tr_task_get_error(_TrTaskState* t)     { EnterCriticalSection(&t->mu); char* e=t->error?t->error:""; LeaveCriticalSection(&t->mu); return e; }
static void  _tr_task_free(_TrTaskState* t) {
    if(!t)return; int sf; EnterCriticalSection(&t->mu); sf=(--t->refcount<=0); LeaveCriticalSection(&t->mu);
    if(sf){DeleteCriticalSection(&t->mu);free(t);}
}

/* ── Heap mutex ──────────────────────────────────────────────────────── */
typedef struct { CRITICAL_SECTION cs; } _TrMutexH;
static _TrMutexH* _tr_mutex_new(void)          { _TrMutexH* m=(_TrMutexH*)malloc(sizeof(_TrMutexH)); InitializeCriticalSection(&m->cs); return m; }
static void _tr_mutex_hlock(_TrMutexH* m)      { EnterCriticalSection(&m->cs); }
static void _tr_mutex_hunlock(_TrMutexH* m)    { LeaveCriticalSection(&m->cs); }
static bool _tr_mutex_htrylock(_TrMutexH* m)   { return TryEnterCriticalSection(&m->cs)!=0; }
static void _tr_mutex_hfree(_TrMutexH* m)      { if(!m)return; DeleteCriticalSection(&m->cs); free(m); }

/* ── Read-write lock (SRWLOCK) ───────────────────────────────────────── */
typedef struct { SRWLOCK l; } _TrRWL;
static _TrRWL* _tr_rwl_new(void)               { _TrRWL* r=(_TrRWL*)malloc(sizeof(_TrRWL)); InitializeSRWLock(&r->l); return r; }
static void _tr_rwl_read_lock(_TrRWL* r)       { AcquireSRWLockShared(&r->l); }
static void _tr_rwl_read_unlock(_TrRWL* r)     { ReleaseSRWLockShared(&r->l); }
static void _tr_rwl_write_lock(_TrRWL* r)      { AcquireSRWLockExclusive(&r->l); }
static void _tr_rwl_write_unlock(_TrRWL* r)    { ReleaseSRWLockExclusive(&r->l); }
static void _tr_rwl_free(_TrRWL* r)            { free(r); }

/* ── Counting semaphore ──────────────────────────────────────────────── */
typedef struct { HANDLE h; } _TrSema;
static _TrSema* _tr_sema_new(long long init, long long maxv) {
    _TrSema* s=(_TrSema*)malloc(sizeof(_TrSema));
    s->h=CreateSemaphoreA(NULL,(LONG)init,(LONG)(maxv>0?maxv:0x7fffffff),NULL); return s;
}
static void _tr_sema_acquire(_TrSema* s)             { WaitForSingleObject(s->h,INFINITE); }
static bool _tr_sema_try_acquire(_TrSema* s)         { return WaitForSingleObject(s->h,0)==WAIT_OBJECT_0; }
static bool _tr_sema_acquire_timeout(_TrSema* s, long long ms) { return WaitForSingleObject(s->h,(DWORD)ms)==WAIT_OBJECT_0; }
static void _tr_sema_release(_TrSema* s)             { ReleaseSemaphore(s->h,1,NULL); }
static void _tr_sema_free(_TrSema* s)                { if(!s)return; CloseHandle(s->h); free(s); }

/* ── WaitGroup ───────────────────────────────────────────────────────── */
typedef struct { volatile long long count; CRITICAL_SECTION mu; CONDITION_VARIABLE cv; } _TrWG;
static _TrWG* _tr_wg_new(void) { _TrWG* w=(_TrWG*)calloc(1,sizeof(_TrWG)); InitializeCriticalSection(&w->mu); InitializeConditionVariable(&w->cv); return w; }
static void _tr_wg_add(_TrWG* w, long long n)  { EnterCriticalSection(&w->mu); w->count+=n; if(w->count<=0)WakeAllConditionVariable(&w->cv); LeaveCriticalSection(&w->mu); }
static void _tr_wg_done(_TrWG* w)              { EnterCriticalSection(&w->mu); w->count--; if(w->count<=0)WakeAllConditionVariable(&w->cv); LeaveCriticalSection(&w->mu); }
static void _tr_wg_wait(_TrWG* w)              { EnterCriticalSection(&w->mu); while(w->count>0)SleepConditionVariableCS(&w->cv,&w->mu,INFINITE); LeaveCriticalSection(&w->mu); }
static bool _tr_wg_wait_timeout(_TrWG* w, long long ms) {
    EnterCriticalSection(&w->mu); ULONGLONG dl=GetTickCount64()+(ULONGLONG)ms; bool ok=true;
    while(w->count>0){ULONGLONG now=GetTickCount64();if(now>=dl||!SleepConditionVariableCS(&w->cv,&w->mu,(DWORD)(dl-now))){ok=false;break;}}
    LeaveCriticalSection(&w->mu); return ok;
}
static void _tr_wg_free(_TrWG* w) { if(!w)return; DeleteCriticalSection(&w->mu); free(w); }

/* ── Cyclic barrier ──────────────────────────────────────────────────── */
typedef struct { long long total,count,gen; CRITICAL_SECTION mu; CONDITION_VARIABLE cv; } _TrBarrier;
static _TrBarrier* _tr_barrier_new(long long n) { _TrBarrier* b=(_TrBarrier*)calloc(1,sizeof(_TrBarrier)); b->total=b->count=n; InitializeCriticalSection(&b->mu); InitializeConditionVariable(&b->cv); return b; }
static void _tr_barrier_wait(_TrBarrier* b) {
    EnterCriticalSection(&b->mu); long long g=b->gen;
    if(--b->count==0){b->gen++;b->count=b->total;WakeAllConditionVariable(&b->cv);}
    else while(b->gen==g) SleepConditionVariableCS(&b->cv,&b->mu,INFINITE);
    LeaveCriticalSection(&b->mu);
}
static void _tr_barrier_free(_TrBarrier* b) { if(!b)return; DeleteCriticalSection(&b->mu); free(b); }

/* ── Run-once guard: lockless atomic CAS — zero kernel object, zero heap mutex */
typedef struct { _Atomic int done; } _TrOnce;
static _TrOnce* _tr_once_new(void) {
    _TrOnce* o = (_TrOnce*)calloc(1, sizeof(_TrOnce));
    atomic_init(&o->done, 0); return o;
}
static bool _tr_once_do(_TrOnce* o) {
    int z = 0;
    return atomic_compare_exchange_strong_explicit(&o->done, &z, 1,
        memory_order_acq_rel, memory_order_relaxed);
}
static void _tr_once_free(_TrOnce* o) { if (o) free(o); }

/* ── Timer / Ticker ──────────────────────────────────────────────────── */
typedef struct { _TrChan* ch; long long ms; int periodic; volatile int stopped; CRITICAL_SECTION stop_mu; } _TrTimerState;
static DWORD WINAPI _tr_timer_thread_fn(LPVOID arg) {
    _TrTimerState* s=(_TrTimerState*)arg;
    do {
        Sleep((DWORD)s->ms);
        EnterCriticalSection(&s->stop_mu); int stopped=s->stopped; LeaveCriticalSection(&s->stop_mu);
        if(stopped) break;
        _tr_chan_try_send(s->ch, 1LL);
    } while(s->periodic);
    return 0;
}
static _TrTimerState* _tr_timer_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms;
    InitializeCriticalSection(&s->stop_mu);
    HANDLE t=CreateThread(NULL,0,_tr_timer_thread_fn,s,0,NULL); CloseHandle(t); return s;
}
static _TrTimerState* _tr_ticker_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms; s->periodic=1;
    InitializeCriticalSection(&s->stop_mu);
    HANDLE t=CreateThread(NULL,0,_tr_timer_thread_fn,s,0,NULL); CloseHandle(t); return s;
}
static void _tr_timer_stop(_TrTimerState* s) {
    if(!s)return; EnterCriticalSection(&s->stop_mu); s->stopped=1; LeaveCriticalSection(&s->stop_mu);
    _tr_chan_close(s->ch);
}

/* ── Thread-local storage (Win32 TLS slots) ──────────────────────────── */
typedef struct { DWORD key; } _TrTLS;
_TR_XLINK _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)malloc(sizeof(_TrTLS));
    t->key = TlsAlloc();
    TlsSetValue(t->key, (LPVOID)(uintptr_t)(unsigned long long)init);
    return t;
}
_TR_XLINK long long _tr_tls_get(_TrTLS* t) {
    return t ? (long long)(uintptr_t)TlsGetValue(t->key) : 0LL;
}
_TR_XLINK void _tr_tls_set(_TrTLS* t, long long v) {
    if (t) TlsSetValue(t->key, (LPVOID)(uintptr_t)(unsigned long long)v);
}
_TR_XLINK void _tr_tls_free(_TrTLS* t) { if (!t) return; TlsFree(t->key); free(t); }

#elif defined(TAURARO_NO_THREADS)
/* ═══════════════════════════════════════════════════════════════════════════
 * BARE/WASM/WASI: single-threaded async stubs — no locking, no blocking.
 * Channels are lock-free ring buffers; mutexes/semaphores are no-ops.
 * ═══════════════════════════════════════════════════════════════════════════*/

typedef struct {
    long long* buf; long long head, tail, count, cap; volatile int closed;
} _TrChan;
_TR_XLINK _TrChan* _tr_chan_new(long long cap) {
    if (cap < 1) cap = 1;
    _TrChan* c = (_TrChan*)TAURARO_CALLOC(1, sizeof(_TrChan));
    c->buf = (long long*)TAURARO_CALLOC((size_t)cap, sizeof(long long)); c->cap = cap;
    return c;
}
_TR_XLINK void _tr_chan_send(_TrChan* c, long long val) {
    if (!c || c->closed || c->count >= c->cap) return;
    c->buf[c->tail] = val; c->tail = (c->tail+1)%c->cap; c->count++;
}
_TR_XLINK long long _tr_chan_recv(_TrChan* c) {
    if (!c || c->count == 0) return 0LL;
    long long v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
    return v;
}
_TR_XLINK bool _tr_chan_try_send(_TrChan* c, long long val) {
    if (!c || c->closed || c->count >= c->cap) return false;
    c->buf[c->tail]=val; c->tail=(c->tail+1)%c->cap; c->count++; return true;
}
_TR_XLINK long long _tr_chan_try_recv_val(_TrChan* c) {
    if (!c || c->count == 0) return LLONG_MIN;
    long long v=c->buf[c->head]; c->head=(c->head+1)%c->cap; c->count--; return v;
}
_TR_XLINK bool _tr_chan_send_timeout(_TrChan* c, long long val, long long ms)  { (void)ms; return _tr_chan_try_send(c, val); }
_TR_XLINK long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms)         { (void)ms; return _tr_chan_try_recv_val(c); }
_TR_XLINK void _tr_chan_close(_TrChan* c)          { if (c) c->closed = 1; }
_TR_XLINK bool _tr_chan_is_closed(_TrChan* c)      { return c && c->closed; }
_TR_XLINK long long _tr_chan_len(_TrChan* c)         { return c ? c->count : 0LL; }
static long long _tr_chan_cap(_TrChan* c)         { return c ? c->cap : 0LL; }
static void  _tr_chan_free(_TrChan* c)            { if (!c) return; TAURARO_FREE(c->buf); TAURARO_FREE(c); }
_TR_XLINK long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
    if (c && c->count > 0) {
        long long v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        *ok = 1; return v;
    }
    *ok = 0; return 0LL;
}

typedef struct { volatile long long result; char* error; volatile int done, cancelled; } _TrTaskState;
static _TrTaskState* _tr_task_new(void) {
    _TrTaskState* t = (_TrTaskState*)TAURARO_CALLOC(1, sizeof(_TrTaskState)); t->error = ""; return t;
}
static void   _tr_task_complete(_TrTaskState* t, long long r)           { if (t&&!t->done){t->result=r;t->done=1;} }
static void   _tr_task_complete_err(_TrTaskState* t, const char* msg)   { if (t&&!t->done){t->error=msg?(char*)msg:"error";t->done=1;} }
static void   _tr_task_cancel(_TrTaskState* t)                           { if (t&&!t->done){t->cancelled=1;t->done=1;} }
static long long _tr_task_await(_TrTaskState* t)                         { return t?t->result:0LL; }
static bool   _tr_task_await_timeout(_TrTaskState* t, long long ms, long long* out) {
    (void)ms; if (t && out) *out = t->result; return t && t->done;
}
static bool   _tr_task_is_done(_TrTaskState* t)      { return t && t->done; }
static bool   _tr_task_is_cancelled(_TrTaskState* t) { return t && t->cancelled; }
static bool   _tr_task_has_error(_TrTaskState* t)    { return t && t->error && t->error[0]; }
static char*  _tr_task_get_error(_TrTaskState* t)    { return t && t->error ? t->error : ""; }
static void   _tr_task_free(_TrTaskState* t)          { if (t) TAURARO_FREE(t); }

typedef struct { int dummy; } _TrMutexH;
static _TrMutexH* _tr_mutex_new(void)             { return (_TrMutexH*)TAURARO_CALLOC(1, sizeof(_TrMutexH)); }
static void _tr_mutex_hlock(_TrMutexH* m)         { (void)m; }
static void _tr_mutex_hunlock(_TrMutexH* m)       { (void)m; }
static bool _tr_mutex_htrylock(_TrMutexH* m)      { (void)m; return true; }
static void _tr_mutex_hfree(_TrMutexH* m)         { if (m) TAURARO_FREE(m); }

typedef struct { int dummy; } _TrRWL;
static _TrRWL* _tr_rwl_new(void)                  { return (_TrRWL*)TAURARO_CALLOC(1, sizeof(_TrRWL)); }
static void _tr_rwl_read_lock(_TrRWL* r)          { (void)r; }
static void _tr_rwl_read_unlock(_TrRWL* r)        { (void)r; }
static void _tr_rwl_write_lock(_TrRWL* r)         { (void)r; }
static void _tr_rwl_write_unlock(_TrRWL* r)       { (void)r; }
static void _tr_rwl_free(_TrRWL* r)               { if (r) TAURARO_FREE(r); }

typedef struct { volatile long long count, maxv; } _TrSema;
static _TrSema* _tr_sema_new(long long init, long long maxv) {
    _TrSema* s = (_TrSema*)TAURARO_CALLOC(1, sizeof(_TrSema));
    s->count = init; s->maxv = maxv > 0 ? maxv : (long long)0x7fffffffffffffffLL; return s;
}
static void _tr_sema_acquire(_TrSema* s)           { if (s && s->count > 0) s->count--; }
static bool _tr_sema_try_acquire(_TrSema* s)       { if (s && s->count > 0) { s->count--; return true; } return false; }
static bool _tr_sema_acquire_timeout(_TrSema* s, long long ms) { (void)ms; return _tr_sema_try_acquire(s); }
static void _tr_sema_release(_TrSema* s)           { if (s && s->count < s->maxv) s->count++; }
static void _tr_sema_free(_TrSema* s)              { if (s) TAURARO_FREE(s); }

typedef struct { volatile long long count; } _TrWG;
static _TrWG* _tr_wg_new(void) { return (_TrWG*)TAURARO_CALLOC(1, sizeof(_TrWG)); }
static void _tr_wg_add(_TrWG* w, long long n)      { if (w) w->count += n; }
static void _tr_wg_done(_TrWG* w)                  { if (w && w->count > 0) w->count--; }
static void _tr_wg_wait(_TrWG* w)                  { (void)w; /* no blocking */ }
static bool _tr_wg_wait_timeout(_TrWG* w, long long ms) { (void)ms; return w ? w->count == 0 : true; }
static void _tr_wg_free(_TrWG* w)                  { if (w) TAURARO_FREE(w); }

typedef struct { long long total, count, gen; } _TrBarrier;
static _TrBarrier* _tr_barrier_new(long long n) {
    _TrBarrier* b = (_TrBarrier*)TAURARO_CALLOC(1, sizeof(_TrBarrier)); b->total = b->count = n; return b;
}
static void _tr_barrier_wait(_TrBarrier* b) {
    if (!b) return;
    if (--b->count == 0) { b->gen++; b->count = b->total; }
}
static void _tr_barrier_free(_TrBarrier* b) { if (b) TAURARO_FREE(b); }

/* _TrOnce: zero-cost run-once guard via atomic CAS — no mutex, no OS object */
typedef struct { _Atomic int done; } _TrOnce;
static _TrOnce* _tr_once_new(void) {
    _TrOnce* o = (_TrOnce*)TAURARO_CALLOC(1, sizeof(_TrOnce));
    atomic_init(&o->done, 0); return o;
}
static bool _tr_once_do(_TrOnce* o) {
    if (!o) return false;
    int z = 0;
    return atomic_compare_exchange_strong_explicit(&o->done, &z, 1,
        memory_order_acq_rel, memory_order_relaxed);
}
static void _tr_once_free(_TrOnce* o) { if (o) TAURARO_FREE(o); }

typedef struct { _TrChan* ch; long long ms; int periodic; volatile int stopped; } _TrTimerState;
static void* _tr_timer_thread_fn(void* arg) { (void)arg; return NULL; }
static _TrTimerState* _tr_timer_new(long long ms, _TrChan* ch) {
    _TrTimerState* s = (_TrTimerState*)TAURARO_CALLOC(1, sizeof(_TrTimerState));
    s->ch = ch; s->ms = ms;
    _tr_chan_try_send(ch, 1LL); /* fire immediately — no background thread */
    return s;
}
static _TrTimerState* _tr_ticker_new(long long ms, _TrChan* ch) {
    _TrTimerState* s = (_TrTimerState*)TAURARO_CALLOC(1, sizeof(_TrTimerState));
    s->ch = ch; s->ms = ms; s->periodic = 1;
    _tr_chan_try_send(ch, 1LL); return s;
}
static void _tr_timer_stop(_TrTimerState* s) {
    if (!s) return; s->stopped = 1;
    if (s->ch) _tr_chan_close(s->ch);
}

/* ── Thread-local storage (bare: single thread, single value) ────────── */
typedef struct { long long val; } _TrTLS;
_TR_XLINK _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)TAURARO_ALLOC(sizeof(_TrTLS)); t->val = init; return t;
}
_TR_XLINK long long _tr_tls_get(_TrTLS* t) { return t ? t->val : 0LL; }
_TR_XLINK void _tr_tls_set(_TrTLS* t, long long v) { if (t) t->val = v; }
_TR_XLINK void _tr_tls_free(_TrTLS* t) { if (t) TAURARO_FREE(t); }

/* ── BARE ThreadPool: runs jobs synchronously (no OS threads) ─────────── */
typedef struct { int _dummy; } _TrThreadPool;
_TR_XLINK long long _tr_threadpool_auto_n(void)  { return 1LL; }
_TR_XLINK _TrThreadPool* _tr_threadpool_new(long long n)  { (void)n; return (_TrThreadPool*)TAURARO_CALLOC(1,sizeof(_TrThreadPool)); }
_TR_XLINK _TrThreadPool* _tr_threadpool_auto(void)        { return _tr_threadpool_new(1LL); }
_TR_XLINK void _tr_threadpool_spawn(_TrThreadPool* p, void*(*fn)(void*), void* arg) { (void)p; fn(arg); }
_TR_XLINK void _tr_threadpool_wait(_TrThreadPool* p)      { (void)p; }
_TR_XLINK void _tr_threadpool_free(_TrThreadPool* p)      { if(p)TAURARO_FREE(p); }

#else /* POSIX ─────────────────────────────────────────────────────────── */

typedef struct {
    long long* buf; long long head,tail,count,cap; volatile int closed;
    pthread_mutex_t mu; pthread_cond_t not_empty, not_full;
} _TrChan;
_TR_XLINK _TrChan* _tr_chan_new(long long cap) {
    if(cap<1)cap=1; _TrChan* c=(_TrChan*)calloc(1,sizeof(_TrChan));
    c->buf=(long long*)TAURARO_CALLOC((size_t)cap,sizeof(long long)); c->cap=cap;
    pthread_mutex_init(&c->mu,NULL); pthread_cond_init(&c->not_empty,NULL); pthread_cond_init(&c->not_full,NULL); return c;
}
_TR_XLINK void _tr_chan_send(_TrChan* c, long long val) {
    pthread_mutex_lock(&c->mu);
    while(c->count>=c->cap&&!c->closed) pthread_cond_wait(&c->not_full,&c->mu);
    if(!c->closed){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}
    pthread_mutex_unlock(&c->mu);
}
_TR_XLINK long long _tr_chan_recv(_TrChan* c) {
    pthread_mutex_lock(&c->mu);
    while(c->count==0&&!c->closed) pthread_cond_wait(&c->not_empty,&c->mu);
    long long v=0;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
_TR_XLINK bool _tr_chan_try_send(_TrChan* c, long long val) {
    pthread_mutex_lock(&c->mu); bool ok=!c->closed&&c->count<c->cap;
    if(ok){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}
    pthread_mutex_unlock(&c->mu); return ok;
}
_TR_XLINK long long _tr_chan_try_recv_val(_TrChan* c) {
    pthread_mutex_lock(&c->mu); long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
_TR_XLINK bool _tr_chan_send_timeout(_TrChan* c, long long val, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&c->mu); bool ok=true;
    while(c->count>=c->cap&&!c->closed) if(pthread_cond_timedwait(&c->not_full,&c->mu,&ts)){ok=false;break;}
    if(ok&&!c->closed&&c->count<c->cap){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}else ok=false;
    pthread_mutex_unlock(&c->mu); return ok;
}
_TR_XLINK long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&c->mu);
    while(c->count==0&&!c->closed) if(pthread_cond_timedwait(&c->not_empty,&c->mu,&ts)){pthread_mutex_unlock(&c->mu);return LLONG_MIN;}
    long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
_TR_XLINK void _tr_chan_close(_TrChan* c) {
    pthread_mutex_lock(&c->mu); c->closed=1;
    pthread_cond_broadcast(&c->not_empty); pthread_cond_broadcast(&c->not_full); pthread_mutex_unlock(&c->mu);
}
_TR_XLINK bool _tr_chan_is_closed(_TrChan* c) { pthread_mutex_lock(&c->mu); bool r=c->closed!=0; pthread_mutex_unlock(&c->mu); return r; }
_TR_XLINK long long _tr_chan_len(_TrChan* c)    { pthread_mutex_lock(&c->mu); long long n=c->count; pthread_mutex_unlock(&c->mu); return n; }
static long long _tr_chan_cap(_TrChan* c)    { return c?c->cap:0; }
static void   _tr_chan_free(_TrChan* c)      { if(!c)return; pthread_mutex_destroy(&c->mu); pthread_cond_destroy(&c->not_empty); pthread_cond_destroy(&c->not_full); _tr_free(c->buf); _tr_free(c); }
_TR_XLINK long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
    pthread_mutex_lock(&c->mu);
    while (c->count == 0 && !c->closed) pthread_cond_wait(&c->not_empty, &c->mu);
    long long v = 0; *ok = 0;
    if (c->count > 0) {
        v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        pthread_cond_signal(&c->not_full); *ok = 1;
    }
    pthread_mutex_unlock(&c->mu); return v;
}

/* refcount=2: one for caller (_tr_task_free), one for worker (_tr_task_complete). */
typedef struct {
    volatile long long result; char* error; volatile int done, cancelled, refcount;
    pthread_mutex_t mu; pthread_cond_t cv;
} _TrTaskState;
static _TrTaskState* _tr_task_new(void) {
    _TrTaskState* t=(_TrTaskState*)calloc(1,sizeof(_TrTaskState));
    pthread_mutex_init(&t->mu,NULL); pthread_cond_init(&t->cv,NULL); t->error=""; t->refcount=2; return t;
}
static void _tr_task_complete(_TrTaskState* t, long long r) {
    int sf; pthread_mutex_lock(&t->mu); if(!t->done){t->result=r;t->done=1;} pthread_cond_broadcast(&t->cv); sf=(--t->refcount<=0); pthread_mutex_unlock(&t->mu);
    if(sf){pthread_mutex_destroy(&t->mu);pthread_cond_destroy(&t->cv);free(t);}
}
static void _tr_task_complete_err(_TrTaskState* t, const char* m) {
    int sf; pthread_mutex_lock(&t->mu); if(!t->done){t->error=m?(char*)m:"error";t->done=1;} pthread_cond_broadcast(&t->cv); sf=(--t->refcount<=0); pthread_mutex_unlock(&t->mu);
    if(sf){pthread_mutex_destroy(&t->mu);pthread_cond_destroy(&t->cv);free(t);}
}
static void _tr_task_cancel(_TrTaskState* t)                      { pthread_mutex_lock(&t->mu); if(!t->done){t->cancelled=1;t->done=1;} pthread_cond_broadcast(&t->cv); pthread_mutex_unlock(&t->mu); }
static long long _tr_task_await(_TrTaskState* t) {
    pthread_mutex_lock(&t->mu); while(!t->done) pthread_cond_wait(&t->cv,&t->mu);
    long long r=t->result; pthread_mutex_unlock(&t->mu); return r;
}
static bool _tr_task_await_timeout(_TrTaskState* t, long long ms, long long* out) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&t->mu);
    while(!t->done) if(pthread_cond_timedwait(&t->cv,&t->mu,&ts)){pthread_mutex_unlock(&t->mu);return false;}
    *out=t->result; pthread_mutex_unlock(&t->mu); return true;
}
static bool  _tr_task_is_done(_TrTaskState* t)      { pthread_mutex_lock(&t->mu); bool r=t->done!=0;      pthread_mutex_unlock(&t->mu); return r; }
static bool  _tr_task_is_cancelled(_TrTaskState* t)  { pthread_mutex_lock(&t->mu); bool r=t->cancelled!=0; pthread_mutex_unlock(&t->mu); return r; }
static bool  _tr_task_has_error(_TrTaskState* t)     { pthread_mutex_lock(&t->mu); bool r=t->error&&t->error[0]; pthread_mutex_unlock(&t->mu); return r; }
static char* _tr_task_get_error(_TrTaskState* t)     { pthread_mutex_lock(&t->mu); char* e=t->error?t->error:""; pthread_mutex_unlock(&t->mu); return e; }
static void  _tr_task_free(_TrTaskState* t) {
    if(!t)return; int sf; pthread_mutex_lock(&t->mu); sf=(--t->refcount<=0); pthread_mutex_unlock(&t->mu);
    if(sf){pthread_mutex_destroy(&t->mu);pthread_cond_destroy(&t->cv);free(t);}
}

typedef struct { pthread_mutex_t mu; } _TrMutexH;
static _TrMutexH* _tr_mutex_new(void)          { _TrMutexH* m=(_TrMutexH*)malloc(sizeof(_TrMutexH)); pthread_mutex_init(&m->mu,NULL); return m; }
static void _tr_mutex_hlock(_TrMutexH* m)      { pthread_mutex_lock(&m->mu); }
static void _tr_mutex_hunlock(_TrMutexH* m)    { pthread_mutex_unlock(&m->mu); }
static bool _tr_mutex_htrylock(_TrMutexH* m)   { return pthread_mutex_trylock(&m->mu)==0; }
static void _tr_mutex_hfree(_TrMutexH* m)      { if(!m)return; pthread_mutex_destroy(&m->mu); free(m); }

typedef struct { pthread_rwlock_t l; } _TrRWL;
static _TrRWL* _tr_rwl_new(void)               { _TrRWL* r=(_TrRWL*)malloc(sizeof(_TrRWL)); pthread_rwlock_init(&r->l,NULL); return r; }
static void _tr_rwl_read_lock(_TrRWL* r)       { pthread_rwlock_rdlock(&r->l); }
static void _tr_rwl_read_unlock(_TrRWL* r)     { pthread_rwlock_unlock(&r->l); }
static void _tr_rwl_write_lock(_TrRWL* r)      { pthread_rwlock_wrlock(&r->l); }
static void _tr_rwl_write_unlock(_TrRWL* r)    { pthread_rwlock_unlock(&r->l); }
static void _tr_rwl_free(_TrRWL* r)            { if(!r)return; pthread_rwlock_destroy(&r->l); free(r); }

typedef struct { volatile long long count, maxv; pthread_mutex_t mu; pthread_cond_t cv; } _TrSema;
static _TrSema* _tr_sema_new(long long init, long long maxv) {
    _TrSema* s=(_TrSema*)calloc(1,sizeof(_TrSema)); s->count=init; s->maxv=maxv>0?maxv:(long long)0x7fffffffffffffffLL;
    pthread_mutex_init(&s->mu,NULL); pthread_cond_init(&s->cv,NULL); return s;
}
static void _tr_sema_acquire(_TrSema* s)       { pthread_mutex_lock(&s->mu); while(s->count<=0)pthread_cond_wait(&s->cv,&s->mu); s->count--; pthread_mutex_unlock(&s->mu); }
static bool _tr_sema_try_acquire(_TrSema* s)   { pthread_mutex_lock(&s->mu); bool ok=s->count>0; if(ok)s->count--; pthread_mutex_unlock(&s->mu); return ok; }
static bool _tr_sema_acquire_timeout(_TrSema* s, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&s->mu);
    while(s->count<=0) if(pthread_cond_timedwait(&s->cv,&s->mu,&ts)){pthread_mutex_unlock(&s->mu);return false;}
    s->count--; pthread_mutex_unlock(&s->mu); return true;
}
static void _tr_sema_release(_TrSema* s)       { pthread_mutex_lock(&s->mu); if(s->count<s->maxv){s->count++;pthread_cond_signal(&s->cv);} pthread_mutex_unlock(&s->mu); }
static void _tr_sema_free(_TrSema* s)          { if(!s)return; pthread_mutex_destroy(&s->mu); pthread_cond_destroy(&s->cv); free(s); }

typedef struct { volatile long long count; pthread_mutex_t mu; pthread_cond_t cv; } _TrWG;
static _TrWG* _tr_wg_new(void) { _TrWG* w=(_TrWG*)calloc(1,sizeof(_TrWG)); pthread_mutex_init(&w->mu,NULL); pthread_cond_init(&w->cv,NULL); return w; }
static void _tr_wg_add(_TrWG* w, long long n)  { pthread_mutex_lock(&w->mu); w->count+=n; if(w->count<=0)pthread_cond_broadcast(&w->cv); pthread_mutex_unlock(&w->mu); }
static void _tr_wg_done(_TrWG* w)              { pthread_mutex_lock(&w->mu); w->count--; if(w->count<=0)pthread_cond_broadcast(&w->cv); pthread_mutex_unlock(&w->mu); }
static void _tr_wg_wait(_TrWG* w)              { pthread_mutex_lock(&w->mu); while(w->count>0)pthread_cond_wait(&w->cv,&w->mu); pthread_mutex_unlock(&w->mu); }
static bool _tr_wg_wait_timeout(_TrWG* w, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&w->mu); bool ok=true;
    while(w->count>0) if(pthread_cond_timedwait(&w->cv,&w->mu,&ts)){ok=false;break;}
    pthread_mutex_unlock(&w->mu); return ok;
}
static void _tr_wg_free(_TrWG* w) { if(!w)return; pthread_mutex_destroy(&w->mu); pthread_cond_destroy(&w->cv); free(w); }

typedef struct { long long total,count,gen; pthread_mutex_t mu; pthread_cond_t cv; } _TrBarrier;
static _TrBarrier* _tr_barrier_new(long long n) { _TrBarrier* b=(_TrBarrier*)calloc(1,sizeof(_TrBarrier)); b->total=b->count=n; pthread_mutex_init(&b->mu,NULL); pthread_cond_init(&b->cv,NULL); return b; }
static void _tr_barrier_wait(_TrBarrier* b) {
    pthread_mutex_lock(&b->mu); long long g=b->gen;
    if(--b->count==0){b->gen++;b->count=b->total;pthread_cond_broadcast(&b->cv);}
    else while(b->gen==g) pthread_cond_wait(&b->cv,&b->mu);
    pthread_mutex_unlock(&b->mu);
}
static void _tr_barrier_free(_TrBarrier* b) { if(!b)return; pthread_mutex_destroy(&b->mu); pthread_cond_destroy(&b->cv); free(b); }

/* _TrOnce: lockless atomic CAS — no pthread_mutex, no heap lock object */
typedef struct { _Atomic int done; } _TrOnce;
static _TrOnce* _tr_once_new(void) {
    _TrOnce* o = (_TrOnce*)calloc(1, sizeof(_TrOnce));
    atomic_init(&o->done, 0); return o;
}
static bool _tr_once_do(_TrOnce* o) {
    int z = 0;
    return atomic_compare_exchange_strong_explicit(&o->done, &z, 1,
        memory_order_acq_rel, memory_order_relaxed);
}
static void _tr_once_free(_TrOnce* o) { if (o) free(o); }

typedef struct { _TrChan* ch; long long ms; int periodic; volatile int stopped; pthread_mutex_t stop_mu; } _TrTimerState;
static void* _tr_timer_thread_fn(void* arg) {
    _TrTimerState* s=(_TrTimerState*)arg;
    do {
        struct timespec ts={s->ms/1000,(s->ms%1000)*1000000LL}; nanosleep(&ts,NULL);
        pthread_mutex_lock(&s->stop_mu); int stopped=s->stopped; pthread_mutex_unlock(&s->stop_mu);
        if(stopped) break;
        _tr_chan_try_send(s->ch,1LL);
    } while(s->periodic);
    return NULL;
}
static _TrTimerState* _tr_timer_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms;
    pthread_mutex_init(&s->stop_mu,NULL);
    pthread_t t; pthread_create(&t,NULL,_tr_timer_thread_fn,s); pthread_detach(t); return s;
}
static _TrTimerState* _tr_ticker_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms; s->periodic=1;
    pthread_mutex_init(&s->stop_mu,NULL);
    pthread_t t; pthread_create(&t,NULL,_tr_timer_thread_fn,s); pthread_detach(t); return s;
}
static void _tr_timer_stop(_TrTimerState* s) {
    if(!s)return; pthread_mutex_lock(&s->stop_mu); s->stopped=1; pthread_mutex_unlock(&s->stop_mu);
    _tr_chan_close(s->ch);
}

/* ── Thread-local storage (POSIX pthread_key_t) ──────────────────────── */
typedef struct { pthread_key_t key; } _TrTLS;
_TR_XLINK _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)malloc(sizeof(_TrTLS));
    pthread_key_create(&t->key, NULL);
    pthread_setspecific(t->key, (void*)(uintptr_t)(unsigned long long)init);
    return t;
}
_TR_XLINK long long _tr_tls_get(_TrTLS* t) {
    return t ? (long long)(uintptr_t)pthread_getspecific(t->key) : 0LL;
}
_TR_XLINK void _tr_tls_set(_TrTLS* t, long long v) {
    if (t) pthread_setspecific(t->key, (void*)(uintptr_t)(unsigned long long)v);
}
_TR_XLINK void _tr_tls_free(_TrTLS* t) {
    if (!t) return; pthread_key_delete(t->key); free(t);
}

#endif /* POSIX async primitives */

/* ── Lock-ownership tracking (thread-local) ──────────────────────────── */
/* Each thread maintains small stacks of box pointers it currently holds.
 * lock_get pushes; set_unlock/unlock pop; the RAII cleanup fires only when
 * it can pop — i.e. only when THIS thread actually holds the lock.
 * All stack reads/writes are thread-local: no sharing, no data race. */
#ifndef _TR_LOCK_DEPTH
#define _TR_LOCK_DEPTH 8
#endif
typedef struct { void* s[_TR_LOCK_DEPTH]; int n; } _TrLockStack;
static _Thread_local _TrLockStack _tr_tl_mu_stk;
static _Thread_local _TrLockStack _tr_tl_rwl_r_stk;
static _Thread_local _TrLockStack _tr_tl_rwl_w_stk;
static inline void  _tr_lstack_push(_TrLockStack* ls, void* b) {
    if (ls->n < _TR_LOCK_DEPTH) ls->s[ls->n++] = b;
}
static inline int   _tr_lstack_pop(_TrLockStack* ls, void* b) {
    for (int i = ls->n - 1; i >= 0; i--)
        if (ls->s[i] == b) { ls->s[i] = ls->s[--ls->n]; return 1; }
    return 0;
}

/* ── MutexBox<T>: mutex-guarded single value ─────────────────────────── */
/* `pdrop` releases the guarded payload when the box owns it (created from a
 * FRESH, unaliased T — see the codegen's _mutex_owns_payload). It is NULL when
 * the payload is a borrow (e.g. `Mutex[App].init(self)`), so a borrowed payload
 * is never double-freed. Guarded by rc so a Shared[Mutex]/clone only frees at 0. */
/* pdrop: heap-CLASS payload field-drop (used via _tr_obj_release, F-3).
 * cdrop: COLLECTION payload direct-destroy (calls the collection's free, F-4) —
 *        a collection is not an rc-object, so it must NOT go through _tr_obj_release. */
typedef struct { _TrMutexH* mu; long long data; void (*pdrop)(void*); void (*cdrop)(void*); _Atomic int rc; } _TrMutexBox;
_TR_XLINK _TrMutexBox* _tr_mutexbox_new(long long init) {
    _TrMutexBox* b = (_TrMutexBox*)TAURARO_ALLOC(sizeof(_TrMutexBox));
    b->mu = _tr_mutex_new(); b->data = init; b->pdrop = 0; b->cdrop = 0;
    atomic_store(&b->rc, 1); return b;
}
/* Owning constructor: the box takes ownership of the payload and releases it via
 * `pdrop` when the box is freed (rc reaches 0). */
_TR_XLINK _TrMutexBox* _tr_mutexbox_new_owned(long long init, void (*pdrop)(void*)) {
    _TrMutexBox* b = _tr_mutexbox_new(init); b->pdrop = pdrop; return b;
}
/* F-4: the box owns a FRESH collection payload; `cdrop` fully frees it (and its
 * owned elements) at rc==0. Codegen emits a per-payload `_mtxcd_*` wrapper. */
_TR_XLINK _TrMutexBox* _tr_mutexbox_new_owned_coll(long long init, void (*cdrop)(void*)) {
    _TrMutexBox* b = _tr_mutexbox_new(init); b->cdrop = cdrop; return b;
}
_TR_XLINK long long _tr_mutexbox_lock_get(_TrMutexBox* b) {
    _tr_mutex_hlock(b->mu); _tr_lstack_push(&_tr_tl_mu_stk, b); return b->data;
}
_TR_XLINK void _tr_mutexbox_set_unlock(_TrMutexBox* b, long long v) {
    b->data = v; _tr_lstack_pop(&_tr_tl_mu_stk, b); _tr_mutex_hunlock(b->mu);
}
_TR_XLINK void _tr_mutexbox_unlock(_TrMutexBox* b) { _tr_lstack_pop(&_tr_tl_mu_stk, b); _tr_mutex_hunlock(b->mu); }
_TR_XLINK void _tr_mutexbox_free(_TrMutexBox* b) {
    if (!b) return;
    /* Release the lock if still held (e.g. a `.get()` guard that relied on scope
     * auto-unlock): pop the box off the TLS guard stack so the guard's cleanup
     * becomes a no-op, and don't free a still-locked CRITICAL_SECTION. */
    if (_tr_lstack_pop(&_tr_tl_mu_stk, b)) _tr_mutex_hunlock(b->mu);
    /* Owned heap-class payload: release one strong ref (rc-- -> free + field-drop
     * at 0). pdrop is the class's _trdrop_<T> field-releaser; non-NULL iff the box
     * owns the payload (created via _tr_mutexbox_new_owned from a FRESH T). */
    if (b->cdrop && b->data)      b->cdrop((void*)(intptr_t)b->data);              /* F-4: free the owned collection */
    else if (b->pdrop && b->data) _tr_obj_release((void*)(intptr_t)b->data, b->pdrop); /* F-3: release the owned heap-class */
    _tr_mutex_hfree(b->mu); TAURARO_FREE(b);
}
_TR_XLINK _TrMutexBox* _tr_mutexbox_clone(_TrMutexBox* b) {
    if (b) atomic_fetch_add(&b->rc, 1); return b;
}
_TR_XLINK void _tr_mutexbox_drop(_TrMutexBox* b) {
    if (!b || atomic_fetch_sub(&b->rc, 1) > 1) return; _tr_mutexbox_free(b);
}
/* Auto-unlock cleanup — used by __attribute__((cleanup)) RAII guard in codegen.
 * Fires when the guard variable goes out of scope. No-op if already unlocked
 * (set_unlock/unlock already popped the box from the TLS stack). */
_TR_XLINK void _tr_mutexbox_cleanup(_TrMutexBox** bp) {
    if (bp && *bp && _tr_lstack_pop(&_tr_tl_mu_stk, *bp)) _tr_mutex_hunlock((*bp)->mu);
}

/* ── RwLockBox<T>: reader-writer guarded single value ────────────────── */
typedef struct { _TrRWL* rw; long long data; _Atomic int rc; } _TrRWLBox;
_TR_XLINK _TrRWLBox* _tr_rwlbox_new(long long init) {
    _TrRWLBox* b = (_TrRWLBox*)TAURARO_ALLOC(sizeof(_TrRWLBox));
    b->rw = _tr_rwl_new(); b->data = init;
    atomic_store(&b->rc, 1); return b;
}
_TR_XLINK long long _tr_rwlbox_read_get(_TrRWLBox* b) {
    _tr_rwl_read_lock(b->rw); _tr_lstack_push(&_tr_tl_rwl_r_stk, b); return b->data;
}
_TR_XLINK void _tr_rwlbox_read_unlock(_TrRWLBox* b) { _tr_lstack_pop(&_tr_tl_rwl_r_stk, b); _tr_rwl_read_unlock(b->rw); }
_TR_XLINK long long _tr_rwlbox_write_get(_TrRWLBox* b) {
    _tr_rwl_write_lock(b->rw); _tr_lstack_push(&_tr_tl_rwl_w_stk, b); return b->data;
}
_TR_XLINK void _tr_rwlbox_write_set_unlock(_TrRWLBox* b, long long v) {
    b->data = v; _tr_lstack_pop(&_tr_tl_rwl_w_stk, b); _tr_rwl_write_unlock(b->rw);
}
_TR_XLINK void _tr_rwlbox_free(_TrRWLBox* b) {
    if (!b) return; _tr_rwl_free(b->rw); TAURARO_FREE(b);
}
_TR_XLINK _TrRWLBox* _tr_rwlbox_clone(_TrRWLBox* b) {
    if (b) atomic_fetch_add(&b->rc, 1); return b;
}
_TR_XLINK void _tr_rwlbox_drop(_TrRWLBox* b) {
    if (!b || atomic_fetch_sub(&b->rc, 1) > 1) return; _tr_rwlbox_free(b);
}
/* Auto-unlock cleanup for read/write guards. */
_TR_XLINK void _tr_rwlbox_cleanup_r(_TrRWLBox** bp) {
    if (bp && *bp && _tr_lstack_pop(&_tr_tl_rwl_r_stk, *bp)) _tr_rwl_read_unlock((*bp)->rw);
}
_TR_XLINK void _tr_rwlbox_cleanup_w(_TrRWLBox** bp) {
    if (bp && *bp && _tr_lstack_pop(&_tr_tl_rwl_w_stk, *bp)) _tr_rwl_write_unlock((*bp)->rw);
}

/* ── ThreadPool: fixed-N worker pool with a channel work queue ────────── */
/* Single-threaded (bare/wasi) stub is defined inside the NO_THREADS block above. */
#ifndef TAURARO_NO_THREADS
typedef struct { void*(*fn)(void*); void* arg; } _TrPoolItem;
typedef struct {
    _TrChan* queue; _TrThread* workers; int n_workers;
    _TrWG* wg; volatile int shutdown;
} _TrThreadPool;
static void* _tr_pool_worker(void* arg) {
    _TrThreadPool* pool = (_TrThreadPool*)arg;
    for (;;) {
        int ok = 0;
        long long item_val = _tr_chan_recv_ok(pool->queue, &ok);
        if (!ok) break;
        /* uintptr_t cast is safe on both 32-bit and 64-bit platforms */
        _TrPoolItem* item = (_TrPoolItem*)(uintptr_t)(unsigned long long)item_val;
        item->fn(item->arg);
        TAURARO_FREE(item);
        _tr_wg_done(pool->wg);
    }
    return NULL;
}
_TR_XLINK long long _tr_threadpool_auto_n(void) {
#ifdef _WIN32
    SYSTEM_INFO si; GetSystemInfo(&si); return (long long)si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
    /* _SC_NPROCESSORS_ONLN may not exist on all POSIX systems (Haiku, QNX, old BSDs) */
    long n = sysconf(_SC_NPROCESSORS_ONLN); return n > 0 ? (long long)n : 1LL;
#elif defined(HW_NCPU) /* BSD/macOS fallback via sysctl */
    int mib[2] = {CTL_HW, HW_NCPU}; int ncpu = 1; size_t len = sizeof(ncpu);
    sysctl(mib, 2, &ncpu, &len, NULL, 0); return (long long)(ncpu > 0 ? ncpu : 1);
#else
    return 1LL;
#endif
}
_TR_XLINK _TrThreadPool* _tr_threadpool_new(long long n) {
    if (n < 1) n = 1;
    _TrThreadPool* p = (_TrThreadPool*)TAURARO_CALLOC(1, sizeof(_TrThreadPool));
    p->n_workers = (int)n;
    p->workers = (_TrThread*)TAURARO_ALLOC((size_t)n * sizeof(_TrThread));
    p->queue = _tr_chan_new(n * 4 + 16);
    p->wg = _tr_wg_new();
    for (int i = 0; i < (int)n; i++)
        p->workers[i] = _tr_thread_start(_tr_pool_worker, p);
    return p;
}
_TR_XLINK _TrThreadPool* _tr_threadpool_auto(void) {
    return _tr_threadpool_new(_tr_threadpool_auto_n());
}
_TR_XLINK void _tr_threadpool_spawn(_TrThreadPool* p, void*(*fn)(void*), void* arg) {
    _TrPoolItem* item = (_TrPoolItem*)TAURARO_ALLOC(sizeof(_TrPoolItem));
    item->fn = fn; item->arg = arg;
    _tr_wg_add(p->wg, 1);
    /* uintptr_t cast: safe on 32-bit and 64-bit; avoids sign-extension of intptr_t */
    _tr_chan_send(p->queue, (long long)(uintptr_t)(void*)item);
}
_TR_XLINK void _tr_threadpool_wait(_TrThreadPool* p) { _tr_wg_wait(p->wg); }
_TR_XLINK void _tr_threadpool_free(_TrThreadPool* p) {
    if (!p) return;
    _tr_chan_close(p->queue);
    for (int i = 0; i < p->n_workers; i++) _tr_thread_join_wait(p->workers[i]);
    _tr_chan_free(p->queue); _tr_wg_free(p->wg);
    TAURARO_FREE(p->workers); TAURARO_FREE(p);
}
#endif /* !TAURARO_NO_THREADS */

/* Global async pool — submits work to the thread pool; falls back to sync if pool is NULL */
static inline void _tr_async_pool_submit(_TrThreadPool* p, void*(*fn)(void*), void* arg) {
    if (p) _tr_threadpool_spawn(p, fn, arg);
    else fn(arg); /* synchronous fallback (BARE / pre-init) */
}

/* ── Atomic[T]: lock-free atomic integer (C11 _Atomic) ───────────────── */
typedef struct { _Atomic long long val; } _TrAtomic;
_TR_XLINK _TrAtomic* _tr_atomic_new(long long init) {
    _TrAtomic* a = (_TrAtomic*)TAURARO_ALLOC(sizeof(_TrAtomic));
    atomic_init(&a->val, init); return a;
}
/* Hot-path ops: null-check removed — codegen never emits NULL _TrAtomic* */
_TR_XLINK long long _tr_atomic_load(_TrAtomic* a)               { return atomic_load(&a->val); }
_TR_XLINK void _tr_atomic_store(_TrAtomic* a, long long v)  { atomic_store(&a->val, v); }
_TR_XLINK long long _tr_atomic_add(_TrAtomic* a, long long v)    { return atomic_fetch_add(&a->val, v); }
_TR_XLINK long long _tr_atomic_sub(_TrAtomic* a, long long v)    { return atomic_fetch_sub(&a->val, v); }
_TR_XLINK long long _tr_atomic_swap(_TrAtomic* a, long long v)   { return atomic_exchange(&a->val, v); }
_TR_XLINK bool _tr_atomic_cas(_TrAtomic* a, long long expected, long long desired) {
    return atomic_compare_exchange_strong(&a->val, &expected, desired);
}
_TR_XLINK void _tr_atomic_free(_TrAtomic* a) { if (a) TAURARO_FREE(a); }

/* Atomic[T]: explicit memory-order variants (C11 stdatomic) */
_TR_XLINK long long _tr_atomic_load_relaxed(_TrAtomic* a) { return atomic_load_explicit(&a->val, memory_order_relaxed); }
_TR_XLINK long long _tr_atomic_load_acquire(_TrAtomic* a) { return atomic_load_explicit(&a->val, memory_order_acquire); }
_TR_XLINK long long _tr_atomic_load_seqcst(_TrAtomic* a)  { return atomic_load_explicit(&a->val, memory_order_seq_cst); }
_TR_XLINK void _tr_atomic_store_relaxed(_TrAtomic* a, long long v) { atomic_store_explicit(&a->val, v, memory_order_relaxed); }
_TR_XLINK void _tr_atomic_store_release(_TrAtomic* a, long long v) { atomic_store_explicit(&a->val, v, memory_order_release); }
_TR_XLINK void _tr_atomic_store_seqcst(_TrAtomic* a, long long v)  { atomic_store_explicit(&a->val, v, memory_order_seq_cst); }
_TR_XLINK long long _tr_atomic_add_relaxed(_TrAtomic* a, long long v) { return atomic_fetch_add_explicit(&a->val, v, memory_order_relaxed); }
_TR_XLINK long long _tr_atomic_add_release(_TrAtomic* a, long long v) { return atomic_fetch_add_explicit(&a->val, v, memory_order_release); }
_TR_XLINK long long _tr_atomic_add_acqrel(_TrAtomic* a, long long v)  { return atomic_fetch_add_explicit(&a->val, v, memory_order_acq_rel); }
_TR_XLINK long long _tr_atomic_sub_relaxed(_TrAtomic* a, long long v) { return atomic_fetch_sub_explicit(&a->val, v, memory_order_relaxed); }
_TR_XLINK long long _tr_atomic_sub_release(_TrAtomic* a, long long v) { return atomic_fetch_sub_explicit(&a->val, v, memory_order_release); }
_TR_XLINK bool _tr_atomic_cas_weak(_TrAtomic* a, long long exp, long long des)   { return atomic_compare_exchange_weak(&a->val, &exp, des); }
_TR_XLINK bool _tr_atomic_cas_acqrel(_TrAtomic* a, long long exp, long long des) {
    return atomic_compare_exchange_strong_explicit(&a->val, &exp, des, memory_order_acq_rel, memory_order_relaxed);
}

/* ── Thread object: joinable OS-thread handle with panic recovery ──── */
typedef struct {
    _TrThread     handle;
    volatile int  done;
    _TrSpawnResult result; /* filled by trampoline on thread exit */
} _TrThreadObj;

/* Internal: thread_start variant that wires result into the trampoline. */
#ifdef _WIN32
static inline _TrThread _tr_thread_start_result(void*(*fn)(void*), void* arg, _TrSpawnResult* res) {
    _TrWin32StartArg* s = (_TrWin32StartArg*)malloc(sizeof(_TrWin32StartArg));
    s->fn = fn; s->arg = arg; s->result = res;
    SIZE_T ss = (SIZE_T)TAURARO_THREAD_STACK_SIZE;
    return CreateThread(NULL, ss, _tr_thread_start_trampoline, s, 0, NULL);
}
#elif !defined(TAURARO_NO_THREADS)
static inline _TrThread _tr_thread_start_result(void*(*fn)(void*), void* arg, _TrSpawnResult* res) {
    _TrPosixStartArg* s = (_TrPosixStartArg*)malloc(sizeof(_TrPosixStartArg));
    s->fn = fn; s->arg = arg; s->result = res;
    pthread_attr_t attr; pthread_attr_init(&attr);
    if (TAURARO_THREAD_STACK_SIZE > 0)
        pthread_attr_setstacksize(&attr, (size_t)TAURARO_THREAD_STACK_SIZE);
    pthread_attr_setguardsize(&attr, 4096);
    pthread_t t; pthread_create(&t, &attr, _tr_posix_thread_trampoline, s);
    pthread_attr_destroy(&attr); return t;
}
#else
static inline _TrThread _tr_thread_start_result(void*(*fn)(void*), void* arg, _TrSpawnResult* res) {
    (void)res; return _tr_thread_start(fn, arg);
}
#endif

_TR_XLINK _TrThreadObj* _tr_threadobj_spawn(void*(*fn)(void*), void* arg) {
    _TrThreadObj* t = (_TrThreadObj*)TAURARO_CALLOC(1, sizeof(_TrThreadObj));
    t->result.panicked = 0; t->result.panic_msg = NULL;
    t->handle = _tr_thread_start_result(fn, arg, &t->result);
    return t;
}
_TR_XLINK void _tr_threadobj_join(_TrThreadObj* t) {
    if (!t || t->done) return; t->done = 1; _tr_thread_join_wait(t->handle);
}
/* Re-raise the thread's panic in the calling thread after join */
_TR_XLINK bool _tr_threadobj_panicked(_TrThreadObj* t) {
    return t && t->result.panicked;
}
_TR_XLINK char* _tr_threadobj_panic_msg(_TrThreadObj* t) {
    return (t && t->result.panic_msg) ? t->result.panic_msg : "";
}
_TR_XLINK void _tr_threadobj_detach(_TrThreadObj* t) {
    if (!t || t->done) return; t->done = 1; _tr_thread_detach(t->handle);
}
_TR_XLINK void _tr_threadobj_free(_TrThreadObj* t) { if (t) TAURARO_FREE(t); }

/* ── Thread utilities: current-thread ID and sleep ───────────────────── */
#ifdef _WIN32
_TR_XLINK long long _tr_thread_current_id(void) { return (long long)(uintptr_t)GetCurrentThreadId(); }
#elif defined(TAURARO_NO_THREADS)
_TR_XLINK long long _tr_thread_current_id(void) { return 0LL; }
#else
_TR_XLINK long long _tr_thread_current_id(void) { return (long long)(uintptr_t)pthread_self(); }
#endif
static inline void _tr_thread_sleep_ms(long long ms) { _tr_sleep_ms((long)(ms < 0 ? 0 : ms)); }

/* Monotonic millisecond clock — used by chan_select timeout */
static inline long long _tr_monotonic_ms(void) {
#if defined(_WIN32)
    return (long long)GetTickCount64();
#elif defined(TAURARO_BARE)
    return 0LL;
#else
    struct timespec _ts; clock_gettime(CLOCK_MONOTONIC, &_ts);
    return (long long)_ts.tv_sec * 1000LL + (long long)(_ts.tv_nsec / 1000000LL);
#endif
}

/* ── Platform-independent helpers ────────────────────────────────────── */
static bool _tr_task_await_timeout_ok(_TrTaskState* t, long long ms) {
    long long dummy=0; return _tr_task_await_timeout(t, ms, &dummy);
}

/* ── char* handle wrappers (used by Tauraro extern "C" declarations) ─── *
 * All struct* are cast to/from char* so Tauraro's Pointer[char] type      *
 * matches the C extern prototype without GCC type-mismatch warnings.       */

/* Channel */
_TR_XLINK char* _tr_chan_new_h(long long cap)                              { return (char*)_tr_chan_new(cap); }
_TR_XLINK void  _tr_chan_send_h(char* c, long long v)                      { _tr_chan_send((_TrChan*)c, v); }
_TR_XLINK long long _tr_chan_recv_h(char* c)                               { return _tr_chan_recv((_TrChan*)c); }
_TR_XLINK bool  _tr_chan_try_send_h(char* c, long long v)                  { return _tr_chan_try_send((_TrChan*)c, v); }
_TR_XLINK long long _tr_chan_try_recv_val_h(char* c)                       { return _tr_chan_try_recv_val((_TrChan*)c); }
_TR_XLINK bool  _tr_chan_send_timeout_h(char* c, long long v, long long ms){ return _tr_chan_send_timeout((_TrChan*)c, v, ms); }
_TR_XLINK long long _tr_chan_recv_timeout_val_h(char* c, long long ms)     { return _tr_chan_recv_timeout_val((_TrChan*)c, ms); }
_TR_XLINK void  _tr_chan_close_h(char* c)                                  { _tr_chan_close((_TrChan*)c); }
_TR_XLINK bool  _tr_chan_is_closed_h(char* c)                              { return _tr_chan_is_closed((_TrChan*)c); }
_TR_XLINK long long _tr_chan_len_h(char* c)                                { return _tr_chan_len((_TrChan*)c); }
_TR_XLINK long long _tr_chan_cap_h(char* c)                                { return _tr_chan_cap((_TrChan*)c); }
_TR_XLINK void  _tr_chan_free_h(char* c)                                   { _tr_chan_free((_TrChan*)c); }

/* Task / Future */
_TR_XLINK char* _tr_task_new_h(void)                                       { return (char*)_tr_task_new(); }
_TR_XLINK void  _tr_task_complete_h(char* t, long long r)                  { _tr_task_complete((_TrTaskState*)t, r); }
_TR_XLINK void  _tr_task_complete_err_h(char* t, char* msg)                { _tr_task_complete_err((_TrTaskState*)t, msg); }
_TR_XLINK void  _tr_task_cancel_h(char* t)                                 { _tr_task_cancel((_TrTaskState*)t); }
_TR_XLINK long long _tr_task_await_h(char* t)                              { return _tr_task_await((_TrTaskState*)t); }
_TR_XLINK bool  _tr_task_await_timeout_h(char* t, long long ms)            { return _tr_task_await_timeout_ok((_TrTaskState*)t, ms); }
_TR_XLINK bool  _tr_task_is_done_h(char* t)                                { return _tr_task_is_done((_TrTaskState*)t); }
_TR_XLINK bool  _tr_task_is_cancelled_h(char* t)                           { return _tr_task_is_cancelled((_TrTaskState*)t); }
_TR_XLINK bool  _tr_task_has_error_h(char* t)                              { return _tr_task_has_error((_TrTaskState*)t); }
_TR_XLINK char* _tr_task_get_error_h(char* t)                              { return _tr_rt_str_new(_tr_task_get_error((_TrTaskState*)t)); }
_TR_XLINK void  _tr_task_free_h(char* t)                                   { _tr_task_free((_TrTaskState*)t); }

/* Mutex / RWLock */
_TR_XLINK char* _tr_mutex_new_h(void)                                      { return (char*)_tr_mutex_new(); }
_TR_XLINK void  _tr_mutex_lock_h(char* m)                                  { _tr_mutex_hlock((_TrMutexH*)m); }
_TR_XLINK void  _tr_mutex_unlock_h(char* m)                                { _tr_mutex_hunlock((_TrMutexH*)m); }
_TR_XLINK bool  _tr_mutex_trylock_h(char* m)                               { return _tr_mutex_htrylock((_TrMutexH*)m); }
_TR_XLINK void  _tr_mutex_free_h(char* m)                                  { _tr_mutex_hfree((_TrMutexH*)m); }
_TR_XLINK char* _tr_rwl_new_h(void)                                        { return (char*)_tr_rwl_new(); }
_TR_XLINK void  _tr_rwl_read_lock_h(char* r)                               { _tr_rwl_read_lock((_TrRWL*)r); }
_TR_XLINK void  _tr_rwl_read_unlock_h(char* r)                             { _tr_rwl_read_unlock((_TrRWL*)r); }
_TR_XLINK void  _tr_rwl_write_lock_h(char* r)                              { _tr_rwl_write_lock((_TrRWL*)r); }
_TR_XLINK void  _tr_rwl_write_unlock_h(char* r)                            { _tr_rwl_write_unlock((_TrRWL*)r); }
_TR_XLINK void  _tr_rwl_free_h(char* r)                                    { _tr_rwl_free((_TrRWL*)r); }

/* Semaphore */
_TR_XLINK char* _tr_sema_new_h(long long init, long long maxv)             { return (char*)_tr_sema_new(init, maxv); }
_TR_XLINK void  _tr_sema_acquire_h(char* s)                                { _tr_sema_acquire((_TrSema*)s); }
_TR_XLINK bool  _tr_sema_try_acquire_h(char* s)                            { return _tr_sema_try_acquire((_TrSema*)s); }
_TR_XLINK bool  _tr_sema_acquire_timeout_h(char* s, long long ms)          { return _tr_sema_acquire_timeout((_TrSema*)s, ms); }
_TR_XLINK void  _tr_sema_release_h(char* s)                                { _tr_sema_release((_TrSema*)s); }
_TR_XLINK void  _tr_sema_free_h(char* s)                                   { _tr_sema_free((_TrSema*)s); }

/* WaitGroup */
_TR_XLINK char* _tr_wg_new_h(void)                                         { return (char*)_tr_wg_new(); }
_TR_XLINK void  _tr_wg_add_h(char* w, long long n)                         { _tr_wg_add((_TrWG*)w, n); }
_TR_XLINK void  _tr_wg_done_h(char* w)                                     { _tr_wg_done((_TrWG*)w); }
_TR_XLINK void  _tr_wg_wait_h(char* w)                                     { _tr_wg_wait((_TrWG*)w); }
_TR_XLINK bool  _tr_wg_wait_timeout_h(char* w, long long ms)               { return _tr_wg_wait_timeout((_TrWG*)w, ms); }
_TR_XLINK void  _tr_wg_free_h(char* w)                                     { _tr_wg_free((_TrWG*)w); }

/* Barrier */
_TR_XLINK char* _tr_barrier_new_h(long long n)                             { return (char*)_tr_barrier_new(n); }
_TR_XLINK void  _tr_barrier_wait_h(char* b)                                { _tr_barrier_wait((_TrBarrier*)b); }
_TR_XLINK void  _tr_barrier_free_h(char* b)                                { _tr_barrier_free((_TrBarrier*)b); }

/* Once */
_TR_XLINK char* _tr_once_new_h(void)                                       { return (char*)_tr_once_new(); }
_TR_XLINK bool  _tr_once_do_h(char* o)                                     { return _tr_once_do((_TrOnce*)o); }
_TR_XLINK void  _tr_once_free_h(char* o)                                   { _tr_once_free((_TrOnce*)o); }

/* Timer / Ticker */
_TR_XLINK char* _tr_timer_new_h(long long ms, char* ch)                    { return (char*)_tr_timer_new(ms, (_TrChan*)ch); }
_TR_XLINK char* _tr_ticker_new_h(long long ms, char* ch)                   { return (char*)_tr_ticker_new(ms, (_TrChan*)ch); }
_TR_XLINK void  _tr_timer_stop_h(char* s)                                  { _tr_timer_stop((_TrTimerState*)s); }

/* Thread object (joinable handle) */
typedef void*(*_TrThreadFn)(void*);
_TR_XLINK char* _tr_threadobj_spawn_h(char* fn, char* arg)                 { return (char*)_tr_threadobj_spawn((_TrThreadFn)(uintptr_t)fn, (void*)arg); }
_TR_XLINK void  _tr_threadobj_join_h(char* t)                              { _tr_threadobj_join((_TrThreadObj*)t); }
_TR_XLINK void  _tr_threadobj_detach_h(char* t)                            { _tr_threadobj_detach((_TrThreadObj*)t); }
_TR_XLINK void  _tr_threadobj_free_h(char* t)                              { _tr_threadobj_free((_TrThreadObj*)t); }
_TR_XLINK bool  _tr_threadobj_panicked_h(char* t)                          { return _tr_threadobj_panicked((_TrThreadObj*)t); }
_TR_XLINK char* _tr_threadobj_panic_msg_h(char* t)                         { return _tr_rt_str_new(_tr_threadobj_panic_msg((_TrThreadObj*)t)); }
_TR_XLINK long long _tr_thread_current_id_h(void)                          { return _tr_thread_current_id(); }
_TR_XLINK void  _tr_thread_sleep_ms_h(long long ms)                        { _tr_thread_sleep_ms(ms); }

/* Atomic[T]: lock-free integer */
_TR_XLINK char* _tr_atomic_new_h(long long init)                           { return (char*)_tr_atomic_new(init); }
_TR_XLINK long long _tr_atomic_load_h(char* a)                             { return _tr_atomic_load((_TrAtomic*)a); }
_TR_XLINK void _tr_atomic_store_h(char* a, long long v)                   { _tr_atomic_store((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_add_h(char* a, long long v)                 { return _tr_atomic_add((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_sub_h(char* a, long long v)                 { return _tr_atomic_sub((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_swap_h(char* a, long long v)                { return _tr_atomic_swap((_TrAtomic*)a, v); }
_TR_XLINK bool _tr_atomic_cas_h(char* a, long long expected, long long desired) { return _tr_atomic_cas((_TrAtomic*)a, expected, desired); }
_TR_XLINK void _tr_atomic_free_h(char* a)                                 { _tr_atomic_free((_TrAtomic*)a); }
_TR_XLINK long long _tr_atomic_load_relaxed_h(char* a)                     { return _tr_atomic_load_relaxed((_TrAtomic*)a); }
_TR_XLINK long long _tr_atomic_load_acquire_h(char* a)                     { return _tr_atomic_load_acquire((_TrAtomic*)a); }
_TR_XLINK long long _tr_atomic_load_seqcst_h(char* a)                      { return _tr_atomic_load_seqcst((_TrAtomic*)a); }
_TR_XLINK void _tr_atomic_store_relaxed_h(char* a, long long v)           { _tr_atomic_store_relaxed((_TrAtomic*)a, v); }
_TR_XLINK void _tr_atomic_store_release_h(char* a, long long v)           { _tr_atomic_store_release((_TrAtomic*)a, v); }
_TR_XLINK void _tr_atomic_store_seqcst_h(char* a, long long v)            { _tr_atomic_store_seqcst((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_add_relaxed_h(char* a, long long v)         { return _tr_atomic_add_relaxed((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_add_release_h(char* a, long long v)         { return _tr_atomic_add_release((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_add_acqrel_h(char* a, long long v)          { return _tr_atomic_add_acqrel((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_sub_relaxed_h(char* a, long long v)         { return _tr_atomic_sub_relaxed((_TrAtomic*)a, v); }
_TR_XLINK long long _tr_atomic_sub_release_h(char* a, long long v)         { return _tr_atomic_sub_release((_TrAtomic*)a, v); }
_TR_XLINK bool _tr_atomic_cas_weak_h(char* a, long long exp, long long des)   { return _tr_atomic_cas_weak((_TrAtomic*)a, exp, des); }
_TR_XLINK bool _tr_atomic_cas_acqrel_h(char* a, long long exp, long long des) { return _tr_atomic_cas_acqrel((_TrAtomic*)a, exp, des); }

/* ThreadLocal[T]: per-thread storage */
_TR_XLINK char* _tr_tls_new_h(long long init)                              { return (char*)_tr_tls_new(init); }
_TR_XLINK long long _tr_tls_get_h(char* t)                                 { return _tr_tls_get((_TrTLS*)t); }
_TR_XLINK void _tr_tls_set_h(char* t, long long v)                        { _tr_tls_set((_TrTLS*)t, v); }
_TR_XLINK void _tr_tls_free_h(char* t)                                    { _tr_tls_free((_TrTLS*)t); }

/* ── Core runtime helpers ────────────────────────────────────────────── */

/* stdin-reading builtins: std-tier only (need <stdio.h>'s stdin). Bare-metal has
 * no console — stub to empty so a freestanding program that never calls input()
 * still compiles (and one that does gets "" rather than a link error). */
#ifndef TAURARO_BARE
static char* input(const char* prompt) {
    if (prompt) printf("%s", prompt);
    char* buf = (char*)malloc(256);
    if (fgets(buf, 256, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        return buf;
    }
    free(buf);
    return _tr_empty_heap_str();
}
static char* _tr_read_line(const char* prompt) {
    if (prompt && prompt[0]) printf("%s", prompt);
    char* buf = (char*)malloc(256);
    if (fgets(buf, 256, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0'; /* strip \r on Windows */
        return buf;
    }
    free(buf);
    return _tr_empty_heap_str();
}
#else
static char* input(const char* prompt) { (void)prompt; return _tr_empty_heap_str(); }
static char* _tr_read_line(const char* prompt) { (void)prompt; return _tr_empty_heap_str(); }
#endif
static void yield_val(void* v) { (void)v; }

/* ── LSP / stdio primitives ──────────────────────────────────────────────
 * Low-level stdin/stdout helpers for programs that speak JSON-RPC or
 * any line-framed protocol over stdio.  Declared here so pure-Tauraro
 * code can use them via  extern "C": def _tr_read_stdin_line() -> str
 * without needing a hand-written C shim file.
 * ──────────────────────────────────────────────────────────────────────── */
/* Hosted only: <io.h>/<unistd.h> (isatty, fileno) aren't freestanding headers —
 * bare toolchains like riscv64-unknown-elf lack <unistd.h>. Under TAURARO_BARE there
 * is no stdio/tty, so skip the include; the bare fallbacks below don't use it. */
#ifndef TAURARO_BARE
#ifdef _WIN32
#  ifndef _INC_IO
#    include <io.h>     /* _isatty, _fileno on Windows */
#  endif
#else
#  ifndef _UNISTD_H
#    include <unistd.h> /* isatty, STDIN_FILENO on Unix */
#  endif
#endif
#endif

/* ── New built-in methods added in v0.0.5 ────────────────────────────────── */

/* int.to_binary(n) — base-2 string */
static char* _tr_int_to_binary(int64_t n) {
    if (n == 0) { char* z=(char*)_tr_checked_alloc(2); z[0]='0'; z[1]='\0'; return z; }
    char buf[70]; int pos = 68; buf[69] = '\0';
    uint64_t v = (uint64_t)n;
    while (v > 0) { buf[pos--] = '0' + (int)(v & 1); v >>= 1; }
    char* r = (char*)_tr_checked_alloc(70 - pos);
    memcpy(r, buf + pos + 1, 69 - pos);
    return r;
}
/* Python-style hex()/oct()/bin(): "0x"/"0o"/"0b" prefix, sign kept ("-0xff"). */
static char* _tr_hex_str(int64_t n) {
    char b[32]; unsigned long long u = n < 0 ? (unsigned long long)(-(n)) : (unsigned long long)n;
    if (n < 0) snprintf(b, sizeof(b), "-0x%llx", u); else snprintf(b, sizeof(b), "0x%llx", u);
    char* r = (char*)_tr_checked_alloc(strlen(b) + 1); memcpy(r, b, strlen(b) + 1); return r;
}
static char* _tr_oct_str(int64_t n) {
    char b[32]; unsigned long long u = n < 0 ? (unsigned long long)(-(n)) : (unsigned long long)n;
    if (n < 0) snprintf(b, sizeof(b), "-0o%llo", u); else snprintf(b, sizeof(b), "0o%llo", u);
    char* r = (char*)_tr_checked_alloc(strlen(b) + 1); memcpy(r, b, strlen(b) + 1); return r;
}
static char* _tr_bin_str(int64_t n) {
    char* body = _tr_int_to_binary(n < 0 ? -(n) : n);
    size_t bl = strlen(body); const char* pfx = n < 0 ? "-0b" : "0b"; size_t pl = strlen(pfx);
    char* r = (char*)_tr_checked_alloc(pl + bl + 1);
    memcpy(r, pfx, pl); memcpy(r + pl, body, bl + 1); return r;
}

/* float utility predicates */
static int64_t _tr_float_is_nan(double x) { return (int64_t)(x != x); }
static int64_t _tr_float_is_inf(double x) { return (int64_t)(x == 1.0/0.0 || x == -1.0/0.0); }

/* int.gcd / int.lcm */
static int64_t _tr_int_gcd(int64_t a, int64_t b) {
    a = a < 0 ? -a : a; b = b < 0 ? -b : b;
    while (b) { int64_t t = b; b = a % b; a = t; } return a;
}
static int64_t _tr_int_lcm(int64_t a, int64_t b) {
    int64_t g = _tr_int_gcd(a, b); return g ? (a / g * b) : 0LL;
}

/* List sort/aggregate helpers are defined later in this header,
 * after all List_T typedefs.  See the v0.0.5 section near the end. */

/* Map update/clear defined after TrMap typedef below */

/* stdin/stdout/tty/env primitives (LSP, JSON-RPC, NO_COLOR): std-tier only.
 * Gated so a bare-metal build parses past here; stubs keep any caller linkable. */
#ifndef TAURARO_BARE
/* Read one line from stdin.  Returns the raw line INCLUDING any trailing
 * \r\n so callers can distinguish a blank separator ("\r\n") from true
 * EOF ("").  Never strips — the Tauraro caller calls .trim() itself.   */
static char* _tr_read_stdin_line(void) {
    char* buf = (char*)malloc(8192);
    if (!buf) return _tr_empty_heap_str();
    if (!fgets(buf, 8192, stdin)) { free(buf); return _tr_empty_heap_str(); }
    return buf;
}

/* Read exactly n raw bytes from stdin.  Returns a null-terminated heap
 * string.  Returns "" on EOF or error.                                  */
static char* _tr_read_stdin_bytes(int64_t n) {
    if (n <= 0) return _tr_empty_heap_str();
    char* buf = (char*)malloc((size_t)(n + 1));
    if (!buf) return _tr_empty_heap_str();
    size_t total = 0;
    while ((int64_t)total < n) {
        size_t got = fread(buf + total, 1, (size_t)(n - (int64_t)total), stdin);
        if (got == 0) break;
        total += got;
    }
    buf[total] = '\0';
    return buf;
}

/* Write s to stdout without an extra newline.  Used by JSON-RPC writers
 * that build the full frame (Content-Length: …\r\n\r\n{…}) themselves. */
static void _tr_write_stdout(const char* s) {
    if (s) fputs(s, stdout);
}

/* Flush stdout so the peer receives the message without waiting for
 * the kernel buffer to fill up.                                         */
static void _tr_flush_stdout(void) { fflush(stdout); }

/* 1 if stdin is an interactive terminal (not a pipe); 0 otherwise.
 * Used by LSP servers to print a helpful message when started manually. */
static int64_t _tr_stdin_isatty(void) {
#ifdef _WIN32
    return _isatty(_fileno(stdin)) ? 1 : 0;
#else
    return isatty(STDIN_FILENO) ? 1 : 0;
#endif
}

/* 1 if env var `name` is set to a non-empty value; 0 otherwise. Used for the
 * NO_COLOR convention (https://no-color.org).                               */
_TR_XLINK int64_t _tr_env_set(const char* name) {
    if (!name) return 0;
    const char* v = getenv(name);
    return (v && v[0]) ? 1 : 0;
}
#else  /* TAURARO_BARE: no console / tty / env */
static char* _tr_read_stdin_line(void) { return _tr_empty_heap_str(); }
static char* _tr_read_stdin_bytes(int64_t n) { (void)n; return _tr_empty_heap_str(); }
static void _tr_write_stdout(const char* s) { _TR_WRITE(s); }
static void _tr_flush_stdout(void) { }
static int64_t _tr_stdin_isatty(void) { return 0; }
_TR_XLINK int64_t _tr_env_set(const char* name) { (void)name; return 0; }
#endif

/* The ESC control byte (0x1b) as an owned string. Lets the diagnostics module
 * build ANSI sequences without depending on core.string (StringBuilder).     */
_TR_XLINK char* _tr_ansi_esc(void) { return _tr_str_dup_owned("\x1b"); }


static inline char* _tr_str_substring(const char* s, int start, int end) {
    if (!s) return NULL;
    int len = (int)strlen(s);
    if (start < 0) start = 0;
    if (end > len) end = len;
    int sublen = end - start;
    if (sublen < 0) sublen = 0;
    char* res = (char*)_tr_checked_alloc(sublen + 1);
    memcpy(res, s + start, sublen);
    res[sublen] = '\0';
    return res;
}

_TR_XLINK void _tr_exit(long long code) { exit((int)code); }

#if defined(TAURARO_BARE) && !defined(__wasi__)
static inline long long _tr_getpid(void) { return 0LL; }
#elif defined(_WIN32)
#ifndef _TR_PID_INCLUDED
#define _TR_PID_INCLUDED
#include <process.h>
#endif
static inline long long _tr_getpid(void) { return (long long)_getpid(); }
#else
#include <unistd.h>
#include <time.h>
static inline long long _tr_getpid(void) { return (long long)getpid(); }
#endif

/* time.h is hosted, not freestanding: arm-none-eabi/newlib bundles it but bare
 * toolchains (riscv64-unknown-elf) don't. Use it when the toolchain provides it
 * (real wall-clock + calendar); otherwise the datetime helpers below stub out.
 * _TR_HAS_TIME gates every time.h user (here + the DateTime helpers). */
#if defined(__has_include)
#  if __has_include(<time.h>)
#    include <time.h>
#    define _TR_HAS_TIME 1
#  endif
#endif
#ifdef _TR_HAS_TIME
_TR_XLINK long long _tr_timestamp(void) { return (long long)time(NULL); }
#else
_TR_XLINK long long _tr_timestamp(void) { return 0LL; }  /* no wall clock */
#endif

/* High-resolution millisecond wall-clock: QueryPerformanceCounter on Windows,
   CLOCK_MONOTONIC on POSIX.  Used by std.sys.time.time_ms / elapsed_ms. */
_TR_XLINK long long _tr_time_ms(void) {
#if defined(TAURARO_BARE) && !defined(__wasi__)
    return 0LL;
#elif defined(_WIN32)
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (long long)(count.QuadPart * 1000LL / freq.QuadPart);
#else
    struct timespec _ts;
    clock_gettime(CLOCK_MONOTONIC, &_ts);
    return (long long)_ts.tv_sec * 1000LL + (long long)_ts.tv_nsec / 1000000LL;
#endif
}

/* Enable ANSI/VT100 colour codes on Windows Terminal; no-op elsewhere. */
static inline void _tr_enable_vt100(void) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        GetConsoleMode(h, &mode);
        SetConsoleMode(h, mode | 0x0004 /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */);
    }
#endif
}

/* 1 if stdout can render ANSI color, cross-platform. Linux/macOS: stdout is a
 * TTY. Windows: stdout is a TTY AND we best-effort enable VT processing so even
 * classic conhost interprets the escapes (Windows Terminal/VS Code already do).
 * Returns 0 when piped/redirected so logs and `... | grep` stay plain ASCII.  */
_TR_XLINK int64_t _tr_stdout_supports_ansi(void) {
#if defined(TAURARO_BARE)
    return 0;   /* no console on bare-metal */
#elif defined(_WIN32)
    /* Detect a real console via GetConsoleMode — robust where _isatty under-reports
     * (PowerShell wraps stdout so _isatty(stdout) is often false for a live console).
     * GetConsoleMode succeeds only for an actual console handle, and fails when the
     * output is redirected to a file/pipe (so logs and `| grep` stay plain). */
    HANDLE _h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD _m = 0;
    if (_h == INVALID_HANDLE_VALUE || !GetConsoleMode(_h, &_m)) return 0;
    _tr_enable_vt100();
    return 1;
#else
    return isatty(STDOUT_FILENO) ? 1 : 0;
#endif
}

/* ══════════════════════════════════════════════════════════════════════════
 * _TrIOPoll — Async I/O readiness abstraction
 *
 * Unified API over platform-specific event demultiplexers:
 *   Linux:       epoll (default) + io_uring (opt-in: -DTAURARO_IO_URING)
 *   Windows:     IOCP (I/O Completion Ports)
 *   macOS/BSD:   kqueue
 *   BARE/kernel: polling stub (returns immediately, no OS call)
 *
 * Use:
 *   _TrIOPoll* p = _tr_iopoll_create();
 *   _tr_iopoll_add(p, fd, TAURARO_POLLIN, userdata);
 *   int n = _tr_iopoll_wait(p, events, 64, timeout_ms);
 *   for (int i = 0; i < n; i++) { ... events[i].userdata ... }
 *   _tr_iopoll_destroy(p);
 * ══════════════════════════════════════════════════════════════════════════ */

#define TAURARO_POLLIN   0x01u
#define TAURARO_POLLOUT  0x02u
#define TAURARO_POLLERR  0x04u
#define TAURARO_POLLHUP  0x08u

typedef struct {
    int      fd;
    uint32_t events;
    void*    userdata;
} _TrIOEvent;

#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
/* ── BARE/Kernel: polling stub (no OS event loop) ────────────────────── */
typedef struct { int _dummy; } _TrIOPoll;
_TR_XLINK _TrIOPoll* _tr_iopoll_create(void) {
    return (_TrIOPoll*)TAURARO_CALLOC(1, sizeof(_TrIOPoll));
}
_TR_XLINK void _tr_iopoll_destroy(_TrIOPoll* p) { if (p) TAURARO_FREE(p); }
_TR_XLINK int  _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { (void)p;(void)fd;(void)ev;(void)ud; return 0; }
_TR_XLINK int  _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { (void)p;(void)fd;(void)ev;(void)ud; return 0; }
_TR_XLINK int  _tr_iopoll_del(_TrIOPoll* p, int fd)
    { (void)p;(void)fd; return 0; }
_TR_XLINK int  _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* ev, int maxev, int timeout_ms)
    { (void)p;(void)ev;(void)maxev;(void)timeout_ms; return 0; }

#elif defined(_WIN32)
/* ── Windows: WSAPoll-backed _TrIOPoll ────────────────────────────────────
 * A poll()-style *readiness* reactor (register fd, get notified when it is
 * readable/writable). Replaces the former select() backend: WSAPoll drops
 * select()'s FD_SETSIZE ceiling and the per-wait three-fd_set rebuild, keeping
 * a single persistent, dynamically-grown pollfd array - so it scales to many
 * thousands of connections with a much smaller per-wait constant (still O(n)
 * in registered fds, but no fixed cap and no rebuild). It stays readiness-based
 * (POLLRDNORM/POLLWRNORM) so it works directly with the non-blocking sockets
 * used by recv_nb/send_nb/accept_nb; IOCP would be O(ready) but is completion-
 * based and would require overlapped I/O throughout the TCP layer. FD_SETSIZE
 * is still raised before winsock2.h in case any other code path uses fd_set. */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600   /* Vista+ : required for WSAPoll / WSAPOLLFD */
#endif
#ifndef FD_SETSIZE
#define FD_SETSIZE 4096
#endif
#ifndef _TR_NET_INCLUDED
#define _TR_NET_INCLUDED
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
typedef struct {
    WSAPOLLFD* pfds;       /* fd + requested events (in) + returned revents (out) */
    void**     userdata;   /* parallel to pfds: the parked coro / registration ptr */
    int        count;
    int        cap;
} _TrIOPoll;
_TR_XLINK _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    if (!p) return NULL;
    p->cap = 64;
    p->pfds = (WSAPOLLFD*)TAURARO_CALLOC((size_t)p->cap, sizeof(WSAPOLLFD));
    p->userdata = (void**)TAURARO_CALLOC((size_t)p->cap, sizeof(void*));
    if (!p->pfds || !p->userdata) { _tr_free(p->pfds); _tr_free(p->userdata); _tr_free(p); return NULL; }
    return p;
}
_TR_XLINK void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (p) { _tr_free(p->pfds); _tr_free(p->userdata); _tr_free(p); }
}
static inline SHORT _tr_poll_events(uint32_t ev) {
    SHORT e = 0;
    if (ev & TAURARO_POLLIN)  e |= POLLRDNORM;
    if (ev & TAURARO_POLLOUT) e |= POLLWRNORM;
    return e;
}
_TR_XLINK int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    for (int i = 0; i < p->count; i++) {
        if (p->pfds[i].fd == (SOCKET)fd) {
            p->pfds[i].events = _tr_poll_events(ev);
            p->pfds[i].revents = 0;
            p->userdata[i] = ud;
            return 0;
        }
    }
    if (p->count >= p->cap) {
        int ncap = p->cap * 2;
        WSAPOLLFD* nf = (WSAPOLLFD*)realloc(p->pfds, (size_t)ncap * sizeof(WSAPOLLFD));
        void** nu = (void**)realloc(p->userdata, (size_t)ncap * sizeof(void*));
        if (nf) p->pfds = nf;
        if (nu) p->userdata = nu;
        if (!nf || !nu) return -1;
        p->cap = ncap;
    }
    int idx = p->count++;
    p->pfds[idx].fd = (SOCKET)fd;
    p->pfds[idx].events = _tr_poll_events(ev);
    p->pfds[idx].revents = 0;
    p->userdata[idx] = ud;
    return 0;
}
_TR_XLINK int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { return _tr_iopoll_add(p, fd, ev, ud); }
_TR_XLINK int _tr_iopoll_del(_TrIOPoll* p, int fd) {
    if (!p) return -1;
    for (int i = 0; i < p->count; i++) {
        if (p->pfds[i].fd == (SOCKET)fd) {
            p->count--;
            p->pfds[i]     = p->pfds[p->count];
            p->userdata[i] = p->userdata[p->count];
            return 0;
        }
    }
    return -1;
}
_TR_XLINK int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
    if (!p || !out || maxev <= 0) return 0;
    if (p->count == 0) {
        if (timeout_ms > 0) Sleep((DWORD)timeout_ms);
        return 0;
    }
    int pr = WSAPoll(p->pfds, (ULONG)p->count, timeout_ms);
    if (pr <= 0) return 0;
    int n = 0;
    for (int i = 0; i < p->count && n < maxev; i++) {
        SHORT re = p->pfds[i].revents;
        if (!re) continue;
        uint32_t e = 0;
        if (re & (POLLRDNORM | POLLIN))          e |= TAURARO_POLLIN;
        if (re & (POLLWRNORM | POLLOUT))         e |= TAURARO_POLLOUT;
        if (re & (POLLERR | POLLHUP | POLLNVAL)) e |= TAURARO_POLLERR;
        /* A half-close/error must still wake a read waiter so it can observe
         * EOF via recv()==0, matching select()/epoll behavior. */
        if ((re & (POLLHUP | POLLERR)) && (p->pfds[i].events & POLLRDNORM)) e |= TAURARO_POLLIN;
        if (e) {
            out[n].fd       = (int)p->pfds[i].fd;
            out[n].events   = e;
            out[n].userdata = p->userdata[i];
            n++;
        }
    }
    return n;
}

#elif defined(__linux__)
/* ── Linux: epoll-backed _TrIOPoll ───────────────────────────────────── */
#include <sys/epoll.h>
#include <unistd.h>
typedef struct { int epfd; } _TrIOPoll;
_TR_XLINK _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    p->epfd = epoll_create1(EPOLL_CLOEXEC);
    return p;
}
_TR_XLINK void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (!p) return; if (p->epfd >= 0) close(p->epfd); free(p);
}
_TR_XLINK int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct epoll_event e = {0};
    if (ev & TAURARO_POLLIN)  e.events |= EPOLLIN;
    if (ev & TAURARO_POLLOUT) e.events |= EPOLLOUT;
    e.data.ptr = ud;
    return epoll_ctl(p->epfd, EPOLL_CTL_ADD, fd, &e);
}
_TR_XLINK int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct epoll_event e = {0};
    if (ev & TAURARO_POLLIN)  e.events |= EPOLLIN;
    if (ev & TAURARO_POLLOUT) e.events |= EPOLLOUT;
    e.data.ptr = ud;
    return epoll_ctl(p->epfd, EPOLL_CTL_MOD, fd, &e);
}
_TR_XLINK int _tr_iopoll_del(_TrIOPoll* p, int fd) {
    if (!p) return -1;
    return epoll_ctl(p->epfd, EPOLL_CTL_DEL, fd, NULL);
}
_TR_XLINK int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
    if (!p || !out || maxev <= 0) return 0;
    struct epoll_event evs[256];
    int n = epoll_wait(p->epfd, evs, maxev < 256 ? maxev : 256, timeout_ms);
    if (n <= 0) return 0;
    for (int i = 0; i < n; i++) {
        uint32_t e = 0;
        if (evs[i].events & EPOLLIN)  e |= TAURARO_POLLIN;
        if (evs[i].events & EPOLLOUT) e |= TAURARO_POLLOUT;
        if (evs[i].events & EPOLLERR) e |= TAURARO_POLLERR;
        if (evs[i].events & EPOLLHUP) e |= TAURARO_POLLHUP;
        out[i].fd       = -1; /* epoll doesn't return fd in event */
        out[i].events   = e;
        out[i].userdata = evs[i].data.ptr;
    }
    return n;
}

#if defined(TAURARO_IO_URING)
/* ── io_uring support (Linux ≥5.1, opt-in with -DTAURARO_IO_URING) ─── *
 * Provides zero-syscall-per-op submission/completion ring interface.   *
 * WARNING: incorrect ring usage can crash/oops the kernel. Only use   *
 * after thorough testing. The epoll backend is the safe default.       */
#include <liburing.h>
typedef struct { struct io_uring ring; } _TrIOUring;
static inline _TrIOUring* _tr_iouring_create(unsigned entries) {
    _TrIOUring* u = (_TrIOUring*)calloc(1, sizeof(_TrIOUring));
    if (io_uring_queue_init(entries ? entries : 256u, &u->ring, 0) < 0) {
        free(u); return NULL;
    }
    return u;
}
static inline void _tr_iouring_destroy(_TrIOUring* u) {
    if (!u) return; io_uring_queue_exit(&u->ring); free(u);
}
static inline struct io_uring_sqe* _tr_iouring_get_sqe(_TrIOUring* u) {
    return u ? io_uring_get_sqe(&u->ring) : NULL;
}
static inline int _tr_iouring_submit(_TrIOUring* u) {
    return u ? io_uring_submit(&u->ring) : -1;
}
static inline int _tr_iouring_wait_cqe(_TrIOUring* u, struct io_uring_cqe** cqe) {
    return u ? io_uring_wait_cqe(&u->ring, cqe) : -1;
}
static inline void _tr_iouring_cqe_seen(_TrIOUring* u, struct io_uring_cqe* cqe) {
    if (u) io_uring_cqe_seen(&u->ring, cqe);
}
#endif /* TAURARO_IO_URING */

#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
/* ── macOS/BSD: kqueue-backed _TrIOPoll ──────────────────────────────── */
#include <sys/event.h>
#include <unistd.h>
typedef struct { int kqfd; } _TrIOPoll;
_TR_XLINK _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    p->kqfd = kqueue(); return p;
}
_TR_XLINK void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (!p) return; if (p->kqfd >= 0) close(p->kqfd); free(p);
}
_TR_XLINK int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct kevent changes[2]; int n = 0;
    if (ev & TAURARO_POLLIN)
        EV_SET(&changes[n++], (uintptr_t)fd, EVFILT_READ,  EV_ADD|EV_ENABLE, 0, 0, ud);
    if (ev & TAURARO_POLLOUT)
        EV_SET(&changes[n++], (uintptr_t)fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, ud);
    return kevent(p->kqfd, changes, n, NULL, 0, NULL);
}
_TR_XLINK int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { return _tr_iopoll_add(p, fd, ev, ud); }
_TR_XLINK int _tr_iopoll_del(_TrIOPoll* p, int fd) {
    if (!p) return -1;
    struct kevent changes[2];
    EV_SET(&changes[0], (uintptr_t)fd, EVFILT_READ,  EV_DELETE, 0, 0, NULL);
    EV_SET(&changes[1], (uintptr_t)fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    return kevent(p->kqfd, changes, 2, NULL, 0, NULL);
}
_TR_XLINK int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
    if (!p || !out || maxev <= 0) return 0;
    struct kevent evs[256];
    struct timespec ts = { timeout_ms / 1000, (timeout_ms % 1000) * 1000000L };
    struct timespec* tsp = timeout_ms < 0 ? NULL : &ts;
    int n = kevent(p->kqfd, NULL, 0, evs, maxev < 256 ? maxev : 256, tsp);
    if (n <= 0) return 0;
    for (int i = 0; i < n; i++) {
        uint32_t e = 0;
        if (evs[i].filter == EVFILT_READ)  e |= TAURARO_POLLIN;
        if (evs[i].filter == EVFILT_WRITE) e |= TAURARO_POLLOUT;
        if (evs[i].flags & EV_ERROR)       e |= TAURARO_POLLERR;
        if (evs[i].flags & EV_EOF)         e |= TAURARO_POLLHUP;
        out[i].fd       = (int)evs[i].ident;
        out[i].events   = e;
        out[i].userdata = evs[i].udata;
    }
    return n;
}
#else
/* ── Fallback: no async I/O on unknown platform ───────────────────────── */
typedef struct { int _dummy; } _TrIOPoll;
_TR_XLINK _TrIOPoll* _tr_iopoll_create(void) { return (_TrIOPoll*)calloc(1,sizeof(_TrIOPoll)); }
_TR_XLINK void _tr_iopoll_destroy(_TrIOPoll* p) { if(p) free(p); }
_TR_XLINK int  _tr_iopoll_add(_TrIOPoll* p,int fd,uint32_t ev,void* ud){(void)p;(void)fd;(void)ev;(void)ud;return -1;}
_TR_XLINK int  _tr_iopoll_mod(_TrIOPoll* p,int fd,uint32_t ev,void* ud){(void)p;(void)fd;(void)ev;(void)ud;return -1;}
_TR_XLINK int  _tr_iopoll_del(_TrIOPoll* p,int fd){(void)p;(void)fd;return -1;}
_TR_XLINK int  _tr_iopoll_wait(_TrIOPoll* p,_TrIOEvent* ev,int m,int t){(void)p;(void)ev;(void)m;(void)t;return 0;}
#endif /* _TrIOPoll platform backends */

/* _tr_iopoll_wait_raw: Tauraro-callable version.
 * out_buf is a caller-allocated byte array; each slot is sizeof(_TrIOEvent).
 * Returns number of events written.  Tauraro code reads fd/events/userdata
 * at offsets 0/4/8 within each 16-byte slot. */
_TR_XLINK int _tr_iopoll_wait_raw(char* p_raw, char* out_buf, int maxev, int timeout_ms) {
    _TrIOPoll* p = (_TrIOPoll*)p_raw;
    _TrIOEvent tmp[64];
    if (maxev > 64) maxev = 64;
    int n = _tr_iopoll_wait(p, tmp, maxev, timeout_ms);
    for (int i = 0; i < n; i++) {
        char* slot = out_buf + i * 16;
        int   fd   = tmp[i].fd;
        int   ev   = (int)tmp[i].events;
        int   ud   = (int)(long long)tmp[i].userdata;
        memcpy(slot + 0, &fd, 4);
        memcpy(slot + 4, &ev, 4);
        memcpy(slot + 8, &ud, 4);
    }
    return n;
}

/* IOPoll char*-typed _h wrappers for Tauraro Pointer[char] interop */
_TR_XLINK char* _tr_iopoll_create_h(void)
    { return (char*)_tr_iopoll_create(); }
_TR_XLINK void  _tr_iopoll_destroy_h(char* p)
    { _tr_iopoll_destroy((_TrIOPoll*)p); }
_TR_XLINK int   _tr_iopoll_add_h(char* p, long long fd, long long ev, long long ud)
    { return _tr_iopoll_add((_TrIOPoll*)p,(int)fd,(uint32_t)ev,(void*)(uintptr_t)(unsigned long long)ud); }
_TR_XLINK int   _tr_iopoll_mod_h(char* p, long long fd, long long ev, long long ud)
    { return _tr_iopoll_mod((_TrIOPoll*)p,(int)fd,(uint32_t)ev,(void*)(uintptr_t)(unsigned long long)ud); }
_TR_XLINK int   _tr_iopoll_del_h(char* p, long long fd)
    { return _tr_iopoll_del((_TrIOPoll*)p,(int)fd); }

/* =========================================================================
 * Green-thread scheduler - stackful coroutines + non-blocking reactor.
 *
 * This is Tauraro's async/await engine. Each `async`/`spawn` task is a
 * lightweight stackful coroutine (Windows Fiber / POSIX ucontext) with its
 * own small stack. `await` is a cheap context switch, NOT an OS thread
 * spawn - so a single OS thread cooperatively runs millions of tasks. An
 * `await` on a socket parks the task on fd-readiness via the _TrIOPoll
 * reactor (epoll/IOCP-select/kqueue) and yields, so no thread ever blocks on
 * I/O (Node.js / Redis single-reactor model; multicore = future work).
 * ========================================================================= */
/* Must match the exception-stack depth used by the shared exception-chain
 * object below. Declared unconditionally (not just when coroutines are
 * compiled in) because `_TrExcChain` backs BOTH the per-await-chain state
 * used by `_TrCoro` and the plain root/thread-level state used when no
 * coroutine is running (including BARE/WASM builds, which never define
 * `_TrCoro` at all - see the `struct _TrCoro*` forward-reference below,
 * legal in C without a full definition since it is never dereferenced
 * there). `await`-ing a call spawns a genuinely separate CHILD `_TrCoro`
 * (not inline execution): `try: r = await f()` pushes the try-handler on
 * the AWAITER's exception state, then the awaited callee runs as its own
 * coroutine and, if it raises, must find that SAME handler to unwind
 * into. So the exception stack cannot be private to one `_TrCoro` - it
 * has to be SHARED by the whole nested-await call chain rooted at
 * whichever coroutine (or thread-level call) started it, and that shared
 * chain must still be reachable no matter which OS thread ends up running
 * any given coroutine in the chain (the motivating reason for this struct
 * existing at all: a work-stealing pool can resume a chain member on a
 * different worker than where it suspended). Implemented as a separate
 * heap-allocated, refcounted object so its lifetime outlives any single
 * member coroutine's own free. */
#define _TR_MAX_EXC 64

typedef struct _TrExcChain {
    jmp_buf*         bufs[_TR_MAX_EXC];
    char**           msgs[_TR_MAX_EXC];
    struct _TrCoro*  owner[_TR_MAX_EXC]; /* which coro (or NULL = root) pushed this frame */
    int              sp;
    int              has_panic_buf;
    jmp_buf          panic_jmpbuf;
    char*            panic_message;
    int              refcount;
} _TrExcChain;

static _TrExcChain* _tr_excchain_new(void) {
    _TrExcChain* ch = (_TrExcChain*)calloc(1, sizeof(_TrExcChain));
    ch->refcount = 1;
    return ch;
}

/* `_TR_GLOBAL`/`_TR_THREAD_LOCAL` are normally defined much further down
 * (near the panic-state globals), but `_tr_root_exc_chain` needs them
 * here, ahead of the coroutine section. Guarded so the later, canonical
 * definitions (identical expansions) don't conflict. */
#ifndef _TR_GLOBAL
#ifdef _TR_MAIN
  #define _TR_GLOBAL
#else
  #define _TR_GLOBAL extern
#endif
#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
#  define _TR_THREAD_LOCAL
#elif defined(_MSC_VER)
#  define _TR_THREAD_LOCAL __declspec(thread)
#elif defined(__GNUC__) || defined(__clang__)
#  define _TR_THREAD_LOCAL __thread
#else
#  define _TR_THREAD_LOCAL _Thread_local
#endif
#endif /* _TR_GLOBAL */

/* Thread-level root chain, used whenever no coroutine is currently running
 * (plain code, or - importantly - `async def main()` itself, which is
 * compiled as the real, non-coroutine `int main()` and drives its own
 * top-level `await`s via `_tr_co_await`'s "outside a coroutine" pump; see
 * `_tr_exc_chain_get()` below). Lazily created on first use. */
_TR_GLOBAL _TR_THREAD_LOCAL _TrExcChain* _tr_root_exc_chain;

#if !defined(TAURARO_BARE) && !defined(TAURARO_WASM)
#define _TR_HAS_CORO 1
#endif

#if defined(TAURARO_BARE) || defined(TAURARO_WASM)
/* No coroutines at all in this configuration - the root chain is the only
 * chain that will ever exist, functionally identical to a plain stack. */
static _TrExcChain* _tr_exc_chain_get(void) {
    if (!_tr_root_exc_chain) _tr_root_exc_chain = _tr_excchain_new();
    return _tr_root_exc_chain;
}

/* BARE/WASM AsyncPool/AsyncTask: no coroutines, no OS threads - every
 * submitted call just runs synchronously in place, exactly like
 * `_TrThreadPool`'s own BARE stub above (`_tr_threadpool_spawn` calling
 * `fn(arg)` directly). The real, work-stealing versions of these types
 * live inside the `#if !defined(TAURARO_BARE) && !defined(TAURARO_WASM)`
 * coroutine section below; this stub exists so code that references them
 * UNCONDITIONALLY (`_TrAsyncTaskGroup`/`_tr_atg_*`, which back `await_all`
 * and must compile in every configuration) still has something to link
 * against on a target with no coroutine/thread support at all. */
typedef struct { int _dummy; } _TrAsyncTask;
typedef struct { int _dummy; } _TrAsyncPool;
static _TrAsyncPool* _tr_asyncpool_new(long long n) { (void)n; return (_TrAsyncPool*)TAURARO_CALLOC(1, sizeof(_TrAsyncPool)); }
static _TrAsyncTask* _tr_asyncpool_spawn(_TrAsyncPool* p, void*(*fn)(void*), void* arg) { (void)p; fn(arg); return NULL; }
static long long _tr_asynctask_await(_TrAsyncTask* t) { (void)t; return 0; }
static int _tr_asynctask_done(_TrAsyncTask* t) { (void)t; return 1; }
static void _tr_asynctask_free(_TrAsyncTask* t) { (void)t; }
static void _tr_asyncpool_free(_TrAsyncPool* p) { if (p) TAURARO_FREE(p); }
static _TrAsyncPool* _tr_asyncpool_default(void) { return NULL; }
static void _tr_asyncpool_default_shutdown(void) { }
#endif

#if !defined(TAURARO_BARE) && !defined(TAURARO_WASM)

#if defined(_WIN32)
typedef LPVOID _tr_coctx_t;
#else
#include <ucontext.h>
#include <sys/mman.h>
typedef ucontext_t _tr_coctx_t;
#endif

typedef void* (*_tr_coro_fn)(void*);
typedef enum { _TRC_READY, _TRC_RUN, _TRC_SUSP, _TRC_DONE } _tr_costate;

typedef struct _TrCoro {
    _tr_coctx_t      ctx;
#if !defined(_WIN32)
    char*            stack;
#endif
    _tr_coro_fn      fn;
    void*            arg;
    _tr_costate      state;
    long long        result;
    long long        wake_at;     /* timer deadline (ms); 0 = not sleeping  */
    int              io_fd;       /* fd this coro is parked on right now; -1 = none */
    int              io_armed_fd; /* fd currently REGISTERED in the reactor; -1 = none.
                                   * Kept across awaits so a keep-alive connection
                                   * re-arms with no epoll_ctl syscall (persistent
                                   * registration); dropped when the coro is freed. */
    uint32_t         io_armed_ev; /* event mask registered for io_armed_fd  */
    int              detached;    /* 1 = scheduler frees it on completion   */
    struct _TrCoro*  joiner;      /* coro waiting for this one to finish    */
    struct _TrCoro*  next;        /* ready-queue link                       */
    struct _TrCoro*  snext;       /* sleep-list link                        */
    _TrExcChain*     exc_chain;   /* shared with every coro in this await-chain */
    /* Set instead of completing normally when an uncaught exception needs
     * to unwind into a handler owned by a DIFFERENT coroutine (or the root)
     * - see _tr_exc_raise's cross-coroutine branch. Never transitions to
     * _TRC_DONE; this coroutine is deliberately abandoned (see that
     * function's comment on the resulting resource leak, a known, accepted
     * tradeoff vs. the alternative of a fiber-boundary-crossing longjmp). */
    int              failed;
    char*            fail_msg;
} _TrCoro;

typedef struct {
    _tr_coctx_t  main_ctx;
    _TrCoro*     current;
    _TrCoro*     rhead;
    _TrCoro*     rtail;
    _TrCoro*     shead;           /* timer-parked coros                     */
    _TrIOPoll*   reactor;
    int          n_sleep;
    int          n_io;
    int          inited;
} _TrSchedG;
/* Per-OS-thread scheduler (thread-per-core multicore = N worker threads, each
 * with its own independent scheduler + reactor). It MUST be a single shared
 * global across all translation units - a `static` per-TU copy would mean a
 * coroutine that parks in one module (e.g. std/net) and the scheduler that
 * runs it in another (e.g. main) see different state. Defined once in the
 * _TR_MAIN TU, extern everywhere else; thread-local so each worker OS thread
 * still gets its own instance. */
#ifdef _TR_MAIN
__thread _TrSchedG _tr_g = {0};
#else
extern __thread _TrSchedG _tr_g;
#endif

/* Coroutine-aware version of the chain accessor declared earlier (see its
 * BARE/WASM sibling above `_tr_root_exc_chain`) - now that `_tr_g` exists,
 * a running coroutine's OWN shared chain takes priority; otherwise falls
 * back to the same lazily-created root chain plain/root code uses. */
static _TrExcChain* _tr_exc_chain_get(void) {
    if (_tr_g.current) return _tr_g.current->exc_chain;
    if (!_tr_root_exc_chain) _tr_root_exc_chain = _tr_excchain_new();
    return _tr_root_exc_chain;
}

static long long _tr_mono_ms(void) {
#if defined(_WIN32)
    return (long long)GetTickCount64();
#else
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}

static void _tr_sched_ensure(void) {
    if (_tr_g.inited) return;
    _tr_g.inited = 1;
    _tr_g.current = NULL; _tr_g.rhead = _tr_g.rtail = NULL; _tr_g.shead = NULL;
    _tr_g.reactor = NULL; _tr_g.n_sleep = 0; _tr_g.n_io = 0;
#if defined(_WIN32)
    _tr_g.main_ctx = ConvertThreadToFiber(NULL);
    if (!_tr_g.main_ctx) {
        /* Thread was already a fiber (e.g. nested) - fetch the current one. */
        _tr_g.main_ctx = GetCurrentFiber();
    }
#endif
}

static void _tr_rpush(_TrCoro* c) {
    c->next = NULL;
    if (_tr_g.rtail) _tr_g.rtail->next = c; else _tr_g.rhead = c;
    _tr_g.rtail = c;
    c->state = _TRC_READY;
}
static _TrCoro* _tr_rpop(void) {
    _TrCoro* c = _tr_g.rhead;
    if (!c) return NULL;
    _tr_g.rhead = c->next;
    if (!_tr_g.rhead) _tr_g.rtail = NULL;
    c->next = NULL;
    return c;
}

static void _tr_co_to_sched(_TrCoro* from) {
#if defined(_WIN32)
    (void)from; SwitchToFiber(_tr_g.main_ctx);
#else
    swapcontext(&from->ctx, &_tr_g.main_ctx);
#endif
}
static void _tr_co_to_coro(_TrCoro* to) {
#if defined(_WIN32)
    SwitchToFiber(to->ctx);
#else
    swapcontext(&_tr_g.main_ctx, &to->ctx);
#endif
}

#if defined(_WIN32)
static void CALLBACK _tr_co_entry(LPVOID p) {
    _TrCoro* c = (_TrCoro*)p;
    c->result = (long long)(uintptr_t)c->fn(c->arg);
    c->state = _TRC_DONE;
    if (c->joiner) { _TrCoro* j = c->joiner; c->joiner = NULL; _tr_rpush(j); }
    SwitchToFiber(_tr_g.main_ctx);   /* control returns to the scheduler */
}
#else
static void _tr_co_entry(void) {
    _TrCoro* c = _tr_g.current;
    c->result = (long long)(uintptr_t)c->fn(c->arg);
    c->state = _TRC_DONE;
    if (c->joiner) { _TrCoro* j = c->joiner; c->joiner = NULL; _tr_rpush(j); }
    /* uc_link returns us to main_ctx automatically. */
}
#endif

#define _TR_CORO_STACK (256 * 1024)

/* Spawn a coroutine running fn(arg); returns its handle. */
static _TrCoro* _tr_co_go(_tr_coro_fn fn, void* arg) {
    _tr_sched_ensure();
    _TrCoro* c = (_TrCoro*)calloc(1, sizeof(_TrCoro));
    c->fn = fn; c->arg = arg; c->io_fd = -1; c->io_armed_fd = -1;
    /* Inherit whatever exception chain is currently active - the awaiter's
     * own coroutine chain for a nested `await` inside a `try`, or the
     * thread's root chain for a top-level spawn (e.g. `async def main()`'s
     * own first `await`, or one inside main()'s own top-level `try`) - so
     * a raise deep inside this new coroutine can still find and unwind
     * into a handler owned by a NON-coroutine caller. See _TrExcChain. */
    c->exc_chain = _tr_exc_chain_get();
    c->exc_chain->refcount++;
#if defined(_WIN32)
    c->ctx = CreateFiber(_TR_CORO_STACK, _tr_co_entry, c);
#else
    /* mmap the coroutine stack instead of malloc: anonymous pages are
     * zero-fill-on-demand, so only the pages a handler actually touches become
     * resident. A 256 KiB stack that uses ~16 KiB costs ~16 KiB RSS, not 256 KiB.
     * Under N concurrent keep-alive connections (one stackful coro each) this is
     * the difference between ~N*256KiB and ~N*16KiB of resident memory - the main
     * reason a stackful green-thread server's RSS otherwise dwarfs a stackless
     * one (tokio/asyncio). Falls back to malloc if mmap is unavailable. */
    c->stack = (char*)mmap(NULL, _TR_CORO_STACK, PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (c->stack == MAP_FAILED) { _TR_OOM_ABORT(); }
    getcontext(&c->ctx);
    c->ctx.uc_stack.ss_sp = c->stack;
    c->ctx.uc_stack.ss_size = _TR_CORO_STACK;
    c->ctx.uc_link = &_tr_g.main_ctx;
    makecontext(&c->ctx, _tr_co_entry, 0);
#endif
    _tr_rpush(c);
    return c;
}

static void _tr_co_free(_TrCoro* c) {
    if (!c) return;
    /* Drop any lingering reactor registration before freeing this coro, so a
     * later readiness event on its fd can never deliver to (and dereference) a
     * freed coro. Runs on the scheduler thread the instant the coro returns:
     * the handler has just closed the fd (epoll/kqueue auto-removed it, so the
     * del is a harmless no-op; WSAPoll still needs it), and the fd cannot have
     * been reused yet because no accept has run in between. */
    if (c->io_armed_fd >= 0 && _tr_g.reactor) {
        _tr_iopoll_del(_tr_g.reactor, c->io_armed_fd);
        c->io_armed_fd = -1;
    }
    if (c->exc_chain && --c->exc_chain->refcount == 0) free(c->exc_chain);
#if defined(_WIN32)
    if (c->ctx) DeleteFiber(c->ctx);
#else
    if (c->stack) munmap(c->stack, _TR_CORO_STACK);
#endif
    free(c);
}

/* Run one scheduler step: dispatch a ready coro, or block on the reactor /
 * timers until one becomes runnable. Returns 0 when nothing remains to do. */
static int _tr_sched_step(void) {
    _TrCoro* c = _tr_rpop();
    if (c) {
        _tr_g.current = c;
        c->state = _TRC_RUN;
        _tr_co_to_coro(c);
        _tr_g.current = NULL;
        /* A detached task (Coro.spawn / per-connection handler) has no joiner
         * to free it - the scheduler reclaims it once it finishes. */
        if (c->state == _TRC_DONE && c->detached) _tr_co_free(c);
        return 1;
    }
    if (_tr_g.n_sleep == 0 && _tr_g.n_io == 0) return 0;   /* fully idle */

    /* Compute the next timer deadline. */
    long long now = _tr_mono_ms();
    long long earliest = -1;
    for (_TrCoro* s = _tr_g.shead; s; s = s->snext) {
        if (earliest < 0 || s->wake_at < earliest) earliest = s->wake_at;
    }
    int timeout = -1;   /* block indefinitely if only I/O is pending */
    if (earliest >= 0) { timeout = (int)(earliest - now); if (timeout < 0) timeout = 0; }

    if (_tr_g.n_io > 0 && _tr_g.reactor) {
        /* Drain up to 256 ready connections per epoll/kqueue/WSAPoll wakeup
         * (was 64): under 1000+ concurrent connections, readiness arrives in
         * bursts, so a small batch means many syscalls to clear the ready set
         * and head-of-line latency for the connections at the back. */
        _TrIOEvent evs[256];
        int n = _tr_iopoll_wait(_tr_g.reactor, evs, 256, timeout);
        for (int i = 0; i < n; i++) {
            _TrCoro* k = (_TrCoro*)evs[i].userdata;
            if (k && k->state == _TRC_SUSP && k->io_fd >= 0) {
                /* Persistent registration: do NOT _tr_iopoll_del here. The fd
                 * stays armed (k->io_armed_fd) so the next await on the same
                 * fd+events is a no-op instead of an ADD - saving 2 epoll_ctl
                 * syscalls per keep-alive request. The registration is dropped
                 * when the coro is freed (_tr_co_free). A registered fd that is
                 * level-readable while its coro is already runnable is returned
                 * again by epoll but skipped here (state != _TRC_SUSP); the
                 * scheduler drains the ready queue before polling, so this does
                 * not spin. */
                k->io_fd = -1; _tr_g.n_io--;
                _tr_rpush(k);
            }
        }
    } else if (timeout > 0) {
#if defined(_WIN32)
        Sleep((DWORD)timeout);
#else
        struct timespec ts; ts.tv_sec = timeout / 1000; ts.tv_nsec = (timeout % 1000) * 1000000; nanosleep(&ts, NULL);
#endif
    }

    /* Wake any timers that are now due. */
    now = _tr_mono_ms();
    _TrCoro** pp = &_tr_g.shead;
    while (*pp) {
        _TrCoro* s = *pp;
        if (s->wake_at <= now) {
            *pp = s->snext; s->snext = NULL; s->wake_at = 0; _tr_g.n_sleep--;
            _tr_rpush(s);
        } else {
            pp = &s->snext;
        }
    }
    return 1;
}

/* Drain the scheduler until every coroutine has finished. */
static void _tr_sched_run(void) { while (_tr_sched_step()) {} }

/* Cooperative yield: requeue the current coro behind the others. */
static void _tr_co_yield(void) {
    _TrCoro* c = _tr_g.current;
    if (!c) return;
    _tr_rpush(c);
    _tr_co_to_sched(c);
}

/* Suspend the current coro for `ms` milliseconds (timer-parked). Outside a
 * coroutine this is a plain sleep. */
static void _tr_co_sleep_ms(long long ms) {
    _TrCoro* c = _tr_g.current;
    if (!c) {
#if defined(_WIN32)
        Sleep((DWORD)ms);
#else
        struct timespec ts; ts.tv_sec = ms / 1000; ts.tv_nsec = (ms % 1000) * 1000000; nanosleep(&ts, NULL);
#endif
        return;
    }
    c->wake_at = _tr_mono_ms() + ms;
    c->state = _TRC_SUSP;
    c->snext = _tr_g.shead; _tr_g.shead = c; _tr_g.n_sleep++;
    _tr_co_to_sched(c);
}

/* Park the current coro until `fd` is ready for `events` (TAURARO_POLLIN/OUT).
 * Returns immediately (1) outside a coroutine - caller should use blocking I/O
 * there. Inside a coro: registers with the reactor and yields. */
/* Return the dead stack pages this coro touched (during its last request)
 * back to the OS before it parks on I/O. Under HTTP keep-alive, nearly all
 * connections sit suspended here awaiting their next request; without this,
 * each one keeps its deepest touched stack (tens of KB of JSON/templating
 * frames) resident for its whole lifetime, so peak RSS scales with the
 * connection count, not the worker count. The region BELOW the current frame
 * (lower addresses, since the stack grows down) is unused until we resume;
 * MADV_DONTNEED drops it and the kernel re-supplies zero pages on next touch.
 * A 16 KB margin below the live SP keeps the current frame + red zone safe.
 * Linux-only (MADV_DONTNEED's zero-on-reuse semantics); opt out with
 * TAURARO_CORO_NORECLAIM=1. */
#if defined(__linux__) && !defined(_WIN32)
static void _tr_co_reclaim_stack(_TrCoro* c) {
    static int enabled = -1;
    if (enabled < 0) enabled = (getenv("TAURARO_CORO_NORECLAIM") == NULL) ? 1 : 0;
    if (!enabled || !c->stack) return;
    volatile char probe;
    uintptr_t sp   = (uintptr_t)&probe;
    uintptr_t base = (uintptr_t)c->stack;
    uintptr_t lo   = (base + 4095u) & ~(uintptr_t)4095u;   /* first whole page at/after base */
    uintptr_t hi   = (sp - 16384u) & ~(uintptr_t)4095u;    /* page below a 16 KB safety margin */
    if (hi > lo && (hi - lo) >= 65536u)                    /* only when >=64 KB is reclaimable */
        madvise((void*)lo, (size_t)(hi - lo), MADV_DONTNEED);
}
#endif

static int _tr_co_await_fd(int fd, unsigned int events) {
    _TrCoro* c = _tr_g.current;
    if (!c) return 1;
    if (!_tr_g.reactor) _tr_g.reactor = _tr_iopoll_create();
    c->io_fd = fd;
    c->state = _TRC_SUSP;
    /* Persistent registration: keep the fd armed across awaits. The common
     * keep-alive path re-awaits the SAME fd for the SAME events every request,
     * so this becomes a no-op (no syscall) after the first ADD. Only ADD when
     * the fd is new to this coro, and MOD when the interest changes (e.g.
     * readable -> writable for a backpressured send). The DEL is deferred to
     * _tr_co_free. (epoll/kqueue: ADD/MOD/DEL are syscalls, so this saves two
     * per request; WSAPoll: they are cheap in-memory set ops, so it is a wash
     * but still correct.) */
    if (c->io_armed_fd == fd) {
        if (c->io_armed_ev != (uint32_t)events) {
            _tr_iopoll_mod(_tr_g.reactor, fd, events, (void*)c);
            c->io_armed_ev = (uint32_t)events;
        }
        /* else: already armed for this fd+events - no syscall */
    } else {
        if (c->io_armed_fd >= 0) _tr_iopoll_del(_tr_g.reactor, c->io_armed_fd);
        _tr_iopoll_add(_tr_g.reactor, fd, events, (void*)c);
        c->io_armed_fd = fd;
        c->io_armed_ev = (uint32_t)events;
    }
    _tr_g.n_io++;
#if defined(__linux__) && !defined(_WIN32)
    _tr_co_reclaim_stack(c);
#endif
    _tr_co_to_sched(c);
    return 1;
}

/* Forward declaration: defined much later in the file (needs _TrExcChain's
 * accessor machinery), but _tr_co_await needs to call it - see the
 * "target->failed" checks below, part of the fiber-safe cross-coroutine
 * exception unwind (no cross-fiber longjmp; see _tr_exc_raise's own
 * comment on why and how). */
_TR_XLINK void _tr_exc_raise(char* msg);

/* Await another coroutine's completion and return its result. Works both
 * inside a coro (cooperative suspend) and from the top level (pumps the
 * scheduler until the target finishes). */
static long long _tr_co_await(_TrCoro* target) {
    if (!target) return 0;
    if (_tr_g.current) {
        if (target->state != _TRC_DONE && !target->failed) {
            target->joiner = _tr_g.current;
            _tr_g.current->state = _TRC_SUSP;
            _tr_co_to_sched(_tr_g.current);
        }
        /* `target` failed (raised, uncaught within its own body) rather
         * than completing normally: re-raise HERE, now that we are
         * genuinely executing on OUR OWN fiber (having just resumed via
         * the ordinary, safe suspend/resume path above, or never having
         * suspended at all if `target` had already failed before we even
         * got here) - a same-fiber longjmp from this point is always safe,
         * exactly like the plain synchronous (non-coroutine) case. If we
         * don't own the top handler either, this call's own cross-
         * coroutine branch continues the same hand-off one hop further. */
        if (target->failed) _tr_exc_raise(target->fail_msg);
        return target->result;
    }
    while (target->state != _TRC_DONE && !target->failed) {
        if (!_tr_sched_step()) break;
    }
    if (target->failed) _tr_exc_raise(target->fail_msg);
    return target->result;
}

static int       _tr_co_done(_TrCoro* c)         { return c && c->state == _TRC_DONE; }
static long long  _tr_co_result(_TrCoro* c)       { return c ? c->result : 0; }

/* Spawn a DETACHED task: runs fn(arg) as a green thread that the scheduler
 * frees on completion (no join). Used for per-connection handlers. */
static void _tr_co_spawn(_tr_coro_fn fn, void* arg) {
    _TrCoro* c = _tr_co_go(fn, arg);
    c->detached = 1;
}

/* Await with a millisecond deadline. Returns 1 if the target finished (out =
 * result), 0 on timeout. Inside a coroutine the timeout is best-effort (we
 * cooperatively join); from the top level the scheduler is pumped until the
 * target finishes or the deadline passes. */
static int _tr_co_await_timeout(_TrCoro* target, long long ms, long long* out) {
    if (!target) { if (out) *out = 0; return 1; }
    if (_tr_g.current) {
        long long r = _tr_co_await(target);
        if (out) *out = r;
        return 1;
    }
    long long deadline = _tr_mono_ms() + ms;
    while (target->state != _TRC_DONE && !target->failed) {
        if (_tr_mono_ms() >= deadline) { if (out) *out = 0; return 0; }
        if (!_tr_sched_step()) break;
    }
    if (target->failed) _tr_exc_raise(target->fail_msg);
    if (out) *out = target->result;
    return target->state == _TRC_DONE;
}

/* ── Chase-Lev lock-free work-stealing deque of _TrCoro* ──────────────────
 * Backs `_TrAsyncPool`'s per-worker local run queues (see below): the
 * OWNING worker pushes/pops its own "bottom" end on the fast path (no CAS
 * needed except on the very last element); other workers ("thieves")
 * steal from the "top" end via CAS when their own queue and the shared
 * injector are both empty. Same non-resizable-array design Tokio's local
 * queues and the classic Chase-Lev/Arora-Blumofe-Plassman deque use.
 * Fixed capacity - a push that finds the deque full spills to the pool's
 * shared injector queue instead of growing this array.
 * Holds opaque `void*` (in practice, `_TrAsyncTask*` - see below), never
 * a mid-flight `_TrCoro*`: the stealable unit is a whole, NOT-YET-STARTED
 * top-level async call. Whichever worker dequeues one runs it to
 * completion using its OWN private per-OS-thread green-thread scheduler
 * (`_tr_g`) exactly like a plain top-level `await` already does today -
 * no changes to that existing, battle-tested machinery, and no cross-
 * thread reactor/exception-chain hazards, since a not-yet-started task
 * has no reactor registration or in-flight try-frame to worry about. */
#define _TR_DEQUE_CAP 1024

typedef struct {
    void*         buf[_TR_DEQUE_CAP];
    _Atomic long long top;    /* thieves CAS this end */
    _Atomic long long bottom; /* owner-only push/pop end */
} _TrDeque;

static void _tr_deque_init(_TrDeque* dq) {
    atomic_init(&dq->top, 0);
    atomic_init(&dq->bottom, 0);
}

/* Owner-thread-only. Returns 0 if the deque is full (caller should spill
 * the task to the pool's shared injector queue instead). */
static int _tr_deque_push(_TrDeque* dq, void* item) {
    long long b = atomic_load_explicit(&dq->bottom, memory_order_relaxed);
    long long t = atomic_load_explicit(&dq->top, memory_order_acquire);
    if (b - t >= _TR_DEQUE_CAP) return 0;
    dq->buf[(size_t)b % _TR_DEQUE_CAP] = item;
    /* Publish the slot write before publishing the new bottom, so a thief
     * that observes the incremented bottom also sees the slot's contents. */
    atomic_thread_fence(memory_order_release);
    atomic_store_explicit(&dq->bottom, b + 1, memory_order_relaxed);
    return 1;
}

/* Owner-thread-only. Pops from the SAME end the owner pushes to (the
 * newest task - best cache locality for the common producer-consumer-
 * same-thread case); thieves take from the opposite (oldest) end, which
 * is what keeps a steal and a local pop from usually colliding. Returns
 * NULL if empty. */
static void* _tr_deque_pop(_TrDeque* dq) {
    long long b = atomic_load_explicit(&dq->bottom, memory_order_relaxed) - 1;
    atomic_store_explicit(&dq->bottom, b, memory_order_relaxed);
    atomic_thread_fence(memory_order_seq_cst);
    long long t = atomic_load_explicit(&dq->top, memory_order_relaxed);
    if (t > b) {
        /* Was already empty (or a thief just took the last element) -
         * restore bottom to a consistent empty state (bottom == top). */
        atomic_store_explicit(&dq->bottom, t, memory_order_relaxed);
        return NULL;
    }
    void* item = dq->buf[(size_t)b % _TR_DEQUE_CAP];
    if (t == b) {
        /* Exactly one element left: races with any concurrent thief for
         * this same slot via CAS on `top`. */
        long long expected = t;
        if (!atomic_compare_exchange_strong_explicit(&dq->top, &expected, t + 1,
                memory_order_seq_cst, memory_order_relaxed)) {
            item = NULL; /* a thief won the race */
        }
        atomic_store_explicit(&dq->bottom, t + 1, memory_order_relaxed);
    }
    return item;
}

/* Any-thread ("thief") steal from the opposite end of the owner's pop.
 * Returns NULL if the deque looked empty or another thief (or the owner,
 * on the last element) won the race - caller should just try the next
 * sibling rather than retry this one. */
static void* _tr_deque_steal(_TrDeque* dq) {
    long long t = atomic_load_explicit(&dq->top, memory_order_acquire);
    atomic_thread_fence(memory_order_seq_cst);
    long long b = atomic_load_explicit(&dq->bottom, memory_order_acquire);
    if (t >= b) return NULL;
    void* item = dq->buf[(size_t)t % _TR_DEQUE_CAP];
    long long expected = t;
    if (!atomic_compare_exchange_strong_explicit(&dq->top, &expected, t + 1,
            memory_order_seq_cst, memory_order_relaxed)) {
        return NULL;
    }
    return item;
}

/* ── AsyncTask: a handle to one work-stealing-pool-submitted async call ──
 * Completion is signaled via a plain mutex+condvar (`_TrCondMutex`, already
 * used elsewhere for `WaitGroup`/`ThreadPool`), NOT via the coroutine
 * scheduler - `.await()` on a task handle is a cross-OS-thread join,
 * exactly like `Thread.join()`/`ThreadPool.wait()` already are, so it
 * blocks the CALLING OS thread until the task completes. This matches
 * existing precedent rather than inventing a new "non-blocking join"
 * mechanism; a task's own INTERNAL `await`s are unaffected and run
 * entirely on whichever worker executes it (see `_TrDeque`'s comment). */
typedef struct _TrAsyncTask {
    _tr_coro_fn   fn;
    void*         arg;
    _Atomic int   done;
    long long     result;
    _TrCondMutex  cv;
} _TrAsyncTask;

static _TrAsyncTask* _tr_asynctask_new(_tr_coro_fn fn, void* arg) {
    _TrAsyncTask* t = (_TrAsyncTask*)calloc(1, sizeof(_TrAsyncTask));
    t->fn = fn; t->arg = arg;
    _tr_condmutex_init(&t->cv);
    return t;
}

/* Runs the task to completion on the CALLING (worker) thread's own private
 * green-thread scheduler, then publishes the result. */
static void _tr_asynctask_run_and_complete(_TrAsyncTask* t) {
    _TrCoro* co = _tr_co_go(t->fn, t->arg);
    long long r = _tr_co_await(co);
    _tr_co_free(co);
    _tr_condmutex_lock(&t->cv);
    t->result = r;
    atomic_store(&t->done, 1);
    _tr_condmutex_signal(&t->cv);
    _tr_condmutex_unlock(&t->cv);
}

/* Callable from any thread (including another pool worker awaiting a task
 * it itself submitted). */
static long long _tr_asynctask_await(_TrAsyncTask* t) {
    _tr_condmutex_lock(&t->cv);
    while (!atomic_load(&t->done)) _tr_condmutex_wait(&t->cv);
    long long r = t->result;
    _tr_condmutex_unlock(&t->cv);
    return r;
}
static int _tr_asynctask_done(_TrAsyncTask* t) { return atomic_load(&t->done); }
static void _tr_asynctask_free(_TrAsyncTask* t) { if (t) free(t); }

/* ── AsyncPool: N-worker, work-stealing pool for top-level async calls ───
 * Each worker is one persistent OS thread with its own `_TrDeque` (see
 * above). `AsyncPool.spawn` round-robins a fresh, not-yet-started task
 * onto a worker's deque (or the shared injector on overflow); an idle
 * worker steals a task from a busy sibling's deque before it ever starts
 * running, so it lands on whichever worker actually has spare capacity.
 * Mirrors the existing `ThreadPool` naming/shape (`_tr_threadpool_*`)
 * deliberately - same mental model, work-stealing instead of one shared
 * queue. Lazily created on first use (see `_tr_asyncpool_default`) so
 * pure-compute programs that never touch this API pay nothing for it. */
typedef struct _TrPoolWorker {
    _TrDeque              dq;
    _TrThread             thread;
    struct _TrAsyncPool*  pool;
    int                   idx;
} _TrPoolWorker;

typedef struct _TrAsyncPool {
    _TrPoolWorker*  workers;
    int             n_workers;
    _TrChan*        injector;   /* overflow + external (non-worker-thread) submissions */
    _TrCondMutex    parkcv;     /* idle-worker park/wake */
    volatile int    shutdown;
    _Atomic long long rr;       /* round-robin submission counter */
} _TrAsyncPool;

/* One worker's attempt to find ONE runnable task without blocking: its
 * own deque, then the shared injector, then a steal from each sibling in
 * turn. Returns NULL if nothing was found anywhere on this pass. */
static _TrAsyncTask* _tr_pool_find_work(_TrAsyncPool* pool, _TrPoolWorker* self) {
    void* item = _tr_deque_pop(&self->dq);
    if (item) return (_TrAsyncTask*)item;
    long long v = _tr_chan_try_recv_val(pool->injector);
    if (v != LLONG_MIN) return (_TrAsyncTask*)(uintptr_t)v;
    for (int i = 0; i < pool->n_workers; i++) {
        if (i == self->idx) continue;
        item = _tr_deque_steal(&pool->workers[i].dq);
        if (item) return (_TrAsyncTask*)item;
    }
    return NULL;
}

static void* _tr_pool_worker_main(void* arg) {
    _TrPoolWorker* self = (_TrPoolWorker*)arg;
    _TrAsyncPool* pool = self->pool;
    for (;;) {
        _TrAsyncTask* t = _tr_pool_find_work(pool, self);
        if (t) { _tr_asynctask_run_and_complete(t); continue; }
        if (pool->shutdown) break;
        /* Nothing anywhere on this pass: park briefly. Re-checked in a
         * loop (standard condvar usage) since a task can arrive, and a
         * submitter can signal, between our last empty check and the
         * lock below - the timeout is just a safety net against a missed
         * wakeup race, not the primary mechanism. */
        _tr_condmutex_lock(&pool->parkcv);
        if (!pool->shutdown) _tr_condmutex_wait(&pool->parkcv);
        _tr_condmutex_unlock(&pool->parkcv);
    }
    return NULL;
}

static _TrAsyncPool* _tr_asyncpool_new(long long n) {
    if (n < 1) n = 1;
    _TrAsyncPool* pool = (_TrAsyncPool*)calloc(1, sizeof(_TrAsyncPool));
    pool->n_workers = (int)n;
    pool->workers = (_TrPoolWorker*)calloc((size_t)n, sizeof(_TrPoolWorker));
    pool->injector = _tr_chan_new(n * 8 + 64);
    _tr_condmutex_init(&pool->parkcv);
    for (int i = 0; i < (int)n; i++) {
        pool->workers[i].pool = pool;
        pool->workers[i].idx = i;
        _tr_deque_init(&pool->workers[i].dq);
    }
    /* Threads started only after every worker's own state is initialized -
     * a freshly-started worker may immediately try to steal from a
     * sibling whose deque must already be valid. */
    for (int i = 0; i < (int)n; i++)
        pool->workers[i].thread = _tr_thread_start(_tr_pool_worker_main, &pool->workers[i]);
    return pool;
}

/* Submit a not-yet-started top-level async call. Callable from ANY
 * thread (a pool worker itself, included - e.g. one task spawning
 * another). Round-robins across workers' own deques (cheap, no
 * contention on the common un-full case); falls back to the shared
 * injector if the chosen worker's deque is momentarily full. Signals a
 * parked worker either way. */
static _TrAsyncTask* _tr_asyncpool_spawn(_TrAsyncPool* pool, _tr_coro_fn fn, void* arg) {
    _TrAsyncTask* t = _tr_asynctask_new(fn, arg);
    long long i = atomic_fetch_add_explicit(&pool->rr, 1, memory_order_relaxed) % pool->n_workers;
    if (!_tr_deque_push(&pool->workers[i].dq, t))
        _tr_chan_try_send(pool->injector, (long long)(uintptr_t)t);
    _tr_condmutex_lock(&pool->parkcv);
    _tr_condmutex_signal(&pool->parkcv);
    _tr_condmutex_unlock(&pool->parkcv);
    return t;
}

static void _tr_asyncpool_free(_TrAsyncPool* pool) {
    if (!pool) return;
    pool->shutdown = 1;
    _tr_condmutex_lock(&pool->parkcv);
    /* Wake every parked worker, not just one - shutdown needs all of them
     * to observe `shutdown` and exit, not just whichever one wakes first. */
    for (int i = 0; i < pool->n_workers; i++) _tr_condmutex_signal(&pool->parkcv);
    _tr_condmutex_unlock(&pool->parkcv);
    for (int i = 0; i < pool->n_workers; i++) _tr_thread_join_wait(pool->workers[i].thread);
    _tr_chan_free(pool->injector);
    free(pool->workers);
    free(pool);
}

/* Shared, lazily-created default pool (one per process) - the same
 * lazy-singleton pattern `_tr_async_pool()`/`_tr_global_async_pool`
 * already established for `ThreadPool`, sized to the machine's core
 * count via the existing `_tr_threadpool_auto_n()`. `await_all` and
 * `AsyncPool.auto()`-without-an-explicit-instance both submit here. */
_TR_GLOBAL _TrAsyncPool* _tr_global_asyncpool;
static _TrAsyncPool* _tr_asyncpool_default(void) {
    if (!_tr_global_asyncpool) _tr_global_asyncpool = _tr_asyncpool_new(_tr_threadpool_auto_n());
    return _tr_global_asyncpool;
}
static void _tr_asyncpool_default_shutdown(void) {
    if (_tr_global_asyncpool) { _tr_asyncpool_free(_tr_global_asyncpool); _tr_global_asyncpool = NULL; }
}

/* Tauraro-callable handle-based wrappers - extern "C" decls in std/async. The C backend
 * (which #includes this header) keeps them `static inline`. The NATIVE/LLVM backend links
 * runtime.o (native_abi.c) and needs them as REAL EXPORTED symbols so `await`/`Coro.*`
 * lower to the true green-thread scheduler — native_abi.c defines _TR_EXPORT_CORO. */
#ifdef _TR_EXPORT_CORO
#define _TR_CO_LINK
#else
#define _TR_CO_LINK static
#endif
_TR_CO_LINK char*     _tr_co_go_h(void* fn, void* arg) { return (char*)_tr_co_go((_tr_coro_fn)fn, arg); }
_TR_CO_LINK void       _tr_co_spawn_h(void* fn, void* arg) { _tr_co_spawn((_tr_coro_fn)fn, arg); }
_TR_CO_LINK long long  _tr_co_await_h(char* c)          { return _tr_co_await((_TrCoro*)c); }
_TR_CO_LINK void       _tr_co_free_h(char* c)           { _tr_co_free((_TrCoro*)c); }
_TR_CO_LINK void       _tr_co_yield_h(void)             { _tr_co_yield(); }
_TR_CO_LINK void       _tr_co_sleep_h(long long ms)     { _tr_co_sleep_ms(ms); }
_TR_CO_LINK int        _tr_co_await_fd_h(long long fd, long long ev) { return _tr_co_await_fd((int)fd, (unsigned int)ev); }
_TR_CO_LINK void       _tr_co_run_h(void)               { _tr_sched_run(); }
_TR_CO_LINK int        _tr_co_done_h(char* c)           { return _tr_co_done((_TrCoro*)c); }

#endif /* green-thread scheduler */

/* ── TCP socket helpers ─────────────────────────────────────────────── */
#if defined(TAURARO_BARE) || defined(TAURARO_WASM)
/* No networking on bare WASM or freestanding targets */
static inline int _tr_net_init(void)                                              { return -1; }
_TR_XLINK int _tr_tcp_connect(const char* h, int p)                           { (void)h;(void)p; return -1; }
_TR_XLINK int _tr_tcp_send(int fd, const char* d, int l)                      { (void)fd;(void)d;(void)l; return -1; }
_TR_XLINK int _tr_tcp_recv(int fd, char* b, int c)                            { (void)fd;(void)b;(void)c; return -1; }
_TR_XLINK void _tr_tcp_close(int fd)                                           { (void)fd; }
_TR_XLINK int _tr_tcp_listen(const char* h, int p, int bl)                    { (void)h;(void)p;(void)bl; return -1; }
_TR_XLINK int _tr_tcp_accept(int s)                                            { (void)s; return -1; }
_TR_XLINK char* _tr_tcp_peer_addr(int fd)                                      { (void)fd; return (char*)""; }
_TR_XLINK int _tr_udp_socket(void)                                             { return -1; }
_TR_XLINK int _tr_udp_bind(int fd, int p)                                      { (void)fd;(void)p; return -1; }
_TR_XLINK int _tr_udp_send_to(int fd, const char* d, int l, const char* h, int p) { (void)fd;(void)d;(void)l;(void)h;(void)p; return -1; }
_TR_XLINK int _tr_udp_recv_from(int fd, char* b, int c, char* src)            { (void)fd;(void)b;(void)c;(void)src; return -1; }
_TR_XLINK void _tr_udp_close(int fd)                                           { (void)fd; }
static inline char* _tr_dns_resolve(const char* host)                              { (void)host; return (char*)""; }
static inline char* _tr_dns_reverse(const char* ip)                                { (void)ip;  return (char*)""; }
#elif defined(_WIN32)
#ifndef _TR_NET_INCLUDED
#define _TR_NET_INCLUDED
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

static inline int _tr_net_init(void) {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,2), &wsa) == 0 ? 0 : -1;
}
_TR_XLINK int _tr_tcp_connect(const char* host, int port) {
    _tr_net_init();
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    char port_buf[16]; snprintf(port_buf, sizeof(port_buf), "%d", port);
    if (getaddrinfo(host, port_buf, &hints, &res) != 0) return -1;
    SOCKET fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == INVALID_SOCKET) { freeaddrinfo(res); return -1; }
    if (connect(fd, res->ai_addr, (int)res->ai_addrlen) != 0) {
        closesocket(fd); freeaddrinfo(res); return -1;
    }
    freeaddrinfo(res);
    return (int)fd;
}
_TR_XLINK int  _tr_tcp_send(int fd, const char* data, int len) { return send((SOCKET)fd, data, len, 0); }
_TR_XLINK int  _tr_tcp_recv(int fd, char* buf, int cap)        { return recv((SOCKET)fd, buf, cap, 0); }
_TR_XLINK void _tr_tcp_close(int fd)                           { closesocket((SOCKET)fd); }

#else  /* POSIX */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

static inline int _tr_net_init(void) { return 0; }
_TR_XLINK int _tr_tcp_connect(const char* host, int port) {
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    char port_buf[16]; snprintf(port_buf, sizeof(port_buf), "%d", port);
    if (getaddrinfo(host, port_buf, &hints, &res) != 0) return -1;
    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd < 0) { freeaddrinfo(res); return -1; }
    if (connect(fd, res->ai_addr, res->ai_addrlen) != 0) {
        close(fd); freeaddrinfo(res); return -1;
    }
    freeaddrinfo(res);
    return fd;
}
_TR_XLINK int  _tr_tcp_send(int fd, const char* data, int len) { return (int)send(fd, data, (size_t)len, 0); }
_TR_XLINK int  _tr_tcp_recv(int fd, char* buf, int cap)        { return (int)recv(fd, buf, (size_t)cap, 0); }
_TR_XLINK void _tr_tcp_close(int fd)                           { close(fd); }
#endif

/* ── Platform detection ──────────────────────────────────────────────── */
_TR_XLINK bool _tr_is_windows(void) {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

/* ── Directory operations (cross-platform) ──────────────────────────── */
#if defined(TAURARO_BARE) && !defined(__wasi__)
/* Bare targets with no filesystem */
static inline int   _tr_mkdir(const char* p)     { (void)p; return -1; }
static inline int   _tr_rmdir(const char* p)     { (void)p; return -1; }
_TR_XLINK bool  _tr_dir_exists(const char* p){ (void)p; return false; }
static inline bool  _tr_is_dir(const char* p)    { (void)p; return false; }
static inline bool  _tr_is_file(const char* p)   { (void)p; return false; }
_TR_XLINK void* _tr_opendir(const char* p)   { (void)p; return NULL; }
_TR_XLINK char* _tr_readdir(void* h)         { (void)h; return strdup(""); }
_TR_XLINK void  _tr_closedir(void* h)        { (void)h; }
#elif defined(_WIN32)
static inline int  _tr_mkdir(const char* path)     { return CreateDirectoryA(path, NULL) ? 0 : -1; }
static inline int  _tr_rmdir(const char* path)     { return RemoveDirectoryA(path) ? 0 : -1; }
_TR_XLINK bool _tr_dir_exists(const char* path) {
    if (!path) return false;
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY));
}
_TR_XLINK bool _tr_is_dir(const char* path)  { return _tr_dir_exists(path); }
static inline bool _tr_is_file(const char* path) {
    if (!path) return false;
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}
typedef struct { HANDLE h; WIN32_FIND_DATAA ffd; int first; } _TrDir;
_TR_XLINK void* _tr_opendir(const char* path) {
    if (!path) return NULL;
    _TrDir* d = (_TrDir*)malloc(sizeof(_TrDir));
    char pat[4096]; snprintf(pat, sizeof(pat), "%s\\*", path);
    d->h = FindFirstFileA(pat, &d->ffd); d->first = 1;
    if (d->h == INVALID_HANDLE_VALUE) { free(d); return NULL; }
    return (void*)d;
}
_TR_XLINK char* _tr_readdir(void* handle) {
    _TrDir* d = (_TrDir*)handle;
    /* Declared `-> str`, so codegen wraps the result as OWNED (rc=1) and will
     * free it. Every path must therefore return heap memory — the end-of-dir
     * sentinel returns strdup("") (NOT a string literal: freeing a literal
     * corrupts the heap). */
    if (!d || d->h == INVALID_HANDLE_VALUE) return strdup("");
    if (d->first) { d->first = 0; return strdup(d->ffd.cFileName); }
    if (FindNextFileA(d->h, &d->ffd)) return strdup(d->ffd.cFileName);
    return strdup("");
}
_TR_XLINK void _tr_closedir(void* handle) {
    _TrDir* d = (_TrDir*)handle;
    if (d) { if (d->h != INVALID_HANDLE_VALUE) FindClose(d->h); free(d); }
}
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
static inline int  _tr_mkdir(const char* path)     { return mkdir(path, 0755) == 0 ? 0 : -1; }
static inline int  _tr_rmdir(const char* path)     { return rmdir(path) == 0 ? 0 : -1; }
_TR_XLINK bool _tr_dir_exists(const char* path) {
    if (!path) return false;
    struct stat st; return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}
_TR_XLINK bool _tr_is_dir(const char* path)  { return _tr_dir_exists(path); }
static inline bool _tr_is_file(const char* path) {
    if (!path) return false;
    struct stat st; return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}
_TR_XLINK void* _tr_opendir(const char* path)  { return (void*)opendir(path); }
_TR_XLINK char* _tr_readdir(void* handle) {
    DIR* d = (DIR*)handle;
    /* Always return OWNED heap (codegen frees it); strdup("") at end-of-dir,
     * never a string literal. */
    if (!d) return strdup("");
    struct dirent* e = readdir(d);
    return e ? strdup(e->d_name) : strdup("");
}
_TR_XLINK void _tr_closedir(void* handle)       { if (handle) closedir((DIR*)handle); }
#endif

/* ── File-system helpers ──────── std-tier only (remove/rename/FILE) ──── */
#ifndef TAURARO_BARE
static inline int  _tr_file_delete(const char* path)                     { return remove(path) == 0 ? 0 : -1; }
static inline int  _tr_file_rename(const char* old_p, const char* new_p) { return rename(old_p, new_p) == 0 ? 0 : -1; }
static inline long long _tr_file_size(const char* path) {
    if (!path) return -1LL;
    FILE* f = fopen(path, "rb"); if (!f) return -1LL;
    fseek(f, 0, SEEK_END); long long sz = (long long)ftell(f); fclose(f); return sz;
}
#else
static inline int  _tr_file_delete(const char* path)                     { (void)path; return -1; }
static inline int  _tr_file_rename(const char* old_p, const char* new_p) { (void)old_p; (void)new_p; return -1; }
static inline long long _tr_file_size(const char* path)                  { (void)path; return -1LL; }
#endif

/* _tr_c_memset defined above */

static inline void _tr_bounds_check(long long i, size_t len) {
    if (__builtin_expect(i < 0 || (size_t)i >= len, 0)) {
        _TR_DIAG("Index %lld out of bounds (length %zu)\n", i, len);
        _TR_TRAP();
    }
}

#ifndef _TR_GLOBAL
#ifdef _TR_MAIN
  #define _TR_GLOBAL
#else
  #define _TR_GLOBAL extern
#endif

/* Thread-local storage qualifier for per-thread exception stacks */
#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
#  define _TR_THREAD_LOCAL
#elif defined(_MSC_VER)
#  define _TR_THREAD_LOCAL __declspec(thread)
#elif defined(__GNUC__) || defined(__clang__)
#  define _TR_THREAD_LOCAL __thread
#else
#  define _TR_THREAD_LOCAL _Thread_local
#endif
#endif /* _TR_GLOBAL */

/* argc/argv made available to std.sys.env at runtime. */
_TR_GLOBAL int    _tr_argc;
_TR_GLOBAL char** _tr_argv;

static inline long long _tr_get_argc(void)       { return (long long)_tr_argc; }
/* Codegen uniformly wraps every `-> str` extern result as OWNED (rc=1,
 * _tr_str_wrap) and frees it via auto-drop (see _tr_str_dup_owned's own
 * comment above) -- but argv[n] is a borrowed pointer into the process's
 * real argv array (owned by the CRT startup code, never malloc'd by
 * Tauraro's allocator). Wrapping it directly as if owned, then later
 * releasing it, calls free() on memory nothing here allocated: a genuine
 * heap-corruption crash (STATUS_HEAP_CORRUPTION on Windows), observed via
 * `Vec[str]().push(_tr_get_arg(i))` or equivalent -- any use that hoists a
 * release-on-drop temp around the call, not just an assignment to a plain
 * local (which happens to outlive the corruption long enough to print
 * correctly once, masking the bug). _tr_str_dup_owned makes a real copy so
 * the later free() is valid, matching every other `-> str` helper's contract
 * (_tr_platform, _tr_cwd, etc.) instead of carving out a special case here. */
static inline char*     _tr_get_arg(long long n) { return _tr_str_dup_owned((_tr_argv && n >= 0 && (int)n < _tr_argc) ? _tr_argv[(int)n] : ""); }

/* ── TaskGroup: spawn threads + join all (dynamic, unlimited) ────────── */
typedef struct { _TrThread* ths; int count; int cap; } _TrTaskGroup;
_TR_GLOBAL _TrTaskGroup _tr_tg;
_TR_GLOBAL _TrThreadPool* _tr_global_async_pool;
/* Shared async worker pool, created LAZILY on first use so pure-compute
   programs (the overwhelming common case) never pay the per-core worker-
   thread stack cost (~1-2 MB/core, tens of MB on big machines). Previously
   this was spawned eagerly in every program's main(), which made even
   "hello world" allocate one stack per CPU. */
static inline _TrThreadPool* _tr_async_pool(void) {
    if (!_tr_global_async_pool) _tr_global_async_pool = _tr_threadpool_auto();
    return _tr_global_async_pool;
}
static inline void _tr_async_pool_shutdown(void) {
    if (_tr_global_async_pool) { _tr_threadpool_free(_tr_global_async_pool); _tr_global_async_pool = NULL; }
}
_TR_XLINK void _tr_tg_begin(void) {
    _tr_tg.cap = 16; _tr_tg.count = 0;
    _tr_tg.ths = (_TrThread*)TAURARO_ALLOC((size_t)_tr_tg.cap * sizeof(_TrThread));
}
_TR_XLINK void _tr_tg_push(_TrThread t) {
    if (_tr_tg.count >= _tr_tg.cap) {
        _tr_tg.cap *= 2;
        _tr_tg.ths = (_TrThread*)TAURARO_REALLOC(_tr_tg.ths, (size_t)_tr_tg.cap * sizeof(_TrThread));
    }
    _tr_tg.ths[_tr_tg.count++] = t;
}
_TR_XLINK void _tr_taskgroup_wait(void) {
    for (int i = 0; i < _tr_tg.count; i++) _tr_thread_join_wait(_tr_tg.ths[i]);
    if (_tr_tg.ths) { TAURARO_FREE(_tr_tg.ths); _tr_tg.ths = NULL; }
    _tr_tg.count = 0; _tr_tg.cap = 0;
}

/* ── AsyncTaskGroup: `await_all`'s pool-backed replacement for _TrTaskGroup ─
 * Exact mirror of _TrTaskGroup/_tr_tg_* above, but collecting `_TrAsyncTask*`
 * handles (submitted to the shared work-stealing AsyncPool, see above)
 * instead of raw `_TrThread` handles - `await_all(f1(), f2(), ...)` used
 * to spawn one throwaway OS thread PER SUB-CALL (unbounded: 1000 sub-calls
 * = 1000 OS threads); now every sub-call is a stealable task on a small,
 * reused, core-sized worker pool instead. Same single-shared-global
 * pattern as `_tr_tg` (so, same pre-existing constraint: an `await_all`
 * cannot nest inside another `await_all` on the same thread - unchanged
 * from before this migration, not a new limitation). */
typedef struct { _TrAsyncTask** tasks; int count; int cap; } _TrAsyncTaskGroup;
_TR_GLOBAL _TrAsyncTaskGroup _tr_atg;

_TR_XLINK void _tr_atg_begin(void) {
    _tr_atg.cap = 16; _tr_atg.count = 0;
    _tr_atg.tasks = (_TrAsyncTask**)TAURARO_ALLOC((size_t)_tr_atg.cap * sizeof(_TrAsyncTask*));
}
_TR_XLINK void _tr_atg_push(_TrAsyncTask* t) {
    if (_tr_atg.count >= _tr_atg.cap) {
        _tr_atg.cap *= 2;
        _tr_atg.tasks = (_TrAsyncTask**)TAURARO_REALLOC(_tr_atg.tasks, (size_t)_tr_atg.cap * sizeof(_TrAsyncTask*));
    }
    _tr_atg.tasks[_tr_atg.count++] = t;
}
/* Submits fn(arg) onto the shared default AsyncPool and tracks the
 * resulting handle for `_tr_atg_wait` - the one-call-site convenience
 * `await_all`'s codegen actually uses (push+spawn combined). */
_TR_XLINK void _tr_atg_spawn(void*(*fn)(void*), void* arg) {
    _tr_atg_push(_tr_asyncpool_spawn(_tr_asyncpool_default(), fn, arg));
}
_TR_XLINK void _tr_atg_wait(void) {
    for (int i = 0; i < _tr_atg.count; i++) {
        _tr_asynctask_await(_tr_atg.tasks[i]);
        _tr_asynctask_free(_tr_atg.tasks[i]);
    }
    if (_tr_atg.tasks) { TAURARO_FREE(_tr_atg.tasks); _tr_atg.tasks = NULL; }
    _tr_atg.count = 0; _tr_atg.cap = 0;
}

/* ── Per-thread panic state (storage definitions for _TR_MAIN TU) ─── */
#if !defined(TAURARO_BARE) && !defined(TAURARO_KERNEL) && !defined(TAURARO_NO_THREADS)
_TR_GLOBAL _TR_THREAD_LOCAL int     _tr_thread_has_panic_buf;
_TR_GLOBAL _TR_THREAD_LOCAL jmp_buf _tr_thread_panic_jmpbuf;
_TR_GLOBAL _TR_THREAD_LOCAL char*   _tr_thread_panic_message;
#endif

/* ── Exception stack (setjmp/longjmp based, per-await-chain) ─────────────
 * Uniformly backed by `_TrExcChain` and `_tr_exc_chain_get()` (both
 * declared earlier, above `_tr_excchain_new`/near `_tr_g`), whether the
 * current context is "inside a coroutine" or plain top-level/OS-thread
 * code. This one unification is what makes an exception raised inside an
 * `await`ed call correctly reach a `try` in a NON-coroutine caller (e.g.
 * `async def main()`'s own top-level try/except, which runs as plain C
 * code, not as a coroutine - `_tr_co_go` inherits whatever chain
 * `_tr_exc_chain_get()` returns at spawn time, root chain included) as
 * well as a `try` in an ancestor coroutine several `await` levels up, and
 * survives a work-stealing pool resuming any one member of that chain on
 * a different OS thread than where it suspended (the chain object itself
 * is just heap memory, not thread-local). */

static void _tr_exc_push(jmp_buf* b, char** m) {
    _TrExcChain* ch = _tr_exc_chain_get();
    if (ch->sp < _TR_MAX_EXC) {
#ifdef _TR_HAS_CORO
        ch->owner[ch->sp] = _tr_g.current;
#endif
        ch->bufs[ch->sp] = b; ch->msgs[ch->sp] = m; ch->sp++;
    }
}
static void _tr_exc_pop(void) {
    _TrExcChain* ch = _tr_exc_chain_get();
    if (ch->sp > 0) ch->sp--;
}
_TR_XLINK void _tr_exc_raise(char* msg) {
    _TrExcChain* ch = _tr_exc_chain_get();
    if (ch->sp > 0) {
#ifdef _TR_HAS_CORO
        /* The top handler belongs to `ch->owner[ch->sp - 1]` (the
         * coroutine - or NULL for plain/root context - that pushed it),
         * which may differ from whoever is currently raising: `await`ing
         * a call inside a `try` means the CALLEE raises but the AWAITER
         * owns the handler.
         *
         * If they differ, we must NOT `longjmp` directly into the owner:
         * on Windows, a raw `longjmp` only restores the stack pointer/
         * registers - it does NOT call `SwitchToFiber`, so Windows' own
         * internal "current fiber" bookkeeping (used by every later
         * SwitchToFiber/GetCurrentFiber/DeleteFiber call) goes stale the
         * moment execution resumes on a DIFFERENT fiber's stack than the
         * one Windows still thinks is active - confirmed to crash with
         * STATUS_INVALID_HANDLE. (POSIX ucontext has no equivalent
         * bookkeeping, so this specific hazard is Windows-only, but the
         * fix below is platform-uniform since it never crosses a fiber
         * boundary via longjmp on ANY platform.)
         *
         * Fix: don't jump there directly. Instead, hand off to our OWN
         * joiner exactly like a normal (successful) coroutine completion
         * already does - `_tr_co_to_sched` is a plain, symmetric fiber
         * switch, never a longjmp, so it is always safe. Mark ourselves
         * `failed` with the message; the joiner (see `_tr_co_await`'s own
         * `target->failed` check) notices this once it resumes and calls
         * `_tr_exc_raise` AGAIN - but that second call runs while the
         * joiner is genuinely executing on ITS OWN fiber, so if it turns
         * out to own the handler, popping and `longjmp`ing right there is
         * the ordinary same-fiber case, already proven safe. If the
         * joiner doesn't own it either, its own call to this same
         * function repeats the exact same hand-off one hop further -
         * telescoping outward through the await chain one safe,
         * already-resumed fiber at a time, never jumping across one. */
        if (_tr_g.current && ch->owner[ch->sp - 1] != _tr_g.current) {
            _TrCoro* c = _tr_g.current;
            c->failed = 1;
            c->fail_msg = msg;
            /* If someone formally awaited us (the "inside a coroutine"
             * branch of `_tr_co_await` sets `target->joiner`), requeue
             * them so the scheduler redispatches them and they notice
             * `failed`. A ROOT-level await (main()'s own top-level
             * `await`, not itself a coroutine) never sets `joiner` at all
             * - it polls `target->state`/`target->failed` directly in its
             * own loop instead, so there is nothing to requeue in that
             * case, but the switch-back below still reaches it: control
             * returns to whichever `_tr_sched_step()` call dispatched us,
             * which for a root await is that very polling loop. Either
             * way, `_tr_co_to_sched` is a plain fiber switch, never a
             * longjmp, so this hand-off is always safe. */
            if (c->joiner) {
                _TrCoro* j = c->joiner;
                c->joiner = NULL;
                _tr_rpush(j);
            }
            /* KNOWN LIMITATION (pre-existing, not introduced or fixed by
             * this change): `c` itself is now abandoned rather than freed
             * - its stack/fiber leaks. Freeing it safely needs a real
             * unwind-and-cleanup pass, not just this hand-off; out of
             * scope here since it trades a leak for a crash if gotten
             * wrong. Separate follow-up work.
             * A TRULY detached coroutine (`Coro.spawn`, no joiner AND no
             * root/ancestor loop directly polling it) has no watcher at
             * all here, so the exception is effectively dropped rather
             * than escalated - a narrower, accepted tradeoff alongside
             * the leak above, distinct from the `await` case this fix
             * targets (which always has SOMEONE polling, joiner or root). */
            _tr_co_to_sched(c);
            return; /* only reached if `c` is ever redispatched, which a
                     * `failed` coroutine never is - defensive only */
        } else {
            /* Same-coroutine (or same root, non-coroutine) unwind: the
             * ordinary, always-safe case - pop and longjmp locally. */
            ch->sp--;
            *ch->msgs[ch->sp] = msg;
            _tr_g.current = ch->owner[ch->sp];
            longjmp(*ch->bufs[ch->sp], 1);
        }
#else
        ch->sp--;
        *ch->msgs[ch->sp] = msg;
        longjmp(*ch->bufs[ch->sp], 1);
#endif
    }
    /* No user try-handler anywhere in this chain: prefer the chain's OWN
     * panic handler if one has been installed (set by a pool worker
     * wrapping one task's run, so a stolen chain's panic boundary stays
     * with the chain, not whichever OS thread happens to run it). Falls
     * through to the CURRENT OS thread's own panic buf otherwise -
     * unchanged behavior for plain/root-chain code and for a coroutine
     * driven from inside a Thread.spawn'd function via the top-level
     * `_tr_co_await` pump, exactly as before this whole section existed.
     * NARROWER, STILL-OPEN HAZARD (pre-existing, not touched by this fix):
     * `_tr_thread_panic_jmpbuf` is captured via `setjmp` on a `Thread.
     * spawn`'d function's ORIGINAL (pre-fiber-conversion) stack. If a
     * DETACHED coroutine created from within that function is the one
     * escalating here (the `ch->has_panic_buf` chain-level check above
     * covers the common case; this is the thread-local fallback beneath
     * it), this `longjmp` would cross the same kind of fiber boundary the
     * rest of this function was rewritten to avoid. Not fixed here: the
     * reported/reproduced bug this change targets is specifically the
     * `try`/`except` + `await` case, fully fixed above; this narrower
     * Thread.spawn+detached-coroutine+panic-buf edge case is a separate,
     * still-open follow-up. */
    if (ch->has_panic_buf) {
        ch->panic_message = msg;
        longjmp(ch->panic_jmpbuf, 1);
    }
    if (_tr_thread_has_panic_buf) {
        _tr_thread_panic_message = msg;
        longjmp(_tr_thread_panic_jmpbuf, 1);
    }
    _TR_DIAG("Unhandled exception: %s\n", msg ? msg : "(null)");
    _TR_TRAP();
}

/* ── String helpers ─────────────────────────────────────────────────── */

static char* _tr_str_concat(const char* a, const char* b) {
    if (!a) a=""; if (!b) b="";
    size_t la=strlen(a), lb=strlen(b);
    char* r=(char*)TAURARO_ALLOC(la+lb+1);
    memcpy(r,a,la); memcpy(r+la,b,lb+1);
    return r;
}
/* TrStr-returning variant: same semantics, refcounted result (rc=1). */
static inline TrStr _tr_strx_concat(const char* a, const char* b) {
    if (!a) a=""; if (!b) b="";
    size_t la=strlen(a), lb=strlen(b);
    TrStr r = _tr_str_new(la+lb);
    memcpy(r.data, a, la); memcpy(r.data+la, b, lb);
    return r;
}
static char* _tr_str_upper(const char* s) {
    if (!s) return _tr_empty_heap_str();
    char* r=(char*)TAURARO_ALLOC(strlen(s)+1);
    for (int i=0; (r[i]=(char)toupper((unsigned char)s[i])) || s[i]; i++);
    return r;
}
static char* _tr_str_lower(const char* s) {
    if (!s) return _tr_empty_heap_str();
    char* r=(char*)TAURARO_ALLOC(strlen(s)+1);
    for (int i=0; (r[i]=(char)tolower((unsigned char)s[i])) || s[i]; i++);
    return r;
}
_TR_XLINK bool _tr_str_contains(const char* s, const char* sub) {
    return s && sub && strstr(s, sub) != NULL;
}
static bool _tr_str_starts_with(const char* s, const char* pre) {
    return s && pre && strncmp(s, pre, strlen(pre)) == 0;
}
static bool _tr_str_ends_with(const char* s, const char* suf) {
    if (!s||!suf) return false;
    size_t sl=strlen(s), sufl=strlen(suf);
    return sl>=sufl && strcmp(s+sl-sufl,suf)==0;
}
static char* _tr_str_strip(const char* s) {
    if (!s) return _tr_empty_heap_str();
    while (isspace((unsigned char)*s)) s++;
    if (!*s) { char* e=(char*)TAURARO_ALLOC(1); *e='\0'; return e; }
    const char* end = s+strlen(s)-1;
    while (end>s && isspace((unsigned char)*end)) end--;
    size_t len=(size_t)(end-s+1);
    char* r=(char*)TAURARO_ALLOC(len+1); memcpy(r,s,len); r[len]='\0'; return r;
}
/* pad_left = right-justify (spaces on the left); pad_right = left-justify. */
static char* _tr_str_pad_left(const char* s, long long w) {
    if (!s) s = "";
    long long n = (long long)strlen(s);
    if (n >= w) { char* c=(char*)TAURARO_ALLOC((size_t)n+1); memcpy(c,s,(size_t)n+1); return c; }
    long long pad = w - n;
    char* r = (char*)TAURARO_ALLOC((size_t)w+1);
    for (long long i=0;i<pad;i++) r[i]=' ';
    memcpy(r+pad, s, (size_t)n); r[w]='\0'; return r;
}
static char* _tr_str_pad_right(const char* s, long long w) {
    if (!s) s = "";
    long long n = (long long)strlen(s);
    if (n >= w) { char* c=(char*)TAURARO_ALLOC((size_t)n+1); memcpy(c,s,(size_t)n+1); return c; }
    char* r = (char*)TAURARO_ALLOC((size_t)w+1);
    memcpy(r, s, (size_t)n);
    for (long long i=n;i<w;i++) r[i]=' ';
    r[w]='\0'; return r;
}
static char* _tr_str_replace(const char* s, const char* old, const char* nw) {
    if (!s||!old||!nw) return (char*)s;
    size_t sl=strlen(s), ol=strlen(old), nl=strlen(nw);
    int cnt=0; const char* p=s;
    while ((p=strstr(p,old))) { cnt++; p+=ol; }
    char* r=(char*)TAURARO_ALLOC(sl+(size_t)cnt*(nl>ol?nl-ol:0)+1);
    char* dst=r; p=s;
    while (*p) {
        if (strncmp(p,old,ol)==0) { memcpy(dst,nw,nl); dst+=nl; p+=ol; }
        else { *dst++=*p++; }
    }
    *dst='\0'; return r;
}
_TR_XLINK char* _tr_int_to_str(long long n)   { char* b=(char*)TAURARO_ALLOC(32); snprintf(b,32,"%lld",n); return b; }
/* Thousands grouping for f"{n:,d}": 1234567 -> "1,234,567" (fresh heap C string). */
static char* _tr_i64_grouped(long long v) {
    char t[24]; int n=0, neg=v<0;
    unsigned long long u = neg ? 0ull-(unsigned long long)v : (unsigned long long)v;
    do { t[n++]=(char)('0'+(u%10)); u/=10; } while (u);
    char* b=(char*)TAURARO_ALLOC(40); int w=0;
    if (neg) b[w++]='-';
    for (int i=n-1,c=0;i>=0;i--,c++) { if (c&&c%3==0) b[w++]=','; b[w++]=t[i]; }
    b[w]=0; return b;
}
_TR_XLINK char* _tr_float_to_str(double n)    { char* b=(char*)TAURARO_ALLOC(32); snprintf(b,32,"%g",n);   return b; }
_TR_XLINK char* _tr_float_to_c_lit(double n) {
    char* b=(char*)TAURARO_ALLOC(32);
    int len = snprintf(b,32,"%.17g",n);
    /* %g on whole numbers (e.g. 7.0 -> "7") drops any marker that tells the C
     * compiler this is a floating-point literal, so "7 / 2" would silently
     * become integer division. Append ".0" when no '.', exponent, or
     * inf/nan marker is present. */
    int has_marker = 0;
    for (int i = 0; i < len; i++) {
        char c = b[i];
        if (c=='.' || c=='e' || c=='E' || c=='n' || c=='N' || c=='i' || c=='I') { has_marker = 1; break; }
    }
    if (!has_marker) { b[len]='.'; b[len+1]='0'; b[len+2]='\0'; }
    return b;
}
static char* _tr_bool_to_str(bool b)       { return b ? "true" : "false"; }

/* _TR_AUTO_STR — convert any scalar to char* for f-string / print with unknown type.
 * Uses _Generic so __auto_type variables work without an explicit type annotation.
 * Each branch is a distinct typed helper to avoid cross-type implicit-cast errors. */
static inline char* _tr__ll_s(long long x)          { return _tr_int_to_str(x); }
static inline char* _tr__ull_s(unsigned long long x) { return _tr_int_to_str((long long)x); }
static inline char* _tr__i32_s(int x)               { return _tr_int_to_str((long long)x); }
static inline char* _tr__u32_s(unsigned int x)       { return _tr_int_to_str((long long)x); }
static inline char* _tr__i16_s(short x)              { return _tr_int_to_str((long long)x); }
static inline char* _tr__u16_s(unsigned short x)     { return _tr_int_to_str((long long)x); }
static inline char* _tr__i8_s(signed char x)         { return _tr_int_to_str((long long)x); }
static inline char* _tr__u8_s(unsigned char x)       { return _tr_int_to_str((long long)x); }
static inline char* _tr__dbl_s(double x)             { return _tr_float_to_str(x); }
static inline char* _tr__flt_s(float x)              { return _tr_float_to_str((double)x); }
static inline char* _tr__bool_s(bool x)              { return x ? "true" : "false"; }
static inline char* _tr__ptr_s(void* x)              { return (char*)x; }
static inline char* _tr__trstr_s(TrStr x)            { return x.data; }
#define _TR_AUTO_STR(x) _Generic((x), \
    long long:          _tr__ll_s,  \
    unsigned long long: _tr__ull_s, \
    int:                _tr__i32_s, \
    unsigned int:       _tr__u32_s, \
    short:              _tr__i16_s, \
    unsigned short:     _tr__u16_s, \
    signed char:        _tr__i8_s,  \
    unsigned char:      _tr__u8_s,  \
    double:             _tr__dbl_s, \
    float:              _tr__flt_s, \
    bool:               _tr__bool_s,\
    TrStr:              _tr__trstr_s,\
    default:            _tr__ptr_s  \
)(x)
static long long _tr_str_to_int(const char* s) { return s ? strtoll(s,NULL,10) : 0LL; }
static double    _tr_str_to_float(const char* s){ return s ? strtod(s,NULL) : 0.0; }
_TR_XLINK long long _tr_strlen(char* s)     { return s ? (long long)strlen(s) : 0LL; }

/* ── String equality ─────────────────────────────────────────────────── */
_TR_XLINK bool _tr_str_eq(const char* a, const char* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return strcmp(a, b) == 0;
}

/* ── String slice (alias for _tr_str_substring) ─────────────────────── */
/* These str helpers are all declared `-> str`, so codegen wraps their result as
 * owned (rc=1) and will free it — every return path must be heap, NEVER a string
 * literal. Use the canonical _tr_empty_heap_str() (defined near _tr_checked_alloc)
 * for the empty-result fallback (freeing a literal corrupts the heap; this is the
 * ownership-lie class that blocked MIR completion of fns that drop these). */
_TR_XLINK char* _tr_str_slice(const char* s, long long start, long long end) {
    if (!s) return _tr_empty_heap_str();
    long long len = (long long)strlen(s);
    if (start < 0) start = 0;
    if (end > len) end = len;
    if (start >= end) return _tr_empty_heap_str();
    long long sz = end - start;
    char* out = (char*)_tr_checked_alloc(sz + 1);
    memcpy(out, s + start, (size_t)sz);
    out[sz] = '\0';
    return out;
}

/* ── Additional string helpers ───────────────────────────────────────── */
static inline char* _tr_str_trim_left(const char* s) {
    if (!s) return _tr_empty_heap_str();
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    size_t n = strlen(s); char* r = (char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1); return r;
}
static inline char* _tr_str_trim_right(const char* s) {
    if (!s) return _tr_empty_heap_str();
    size_t n = strlen(s); const char* e = s+n-1;
    while (n > 0 && (*e==' '||*e=='\t'||*e=='\n'||*e=='\r')) { e--; n--; }
    char* r = (char*)_tr_checked_alloc(n+1); memcpy(r,s,n); r[n]='\0'; return r;
}
static inline char* _tr_str_capitalize(const char* s) {
    if (!s||!*s) return _tr_empty_heap_str();
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1);
    r[0]=(char)toupper((unsigned char)r[0]); for(size_t i=1;i<n;i++) r[i]=(char)tolower((unsigned char)r[i]);
    return r;
}
static inline char* _tr_str_title(const char* s) {
    if (!s) return _tr_empty_heap_str();
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1);
    bool ws=true;
    for(size_t i=0;i<n;i++){
        if(r[i]==' '||r[i]=='\t'||r[i]=='\n'){ws=true;}
        else if(ws){r[i]=(char)toupper((unsigned char)r[i]);ws=false;}
        else{r[i]=(char)tolower((unsigned char)r[i]);}
    }
    return r;
}
static inline char* _tr_str_reverse(const char* s) {
    if (!s) return _tr_empty_heap_str();
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1);
    for(size_t i=0;i<n;i++) r[i]=s[n-1-i]; r[n]='\0'; return r;
}
static inline char* _tr_str_repeat(const char* s, long long times) {
    if (!s||times<=0) { char* e=(char*)_tr_checked_alloc(1); e[0]='\0'; return e; }
    size_t slen=strlen(s); size_t total=(size_t)times*slen;
    char* r=(char*)_tr_checked_alloc(total+1); r[0]='\0';
    for(long long i=0;i<times;i++) memcpy(r+i*slen, s, slen);
    r[total]='\0'; return r;
}
/* TrStr-returning variant: same semantics, refcounted result (rc=1). */
static inline TrStr _tr_strx_repeat(const char* s, long long times) {
    if (!s||times<=0) { return _tr_str_new(0); }
    size_t slen=strlen(s); size_t total=(size_t)times*slen;
    TrStr r = _tr_str_new(total);
    for(long long i=0;i<times;i++) memcpy(r.data+i*slen, s, slen);
    return r;
}
static inline char* _tr_str_replace_first(const char* s, const char* old_s, const char* new_s) {
    if (!s||!old_s||!new_s) return s ? (char*)s : (char*)"";
    const char* p=strstr(s,old_s); if(!p) { size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1); return r; }
    size_t ol=strlen(old_s), nl=strlen(new_s), pre=(size_t)(p-s), sl=strlen(s);
    char* r=(char*)_tr_checked_alloc(sl-ol+nl+1);
    memcpy(r,s,pre); memcpy(r+pre,new_s,nl); memcpy(r+pre+nl,p+ol,sl-pre-ol+1); return r;
}
static inline char* _tr_str_strip_prefix(const char* s, const char* pre) {
    if (!s||!pre) return s?(char*)s:(char*)"";
    size_t pl=strlen(pre);
    if (strncmp(s,pre,pl)==0) { size_t n=strlen(s)-pl; char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s+pl,n+1); return r; }
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1); return r;
}
static inline char* _tr_str_strip_suffix(const char* s, const char* suf) {
    if (!s||!suf) return s?(char*)s:(char*)"";
    size_t sl=strlen(s), sufl=strlen(suf);
    if (sl>=sufl && strcmp(s+sl-sufl,suf)==0) { char* r=(char*)_tr_checked_alloc(sl-sufl+1); memcpy(r,s,sl-sufl); r[sl-sufl]='\0'; return r; }
    char* r=(char*)_tr_checked_alloc(sl+1); memcpy(r,s,sl+1); return r;
}
static inline char* _tr_str_remove_char(const char* s, const char* ch) {
    if (!s||!ch||!*ch) return s?(char*)s:(char*)"";
    char c=ch[0]; size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); size_t j=0;
    for(size_t i=0;i<n;i++) if(s[i]!=c) r[j++]=s[i]; r[j]='\0'; return r;
}
static inline long long _tr_str_index_of(const char* s, const char* sub) {
    if (!s||!sub) return -1LL;
    const char* p=strstr(s,sub); return p ? (long long)(p-s) : -1LL;
}
static inline long long _tr_str_last_index_of(const char* s, const char* sub) {
    if (!s||!sub||!*sub) return -1LL;
    size_t sl=strlen(s), subl=strlen(sub); long long last=-1LL;
    for(size_t i=0;i+subl<=sl;i++) if(strncmp(s+i,sub,subl)==0) last=(long long)i;
    return last;
}
static inline long long _tr_str_count_occ(const char* s, const char* sub) {
    if (!s||!sub||!*sub) return 0LL;
    size_t subl=strlen(sub); long long c=0; const char* p=s;
    while((p=strstr(p,sub))!=NULL){c++;p+=subl;} return c;
}
static inline long long _tr_str_char_at_code(const char* s, long long i) {
    if (!s) return -1LL; long long n=(long long)strlen(s);
    if (i<0||i>=n) return -1LL; return (long long)(unsigned char)s[i];
}
static inline bool _tr_str_contains_char(const char* s, long long c) {
    if (!s) return false; return strchr(s,(char)c)!=NULL;
}
static inline bool _tr_str_parse_bool(const char* s) {
    if (!s) return false;
    return strcmp(s,"true")==0||strcmp(s,"1")==0||strcmp(s,"yes")==0;
}
static inline bool _tr_str_is_digit(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isdigit((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_alpha(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isalpha((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_alnum(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isalnum((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_space(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isspace((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_upper(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(isalpha((unsigned char)*p)&&!isupper((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_lower(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(isalpha((unsigned char)*p)&&!islower((unsigned char)*p)) return false; return true;
}
/* _tr_str_lines and _tr_str_words defined after _tr_str_split below */
static inline char* _tr_str_lpad(const char* s, long long width, const char* pad) {
    if (!s) s=""; if (!pad||!*pad) pad=" ";
    long long slen=(long long)strlen(s); if(slen>=width){size_t n=strlen(s);char*r=(char*)_tr_checked_alloc(n+1);memcpy(r,s,n+1);return r;}
    long long plen=width-slen; char* r=(char*)_tr_checked_alloc((size_t)(plen+slen+1));
    for(long long i=0;i<plen;i++) r[i]=pad[0]; memcpy(r+plen,s,(size_t)slen); r[plen+slen]='\0'; return r;
}
static inline char* _tr_str_rpad(const char* s, long long width, const char* pad) {
    if (!s) s=""; if (!pad||!*pad) pad=" ";
    long long slen=(long long)strlen(s); if(slen>=width){size_t n=strlen(s);char*r=(char*)_tr_checked_alloc(n+1);memcpy(r,s,n+1);return r;}
    long long plen=width-slen; char* r=(char*)_tr_checked_alloc((size_t)(plen+slen+1));
    memcpy(r,s,(size_t)slen); for(long long i=0;i<plen;i++) r[slen+i]=pad[0]; r[slen+plen]='\0'; return r;
}
static inline char* _tr_str_center(const char* s, long long width) {
    if (!s) s="";
    long long slen=(long long)strlen(s); if(slen>=width){size_t n=strlen(s);char*r=(char*)_tr_checked_alloc(n+1);memcpy(r,s,n+1);return r;}
    long long total=width-slen, left=total/2, right=total-left;
    char* r=(char*)_tr_checked_alloc((size_t)(width+1));
    for(long long i=0;i<left;i++) r[i]=' '; memcpy(r+left,s,(size_t)slen); for(long long i=0;i<right;i++) r[left+slen+i]=' '; r[width]='\0'; return r;
}
/* s.zfill(w): left-pad with '0' to width; a leading sign stays first ("-42".zfill(5)="-0042"). */
static inline char* _tr_str_zfill(const char* s, long long width) {
    if (!s) s = "";
    long long n = (long long)strlen(s);
    if (n >= width) { char* c=(char*)_tr_checked_alloc((size_t)n+1); memcpy(c,s,(size_t)n+1); return c; }
    long long pad = width - n, si = 0;
    char* r = (char*)_tr_checked_alloc((size_t)width + 1); long long p = 0;
    if (n > 0 && (s[0]=='-' || s[0]=='+')) { r[p++] = s[0]; si = 1; }
    for (long long i = 0; i < pad; i++) r[p++] = '0';
    for (long long i = si; i < n; i++) r[p++] = s[i];
    r[width] = '\0'; return r;
}

/* ── Char code → 1-char string ───────────────────────────────────────── */
static inline char* _tr_char_to_str(long long code) {
    char* s = (char*)_tr_checked_alloc(2);
    s[0] = (char)(code & 0xFF);
    s[1] = '\0';
    return s;
}
static inline char* _tr_char_to_str_alloc(long long code) { return _tr_char_to_str(code); }

/* ── Shell command execution ─────────────────────────────────────────── */
#ifdef TAURARO_BARE
_TR_XLINK int _tr_system(const char* cmd) { (void)cmd; return -1; }
#else
_TR_XLINK int _tr_system(const char* cmd) { return system(cmd); }
#endif

/* ── Panic / error ───────────────────────────────────────────────────── */
static inline void _tr_panic(const char* msg) {
    if (_tr_thread_has_panic_buf) {
        /* In a spawned thread: unwind to thread boundary, not _TR_TRAP() */
        _tr_thread_panic_message = (char*)msg;
        longjmp(_tr_thread_panic_jmpbuf, 1);
    }
    _TR_DIAG("panic: %s\n", msg ? msg : "(null)");
    _TR_TRAP();
}

/* ── Generic contains (for `in` operator on strings) ────────────────── */
static inline bool _tr_contains(const char* haystack, const char* needle) {
    return haystack && needle && strstr(haystack, needle) != NULL;
}

/* ── Range iteration helper ──────────────────────────────────────────── */
/* Note: range() on for-loops is compiled to C for() loops directly.     */
/* This stub satisfies any residual reference in generic code paths.     */
static inline long long _tr_range(long long start, long long end, long long step) {
    (void)start; (void)end; (void)step;
    return 0LL;
}

/* ── Dict (hash map: str → void*) ───────────────────────────────────── */

typedef struct _DictNode { char* key; void* value; struct _DictNode* next; } _DictNode;
typedef struct { _DictNode** buckets; size_t cap; size_t len; } Dict;

static size_t _dict_hash(const char* k, size_t cap) {
    size_t h=5381; unsigned char c;
    while ((c=(unsigned char)*k++)) h=h*33+c;
    return h%cap;
}
static Dict* Dict_new(void) {
    Dict* d=(Dict*)malloc(sizeof(Dict)); _TR_MEMCOUNT_INC(); _TR_MEMCOUNT_DICT_INC();
    /* Lazy buckets: an empty dict allocates no bucket array (cap=0). The 16-slot
       array is created on first insert. Saves one alloc per dict that stays
       empty - e.g. an HttpRequest's headers/params maps when a handler reads no
       headers and the route binds no params. All accessors below guard cap==0. */
    d->cap=0; d->len=0; d->buckets=NULL;
    return d;
}
static void Dict_set(Dict* d, char* key, void* val) {
    if (!d || !key) return;
    if (d->cap==0) { d->cap=16; d->buckets=(_DictNode**)TAURARO_CALLOC(16,sizeof(_DictNode*)); _TR_MEMCOUNT_INC(); }
    size_t i=_dict_hash(key,d->cap);
    _DictNode* n=d->buckets[i];
    while (n) { if (strcmp(n->key,key)==0) { n->value=val; return; } n=n->next; }
    _DictNode* nd=(_DictNode*)malloc(sizeof(_DictNode)); _TR_MEMCOUNT_INC();
    nd->key=strdup(key); _TR_MEMCOUNT_INC(); nd->value=val; nd->next=d->buckets[i]; d->buckets[i]=nd; d->len++;
}
static void*     Dict_get(Dict* d, char* key) {
    if (!d||!key||d->cap==0) return NULL;
    size_t i=_dict_hash(key,d->cap);
    _DictNode* n=d->buckets[i];
    while (n) { if (strcmp(n->key,key)==0) return n->value; n=n->next; }
    return NULL;
}
/* Key PRESENCE — must walk the chain, NOT test the value. A key stored with a
 * value that is NULL / (void*)0 (e.g. a `Dict[K,bool]`/`Map[K,bool]` holding
 * `false`, or a set element) is still present; `Dict_get(...)!=NULL` wrongly
 * reported it absent, silently breaking every false-valued dict (this is what
 * made the compiler's own `consumes(fn,i)` summary invisible). */
static bool      Dict_has(Dict* d, char* key) {
    if (!d||!key||d->cap==0) return false;
    size_t i=_dict_hash(key,d->cap);
    _DictNode* n=d->buckets[i];
    while (n) { if (strcmp(n->key,key)==0) return true; n=n->next; }
    return false;
}
static long long Dict_len(Dict* d)  { return d?(long long)d->len:0LL; }
static void      Dict_remove(Dict* d, char* key) {
    if (!d||!key||d->cap==0) return;
    size_t i=_dict_hash(key,d->cap);
    _DictNode* n=d->buckets[i]; _DictNode* prev=NULL;
    while (n) {
        if (strcmp(n->key,key)==0) {
            if (prev) prev->next=n->next; else d->buckets[i]=n->next;
            if (n->key) _tr_free(n->key); _tr_free(n);
            if (d->len>0) d->len--;
            return;
        }
        prev=n; n=n->next;
    }
}
static void      Dict_free(Dict* d) {
    if (!d) return;
    _TR_MEMCOUNT_DICT_DEC();
    for (size_t i=0; i<d->cap; i++) {
        _DictNode* n=d->buckets[i];
        while (n) { _DictNode* nx=n->next; if(n->key) _tr_free(n->key); _tr_free(n); n=nx; }
    }
    _tr_free(d->buckets); _tr_free(d);
}
/* Like Dict_free(), but for Dict[K,str]/Map[K,str] whose values are
   _tr_str_box(TrStr)-allocated boxes (#54): unbox+release the TrStr, then
   free the box itself, before freeing the node/key/buckets/struct. */
static void      Dict_free_strval(Dict* d) {
    if (!d) return;
    _TR_MEMCOUNT_DICT_DEC();
    for (size_t i=0; i<d->cap; i++) {
        _DictNode* n=d->buckets[i];
        while (n) {
            _DictNode* nx=n->next;
            if(n->key) _tr_free(n->key);
            if(n->value) { _tr_str_release(*(TrStr*)n->value); _tr_free(n->value); }
            _tr_free(n);
            n=nx;
        }
    }
    _tr_free(d->buckets); _tr_free(d);
}
/* Dict[str, HeapClass]: values are owned refcounted instances (void*); the dict
   co-owns each (insert-retain), so release each before freeing nodes/keys/struct. */
static void      Dict_free_objval(Dict* d, void(*drop)(void*)) {
    if (!d) return;
    _TR_MEMCOUNT_DICT_DEC();
    for (size_t i=0; i<d->cap; i++) {
        _DictNode* n=d->buckets[i];
        while (n) { _DictNode* nx=n->next; if(n->key) _tr_free(n->key); _tr_obj_release(n->value, drop); _tr_free(n); n=nx; }
    }
    _tr_free(d->buckets); _tr_free(d);
}
/* Free all entries (and their key strings) but keep the Dict struct itself
   alive and reusable - used by clear(), unlike Dict_free() which also frees
   the struct (would otherwise leave m a dangling pointer after clear()). */
static void      Dict_clear_entries(Dict* d) {
    if (!d) return;
    for (size_t i=0; i<d->cap; i++) {
        _DictNode* n=d->buckets[i];
        while (n) { _DictNode* nx=n->next; if(n->key) _tr_free(n->key); _tr_free(n); n=nx; }
        d->buckets[i]=NULL;
    }
    d->len=0;
}

typedef Dict TrMap;
static inline TrMap* _tr_dict_new(long long cap) { (void)cap; return Dict_new(); }
static inline void   _tr_dict_set_impl(TrMap* d, char* k, void* v) { if(d) Dict_set(d,k,v); }
/* Macro: casts any type (pointer, bool, int) safely through uintptr_t to void* */
#define _tr_dict_set(d, k, v) _tr_dict_set_impl((d), (k), (void*)(uintptr_t)(v))
static inline void*  _tr_dict_get(TrMap* d, char* k) { return d?Dict_get(d,k):NULL; }
static inline bool   _tr_dict_contains(TrMap* d, char* k) { return d&&Dict_has(d,k); }
static inline void _tr_dict_remove_fn(TrMap* d, char* k) { if (d) Dict_remove(d, k); }
#define _tr_dict_remove(d, k) _tr_dict_remove_fn((d), (k))
static inline long long _tr_dict_len(TrMap* d) { return Dict_len(d); }

/* Map.update / Map.clear / Set[T] defined after List_str below */

/* ── Int-keyed Dict (Dict[int, V]) ────────────────────────────────────── */
typedef struct _TrIDictNode { long long key; void* value; struct _TrIDictNode* next; } _TrIDictNode;
typedef struct { _TrIDictNode** buckets; size_t cap; size_t len; } TrIDict;
static inline TrIDict* _tr_idict_new(long long cap_hint) {
    size_t cap = (size_t)(cap_hint > 8 ? cap_hint : 8);
    TrIDict* d = (TrIDict*)calloc(1, sizeof(TrIDict));
    d->buckets = (_TrIDictNode**)TAURARO_CALLOC(cap, sizeof(_TrIDictNode*));
    d->cap = cap; d->len = 0; return d;
}
static inline void _tr_idict_set_impl(TrIDict* d, long long k, void* v) {
    if (!d) return;
    size_t idx = (size_t)((unsigned long long)k % d->cap);
    _TrIDictNode* n = d->buckets[idx];
    while (n) { if (n->key == k) { n->value = v; return; } n = n->next; }
    _TrIDictNode* nd = (_TrIDictNode*)malloc(sizeof(_TrIDictNode));
    nd->key = k; nd->value = v; nd->next = d->buckets[idx];
    d->buckets[idx] = nd; d->len++;
}
#define _tr_idict_set(d, k, v) _tr_idict_set_impl((d), (k), (void*)(uintptr_t)(v))
static inline void* _tr_idict_get(TrIDict* d, long long k) {
    if (!d) return NULL;
    size_t idx = (size_t)((unsigned long long)k % d->cap);
    _TrIDictNode* n = d->buckets[idx];
    while (n) { if (n->key == k) return n->value; n = n->next; }
    return NULL;
}
static inline bool   _tr_idict_contains(TrIDict* d, long long k) { return _tr_idict_get(d,k) != NULL; }
static inline void   _tr_idict_remove(TrIDict* d, long long k) {
    if (!d) return;
    size_t idx = (size_t)((unsigned long long)k % d->cap);
    _TrIDictNode* n = d->buckets[idx]; _TrIDictNode* prev = NULL;
    while (n) {
        if (n->key == k) {
            if (prev) prev->next = n->next; else d->buckets[idx] = n->next;
            free(n);
            if (d->len > 0) d->len--;
            return;
        }
        prev = n; n = n->next;
    }
}
static inline long long _tr_idict_len(TrIDict* d) { return d ? (long long)d->len : 0LL; }

/* ── Built-in function value (def(...)->R / lambda) ──────────────────────
 * A plain 2-word {fn, env} struct, NOT a tagged pointer. `env==NULL` means a
 * bare named-function reference (call `fn` directly); `env!=NULL` means a
 * closure (call `fn` with `env` as the hidden first arg). A prior tagged-
 * pointer scheme (low bit of a single void* distinguished the two cases)
 * broke on ARM/Thumb, where bit 0 of EVERY function pointer is already fixed
 * to 1 by the ABI (the "Thumb bit", needed for BX/BLX mode switching) -- so
 * the tag test was always true, and a plain function call jumped through
 * garbage read from inside the function's own machine code. This struct form
 * has no pointer-bit dependency and is portable to every target. */
typedef struct { void* fn; void* env; } TrFnVal;

/* List_TrFnVal: vector of function values (Vec[def(...)->R]). Predefined
 * here so codegen needn't lazily emit it (mirrors List_TrTuple below). */
typedef struct { TrFnVal* data; size_t len; size_t capacity; } List_TrFnVal;
static inline List_TrFnVal* List_TrFnVal_new(void) { List_TrFnVal* l=(List_TrFnVal*)malloc(sizeof(List_TrFnVal)); l->data=(TrFnVal*)malloc(sizeof(TrFnVal)*8); l->len=0; l->capacity=8; return l; }
static inline void List_TrFnVal_append(List_TrFnVal* l, TrFnVal val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(TrFnVal*)realloc(l->data,sizeof(TrFnVal)*l->capacity); } l->data[l->len++]=val; }
static inline TrFnVal List_TrFnVal_get(List_TrFnVal* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline TrFnVal List_TrFnVal_pop(List_TrFnVal* l) { if(!l||l->len==0){ TrFnVal z={0}; return z; } l->len--; return l->data[l->len]; }
static inline void List_TrFnVal_set(List_TrFnVal* l, long long i, TrFnVal v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline void List_TrFnVal_free(List_TrFnVal* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── Built-in Tuple (up to 8 elements, all stored as long long) ────────── */
typedef struct { long long data[8]; } TrTuple;

/* List_TrTuple: vector of builtin tuples (Vec[Tuple]). Predefined here so the
   codegen needn't lazily emit it (which races the types-header global decls). */
typedef struct { TrTuple* data; size_t len; size_t capacity; } List_TrTuple;
static inline List_TrTuple* List_TrTuple_new(void) { List_TrTuple* l=(List_TrTuple*)malloc(sizeof(List_TrTuple)); l->data=(TrTuple*)malloc(sizeof(TrTuple)*8); l->len=0; l->capacity=8; return l; }
static inline void List_TrTuple_append(List_TrTuple* l, TrTuple val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(TrTuple*)realloc(l->data,sizeof(TrTuple)*l->capacity); } l->data[l->len++]=val; }
static inline TrTuple List_TrTuple_get(List_TrTuple* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline TrTuple List_TrTuple_pop(List_TrTuple* l) { if(!l||l->len==0) return (TrTuple){0}; l->len--; return l->data[l->len]; }
static inline void List_TrTuple_set(List_TrTuple* l, long long i, TrTuple v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline void List_TrTuple_free(List_TrTuple* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── List types (bootstrap) ───────────────────────────────────────── */

typedef struct { long long* __restrict__ data; size_t len; size_t capacity; } List_i64;
static inline List_i64* List_i64_new(void) { List_i64* l=(List_i64*)malloc(sizeof(List_i64)); l->data=(long long*)malloc(sizeof(long long)*8); l->len=0; l->capacity=8; return l; }
static inline void List_i64_append(List_i64* l, long long val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(long long*)TAURARO_REALLOC(l->data,sizeof(long long)*l->capacity); } l->data[l->len++]=val; }
static inline bool List_i64_contains(List_i64* l, long long val) { for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return true; } return false; }
static inline long long List_i64_pop(List_i64* l) { if(!l||l->len==0) return 0LL; l->len--; return l->data[l->len]; }
static inline void List_i64_set(List_i64* l, long long i, long long v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline long long List_i64_get(List_i64* l, long long i) { if(l&&(size_t)i<l->len) return l->data[i]; return 0LL; }
static inline void List_i64_free(List_i64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { double* __restrict__ data; size_t len; size_t capacity; } List_f64;
static inline List_f64* List_f64_new(void) { List_f64* l=(List_f64*)malloc(sizeof(List_f64)); l->data=(double*)malloc(sizeof(double)*8); l->len=0; l->capacity=8; return l; }
static inline void List_f64_append(List_f64* l, double val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(double*)realloc(l->data,sizeof(double)*l->capacity); } l->data[l->len++]=val; }
static inline double List_f64_pop(List_f64* l) { if(!l||l->len==0) return 0.0; l->len--; return l->data[l->len]; }
static inline void List_f64_free(List_f64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* Undefine the forward-declaration macros before the real typedef */
#undef List_str
#undef List_i64
#undef List_f64
#undef List_ptr
typedef struct { char** data; size_t len; size_t capacity; } List_str;
static inline List_str* List_str_new(void) { List_str* l=(List_str*)malloc(sizeof(List_str)); l->data=(char**)malloc(sizeof(char*)*8); l->len=0; l->capacity=8; return l; }
static inline void List_str_append(List_str* l, char* val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(char**)TAURARO_REALLOC(l->data,sizeof(char*)*l->capacity); } l->data[l->len++]=val; }
static inline char* List_str_pop(List_str* l) { if(!l||l->len==0) return NULL; l->len--; return l->data[l->len]; }
static inline void List_str_free(List_str* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── List_TrStr: refcounted-string element container ──
 * Parallel to List_str (char**); element is the 16-byte TrStr fat
 * pointer. append() retains, free() releases every element. */
typedef struct { TrStr* data; size_t len; size_t capacity; } List_TrStr;
static inline List_TrStr* List_TrStr_new(void) { List_TrStr* l=(List_TrStr*)malloc(sizeof(List_TrStr)); _TR_MEMCOUNT_INC(); _TR_MEMCOUNT_LIST_INC(); l->data=(TrStr*)malloc(sizeof(TrStr)*8); _TR_MEMCOUNT_INC(); l->len=0; l->capacity=8; return l; }
static inline void List_TrStr_append(List_TrStr* l, TrStr val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(TrStr*)realloc(l->data,sizeof(TrStr)*l->capacity); } l->data[l->len++]=_tr_str_retain(val); }
static inline TrStr List_TrStr_pop(List_TrStr* l) { if(!l||l->len==0) return _tr_str_lit(""); l->len--; return l->data[l->len]; }
static inline void List_TrStr_free(List_TrStr* l) { if(l){ _TR_MEMCOUNT_LIST_DEC(); for(size_t i=0;i<l->len;i++) _tr_str_release(l->data[i]); _tr_free(l->data); _tr_free(l); } }
/* Append without retaining: transfers ownership of `val`'s existing
 * reference to the list (used when `val` was just allocated with rc=1
 * specifically for this insertion, e.g. _tr_str_split tokens). */
static inline void List_TrStr_append_owned(List_TrStr* l, TrStr val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(TrStr*)realloc(l->data,sizeof(TrStr)*l->capacity); } l->data[l->len++]=val; }

/* ── List_str helpers (requires List_str typedef above) ─────────────────── */
/* Appends `v` into `l`, transferring ownership if `v` is a freshly-owned
 * TrStr (rc!=NULL, no extra retain), or retaining (no-op for borrowed/literal
 * rc==NULL) otherwise. */
static inline void _tr_trstr_tuple_append(List_TrStr* l, TrStr v) {
    if (v.rc) List_TrStr_append_owned(l, v);
    else List_TrStr_append(l, v);
}
static List_TrStr* _tr_str_tuple2(TrStr a, TrStr b) {
    List_TrStr* l = List_TrStr_new();
    _tr_trstr_tuple_append(l, a);
    _tr_trstr_tuple_append(l, b);
    return l;
}
static List_str* _tr_list_reversed_str(List_str* l) {
    List_str* r = List_str_new();
    if (l) for (int64_t i = l->len - 1; i >= 0; i--) List_str_append(r, l->data[i]);
    return r;
}
static int64_t _tr_list_sum_str(List_str* l) { return 0LL; }
static int64_t _tr_list_min_str(List_str* l) { return 0LL; }
static int64_t _tr_list_max_str(List_str* l) { return 0LL; }
typedef int64_t (*_tr_pred_str_fn)(char*);
static int64_t _tr_list_any_str(List_str* l, _tr_pred_str_fn p) {
    if (!l) return 0LL;
    for (int64_t i=0; i<(int64_t)l->len; i++) if (p(l->data[i])) return 1LL;
    return 0LL;
}
static int64_t _tr_list_all_str(List_str* l, _tr_pred_str_fn p) {
    if (!l) return 1LL;
    for (int64_t i=0; i<(int64_t)l->len; i++) if (!p(l->data[i])) return 0LL;
    return 1LL;
}
static int64_t _tr_list_sum_TrStr(List_TrStr* l) { return 0LL; }
static int64_t _tr_list_min_TrStr(List_TrStr* l) { return 0LL; }
static int64_t _tr_list_max_TrStr(List_TrStr* l) { return 0LL; }
typedef int64_t (*_tr_pred_trstr_fn)(TrStr);
static int64_t _tr_list_any_TrStr(List_TrStr* l, _tr_pred_trstr_fn p) {
    if (!l) return 0LL;
    for (int64_t i=0; i<(int64_t)l->len; i++) if (p(l->data[i])) return 1LL;
    return 0LL;
}
static int64_t _tr_list_all_TrStr(List_TrStr* l, _tr_pred_trstr_fn p) {
    if (!l) return 1LL;
    for (int64_t i=0; i<(int64_t)l->len; i++) if (!p(l->data[i])) return 0LL;
    return 1LL;
}

typedef struct { void** data; size_t len; size_t capacity; } List_ptr;
static inline List_ptr* List_ptr_new(void) { List_ptr* l=(List_ptr*)malloc(sizeof(List_ptr)); _TR_MEMCOUNT_INC(); _TR_MEMCOUNT_LIST_INC(); l->data=(void**)malloc(sizeof(void*)*8); _TR_MEMCOUNT_INC(); l->len=0; l->capacity=8; return l; }
static inline void List_ptr_append(List_ptr* l, void* val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(void**)TAURARO_REALLOC(l->data,sizeof(void*)*l->capacity); } l->data[l->len++]=val; }
static inline void* List_ptr_pop(List_ptr* l) { if(!l||l->len==0) return NULL; l->len--; return l->data[l->len]; }
static inline void List_ptr_free(List_ptr* l) { if(l){ _TR_MEMCOUNT_LIST_DEC(); _tr_free(l->data); _tr_free(l); } }
/* Free a List_ptr whose elements are owned refcounted heap-class instances:
   release each element (via its _trdrop_T) before freeing the buffer. The list
   co-owns each element (append retains), so this balances the count. */
static inline void List_ptr_free_obj(List_ptr* l, void(*drop)(void*)) { if(l){ for(size_t _i=0;_i<l->len;_i++){ _tr_obj_release(l->data[_i], drop); } _TR_MEMCOUNT_LIST_DEC(); _tr_free(l->data); _tr_free(l); } }

typedef struct { _Bool* data; size_t len; size_t capacity; } List_bool;
static inline List_bool* List_bool_new(void) { List_bool* l=(List_bool*)malloc(sizeof(List_bool)); l->data=(_Bool*)malloc(sizeof(_Bool)*8); l->len=0; l->capacity=8; return l; }
static inline void List_bool_append(List_bool* l, _Bool val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(_Bool*)realloc(l->data,sizeof(_Bool)*l->capacity); } l->data[l->len++]=val; }
static inline _Bool List_bool_get(List_bool* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_bool_set(List_bool* l, long long i, _Bool v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_bool_free(List_bool* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { int8_t* data; size_t len; size_t capacity; } List_i8;
static inline List_i8* List_i8_new(void) { List_i8* l=(List_i8*)malloc(sizeof(List_i8)); l->data=(int8_t*)malloc(sizeof(int8_t)*8); l->len=0; l->capacity=8; return l; }
static inline void List_i8_append(List_i8* l, int8_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(int8_t*)realloc(l->data,sizeof(int8_t)*l->capacity); } l->data[l->len++]=val; }
static inline int8_t List_i8_get(List_i8* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_i8_set(List_i8* l, long long i, int8_t v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_i8_free(List_i8* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { int* data; size_t len; size_t capacity; } List_i32;
static inline List_i32* List_i32_new(void) { List_i32* l=(List_i32*)malloc(sizeof(List_i32)); l->data=(int*)malloc(sizeof(int)*8); l->len=0; l->capacity=8; return l; }
static inline void List_i32_append(List_i32* l, int val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(int*)realloc(l->data,sizeof(int)*l->capacity); } l->data[l->len++]=val; }
static inline int List_i32_get(List_i32* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_i32_set(List_i32* l, long long i, int v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_i32_free(List_i32* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { char* data; size_t len; size_t capacity; } List_char;
static inline List_char* List_char_new(void) { List_char* l=(List_char*)malloc(sizeof(List_char)); l->data=(char*)malloc(sizeof(char)*8); l->len=0; l->capacity=8; return l; }
static inline void List_char_append(List_char* l, char val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(char*)realloc(l->data,sizeof(char)*l->capacity); } l->data[l->len++]=val; }
static inline char List_char_get(List_char* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_char_set(List_char* l, long long i, char v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_char_free(List_char* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── Dict[str,V] key/value iteration (after List types are defined) ─────── */
/* Returns keys as TrStr's whose `.data` is shallow-aliased to the Dict's
 * own storage (zero-copy via _tr_str_wrap); releasing these only frees
 * the 8-byte refcount block, never the aliased `.data`, so the Dict's
 * key storage remains valid regardless of this list's lifetime. */
static inline List_TrStr* _tr_dict_keys(TrMap* d) {
    List_TrStr* out = List_TrStr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _DictNode* n = d->buckets[i];
        /* strdup the key: the returned TrStr owns its own buffer (rc=1), so
           freeing the list (List_TrStr_free -> _tr_str_release) doesn't free
           the dict's own key storage (which would dangle d's keys -> a later
           d.get() / d[key] would miss). */
        while (n) { if (n->key && n->value) List_TrStr_append_owned(out, _tr_str_wrap(strdup(n->key))); n = n->next; }
    }
    return out;
}
static inline List_ptr* _tr_dict_values(TrMap* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _DictNode* n = d->buckets[i];
        while (n) { if (n->key && n->value) List_ptr_append(out, n->value); n = n->next; }
    }
    return out;
}
static inline List_i64* _tr_idict_keys(TrIDict* d) {
    List_i64* out = List_i64_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _TrIDictNode* n = d->buckets[i];
        while (n) { if (n->value) List_i64_append(out, n->key); n = n->next; }
    }
    return out;
}
static inline List_ptr* _tr_idict_values(TrIDict* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _TrIDictNode* n = d->buckets[i];
        while (n) { if (n->value) List_ptr_append(out, n->value); n = n->next; }
    }
    return out;
}
/* values() for Dict[K,str]/Map[K,str]: unbox+retain each boxed TrStr value
   into a List_TrStr (#54). The map keeps its own boxed reference, so the
   returned list holds independent retained copies. */
static inline List_TrStr* _tr_dict_values_strval(TrMap* d) {
    List_TrStr* out = List_TrStr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _DictNode* n = d->buckets[i];
        while (n) { if (n->key && n->value) List_TrStr_append(out, _tr_str_unbox(n->value)); n = n->next; }
    }
    return out;
}
static inline List_TrStr* _tr_idict_values_strval(TrIDict* d) {
    List_TrStr* out = List_TrStr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _TrIDictNode* n = d->buckets[i];
        while (n) { if (n->value) List_TrStr_append(out, _tr_str_unbox(n->value)); n = n->next; }
    }
    return out;
}

/* Key-value pair structs for dict.items() */
typedef struct { char* key; void* val; } TrKVPair;
typedef struct { long long key; void* val; } TrIKVPair;

static inline List_ptr* _tr_dict_items(TrMap* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _DictNode* n = d->buckets[i];
        while (n) {
            TrKVPair* p = (TrKVPair*)malloc(sizeof(TrKVPair));
            p->key = n->key; p->val = n->value;
            List_ptr_append(out, p); n = n->next;
        }
    }
    return out;
}
static inline List_ptr* _tr_idict_items(TrIDict* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _TrIDictNode* n = d->buckets[i];
        while (n) {
            TrIKVPair* p = (TrIKVPair*)malloc(sizeof(TrIKVPair));
            p->key = n->key; p->val = n->value;
            List_ptr_append(out, p); n = n->next;
        }
    }
    return out;
}

typedef struct { uint8_t* data; size_t len; size_t capacity; } List_u8;
static inline List_u8* List_u8_new(void) { List_u8* l=(List_u8*)malloc(sizeof(List_u8)); l->data=(uint8_t*)malloc(sizeof(uint8_t)*8); l->len=0; l->capacity=8; return l; }
static inline void List_u8_append(List_u8* l, uint8_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(uint8_t*)realloc(l->data,sizeof(uint8_t)*l->capacity); } l->data[l->len++]=val; }
static inline uint8_t List_u8_get(List_u8* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_u8_set(List_u8* l, long long i, uint8_t v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_u8_free(List_u8* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

static inline List_u8* _tr_bytes_new(const uint8_t* data, size_t len) {
    List_u8* l = (List_u8*)malloc(sizeof(List_u8));
    l->len = len;
    l->capacity = len > 0 ? len : 8;
    l->data = (uint8_t*)TAURARO_ALLOC(l->capacity);
    if (len > 0) memcpy(l->data, data, len);
    return l;
}

typedef struct { uint32_t* data; size_t len; size_t capacity; } List_u32;
static inline List_u32* List_u32_new(void) { List_u32* l=(List_u32*)malloc(sizeof(List_u32)); l->data=(uint32_t*)malloc(sizeof(uint32_t)*8); l->len=0; l->capacity=8; return l; }
static inline void List_u32_append(List_u32* l, uint32_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(uint32_t*)realloc(l->data,sizeof(uint32_t)*l->capacity); } l->data[l->len++]=val; }
static inline void List_u32_free(List_u32* l) { if(l){ _tr_free(l->data); _tr_free(l); } }
/* Pre-size a List to an EXACT capacity in one allocation (grows only; never shrinks).
 * Mirrors C's calloc for known-size arrays (e.g. a sieve): reserving up front means the
 * fill loop never reallocs, so peak memory == the final buffer — no doubling slack and
 * no realloc transient (old+new buffers coexisting) that otherwise ~2x's peak RSS. */
#define _TR_LIST_RESERVE(LT) static inline void LT##_reserve(LT* l, long long cap){ if(l && (size_t)cap > l->capacity){ l->data = realloc(l->data, sizeof(*l->data) * (size_t)cap); l->capacity = (size_t)cap; } }
_TR_LIST_RESERVE(List_bool)
_TR_LIST_RESERVE(List_char)
_TR_LIST_RESERVE(List_f64)
_TR_LIST_RESERVE(List_i32)
_TR_LIST_RESERVE(List_i64)
_TR_LIST_RESERVE(List_i8)
_TR_LIST_RESERVE(List_ptr)
_TR_LIST_RESERVE(List_str)
_TR_LIST_RESERVE(List_TrStr)
_TR_LIST_RESERVE(List_TrTuple)
_TR_LIST_RESERVE(List_TrFnVal)
_TR_LIST_RESERVE(List_u32)
_TR_LIST_RESERVE(List_u8)
/* ── Extended Vec/List operations: remove, swap, clear, is_empty, extend ──── */
static inline void List_i64_remove(List_i64* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_i64_swap(List_i64* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; long long t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_i64_clear(List_i64* l) { if(l) l->len=0; }
static inline bool List_i64_is_empty(List_i64* l) { return !l||l->len==0; }
static inline void List_i64_extend(List_i64* l, List_i64* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_i64_append(l,o->data[i]); }
static inline long long List_i64_index_of(List_i64* l, long long v) { if(!l) return -1LL; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return (long long)i; return -1LL; }
static inline void List_f64_remove(List_f64* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_f64_swap(List_f64* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; double t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_f64_clear(List_f64* l) { if(l) l->len=0; }
static inline bool List_f64_is_empty(List_f64* l) { return !l||l->len==0; }
static inline void List_f64_extend(List_f64* l, List_f64* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_f64_append(l,o->data[i]); }
static inline bool List_f64_contains(List_f64* l, double v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline double List_f64_get(List_f64* l, long long i) { if(l&&(size_t)i<l->len) return l->data[i]; return 0.0; }
static inline void List_f64_set(List_f64* l, long long i, double v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline void List_str_remove(List_str* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_str_swap(List_str* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; char* t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_str_clear(List_str* l) { if(l) l->len=0; }
static inline bool List_str_is_empty(List_str* l) { return !l||l->len==0; }
static inline void List_str_extend(List_str* l, List_str* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_str_append(l,o->data[i]); }
static inline bool List_str_contains(List_str* l, char* v) { if(!l||!v) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]&&strcmp(l->data[i],v)==0) return true; return false; }
static inline long long List_str_index_of(List_str* l, char* v) { if(!l||!v) return -1LL; for(size_t i=0;i<l->len;i++) if(l->data[i]&&strcmp(l->data[i],v)==0) return (long long)i; return -1LL; }
/* ── List_TrStr: extended ops (remove/swap/clear/extend/contains/index_of) ── */
static inline void List_TrStr_remove(List_TrStr* l, long long i) { if(!l||(size_t)i>=l->len) return; _tr_str_release(l->data[i]); for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_TrStr_swap(List_TrStr* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; TrStr t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_TrStr_clear(List_TrStr* l) { if(!l) return; for(size_t i=0;i<l->len;i++) _tr_str_release(l->data[i]); l->len=0; }
static inline bool List_TrStr_is_empty(List_TrStr* l) { return !l||l->len==0; }
static inline void List_TrStr_extend(List_TrStr* l, List_TrStr* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_TrStr_append(l,o->data[i]); }
static inline bool List_TrStr_contains(List_TrStr* l, TrStr v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i].data&&v.data&&strcmp(l->data[i].data,v.data)==0) return true; return false; }
static inline long long List_TrStr_index_of(List_TrStr* l, TrStr v) { if(!l) return -1LL; for(size_t i=0;i<l->len;i++) if(l->data[i].data&&v.data&&strcmp(l->data[i].data,v.data)==0) return (long long)i; return -1LL; }
/* get() returns a retained copy (independent reference); set() releases the old element and retains the new one. */
static inline TrStr List_TrStr_get(List_TrStr* l, long long i) { if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); } _tr_bounds_check(i, l->len); return _tr_str_retain(l->data[i]); }
static inline void List_TrStr_set(List_TrStr* l, long long i, TrStr v) { if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); } _tr_bounds_check(i, l->len); _tr_str_release(l->data[i]); l->data[i]=_tr_str_retain(v); }
static inline void List_ptr_remove(List_ptr* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_ptr_swap(List_ptr* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; void* t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_ptr_clear(List_ptr* l) { if(l) l->len=0; }
static inline bool List_ptr_is_empty(List_ptr* l) { return !l||l->len==0; }
static inline void List_ptr_extend(List_ptr* l, List_ptr* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_ptr_append(l,o->data[i]); }
static inline bool List_ptr_contains(List_ptr* l, void* v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline void List_bool_remove(List_bool* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_bool_swap(List_bool* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; _Bool t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_bool_clear(List_bool* l) { if(l) l->len=0; }
static inline bool List_bool_is_empty(List_bool* l) { return !l||l->len==0; }
static inline void List_bool_extend(List_bool* l, List_bool* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_bool_append(l,o->data[i]); }
static inline bool List_bool_contains(List_bool* l, _Bool v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline long long List_bool_pop(List_bool* l) { if(!l||l->len==0) return 0; l->len--; return l->data[l->len]; }
static inline void List_i8_remove(List_i8* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_i8_swap(List_i8* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; int8_t t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_i8_clear(List_i8* l) { if(l) l->len=0; }
static inline bool List_i8_is_empty(List_i8* l) { return !l||l->len==0; }
static inline void List_i8_extend(List_i8* l, List_i8* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_i8_append(l,o->data[i]); }
static inline bool List_i8_contains(List_i8* l, int8_t v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline int8_t List_i8_pop(List_i8* l) { if(!l||l->len==0) return 0; l->len--; return l->data[l->len]; }
static inline void List_i32_remove(List_i32* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_i32_swap(List_i32* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; int t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_i32_clear(List_i32* l) { if(l) l->len=0; }
static inline bool List_i32_is_empty(List_i32* l) { return !l||l->len==0; }
static inline void List_i32_extend(List_i32* l, List_i32* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_i32_append(l,o->data[i]); }
static inline bool List_i32_contains(List_i32* l, int v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline int List_i32_pop(List_i32* l) { if(!l||l->len==0) return 0; l->len--; return l->data[l->len]; }


typedef struct { long long* data; size_t len; size_t capacity; } Set_i64;
static inline Set_i64* Set_i64_new(void) { Set_i64* l=(Set_i64*)malloc(sizeof(Set_i64)); l->data=(long long*)malloc(sizeof(long long)*8); l->len=0; l->capacity=8; return l; }
static inline void Set_i64_add(Set_i64* l, long long val) { 
    for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(long long*)TAURARO_REALLOC(l->data,sizeof(long long)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_i64_free(Set_i64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }
static inline _Bool Set_i64_contains(Set_i64* l, long long v) { if(!l) return 0; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return 1; return 0; }
static inline long long Set_i64_len(Set_i64* l) { return l ? (long long)l->len : 0LL; }
static inline _Bool Set_i64_is_empty(Set_i64* l) { return !l || l->len==0; }
static inline void Set_i64_remove(Set_i64* l, long long v) { if(!l) return; for(size_t i=0;i<l->len;i++) if(l->data[i]==v){ for(size_t j=i;j+1<l->len;j++) l->data[j]=l->data[j+1]; l->len--; return; } }

typedef struct { void** data; size_t len; size_t capacity; } Set_ptr;
static inline Set_ptr* Set_ptr_new(void) { Set_ptr* l=(Set_ptr*)malloc(sizeof(Set_ptr)); l->data=(void**)malloc(sizeof(void*)*8); l->len=0; l->capacity=8; return l; }
static inline void Set_ptr_add(Set_ptr* l, void* val) { 
    for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(void**)TAURARO_REALLOC(l->data,sizeof(void*)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_ptr_free(Set_ptr* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { char** data; size_t len; size_t capacity; } Set_str;
static inline Set_str* Set_str_new(void) { Set_str* l=(Set_str*)malloc(sizeof(Set_str)); l->data=(char**)malloc(sizeof(char*)*8); l->len=0; l->capacity=8; return l; }
static inline void Set_str_add(Set_str* l, char* val) { 
    for (size_t i = 0; i < l->len; i++) { if (strcmp(l->data[i], val) == 0) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(char**)TAURARO_REALLOC(l->data,sizeof(char*)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_str_free(Set_str* l) { if(l){ _tr_free(l->data); _tr_free(l); } }
static inline _Bool Set_str_contains(Set_str* l, const char* v) { if(!l||!v) return 0; for(size_t i=0;i<l->len;i++) if(l->data[i]&&strcmp(l->data[i],v)==0) return 1; return 0; }
static inline long long Set_str_len(Set_str* l) { return l ? (long long)l->len : 0LL; }
static inline _Bool Set_str_is_empty(Set_str* l) { return !l || l->len==0; }
static inline void Set_str_remove(Set_str* l, const char* v) { if(!l||!v) return; for(size_t i=0;i<l->len;i++) if(l->data[i]&&strcmp(l->data[i],v)==0){ for(size_t j=i;j+1<l->len;j++) l->data[j]=l->data[j+1]; l->len--; return; } }

/* ── Bounds-checked list access ─────────────────────────────────────────── */
static inline List_i64* _tr_range_new(long long start, long long stop, bool inclusive) {
    List_i64* l = List_i64_new();
    long long end = inclusive ? stop : stop - 1;
    for (long long i = start; i <= end; i++) { List_i64_append(l, i); }
    return l;
}
static inline long long _tr_list_i64_get(List_i64* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline long long List_i64_get_index(List_i64* l, long long i) { return _tr_list_i64_get(l, i); }
static inline void _tr_list_i64_set(List_i64* l, long long i, long long v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_i64_set_index(List_i64* l, long long i, long long v) { _tr_list_i64_set(l, i, v); }

static inline double _tr_list_f64_get(List_f64* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline double List_f64_get_index(List_f64* l, long long i) { return _tr_list_f64_get(l, i); }
static inline void _tr_list_f64_set(List_f64* l, long long i, double v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_f64_set_index(List_f64* l, long long i, double v) { _tr_list_f64_set(l, i, v); }

static inline char* _tr_list_str_get(List_str* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline char* List_str_get_index(List_str* l, long long i) { return _tr_list_str_get(l, i); }
static inline char* List_str_get(List_str* l, long long i) { return _tr_list_str_get(l, i); }
static inline void _tr_list_str_set(List_str* l, long long i, char* v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_str_set_index(List_str* l, long long i, char* v) { _tr_list_str_set(l, i, v); }
static inline void List_str_set(List_str* l, long long i, char* v) { _tr_list_str_set(l, i, v); }

/* Vec[str] — always available so main(args: Vec[str]) works without an explicit import. */
#ifndef _TR_VEC_STR_DEFINED
#define _TR_VEC_STR_DEFINED
typedef struct Vec_str Vec_str;
struct Vec_str { List_str* data; long long len; long long cap; };
static inline Vec_str* Vec_str_init(long long cap) {
    Vec_str* v = (Vec_str*)_tr_checked_alloc(sizeof(Vec_str));
    v->data = List_str_new(); v->len = 0; v->cap = cap > 0 ? cap : 8;
    return v;
}
static inline void Vec_str_push(Vec_str* v, char* s) { List_str_append(v->data, s); v->len++; }
static inline char* Vec_str_get(Vec_str* v, long long i) { return List_str_get(v->data, i); }
static inline long long Vec_str_len(Vec_str* v) { return v ? v->len : 0LL; }
#endif

static inline void* _tr_list_ptr_get(List_ptr* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void* List_ptr_get_index(List_ptr* l, long long i) { return _tr_list_ptr_get(l, i); }
static inline void* List_ptr_get(List_ptr* l, long long i) { return _tr_list_ptr_get(l, i); }
static inline void _tr_list_ptr_set(List_ptr* l, long long i, void* v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_ptr_set_index(List_ptr* l, long long i, void* v) { _tr_list_ptr_set(l, i, v); }
static inline void List_ptr_set(List_ptr* l, long long i, void* v) { _tr_list_ptr_set(l, i, v); }

static inline _Bool _tr_list_bool_get(List_bool* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline _Bool List_bool_get_index(List_bool* l, long long i) { return _tr_list_bool_get(l, i); }
static inline void _tr_list_bool_set(List_bool* l, long long i, _Bool v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_bool_set_index(List_bool* l, long long i, _Bool v) { _tr_list_bool_set(l, i, v); }
static inline int8_t _tr_list_i8_get(List_i8* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_i8_set(List_i8* l, long long i, int8_t v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline int _tr_list_i32_get(List_i32* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_i32_set(List_i32* l, long long i, int v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline char _tr_list_char_get(List_char* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_char_set(List_char* l, long long i, char v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline uint8_t _tr_list_u8_get(List_u8* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_u8_set(List_u8* l, long long i, uint8_t v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline uint32_t _tr_list_u32_get(List_u32* l, long long i) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_u32_set(List_u32* l, long long i, uint32_t v) {
    if (!l) { _TR_DIAG("Null list access\n"); _TR_TRAP(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}

static inline char* _tr_str_join(List_str* parts, const char* sep) {
    if (!parts || parts->len == 0) return _tr_empty_heap_str();
    size_t total = 0, seplen = sep ? strlen(sep) : 0;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i]) total += strlen(parts->data[i]);
        if (i + 1 < parts->len) total += seplen;
    }
    char* out = (char*)_tr_checked_alloc(total + 1);
    char* dst = out;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i]) { size_t l = strlen(parts->data[i]); memcpy(dst, parts->data[i], l); dst += l; }
        if (i + 1 < parts->len && seplen) { memcpy(dst, sep, seplen); dst += seplen; }
    }
    *dst = '\0';
    return out;
}
/* TrStr-returning variant: same semantics, refcounted result (rc=1 even for empty). */
static inline TrStr _tr_strx_join(List_str* parts, const char* sep) {
    if (!parts || parts->len == 0) return _tr_str_new(0);
    size_t total = 0, seplen = sep ? strlen(sep) : 0;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i]) total += strlen(parts->data[i]);
        if (i + 1 < parts->len) total += seplen;
    }
    TrStr out = _tr_str_new(total);
    char* dst = out.data;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i]) { size_t l = strlen(parts->data[i]); memcpy(dst, parts->data[i], l); dst += l; }
        if (i + 1 < parts->len && seplen) { memcpy(dst, sep, seplen); dst += seplen; }
    }
    return out;
}

/* List_TrStr-backed join, for List[str].join() under the TrStr migration (#54). */
static inline TrStr _tr_strx_join_trstr(List_TrStr* parts, const char* sep) {
    if (!parts || parts->len == 0) return _tr_str_new(0);
    size_t total = 0, seplen = sep ? strlen(sep) : 0;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i].data) total += strlen(parts->data[i].data);
        if (i + 1 < parts->len) total += seplen;
    }
    TrStr out = _tr_str_new(total);
    char* dst = out.data;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i].data) { size_t l = strlen(parts->data[i].data); memcpy(dst, parts->data[i].data, l); dst += l; }
        if (i + 1 < parts->len && seplen) { memcpy(dst, sep, seplen); dst += seplen; }
    }
    return out;
}

_TR_XLINK List_TrStr* _tr_str_split(const char* s, const char* sep) {
    List_TrStr* l=List_TrStr_new(); if(!s||!sep||!*sep) return l;
    char* cp=(char*)malloc(strlen(s)+1); strcpy(cp,s);
    char* tok=strtok(cp,(char*)sep);
    while(tok){ List_TrStr_append_owned(l,_tr_str_wrap(strdup(tok))); tok=strtok(NULL,(char*)sep); }
    _tr_free(cp); return l;
}
static inline List_TrStr* _tr_str_lines(const char* s) { return _tr_str_split(s, "\n"); }
/* s.format(a, b, ...): replace each "{}" placeholder in order with the given (already
 * stringified) arguments. Extra placeholders / args are ignored. */
static inline char* _tr_str_format(const char* fmt, const char* const* args, long long argc) {
    if (!fmt) fmt = "";
    size_t cap = strlen(fmt);
    for (long long i = 0; i < argc; i++) if (args[i]) cap += strlen(args[i]);
    char* r = (char*)_tr_checked_alloc(cap + 1);
    char* w = r; const char* p = fmt; long long ai = 0;
    while (*p) {
        if (p[0] == '{' && p[1] == '}' && ai < argc) {
            const char* a = args[ai++]; if (a) { size_t l = strlen(a); memcpy(w, a, l); w += l; }
            p += 2;
        } else { *w++ = *p++; }
    }
    *w = '\0'; return r;
}
/* s.chars() -> List[str] of single-character strings. */
static inline List_TrStr* _tr_str_chars(const char* s) {
    List_TrStr* l = List_TrStr_new(); if (!s) return l;
    for (size_t i = 0; s[i]; i++) { char c[2]; c[0] = s[i]; c[1] = '\0'; List_TrStr_append_owned(l, _tr_str_wrap(strdup(c))); }
    return l;
}
static inline List_TrStr* _tr_str_words(const char* s) { return _tr_str_split(s, " "); }
/* TrStr-elements join: build "sep"-joined string from a List_TrStr*. */
static inline TrStr _tr_trstr_join(List_TrStr* parts, const char* sep) {
    if (!parts || parts->len == 0) return _tr_str_new(0);
    size_t total = 0, seplen = sep ? strlen(sep) : 0;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i].data) total += strlen(parts->data[i].data);
        if (i + 1 < parts->len) total += seplen;
    }
    TrStr out = _tr_str_new(total);
    char* dst = out.data;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i].data) { size_t l = strlen(parts->data[i].data); memcpy(dst, parts->data[i].data, l); dst += l; }
        if (i + 1 < parts->len && seplen) { memcpy(dst, sep, seplen); dst += seplen; }
    }
    return out;
}

/* ── Test runner helpers ─────────────────────────────────────────────── */

_TR_GLOBAL int _tr_tests_passed;
_TR_GLOBAL int _tr_tests_failed;

static void _tr_run_test(const char* name, void(*fn)(void)) {
    jmp_buf _buf;
    char* _msg = NULL;
    _tr_exc_push(&_buf, &_msg);
    if (setjmp(_buf) == 0) {
        fn();
        _tr_exc_pop();
        _tr_tests_passed++;
        printf("\033[32mPASS\033[0m %s\n", name);
    } else {
        _tr_tests_failed++;
        printf("\033[31mFAIL\033[0m %s: %s\n", name, _msg ? _msg : "panic");
    }
}

static int _tr_test_report(void) {
    int total = _tr_tests_passed + _tr_tests_failed;
    if (_tr_tests_failed == 0) {
        printf("\n\033[32m%d/%d tests passed\033[0m\n", _tr_tests_passed, total);
    } else {
        printf("\n%d/%d tests passed, \033[31m%d failed\033[0m\n",
               _tr_tests_passed, total, _tr_tests_failed);
    }
    return _tr_tests_failed > 0 ? 1 : 0;
}

#ifndef TAURARO_NO_RT_HELPERS
/* When std library is compiled in, it provides its own StringBuilder and
   file I/O — suppress the lightweight rt.h fallback implementations. */
#ifndef TAURARO_STD_LIB
/* ── StringBuilder (suppressed when std.core.string provides its own) ───── */
#ifndef TAURARO_RT_NO_STRINGBUILDER
/* OOP layout — matches std.core.string.StringBuilder: buf is StringObj* so that
 * the c.tr codegen's sb->buf->len accesses compile correctly. */
typedef struct core_string_StringObj { char* data; long long len; long long capacity; } core_string_StringObj;
typedef core_string_StringObj StringObj;
static inline StringObj* StringObj_init(char* s) {
    StringObj* obj = (StringObj*)_tr_checked_alloc(sizeof(StringObj));
    long long slen = s ? (long long)strlen(s) : 0LL;
    obj->len = slen; obj->capacity = slen + 8;
    obj->data = (char*)_tr_checked_alloc((size_t)(obj->capacity));
    if (slen > 0) memcpy(obj->data, s, (size_t)slen);
    obj->data[slen] = '\0';
    return obj;
}
static inline char* StringObj_as_str(StringObj* obj) { return obj->data; }
typedef struct core_string_StringBuilder { StringObj* buf; } core_string_StringBuilder;
typedef core_string_StringBuilder StringBuilder;

static inline StringBuilder* StringBuilder_init(long long cap) {
    if (cap < 16) cap = 16;
    StringBuilder* sb = (StringBuilder*)_tr_checked_alloc(sizeof(StringBuilder));
    sb->buf = (StringObj*)_tr_checked_alloc(sizeof(StringObj));
    sb->buf->len = 0; sb->buf->capacity = cap + 1;
    sb->buf->data = (char*)_tr_checked_alloc((size_t)(sb->buf->capacity));
    sb->buf->data[0] = '\0';
    return sb;
}
static inline void StringBuilder_append(StringBuilder* sb, char* s) {
    long long slen = (long long)strlen(s);
    if (slen <= 0) return;
    if (sb->buf->len + slen >= sb->buf->capacity) {
        sb->buf->capacity = (sb->buf->len + slen) * 2 + 8;
        sb->buf->data = (char*)TAURARO_REALLOC(sb->buf->data, (size_t)sb->buf->capacity);
    }
    memcpy(sb->buf->data + sb->buf->len, s, (size_t)slen);
    sb->buf->len += slen;
    sb->buf->data[sb->buf->len] = '\0';
}
static inline void StringBuilder_append_char(StringBuilder* sb, long long c) {
    if (sb->buf->len + 1 >= sb->buf->capacity) {
        sb->buf->capacity = sb->buf->capacity * 2 + 8;
        sb->buf->data = (char*)TAURARO_REALLOC(sb->buf->data, (size_t)sb->buf->capacity);
    }
    sb->buf->data[sb->buf->len++] = (char)c;
    sb->buf->data[sb->buf->len] = '\0';
}
static inline StringObj* StringBuilder_to_string(StringBuilder* sb) {
    return StringObj_init(sb->buf->data);
}
static inline char* StringBuilder_to_owned(StringBuilder* sb) {
    long long sz = sb->buf->len + 1;
    char* out = (char*)_tr_checked_alloc(sz);
    memcpy(out, sb->buf->data, sz);
    return out;
}
static inline char* StringBuilder_as_str(StringBuilder* sb) { return sb->buf->data; }
static inline void StringBuilder_append_int(StringBuilder* sb, long long n) {
    char tmp[32]; snprintf(tmp, sizeof(tmp), "%lld", n);
    StringBuilder_append(sb, tmp);
}
static inline void StringBuilder_append_float(StringBuilder* sb, double f) {
    char tmp[32]; snprintf(tmp, sizeof(tmp), "%g", f);
    StringBuilder_append(sb, tmp);
}
static inline long long StringBuilder_length(StringBuilder* sb) { return sb->buf->len; }
static inline void StringBuilder_clear(StringBuilder* sb) {
    if (sb && sb->buf) { sb->buf->len = 0; if (sb->buf->data) sb->buf->data[0] = '\0'; }
}
static inline void StringBuilder_free(StringBuilder* sb) {
    TAURARO_FREE(sb->buf->data); TAURARO_FREE(sb->buf); TAURARO_FREE(sb);
}
#endif /* TAURARO_RT_NO_STRINGBUILDER */

/* ── File I/O helpers ──────── std-tier only (FILE/fopen) ────────────── */
#ifndef TAURARO_BARE
static inline char* read_file(char* path) {
    /* Owned `-> str` (success path allocs `buf`); error paths must also be heap. */
    if (!path || !*path) return _tr_empty_heap_str();
    FILE* f = fopen(path, "rb");
    if (!f) return _tr_empty_heap_str();
    fseek(f, 0, SEEK_END); long sz = ftell(f); rewind(f);
    if (sz < 0) { fclose(f); return _tr_empty_heap_str(); }
    char* buf = (char*)_tr_checked_alloc((size_t)sz + 1);
    size_t rd = fread(buf, 1, (size_t)sz, f); fclose(f);
    buf[rd] = '\0';
    return buf;
}
static inline bool write_file(char* path, char* content) {
    if (!path || !content) return false;
    FILE* f = fopen(path, "wb");
    if (!f) return false;
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    return true;
}
static inline bool append_file(char* path, char* content) {
    if (!path || !content) return false;
    FILE* f = fopen(path, "ab");
    if (!f) return false;
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    return true;
}
static inline bool file_exists(char* path) {
    if (!path || !*path) return false;
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    fclose(f); return true;
}
#else
static inline char* read_file(char* path) { (void)path; return _tr_empty_heap_str(); }
static inline bool write_file(char* path, char* content) { (void)path; (void)content; return false; }
static inline bool append_file(char* path, char* content) { (void)path; (void)content; return false; }
static inline bool file_exists(char* path) { (void)path; return false; }
#endif
#endif /* TAURARO_STD_LIB */
#endif /* TAURARO_NO_RT_HELPERS */

static inline char* _tr_c_strdup(char* s) {
    return s ? strdup(s) : (char*)0;
}
#define _tr_strdup _tr_c_strdup


/* Use compiler builtins (no <math.h>) so these work at every tier. */
static inline double _tr_get_inf(void) { return __builtin_inf(); }
static inline bool   _tr_is_inf(double x) { return __builtin_isinf(x) != 0; }
static inline bool   _tr_is_nan(double x) { return __builtin_isnan(x) != 0; }

/* ── MMIO intrinsics for bare-metal device drivers (std/hal/mmio.tr) ──────
 * Volatile so the compiler never elides or reorders a hardware register access.
 * Available at every tier; the address is a raw device register the caller vouches
 * for (this is the primitive `unsafe` build on). */
/* Address is `usize` (unsigned long long in Tauraro) cast to a pointer, so the
 * ABI matches on both 32- and 64-bit targets. */
static inline void          _tr_mmio_write32(unsigned long long a, unsigned int v)  { *(volatile uint32_t*)(size_t)a = (uint32_t)v; }
static inline unsigned int  _tr_mmio_read32 (unsigned long long a)                  { return (unsigned int)*(volatile uint32_t*)(size_t)a; }
static inline void          _tr_mmio_write8 (unsigned long long a, unsigned char v) { *(volatile uint8_t*)(size_t)a  = (uint8_t)v; }
static inline unsigned char _tr_mmio_read8  (unsigned long long a)                  { return (unsigned char)*(volatile uint8_t*)(size_t)a; }


#ifdef _WIN32
static inline void _tr_init_console(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}
#else
static inline void _tr_init_console(void) {}
#endif


/* ==========================================================================
   Extended runtime helpers: datetime, OS, net-server, UDP, DNS, random
   ========================================================================== */

/* -- DateTime helpers ------------------------------------------------------ */
#ifdef _TR_HAS_TIME
static inline int    _tr_tm_year(long long ts)    { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_year+1900; }
static inline int    _tr_tm_month(long long ts)   { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_mon+1; }
static inline int    _tr_tm_day(long long ts)     { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_mday; }
static inline int    _tr_tm_hour(long long ts)    { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_hour; }
static inline int    _tr_tm_min(long long ts)     { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_min; }
static inline int    _tr_tm_sec(long long ts)     { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_sec; }
static inline int    _tr_tm_weekday(long long ts) { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_wday; }
static inline int    _tr_tm_yearday(long long ts) { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_yday+1; }
static inline long long _tr_tm_make(int year,int month,int day,int hour,int mi,int sec) {
    struct tm t; memset(&t,0,sizeof(t));
    t.tm_year=year-1900; t.tm_mon=month-1; t.tm_mday=day;
    t.tm_hour=hour; t.tm_min=mi; t.tm_sec=sec; t.tm_isdst=-1;
    return (long long)mktime(&t);
}
static inline char* _tr_strftime(long long ts, const char* fmt) {
    time_t t=(time_t)ts; struct tm* m=localtime(&t);
    char* buf=(char*)_tr_c_malloc(256); if(!buf) return _tr_empty_heap_str();
    strftime(buf,256,fmt,m); return buf;
}
#else  /* no <time.h> (bare toolchain): no calendar/RTC — stub the datetime helpers */
static inline int    _tr_tm_year(long long ts)    { (void)ts; return 1970; }
static inline int    _tr_tm_month(long long ts)   { (void)ts; return 1; }
static inline int    _tr_tm_day(long long ts)     { (void)ts; return 1; }
static inline int    _tr_tm_hour(long long ts)    { (void)ts; return 0; }
static inline int    _tr_tm_min(long long ts)     { (void)ts; return 0; }
static inline int    _tr_tm_sec(long long ts)     { (void)ts; return 0; }
static inline int    _tr_tm_weekday(long long ts) { (void)ts; return 0; }
static inline int    _tr_tm_yearday(long long ts) { (void)ts; return 1; }
static inline long long _tr_tm_make(int year,int month,int day,int hour,int mi,int sec) {
    (void)year;(void)month;(void)day;(void)hour;(void)mi;(void)sec; return 0LL;
}
static inline char* _tr_strftime(long long ts, const char* fmt) {
    (void)ts;(void)fmt; return _tr_empty_heap_str();
}
#endif

/* -- OS / System helpers (platform-specific) ------------------------------- */
/* This section mixes OS helpers with the blocking-socket TCP API in its real
 * branches, so it is gated on NO_NET: wasi (no sockets) takes the stub branch —
 * its OS helpers degrade to sandbox-appropriate values (hostname "embedded",
 * cpu_count 1) and it uses the tcp stubs already defined above. */
#if defined(TAURARO_NO_NET)
/* Bare / wasi / freestanding: no OS network services */
static inline char* _tr_hostname(void)          { return (char*)"embedded"; }
static inline char* _tr_username(void)          { return (char*)""; }
static inline int   _tr_cpu_count(void)         { return 1; }
static inline char* _tr_cwd(void)               { return (char*)"/"; }
static inline int   _tr_chdir(const char* p)    { (void)p; return -1; }
static inline char* _tr_platform(void)          { return (char*)"embedded"; }
static inline char* _tr_os_machine(void)        {
#if defined(__aarch64__)
    return (char*)"arm64";
#elif defined(__arm__)
    return (char*)"arm";
#elif defined(__riscv)
    return (char*)"riscv";
#else
    return (char*)"unknown";
#endif
}
static inline long long _tr_memory_total_mb(void) { return 0LL; }
static inline void _tr_console_color(int code)  { (void)code; }
static inline void _tr_console_reset(void)      {}
static inline void _tr_console_clear(void)      {}
#elif defined(_WIN32)
static inline char* _tr_hostname(void) { char* b=(char*)_tr_c_malloc(256); DWORD n=256; GetComputerNameA(b,&n); return b; }
static inline char* _tr_username(void) { char* b=(char*)_tr_c_malloc(256); DWORD n=256; GetUserNameA(b,&n); return b; }
static inline int   _tr_cpu_count(void) { SYSTEM_INFO si; GetSystemInfo(&si); return (int)si.dwNumberOfProcessors; }
static inline char* _tr_cwd(void)       { char* b=(char*)_tr_c_malloc(4096); GetCurrentDirectoryA(4096,b); return b; }
static inline int   _tr_chdir(const char* p) { return SetCurrentDirectoryA(p)?0:-1; }
static inline char* _tr_platform(void) { return _tr_str_dup_owned("windows"); }
static inline char* _tr_os_machine(void) {
    SYSTEM_INFO si; GetSystemInfo(&si);
    if(si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64) return _tr_str_dup_owned("x86_64");
    if(si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_ARM64) return _tr_str_dup_owned("arm64");
    return _tr_str_dup_owned("x86");
}
static inline long long _tr_memory_total_mb(void) {
    MEMORYSTATUSEX ms; ms.dwLength=sizeof(ms); GlobalMemoryStatusEx(&ms);
    return (long long)(ms.ullTotalPhys/(1024LL*1024LL));
}
_TR_XLINK int _tr_tcp_listen(const char* host,int port,int backlog) {
    _tr_net_init();
    SOCKET s=socket(AF_INET,SOCK_STREAM,0); if(s==INVALID_SOCKET) return -1;
    int opt=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt));
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    if(bind(s,(struct sockaddr*)&a,sizeof(a))!=0){closesocket(s);return -1;}
    if(listen(s,backlog)!=0){closesocket(s);return -1;}
    return (int)s;
}
/* Disable Nagle's algorithm: without this, every small HTTP response gets
 * delayed ~40ms by Nagle + the peer's delayed-ACK timer, capping keep-alive
 * request latency at ~20-40ms regardless of how fast the handler itself is. */
_TR_XLINK void _tr_tcp_set_nodelay(int fd) {
    int one = 1;
    setsockopt((SOCKET)fd, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(one));
}
_TR_XLINK int   _tr_tcp_accept(int srv) { SOCKET c=accept((SOCKET)srv,NULL,NULL); if(c!=INVALID_SOCKET) _tr_tcp_set_nodelay((int)c); return (c==INVALID_SOCKET)?-1:(int)c; }
_TR_XLINK char* _tr_tcp_peer_addr(int fd) {
    struct sockaddr_in a; int al=sizeof(a);
    if(getpeername((SOCKET)fd,(struct sockaddr*)&a,&al)!=0) return _tr_empty_heap_str();
    char* buf=(char*)_tr_c_malloc(64); char ip[32];
    inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));
    snprintf(buf,64,"%s:%d",ip,(int)ntohs(a.sin_port)); return buf;
}
_TR_XLINK int  _tr_udp_socket(void) { _tr_net_init(); SOCKET s=socket(AF_INET,SOCK_DGRAM,0); return (s==INVALID_SOCKET)?-1:(int)s; }
_TR_XLINK int  _tr_udp_bind(int fd,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    return bind((SOCKET)fd,(struct sockaddr*)&a,sizeof(a))==0?0:-1;
}
_TR_XLINK int  _tr_udp_send_to(int fd,const char* data,int len,const char* host,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=inet_addr(host);
    return (int)sendto((SOCKET)fd,data,len,0,(struct sockaddr*)&a,sizeof(a));
}
_TR_XLINK int  _tr_udp_recv_from(int fd,char* buf,int cap,char* src) {
    struct sockaddr_in a; int al=sizeof(a);
    int n=(int)recvfrom((SOCKET)fd,buf,cap,0,(struct sockaddr*)&a,&al);
    if(n>0&&src){char ip[32];inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));snprintf(src,64,"%s:%d",ip,(int)ntohs(a.sin_port));}
    return n;
}
_TR_XLINK void _tr_udp_close(int fd) { closesocket((SOCKET)fd); }
static inline char* _tr_dns_resolve(const char* host) {
    _tr_net_init();
    struct addrinfo hints={0},*res=NULL; hints.ai_family=AF_INET;
    if(getaddrinfo(host,NULL,&hints,&res)!=0) return _tr_empty_heap_str();
    char* ip=(char*)_tr_c_malloc(64);
    inet_ntop(AF_INET,&((struct sockaddr_in*)res->ai_addr)->sin_addr,ip,64);
    freeaddrinfo(res); return ip;
}
static inline char* _tr_dns_reverse(const char* ip) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; inet_pton(AF_INET,ip,&a.sin_addr);
    char* buf=(char*)_tr_c_malloc(256);
    return (getnameinfo((struct sockaddr*)&a,sizeof(a),buf,256,NULL,0,0)==0)?buf:_tr_empty_heap_str();
}
static inline void _tr_console_color(int code) {
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE); int attr=0;
    if(code==31||code==91) attr=FOREGROUND_RED;
    else if(code==32||code==92) attr=FOREGROUND_GREEN;
    else if(code==33||code==93) attr=FOREGROUND_RED|FOREGROUND_GREEN;
    else if(code==34||code==94) attr=FOREGROUND_BLUE;
    else if(code==35||code==95) attr=FOREGROUND_RED|FOREGROUND_BLUE;
    else if(code==36||code==96) attr=FOREGROUND_GREEN|FOREGROUND_BLUE;
    else attr=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
    if(code>=90) attr|=FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(h,(WORD)attr);
}
static inline void _tr_console_reset(void) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); }
static inline void _tr_console_clear(void) { system("cls"); }
#else
#include <unistd.h>
static inline char* _tr_hostname(void) { char* b=(char*)_tr_c_malloc(256); gethostname(b,256); return b; }
static inline char* _tr_username(void) {
    const char* u=getenv("USER"); if(!u) u=getenv("LOGNAME"); return u?(char*)u:(char*)"";
}
static inline int   _tr_cpu_count(void) {
#if defined(_SC_NPROCESSORS_ONLN)
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(HW_NCPU)
    int mib[2]={CTL_HW,HW_NCPU}; int n=1; size_t l=sizeof(n); sysctl(mib,2,&n,&l,NULL,0); return n>0?n:1;
#else
    return 1;
#endif
}
static inline char* _tr_cwd(void)       { char* b=(char*)_tr_c_malloc(4096); return getcwd(b,4096); }
static inline int   _tr_chdir(const char* p) { return chdir(p); }
#ifdef __APPLE__
#  if defined(TAURARO_IOS)
static inline char* _tr_platform(void) { return _tr_str_dup_owned("ios"); }
#  else
static inline char* _tr_platform(void) { return _tr_str_dup_owned("macos"); }
#  endif
#elif defined(TAURARO_ANDROID)
static inline char* _tr_platform(void) { return _tr_str_dup_owned("android"); }
#elif defined(TAURARO_WASM)
static inline char* _tr_platform(void) { return _tr_str_dup_owned("wasm"); }
#else
static inline char* _tr_platform(void) { return _tr_str_dup_owned("linux"); }
#endif
static inline char* _tr_os_machine(void) {
#if defined(__x86_64__)||defined(__amd64__)
    return _tr_str_dup_owned("x86_64");
#elif defined(__aarch64__)
    return _tr_str_dup_owned("arm64");
#elif defined(__arm__)
    return _tr_str_dup_owned("arm");
#else
    return _tr_str_dup_owned("unknown");
#endif
}
static inline long long _tr_memory_total_mb(void) {
    long p=sysconf(_SC_PHYS_PAGES),s=sysconf(_SC_PAGE_SIZE);
    return (p>0&&s>0)?(long long)p*s/(1024LL*1024LL):0;
}
_TR_XLINK int _tr_tcp_listen(const char* host,int port,int backlog) {
    int s=socket(AF_INET,SOCK_STREAM,0); if(s<0) return -1;
    int opt=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    if(bind(s,(struct sockaddr*)&a,sizeof(a))<0){close(s);return -1;}
    if(listen(s,backlog)<0){close(s);return -1;} return s;
}
/* Disable Nagle's algorithm: without this, every small HTTP response gets
 * delayed ~40ms by Nagle + the peer's delayed-ACK timer, capping keep-alive
 * request latency at ~20-40ms regardless of how fast the handler itself is. */
_TR_XLINK void _tr_tcp_set_nodelay(int fd) {
    int one = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
}
_TR_XLINK int   _tr_tcp_accept(int srv) { int c=accept(srv,NULL,NULL); if(c>=0) _tr_tcp_set_nodelay(c); return c; }
_TR_XLINK char* _tr_tcp_peer_addr(int fd) {
    struct sockaddr_in a; socklen_t al=sizeof(a);
    if(getpeername(fd,(struct sockaddr*)&a,&al)<0) return _tr_empty_heap_str();
    char* buf=(char*)_tr_c_malloc(64); char ip[32];
    inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));
    snprintf(buf,63,"%s:%d",ip,(int)ntohs(a.sin_port)); return buf;
}
_TR_XLINK int  _tr_udp_socket(void) { return socket(AF_INET,SOCK_DGRAM,0); }
_TR_XLINK int  _tr_udp_bind(int fd,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    return bind(fd,(struct sockaddr*)&a,sizeof(a))==0?0:-1;
}
_TR_XLINK int  _tr_udp_send_to(int fd,const char* data,int len,const char* host,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=inet_addr(host);
    return (int)sendto(fd,data,len,0,(struct sockaddr*)&a,sizeof(a));
}
_TR_XLINK int  _tr_udp_recv_from(int fd,char* buf,int cap,char* src) {
    struct sockaddr_in a; socklen_t al=sizeof(a);
    int n=(int)recvfrom(fd,buf,cap,0,(struct sockaddr*)&a,&al);
    if(n>0&&src){char ip[32];inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));snprintf(src,63,"%s:%d",ip,(int)ntohs(a.sin_port));}
    return n;
}
_TR_XLINK void _tr_udp_close(int fd) { close(fd); }
static inline char* _tr_dns_resolve(const char* host) {
    struct addrinfo hints={0},*res=NULL; hints.ai_family=AF_INET;
    if(getaddrinfo(host,NULL,&hints,&res)!=0) return _tr_empty_heap_str();
    char* ip=(char*)_tr_c_malloc(64);
    inet_ntop(AF_INET,&((struct sockaddr_in*)res->ai_addr)->sin_addr,ip,64);
    freeaddrinfo(res); return ip;
}
static inline char* _tr_dns_reverse(const char* ip) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; inet_pton(AF_INET,ip,&a.sin_addr);
    char* buf=(char*)_tr_c_malloc(256);
    return (getnameinfo((struct sockaddr*)&a,sizeof(a),buf,256,NULL,0,NI_NAMEREQD)==0)?buf:(char*)"";
}
static inline void _tr_console_color(int code) { printf("\033[%dm",code); fflush(stdout); }
static inline void _tr_console_reset(void)     { printf("\033[0m"); fflush(stdout); }
static inline void _tr_console_clear(void)     { printf("\033[2J\033[H"); fflush(stdout); }
#endif

/* ══════════════════════════════════════════════════════════════════════════
 * Non-blocking socket API
 *
 * Return values:
 *   >= 0  : success / bytes transferred
 *   -1    : hard error
 *   TAURARO_WOULD_BLOCK (-2) : operation would block (EAGAIN/EWOULDBLOCK/WSAEWOULDBLOCK)
 *
 * Typical pattern with _TrIOPoll:
 *   int fd = _tr_tcp_connect_nb(host, port);   // initiates connect, may return fd immediately
 *   _tr_iopoll_add(poll, fd, TAURARO_POLLOUT, ctx); // wait for writable = connect done
 *   int n = _tr_tcp_recv_nb(fd, buf, cap);      // -2 means try again later
 * ══════════════════════════════════════════════════════════════════════════ */
#define TAURARO_WOULD_BLOCK (-2)

#if defined(TAURARO_NO_NET)
_TR_XLINK int  _tr_tcp_set_nonblocking(int fd)                    { (void)fd; return -1; }
_TR_XLINK int  _tr_tcp_recv_nb(int fd, char* b, int c)            { (void)fd;(void)b;(void)c; return -1; }
_TR_XLINK int  _tr_tcp_send_nb(int fd, const char* d, int l)      { (void)fd;(void)d;(void)l; return -1; }
_TR_XLINK int  _tr_tcp_accept_nb(int fd)                          { (void)fd; return TAURARO_WOULD_BLOCK; }
_TR_XLINK int  _tr_tcp_connect_nb(const char* h, int p)           { (void)h;(void)p; return -1; }

#elif defined(_WIN32)
#ifndef _TR_NET_INCLUDED
#define _TR_NET_INCLUDED
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
_TR_XLINK int _tr_tcp_set_nonblocking(int fd) {
    u_long mode = 1;
    return ioctlsocket((SOCKET)fd, FIONBIO, &mode) == 0 ? 0 : -1;
}
_TR_XLINK int _tr_tcp_recv_nb(int fd, char* buf, int cap) {
    int n = recv((SOCKET)fd, buf, cap, 0);
    if (n < 0 && WSAGetLastError() == WSAEWOULDBLOCK) return TAURARO_WOULD_BLOCK;
    return n;
}
_TR_XLINK int _tr_tcp_send_nb(int fd, const char* data, int len) {
    int n = send((SOCKET)fd, data, len, 0);
    if (n < 0 && WSAGetLastError() == WSAEWOULDBLOCK) return TAURARO_WOULD_BLOCK;
    return n;
}
_TR_XLINK int _tr_tcp_accept_nb(int server_fd) {
    SOCKET s = accept((SOCKET)server_fd, NULL, NULL);
    if (s == INVALID_SOCKET) {
        return (WSAGetLastError() == WSAEWOULDBLOCK) ? TAURARO_WOULD_BLOCK : -1;
    }
    _tr_tcp_set_nodelay((int)s);
    return (int)s;
}
_TR_XLINK int _tr_tcp_connect_nb(const char* host, int port) {
    _tr_net_init();
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
    char pbuf[16]; snprintf(pbuf, sizeof(pbuf), "%d", port);
    if (getaddrinfo(host, pbuf, &hints, &res) != 0) return -1;
    SOCKET fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == INVALID_SOCKET) { freeaddrinfo(res); return -1; }
    u_long mode = 1; ioctlsocket(fd, FIONBIO, &mode);
    connect(fd, res->ai_addr, (int)res->ai_addrlen); /* WSAEWOULDBLOCK is expected */
    freeaddrinfo(res);
    return (int)fd;
}

#else /* POSIX */
#include <fcntl.h>
#include <errno.h>
#include <netinet/tcp.h>
_TR_XLINK int _tr_tcp_set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0 ? 0 : -1;
}
_TR_XLINK int _tr_tcp_recv_nb(int fd, char* buf, int cap) {
    int n = (int)recv(fd, buf, (size_t)cap, 0);
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return TAURARO_WOULD_BLOCK;
    return n;
}
_TR_XLINK int _tr_tcp_send_nb(int fd, const char* data, int len) {
    int n = (int)send(fd, data, (size_t)len, 0);
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return TAURARO_WOULD_BLOCK;
    return n;
}
_TR_XLINK int _tr_tcp_accept_nb(int server_fd) {
    int fd = accept(server_fd, NULL, NULL);
    if (fd < 0) return (errno == EAGAIN || errno == EWOULDBLOCK) ? TAURARO_WOULD_BLOCK : -1;
    _tr_tcp_set_nodelay(fd);
    return fd;
}
_TR_XLINK int _tr_tcp_connect_nb(const char* host, int port) {
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
    char pbuf[16]; snprintf(pbuf, sizeof(pbuf), "%d", port);
    if (getaddrinfo(host, pbuf, &hints, &res) != 0) return -1;
    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd < 0) { freeaddrinfo(res); return -1; }
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    connect(fd, res->ai_addr, res->ai_addrlen); /* EINPROGRESS expected */
    freeaddrinfo(res);
    return fd;
}
#endif /* non-blocking socket API */

/* Binary-safe non-blocking send: like _tr_tcp_send_nb but takes a raw byte
 * pointer + explicit length (no NUL-terminated str), for framed protocols
 * (e.g. WebSocket) whose payloads contain NUL bytes. */
_TR_XLINK int _tr_tcp_send_raw(int fd, char* buf, int len) { return _tr_tcp_send_nb(fd, buf, len); }

/* -- Random (LCG-64) ------------------------------------------------------- */
typedef struct { unsigned long long s; } _TrRng;
static inline _TrRng* _tr_rng_new(long long seed) {
    _TrRng* r=(_TrRng*)_tr_c_malloc(sizeof(_TrRng));
    r->s=(unsigned long long)(seed^0xdeadbeefcafeULL); return r;
}
static inline long long _tr_rng_next(_TrRng* r) {
    r->s=r->s*6364136223846793005ULL+1442695040888963407ULL;
    return (long long)((r->s>>1)&0x7FFFFFFFFFFFFFFFLL);
}
static inline void _tr_rng_free(_TrRng* r) { _tr_free(r); }


static inline char* _tr_float_fmt(double f, int decimals) {
    /* SECURITY [V-001]: bound the format. A large `decimals` (possibly user-controlled)
     * or a large-magnitude double (integer part up to ~309 digits for 1e308) overflowed
     * the old fixed 64-byte buffer via unbounded sprintf. Clamp precision, measure the
     * exact length with snprintf, then allocate to fit. */
    int d = decimals < 0 ? 6 : decimals;
    if (d > 40) d = 40;                       /* >17 significant digits is meaningless for double */
    char fmt[16];
    snprintf(fmt, sizeof(fmt), "%%.%df", d);
    int need = snprintf((char*)0, 0, fmt, f); /* measure required length (writes nothing) */
    if (need < 0) need = 63;
    char* buf = (char*)_tr_c_malloc((size_t)need + 1);
    if(!buf) return _tr_empty_heap_str();
    snprintf(buf, (size_t)need + 1, fmt, f);
    return buf;
}

/* ── Platform capability detection ──────────────────────────────────────
 * Call from Tauraro via  extern "C":  def _tr_target_has_filesystem() -> bool
 * ─────────────────────────────────────────────────────────────────────── */
static inline bool _tr_target_has_filesystem(void) {
#if defined(TAURARO_BARE) && !defined(__wasi__)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_target_has_networking(void) {
#if defined(TAURARO_BARE) || defined(TAURARO_WASM)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_target_has_threads(void) {
#ifdef TAURARO_BARE
    return false;
#else
    return true;
#endif
}
static inline bool _tr_target_has_os_services(void) {
#if defined(TAURARO_BARE) && !defined(__wasi__)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_is_android(void) {
#ifdef TAURARO_ANDROID
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_ios(void) {
#ifdef TAURARO_IOS
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_wasm(void) {
#ifdef TAURARO_WASM
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_embedded(void) {
#if defined(TAURARO_BARE) && !defined(TAURARO_WASM)
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_posix(void) {
#if defined(_WIN32) || defined(TAURARO_BARE)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_is_mobile(void) {
#if defined(TAURARO_ANDROID) || defined(TAURARO_IOS)
    return true;
#else
    return false;
#endif
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Executable directory — returns directory containing the running binary.
 * ═══════════════════════════════════════════════════════════════════════════ */
#if defined(__APPLE__)
#  include <mach-o/dyld.h>
#endif
_TR_XLINK char* _tr_exe_dir(void) {
#if defined(_WIN32)
    char* buf=(char*)_tr_c_malloc(4096);
    DWORD n=GetModuleFileNameA(NULL,buf,4096);
    if(!n){buf[0]='.';buf[1]='\0';return buf;}
    for(int i=(int)n-1;i>0;i--){if(buf[i]=='\\'||buf[i]=='/'){buf[i]='\0';break;}}
    return buf;
#elif defined(__APPLE__)
    char tmp[4096]; uint32_t sz=sizeof(tmp);
    if(_NSGetExecutablePath(tmp,&sz)!=0) return _tr_str_dup_owned(".");
    char* buf=(char*)_tr_c_malloc(4096);
    if(!realpath(tmp,buf)){strcpy(buf,".");return buf;}
    for(int i=(int)strlen(buf)-1;i>0;i--){if(buf[i]=='/'){buf[i]='\0';break;}}
    return buf;
#elif defined(__linux__)
    char* buf=(char*)_tr_c_malloc(4096);
    ssize_t n=readlink("/proc/self/exe",buf,4095);
    if(n<=0){buf[0]='.';buf[1]='\0';return buf;}
    buf[n]='\0';
    for(int i=(int)n-1;i>0;i--){if(buf[i]=='/'){buf[i]='\0';break;}}
    return buf;
#else
    return _tr_str_dup_owned(".");
#endif
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Shutdown signal — Ctrl+C (SIGINT) / SIGTERM sets a flag, polled by server
 * accept loops to exit cleanly instead of being killed mid-request.
 * ═══════════════════════════════════════════════════════════════════════════ */
/* wasi has no signals (needs -D_WASI_EMULATED_SIGNAL); take the no-op stubs. */
#if !defined(TAURARO_BARE) && !defined(__wasi__)
#include <signal.h>
static volatile int _tr_shutdown_flag = 0;
static void _tr_shutdown_signal_handler(int sig) { (void)sig; _tr_shutdown_flag = 1; }
static inline void _tr_install_shutdown_handler(void) {
    signal(SIGINT, _tr_shutdown_signal_handler);
#ifdef SIGTERM
    signal(SIGTERM, _tr_shutdown_signal_handler);
#endif
}
static inline bool _tr_shutdown_requested(void) { return _tr_shutdown_flag != 0; }
#else
static inline void _tr_install_shutdown_handler(void) {}
static inline bool _tr_shutdown_requested(void) { return false; }
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * REGEX — POSIX regex.h on Linux/Mac; stubs on Windows and bare-metal.
 * ═══════════════════════════════════════════════════════════════════════════ */
#ifndef TAURARO_BARE
#  if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#    include <regex.h>
#    define TAURARO_HAVE_REGEX 1
#  endif
#endif

#ifdef TAURARO_HAVE_REGEX
typedef struct { regex_t re; } _TrRegex;
static inline char* _tr_regex_compile(char* pattern, int icase) {
    _TrRegex* r = (_TrRegex*)TAURARO_ALLOC(sizeof(_TrRegex));
    if (!r) return NULL;
    int flags = REG_EXTENDED; if (icase) flags |= REG_ICASE;
    if (regcomp(&r->re, pattern, flags) != 0) { TAURARO_FREE(r); return NULL; }
    return (char*)r;
}
static inline bool _tr_regex_match(char* handle, char* text) {
    if (!handle || !text) return false;
    return regexec(&((_TrRegex*)handle)->re, text, 0, NULL, 0) == 0;
}
static inline int _tr_regex_find_start(char* handle, char* text, int from) {
    if (!handle || !text) return -1;
    regmatch_t m;
    if (regexec(&((_TrRegex*)handle)->re, text + from, 1, &m, 0) != 0) return -1;
    return from + (int)m.rm_so;
}
static inline int _tr_regex_find_len(char* handle, char* text, int from) {
    if (!handle || !text) return -1;
    regmatch_t m;
    if (regexec(&((_TrRegex*)handle)->re, text + from, 1, &m, 0) != 0) return -1;
    return (int)(m.rm_eo - m.rm_so);
}
static inline char* _tr_regex_replace_first(char* handle, char* text, char* repl) {
    if (!handle || !text || !repl) return _tr_strdup(text ? text : "");
    regmatch_t m;
    if (regexec(&((_TrRegex*)handle)->re, text, 1, &m, 0) != 0) return _tr_strdup(text);
    size_t pre = (size_t)m.rm_so, rlen = strlen(repl), post = strlen(text) - (size_t)m.rm_eo;
    char* out = (char*)TAURARO_ALLOC(pre + rlen + post + 1); if (!out) return _tr_strdup(text);
    memcpy(out, text, pre); memcpy(out+pre, repl, rlen); memcpy(out+pre+rlen, text+m.rm_eo, post);
    out[pre+rlen+post] = '\0'; return out;
}
static inline char* _tr_regex_replace_all(char* handle, char* text, char* repl) {
    if (!handle || !text || !repl) return _tr_strdup(text ? text : "");
    _TrRegex* r = (_TrRegex*)handle; regmatch_t m;
    size_t rlen = strlen(repl), cur = 0, tlen = strlen(text);
    char* result = _tr_strdup("");
    while (cur < tlen && regexec(&r->re, text + cur, 1, &m, 0) == 0) {
        size_t pre = (size_t)m.rm_so, olen = strlen(result);
        char* tmp = (char*)TAURARO_ALLOC(olen + pre + rlen + 1); if (!tmp) break;
        memcpy(tmp, result, olen); memcpy(tmp+olen, text+cur, pre);
        memcpy(tmp+olen+pre, repl, rlen); tmp[olen+pre+rlen] = '\0';
        TAURARO_FREE(result); result = tmp;
        size_t adv = (size_t)(m.rm_eo - m.rm_so); if (adv == 0) adv = 1;
        cur += pre + adv;
    }
    size_t rem = tlen - cur, olen2 = strlen(result);
    char* out = (char*)TAURARO_ALLOC(olen2 + rem + 1);
    if (out) { memcpy(out, result, olen2); memcpy(out+olen2, text+cur, rem); out[olen2+rem]='\0'; TAURARO_FREE(result); return out; }
    return result;
}
static inline int _tr_regex_count(char* handle, char* text) {
    if (!handle || !text) return 0;
    regmatch_t m; int n = 0; size_t cur = 0, tlen = strlen(text);
    while (cur < tlen && regexec(&((_TrRegex*)handle)->re, text+cur, 1, &m, 0) == 0) {
        n++; size_t adv=(size_t)(m.rm_eo-m.rm_so); if(adv==0)adv=1; cur+=(size_t)m.rm_so+adv;
    }
    return n;
}
static inline void _tr_regex_free(char* handle) {
    if (!handle) return; regfree(&((_TrRegex*)handle)->re); TAURARO_FREE(handle);
}
#else
static inline char* _tr_regex_compile(char* p, int i) { (void)p;(void)i; return NULL; }
static inline bool  _tr_regex_match(char* h, char* t) { (void)h;(void)t; return false; }
static inline int   _tr_regex_find_start(char* h, char* t, int f) { (void)h;(void)t;(void)f; return -1; }
static inline int   _tr_regex_find_len(char* h, char* t, int f) { (void)h;(void)t;(void)f; return -1; }
static inline char* _tr_regex_replace_first(char* h, char* t, char* r) { (void)h;(void)r; return _tr_strdup(t?t:""); }
static inline char* _tr_regex_replace_all(char* h, char* t, char* r) { (void)h;(void)r; return _tr_strdup(t?t:""); }
static inline int   _tr_regex_count(char* h, char* t) { (void)h;(void)t; return 0; }
static inline void  _tr_regex_free(char* h) { (void)h; }
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * SHA-256 — pure C, no external dependencies.
 * ═══════════════════════════════════════════════════════════════════════════ */
#define _TR_ROTR32(x,n) (((x)>>(n))|((x)<<(32-(n))))
#define _TR_S0(x) (_TR_ROTR32(x,2)^_TR_ROTR32(x,13)^_TR_ROTR32(x,22))
#define _TR_S1(x) (_TR_ROTR32(x,6)^_TR_ROTR32(x,11)^_TR_ROTR32(x,25))
#define _TR_s0(x) (_TR_ROTR32(x,7)^_TR_ROTR32(x,18)^((x)>>3))
#define _TR_s1(x) (_TR_ROTR32(x,17)^_TR_ROTR32(x,19)^((x)>>10))
#define _TR_CH(x,y,z) (((x)&(y))^(~(x)&(z)))
#define _TR_MAJ(x,y,z) (((x)&(y))^((x)&(z))^((y)&(z)))

static const uint32_t _tr_sha256_K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

typedef struct { uint32_t h[8]; uint8_t buf[64]; uint64_t bits; uint32_t buf_len; } _TrSHA256Ctx;

static inline void _tr_sha256_init(_TrSHA256Ctx* c) {
    c->h[0]=0x6a09e667;c->h[1]=0xbb67ae85;c->h[2]=0x3c6ef372;c->h[3]=0xa54ff53a;
    c->h[4]=0x510e527f;c->h[5]=0x9b05688c;c->h[6]=0x1f83d9ab;c->h[7]=0x5be0cd19;
    c->bits=0; c->buf_len=0;
}
static inline void _tr_sha256_block(_TrSHA256Ctx* c, const uint8_t* blk) {
    uint32_t w[64],a,b,cc,d,e,f,g,h,t1,t2; int i;
    for(i=0;i<16;i++) w[i]=((uint32_t)blk[i*4]<<24)|((uint32_t)blk[i*4+1]<<16)|((uint32_t)blk[i*4+2]<<8)|(uint32_t)blk[i*4+3];
    for(i=16;i<64;i++) w[i]=_TR_s1(w[i-2])+w[i-7]+_TR_s0(w[i-15])+w[i-16];
    a=c->h[0];b=c->h[1];cc=c->h[2];d=c->h[3];e=c->h[4];f=c->h[5];g=c->h[6];h=c->h[7];
    for(i=0;i<64;i++){
        t1=h+_TR_S1(e)+_TR_CH(e,f,g)+_tr_sha256_K[i]+w[i];
        t2=_TR_S0(a)+_TR_MAJ(a,b,cc);
        h=g;g=f;f=e;e=d+t1;d=cc;cc=b;b=a;a=t1+t2;
    }
    c->h[0]+=a;c->h[1]+=b;c->h[2]+=cc;c->h[3]+=d;c->h[4]+=e;c->h[5]+=f;c->h[6]+=g;c->h[7]+=h;
}
static inline void _tr_sha256_update(_TrSHA256Ctx* c, const uint8_t* data, size_t len) {
    for(size_t i=0;i<len;i++){
        c->buf[c->buf_len++]=data[i]; c->bits+=8;
        if(c->buf_len==64){_tr_sha256_block(c,c->buf);c->buf_len=0;}
    }
}
static inline void _tr_sha256_final(_TrSHA256Ctx* c, uint8_t* dig) {
    uint64_t bits=c->bits;
    c->buf[c->buf_len++]=0x80;
    while(c->buf_len!=56){if(c->buf_len==64){_tr_sha256_block(c,c->buf);c->buf_len=0;}c->buf[c->buf_len++]=0;}
    for(int i=7;i>=0;i--){c->buf[56+(7-i)]=(uint8_t)(bits>>((uint64_t)i*8));}
    _tr_sha256_block(c,c->buf);
    for(int i=0;i<8;i++){dig[i*4]=(uint8_t)(c->h[i]>>24);dig[i*4+1]=(uint8_t)(c->h[i]>>16);dig[i*4+2]=(uint8_t)(c->h[i]>>8);dig[i*4+3]=(uint8_t)c->h[i];}
}
static const char _tr_hex_lc[] = "0123456789abcdef";
static inline char* _tr_sha256_hex(char* input) {
    _TrSHA256Ctx ctx; uint8_t dig[32];
    _tr_sha256_init(&ctx);
    if(input) _tr_sha256_update(&ctx,(const uint8_t*)input,strlen(input));
    _tr_sha256_final(&ctx,dig);
    char* out=(char*)TAURARO_ALLOC(65); if(!out) return NULL;
    for(int i=0;i<32;i++){out[i*2]=_tr_hex_lc[dig[i]>>4];out[i*2+1]=_tr_hex_lc[dig[i]&15];}
    out[64]='\0'; return out;
}
static inline char* _tr_sha256_bytes_of(char* input, int ilen) {
    _TrSHA256Ctx ctx; uint8_t dig[32];
    _tr_sha256_init(&ctx);
    if(input&&ilen>0) _tr_sha256_update(&ctx,(const uint8_t*)input,(size_t)ilen);
    _tr_sha256_final(&ctx,dig);
    char* out=(char*)TAURARO_ALLOC(32); if(!out) return NULL;
    memcpy(out,dig,32); return out;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Ed25519 signatures — TweetNaCl subset (public domain), pure C.
 * Exposes hex-string wrappers: _tr_ed25519_{gen_seed,pubkey,sign,verify}.
 * ═══════════════════════════════════════════════════════════════════════════ */
typedef int64_t _tr_gf[16];
static int _tr_cv32(const uint8_t*, const uint8_t*);
static const uint64_t _tr_edK[80] = {
  0x428a2f98d728ae22ULL,0x7137449123ef65cdULL,0xb5c0fbcfec4d3b2fULL,0xe9b5dba58189dbbcULL,
  0x3956c25bf348b538ULL,0x59f111f1b605d019ULL,0x923f82a4af194f9bULL,0xab1c5ed5da6d8118ULL,
  0xd807aa98a3030242ULL,0x12835b0145706fbeULL,0x243185be4ee4b28cULL,0x550c7dc3d5ffb4e2ULL,
  0x72be5d74f27b896fULL,0x80deb1fe3b1696b1ULL,0x9bdc06a725c71235ULL,0xc19bf174cf692694ULL,
  0xe49b69c19ef14ad2ULL,0xefbe4786384f25e3ULL,0x0fc19dc68b8cd5b5ULL,0x240ca1cc77ac9c65ULL,
  0x2de92c6f592b0275ULL,0x4a7484aa6ea6e483ULL,0x5cb0a9dcbd41fbd4ULL,0x76f988da831153b5ULL,
  0x983e5152ee66dfabULL,0xa831c66d2db43210ULL,0xb00327c898fb213fULL,0xbf597fc7beef0ee4ULL,
  0xc6e00bf33da88fc2ULL,0xd5a79147930aa725ULL,0x06ca6351e003826fULL,0x142929670a0e6e70ULL,
  0x27b70a8546d22ffcULL,0x2e1b21385c26c926ULL,0x4d2c6dfc5ac42aedULL,0x53380d139d95b3dfULL,
  0x650a73548baf63deULL,0x766a0abb3c77b2a8ULL,0x81c2c92e47edaee6ULL,0x92722c851482353bULL,
  0xa2bfe8a14cf10364ULL,0xa81a664bbc423001ULL,0xc24b8b70d0f89791ULL,0xc76c51a30654be30ULL,
  0xd192e819d6ef5218ULL,0xd69906245565a910ULL,0xf40e35855771202aULL,0x106aa07032bbd1b8ULL,
  0x19a4c116b8d2d0c8ULL,0x1e376c085141ab53ULL,0x2748774cdf8eeb99ULL,0x34b0bcb5e19b48a8ULL,
  0x391c0cb3c5c95a63ULL,0x4ed8aa4ae3418acbULL,0x5b9cca4f7763e373ULL,0x682e6ff3d6b2b8a3ULL,
  0x748f82ee5defb2fcULL,0x78a5636f43172f60ULL,0x84c87814a1f0ab72ULL,0x8cc702081a6439ecULL,
  0x90befffa23631e28ULL,0xa4506cebde82bde9ULL,0xbef9a3f7b2c67915ULL,0xc67178f2e372532bULL,
  0xca273eceea26619cULL,0xd186b8c721c0c207ULL,0xeada7dd6cde0eb1eULL,0xf57d4f7fee6ed178ULL,
  0x06f067aa72176fbaULL,0x0a637dc5a2c898a6ULL,0x113f9804bef90daeULL,0x1b710b35131c471bULL,
  0x28db77f523047d84ULL,0x32caab7b40c72493ULL,0x3c9ebe0a15c9bebcULL,0x431d67c49c100d4cULL,
  0x4cc5d4becb3e42b6ULL,0x597f299cfc657e2aULL,0x5fcb6fab3ad6faecULL,0x6c44198c4a475817ULL
};
static uint64_t _tr_edl64(const uint8_t*x){ uint64_t i,u=0; for(i=0;i<8;i++) u=(u<<8)|x[i]; return u; }
static void _tr_edts64(uint8_t*x,uint64_t u){ int i; for(i=7;i>=0;--i){ x[i]=(uint8_t)u; u>>=8; } }
#define _TR_EDR(x,c) (((x)>>(c))|((x)<<(64-(c))))
#define _TR_EDCh(x,y,z) ((x&y)^(~x&z))
#define _TR_EDMaj(x,y,z) ((x&y)^(x&z)^(y&z))
#define _TR_EDSg0(x) (_TR_EDR(x,28)^_TR_EDR(x,34)^_TR_EDR(x,39))
#define _TR_EDSg1(x) (_TR_EDR(x,14)^_TR_EDR(x,18)^_TR_EDR(x,41))
#define _TR_EDsg0(x) (_TR_EDR(x,1)^_TR_EDR(x,8)^((x)>>7))
#define _TR_EDsg1(x) (_TR_EDR(x,19)^_TR_EDR(x,61)^((x)>>6))
static int _tr_edhashblocks(uint8_t*x,const uint8_t*m,uint64_t n){
  uint64_t z[8],b[8],a[8],w[16],t; int i,j;
  for(i=0;i<8;i++) z[i]=a[i]=_tr_edl64(x+8*i);
  while(n>=128){
    for(i=0;i<16;i++) w[i]=_tr_edl64(m+8*i);
    for(i=0;i<80;i++){
      for(j=0;j<8;j++) b[j]=a[j];
      t=a[7]+_TR_EDSg1(a[4])+_TR_EDCh(a[4],a[5],a[6])+_tr_edK[i]+w[i%16];
      b[7]=t+_TR_EDSg0(a[0])+_TR_EDMaj(a[0],a[1],a[2]);
      b[3]+=t;
      for(j=0;j<8;j++) a[(j+1)%8]=b[j];
      if(i%16==15) for(j=0;j<16;j++) w[j]+=w[(j+9)%16]+_TR_EDsg0(w[(j+1)%16])+_TR_EDsg1(w[(j+14)%16]);
    }
    for(i=0;i<8;i++){ a[i]+=z[i]; z[i]=a[i]; }
    m+=128; n-=128;
  }
  for(i=0;i<8;i++) _tr_edts64(x+8*i,z[i]);
  return (int)n;
}
static const uint8_t _tr_edIv[64]={
  0x6a,0x09,0xe6,0x67,0xf3,0xbc,0xc9,0x08,0xbb,0x67,0xae,0x85,0x84,0xca,0xa7,0x3b,
  0x3c,0x6e,0xf3,0x72,0xfe,0x94,0xf8,0x2b,0xa5,0x4f,0xf5,0x3a,0x5f,0x1d,0x36,0xf1,
  0x51,0x0e,0x52,0x7f,0xad,0xe6,0x82,0xd1,0x9b,0x05,0x68,0x8c,0x2b,0x3e,0x6c,0x1f,
  0x1f,0x83,0xd9,0xab,0xfb,0x41,0xbd,0x6b,0x5b,0xe0,0xcd,0x19,0x13,0x7e,0x21,0x79
};
static void _tr_edhash(uint8_t*out,const uint8_t*m,uint64_t n){
  uint8_t h[64],x[256]; uint64_t i,b=n;
  for(i=0;i<64;i++) h[i]=_tr_edIv[i];
  _tr_edhashblocks(h,m,n);
  m+=n; n&=127; m-=n;
  for(i=0;i<256;i++) x[i]=0;
  for(i=0;i<n;i++) x[i]=m[i];
  x[n]=128;
  n=256-128*(n<112);
  x[n-9]=(uint8_t)(b>>61);
  _tr_edts64(x+n-8,b<<3);
  _tr_edhashblocks(h,x,n);
  for(i=0;i<64;i++) out[i]=h[i];
}
static const _tr_gf _tr_gf0={0}, _tr_gf1={1},
  _tr_edD={0x78a3,0x1359,0x4dca,0x75eb,0xd8ab,0x4141,0x0a4d,0x0070,0xe898,0x7779,0x4079,0x8cc7,0xfe73,0x2b6f,0x6cee,0x5203},
  _tr_edD2={0xf159,0x26b2,0x9b94,0xebd6,0xb156,0x8283,0x149a,0x00e0,0xd130,0xeef3,0x80f2,0x198e,0xfce7,0x56df,0xd9dc,0x2406},
  _tr_edX={0xd51a,0x8f25,0x2d60,0xc956,0xa7b2,0x9525,0xc760,0x692c,0xdc5c,0xfdd6,0xe231,0xc0a4,0x53fe,0xcd6e,0x36d3,0x2169},
  _tr_edY={0x6658,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666,0x6666},
  _tr_edI={0xa0b0,0x4a0e,0x1b27,0xc4ee,0xe478,0xad2f,0x1806,0x2f43,0xd7a7,0x3dfb,0x0099,0x2b4d,0xdf0b,0x4fc1,0x2480,0x2b83};
static void _tr_edset(_tr_gf r,const _tr_gf a){ int i; for(i=0;i<16;i++) r[i]=a[i]; }
static void _tr_edcar(_tr_gf o){ int i; int64_t c; for(i=0;i<16;i++){ o[i]+=(1LL<<16); c=o[i]>>16; o[(i+1)*(i<15)]+=c-1+37*(c-1)*(i==15); o[i]-=c<<16; } }
static void _tr_edsel(_tr_gf p,_tr_gf q,int b){ int64_t t,i,c=~(b-1); for(i=0;i<16;i++){ t=c&(p[i]^q[i]); p[i]^=t; q[i]^=t; } }
static void _tr_edpack25519(uint8_t*o,const _tr_gf n){
  int i,j,b; _tr_gf m,t;
  for(i=0;i<16;i++) t[i]=n[i];
  _tr_edcar(t); _tr_edcar(t); _tr_edcar(t);
  for(j=0;j<2;j++){
    m[0]=t[0]-0xffed;
    for(i=1;i<15;i++){ m[i]=t[i]-0xffff-((m[i-1]>>16)&1); m[i-1]&=0xffff; }
    m[15]=t[15]-0x7fff-((m[14]>>16)&1);
    b=(int)((m[15]>>16)&1);
    m[14]&=0xffff;
    _tr_edsel(t,m,1-b);
  }
  for(i=0;i<16;i++){ o[2*i]=(uint8_t)(t[i]&0xff); o[2*i+1]=(uint8_t)(t[i]>>8); }
}
static int _tr_edneq(const _tr_gf a,const _tr_gf b){ uint8_t c[32],d[32]; _tr_edpack25519(c,a); _tr_edpack25519(d,b); return _tr_cv32(c,d); }
static uint8_t _tr_edpar(const _tr_gf a){ uint8_t d[32]; _tr_edpack25519(d,a); return d[0]&1; }
static void _tr_edunpack25519(_tr_gf o,const uint8_t*n){ int i; for(i=0;i<16;i++) o[i]=n[2*i]+((int64_t)n[2*i+1]<<8); o[15]&=0x7fff; }
static void _tr_edA(_tr_gf o,const _tr_gf a,const _tr_gf b){ int i; for(i=0;i<16;i++) o[i]=a[i]+b[i]; }
static void _tr_edZ(_tr_gf o,const _tr_gf a,const _tr_gf b){ int i; for(i=0;i<16;i++) o[i]=a[i]-b[i]; }
static void _tr_edM(_tr_gf o,const _tr_gf a,const _tr_gf b){ int64_t i,j,t[31]; for(i=0;i<31;i++) t[i]=0; for(i=0;i<16;i++) for(j=0;j<16;j++) t[i+j]+=a[i]*b[j]; for(i=0;i<15;i++) t[i]+=38*t[i+16]; for(i=0;i<16;i++) o[i]=t[i]; _tr_edcar(o); _tr_edcar(o); }
static void _tr_edS(_tr_gf o,const _tr_gf a){ _tr_edM(o,a,a); }
static void _tr_edinv(_tr_gf o,const _tr_gf i){ _tr_gf c; int a; for(a=0;a<16;a++) c[a]=i[a]; for(a=253;a>=0;a--){ _tr_edS(c,c); if(a!=2&&a!=4) _tr_edM(c,c,i); } for(a=0;a<16;a++) o[a]=c[a]; }
static void _tr_edpow2523(_tr_gf o,const _tr_gf i){ _tr_gf c; int a; for(a=0;a<16;a++) c[a]=i[a]; for(a=250;a>=0;a--){ _tr_edS(c,c); if(a!=1) _tr_edM(c,c,i); } for(a=0;a<16;a++) o[a]=c[a]; }
static int _tr_cv32(const uint8_t*x,const uint8_t*y){ uint32_t d=0; int i; for(i=0;i<32;i++) d|=(uint32_t)(x[i]^y[i]); return (int)((1&((d-1)>>8))-1); }
static void _tr_edadd(_tr_gf p[4],_tr_gf q[4]){
  _tr_gf a,b,c,d,t,e,f,g,h;
  _tr_edZ(a,p[1],p[0]); _tr_edZ(t,q[1],q[0]); _tr_edM(a,a,t);
  _tr_edA(b,p[0],p[1]); _tr_edA(t,q[0],q[1]); _tr_edM(b,b,t);
  _tr_edM(c,p[3],q[3]); _tr_edM(c,c,_tr_edD2);
  _tr_edM(d,p[2],q[2]); _tr_edA(d,d,d);
  _tr_edZ(e,b,a); _tr_edZ(f,d,c); _tr_edA(g,d,c); _tr_edA(h,b,a);
  _tr_edM(p[0],e,f); _tr_edM(p[1],h,g); _tr_edM(p[2],g,f); _tr_edM(p[3],e,h);
}
static void _tr_edcswap(_tr_gf p[4],_tr_gf q[4],uint8_t b){ int i; for(i=0;i<4;i++) _tr_edsel(p[i],q[i],b); }
static void _tr_edpack(uint8_t*r,_tr_gf p[4]){
  _tr_gf tx,ty,zi;
  _tr_edinv(zi,p[2]); _tr_edM(tx,p[0],zi); _tr_edM(ty,p[1],zi);
  _tr_edpack25519(r,ty); r[31]^=_tr_edpar(tx)<<7;
}
static void _tr_edscalarmult(_tr_gf p[4],_tr_gf q[4],const uint8_t*s){
  int i;
  _tr_edset(p[0],_tr_gf0); _tr_edset(p[1],_tr_gf1); _tr_edset(p[2],_tr_gf1); _tr_edset(p[3],_tr_gf0);
  for(i=255;i>=0;--i){ uint8_t b=(s[i/8]>>(i&7))&1; _tr_edcswap(p,q,b); _tr_edadd(q,p); _tr_edadd(p,p); _tr_edcswap(p,q,b); }
}
static void _tr_edscalarbase(_tr_gf p[4],const uint8_t*s){
  _tr_gf q[4];
  _tr_edset(q[0],_tr_edX); _tr_edset(q[1],_tr_edY); _tr_edset(q[2],_tr_gf1); _tr_edM(q[3],_tr_edX,_tr_edY);
  _tr_edscalarmult(p,q,s);
}
static void _tr_edkeypair(uint8_t*pk,uint8_t*sk,const uint8_t*seed){
  uint8_t d[64]; _tr_gf p[4]; int i;
  for(i=0;i<32;i++) sk[i]=seed[i];
  _tr_edhash(d,sk,32); d[0]&=248; d[31]&=127; d[31]|=64;
  _tr_edscalarbase(p,d); _tr_edpack(pk,p);
  for(i=0;i<32;i++) sk[32+i]=pk[i];
}
static const uint64_t _tr_edL[32]={0xed,0xd3,0xf5,0x5c,0x1a,0x63,0x12,0x58,0xd6,0x9c,0xf7,0xa2,0xde,0xf9,0xde,0x14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x10};
static void _tr_edmodL(uint8_t*r,int64_t x[64]){
  int64_t carry,i,j;
  for(i=63;i>=32;--i){
    carry=0;
    for(j=i-32;j<i-12;++j){ x[j]+=carry-16*x[i]*_tr_edL[j-(i-32)]; carry=(x[j]+128)>>8; x[j]-=carry<<8; }
    x[j]+=carry; x[i]=0;
  }
  carry=0;
  for(j=0;j<32;j++){ x[j]+=carry-(x[31]>>4)*_tr_edL[j]; carry=x[j]>>8; x[j]&=255; }
  for(j=0;j<32;j++) x[j]-=carry*_tr_edL[j];
  for(i=0;i<32;i++){ x[i+1]+=x[i]>>8; r[i]=(uint8_t)(x[i]&255); }
}
static void _tr_edreduce(uint8_t*r){ int64_t x[64],i; for(i=0;i<64;i++) x[i]=(uint64_t)r[i]; for(i=0;i<64;i++) r[i]=0; _tr_edmodL(r,x); }
static void _tr_edsign(uint8_t*sm,uint64_t*smlen,const uint8_t*m,uint64_t n,const uint8_t*sk){
  uint8_t d[64],h[64],r[64]; int64_t i,j,x[64]; _tr_gf p[4];
  _tr_edhash(d,sk,32); d[0]&=248; d[31]&=127; d[31]|=64;
  *smlen=n+64;
  for(i=0;i<(int64_t)n;i++) sm[64+i]=m[i];
  for(i=0;i<32;i++) sm[32+i]=d[32+i];
  _tr_edhash(r,sm+32,n+32); _tr_edreduce(r);
  _tr_edscalarbase(p,r); _tr_edpack(sm,p);
  for(i=0;i<32;i++) sm[i+32]=sk[i+32];
  _tr_edhash(h,sm,n+64); _tr_edreduce(h);
  for(i=0;i<64;i++) x[i]=0;
  for(i=0;i<32;i++) x[i]=(uint64_t)r[i];
  for(i=0;i<32;i++) for(j=0;j<32;j++) x[i+j]+=(int64_t)h[i]*(uint64_t)d[j];
  _tr_edmodL(sm+32,x);
}
static int _tr_edunpackneg(_tr_gf r[4],const uint8_t p[32]){
  _tr_gf t,chk,num,den,den2,den4,den6;
  _tr_edset(r[2],_tr_gf1); _tr_edunpack25519(r[1],p);
  _tr_edS(num,r[1]); _tr_edM(den,num,_tr_edD); _tr_edZ(num,num,r[2]); _tr_edA(den,r[2],den);
  _tr_edS(den2,den); _tr_edS(den4,den2); _tr_edM(den6,den4,den2); _tr_edM(t,den6,num); _tr_edM(t,t,den);
  _tr_edpow2523(t,t); _tr_edM(t,t,num); _tr_edM(t,t,den); _tr_edM(t,t,den); _tr_edM(r[0],t,den);
  _tr_edS(chk,r[0]); _tr_edM(chk,chk,den);
  if(_tr_edneq(chk,num)) _tr_edM(r[0],r[0],_tr_edI);
  _tr_edS(chk,r[0]); _tr_edM(chk,chk,den);
  if(_tr_edneq(chk,num)) return -1;
  if(_tr_edpar(r[0])==(p[31]>>7)) _tr_edZ(r[0],_tr_gf0,r[0]);
  _tr_edM(r[3],r[0],r[1]);
  return 0;
}
static int _tr_edsign_open(uint8_t*m,uint64_t*mlen,const uint8_t*sm,uint64_t n,const uint8_t*pk){
  uint8_t t[32],h[64]; _tr_gf p[4],q[4]; uint64_t i;
  *mlen=(uint64_t)-1;
  if(n<64) return -1;
  if(_tr_edunpackneg(q,pk)) return -1;
  for(i=0;i<n;i++) m[i]=sm[i];
  for(i=0;i<32;i++) m[i+32]=pk[i];
  _tr_edhash(h,m,n); _tr_edreduce(h);
  _tr_edscalarmult(p,q,h); _tr_edscalarbase(q,sm+32); _tr_edadd(p,q); _tr_edpack(t,p);
  n-=64;
  if(_tr_cv32(sm,t)){ for(i=0;i<n;i++) m[i]=0; return -1; }
  for(i=0;i<n;i++) m[i]=sm[i+64];
  *mlen=n; return 0;
}
/* Secure OS randomness (BCryptGenRandom on Windows, /dev/urandom on POSIX). */
static int _tr_os_random(uint8_t*buf,int n){
#if defined(_WIN32) && !defined(TAURARO_BARE)
    if(BCryptGenRandom(NULL,buf,(unsigned long)n,BCRYPT_USE_SYSTEM_PREFERRED_RNG)==0) return 1;
    { int i; for(i=0;i<n;i++) buf[i]=(uint8_t)(rand()&0xff); } return 0;
#elif !defined(TAURARO_BARE)
    FILE* f=fopen("/dev/urandom","rb");
    if(f){ size_t r=fread(buf,1,(size_t)n,f); fclose(f); if(r==(size_t)n) return 1; }
    { int i; for(i=0;i<n;i++) buf[i]=(uint8_t)(rand()&0xff); } return 0;
#else
    { int i; for(i=0;i<n;i++) buf[i]=(uint8_t)(rand()&0xff); } return 0;
#endif
}
static int _tr_ed_hexval(char c){ if(c>='0'&&c<='9') return c-'0'; if(c>='a'&&c<='f') return c-'a'+10; if(c>='A'&&c<='F') return c-'A'+10; return 0; }
static void _tr_ed_unhex(const char*h,uint8_t*out,int nbytes){ int i; for(i=0;i<nbytes;i++) out[i]=(uint8_t)((_tr_ed_hexval(h[2*i])<<4)|_tr_ed_hexval(h[2*i+1])); }
static char* _tr_ed_tohex(const uint8_t*b,int n){ char* o=(char*)TAURARO_ALLOC((size_t)(2*n+1)); if(!o) return NULL; int i; for(i=0;i<n;i++){ o[2*i]=_tr_hex_lc[b[i]>>4]; o[2*i+1]=_tr_hex_lc[b[i]&15]; } o[2*n]='\0'; return o; }

/* ── Tauraro-facing hex wrappers ── */
/* 32-byte random seed as 64-char hex (the private key). */
static inline char* _tr_ed25519_gen_seed(void){ uint8_t s[32]; _tr_os_random(s,32); return _tr_ed_tohex(s,32); }
/* Public key (64-char hex) derived from a 64-char seed hex. */
static inline char* _tr_ed25519_pubkey(char* seed_hex){
    uint8_t seed[32],pk[32],sk[64];
    if(!seed_hex||strlen(seed_hex)<64) return _tr_strdup("");
    _tr_ed_unhex(seed_hex,seed,32); _tr_edkeypair(pk,sk,seed);
    return _tr_ed_tohex(pk,32);
}
/* Detached signature (128-char hex) of `msg` (ilen bytes) under `seed_hex`. */
static inline char* _tr_ed25519_sign(char* msg, int ilen, char* seed_hex){
    uint8_t seed[32],pk[32],sk[64];
    if(!seed_hex||strlen(seed_hex)<64) return _tr_strdup("");
    _tr_ed_unhex(seed_hex,seed,32); _tr_edkeypair(pk,sk,seed);
    if(ilen<0) ilen=0;
    uint8_t* sm=(uint8_t*)TAURARO_ALLOC((size_t)ilen+64); if(!sm) return _tr_strdup("");
    uint64_t smlen;
    _tr_edsign(sm,&smlen,(const uint8_t*)(msg?msg:""),(uint64_t)ilen,sk);
    char* out=_tr_ed_tohex(sm,64); TAURARO_FREE(sm); return out;
}
/* Verify a detached signature. Returns 1 if valid, 0 otherwise. */
static inline int _tr_ed25519_verify(char* msg, int ilen, char* sig_hex, char* pk_hex){
    uint8_t sig[64],pk[32];
    if(!sig_hex||strlen(sig_hex)<128||!pk_hex||strlen(pk_hex)<64) return 0;
    _tr_ed_unhex(sig_hex,sig,64); _tr_ed_unhex(pk_hex,pk,32);
    if(ilen<0) ilen=0;
    uint8_t* sm=(uint8_t*)TAURARO_ALLOC((size_t)ilen+64);
    uint8_t* mo=(uint8_t*)TAURARO_ALLOC((size_t)ilen+64);
    if(!sm||!mo){ if(sm)TAURARO_FREE(sm); if(mo)TAURARO_FREE(mo); return 0; }
    memcpy(sm,sig,64); if(ilen>0) memcpy(sm+64,msg,(size_t)ilen);
    uint64_t mlen; int rc=_tr_edsign_open(mo,&mlen,sm,(uint64_t)ilen+64,pk);
    TAURARO_FREE(sm); TAURARO_FREE(mo);
    return rc==0?1:0;
}

/* ── SHA-1 + WebSocket accept key ─────────────────────────────────────────
 * SHA-1 is only used for the RFC 6455 WebSocket handshake (it is NOT a secure
 * hash and must not be used for anything else). _tr_ws_accept(key) computes
 * base64(SHA1(key + WS_GUID)), the Sec-WebSocket-Accept response value. */
typedef struct { uint32_t h[5]; uint64_t len; uint8_t buf[64]; size_t n; } _TrSHA1Ctx;
static inline uint32_t _tr_sha1_rol(uint32_t v, int b){ return (v<<b)|(v>>(32-b)); }
static inline void _tr_sha1_block(_TrSHA1Ctx* c, const uint8_t* p){
    uint32_t w[80];
    for(int i=0;i<16;i++) w[i]=((uint32_t)p[i*4]<<24)|((uint32_t)p[i*4+1]<<16)|((uint32_t)p[i*4+2]<<8)|((uint32_t)p[i*4+3]);
    for(int i=16;i<80;i++) w[i]=_tr_sha1_rol(w[i-3]^w[i-8]^w[i-14]^w[i-16],1);
    uint32_t a=c->h[0],b=c->h[1],cc=c->h[2],d=c->h[3],e=c->h[4];
    for(int i=0;i<80;i++){
        uint32_t f,k;
        if(i<20){f=(b&cc)|((~b)&d);k=0x5A827999;}
        else if(i<40){f=b^cc^d;k=0x6ED9EBA1;}
        else if(i<60){f=(b&cc)|(b&d)|(cc&d);k=0x8F1BBCDC;}
        else {f=b^cc^d;k=0xCA62C1D6;}
        uint32_t t=_tr_sha1_rol(a,5)+f+e+k+w[i];
        e=d;d=cc;cc=_tr_sha1_rol(b,30);b=a;a=t;
    }
    c->h[0]+=a;c->h[1]+=b;c->h[2]+=cc;c->h[3]+=d;c->h[4]+=e;
}
static inline void _tr_sha1_init(_TrSHA1Ctx* c){ c->h[0]=0x67452301;c->h[1]=0xEFCDAB89;c->h[2]=0x98BADCFE;c->h[3]=0x10325476;c->h[4]=0xC3D2E1F0;c->len=0;c->n=0; }
static inline void _tr_sha1_update(_TrSHA1Ctx* c, const uint8_t* d, size_t len){
    c->len += (uint64_t)len*8;
    while(len){ size_t k=64-c->n; if(k>len)k=len; memcpy(c->buf+c->n,d,k); c->n+=k; d+=k; len-=k; if(c->n==64){ _tr_sha1_block(c,c->buf); c->n=0; } }
}
static inline void _tr_sha1_final(_TrSHA1Ctx* c, uint8_t* out){
    uint64_t total = c->len;   /* message bit-length, fixed BEFORE padding bytes bump c->len */
    uint8_t pad=0x80; _tr_sha1_update(c,&pad,1);
    uint8_t z=0; while(c->n!=56) _tr_sha1_update(c,&z,1);
    uint8_t lb[8]; for(int i=0;i<8;i++) lb[i]=(uint8_t)(total>>(56-i*8)); _tr_sha1_update(c,lb,8);
    for(int i=0;i<5;i++){ out[i*4]=(uint8_t)(c->h[i]>>24);out[i*4+1]=(uint8_t)(c->h[i]>>16);out[i*4+2]=(uint8_t)(c->h[i]>>8);out[i*4+3]=(uint8_t)c->h[i]; }
}
static inline char* _tr_ws_accept(char* key){
    static const char* GUID="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    static const char* B64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    _TrSHA1Ctx c; uint8_t dig[20];
    _tr_sha1_init(&c);
    if(key) _tr_sha1_update(&c,(const uint8_t*)key,strlen(key));
    _tr_sha1_update(&c,(const uint8_t*)GUID,strlen(GUID));
    _tr_sha1_final(&c,dig);
    char* out=(char*)TAURARO_ALLOC(29); if(!out) return NULL;  /* 20 bytes -> 28 b64 chars + NUL */
    int o=0;
    for(int i=0;i<18;i+=3){
        uint32_t v=((uint32_t)dig[i]<<16)|((uint32_t)dig[i+1]<<8)|dig[i+2];
        out[o++]=B64[(v>>18)&63];out[o++]=B64[(v>>12)&63];out[o++]=B64[(v>>6)&63];out[o++]=B64[v&63];
    }
    /* final 2 bytes (18,19) -> 3 chars + '=' */
    uint32_t v=((uint32_t)dig[18]<<16)|((uint32_t)dig[19]<<8);
    out[o++]=B64[(v>>18)&63];out[o++]=B64[(v>>12)&63];out[o++]=B64[(v>>6)&63];out[o++]='=';
    out[o]='\0';
    return out;
}

/* ── HMAC-SHA256 ────────────────────────────────────────────────────────── */
static inline char* _tr_hmac_sha256(char* key, int klen, char* msg) {
    uint8_t k[64]={0}; _TrSHA256Ctx ctx;
    if(klen>64){_tr_sha256_init(&ctx);_tr_sha256_update(&ctx,(const uint8_t*)key,(size_t)klen);uint8_t tmp[32];_tr_sha256_final(&ctx,tmp);memcpy(k,tmp,32);}
    else memcpy(k,key,(size_t)klen);
    uint8_t ipad[64],opad[64];
    for(int i=0;i<64;i++){ipad[i]=k[i]^0x36;opad[i]=k[i]^0x5c;}
    uint8_t inner[32];
    _tr_sha256_init(&ctx);_tr_sha256_update(&ctx,ipad,64);
    if(msg)_tr_sha256_update(&ctx,(const uint8_t*)msg,strlen(msg));
    _tr_sha256_final(&ctx,inner);
    _tr_sha256_init(&ctx);_tr_sha256_update(&ctx,opad,64);_tr_sha256_update(&ctx,inner,32);
    uint8_t dig[32]; _tr_sha256_final(&ctx,dig);
    char* out=(char*)TAURARO_ALLOC(65); if(!out) return NULL;
    for(int i=0;i<32;i++){out[i*2]=_tr_hex_lc[dig[i]>>4];out[i*2+1]=_tr_hex_lc[dig[i]&15];}
    out[64]='\0'; return out;
}

/* ── UUID v4 ────────────────────────────────────────────────────────────── */
static inline char* _tr_uuid_v4(void) {
    uint8_t b[16];
#if (!defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)) && !defined(TAURARO_BARE)
    FILE* f=fopen("/dev/urandom","rb");
    if(f){fread(b,1,16,f);fclose(f);}
    else{for(int i=0;i<16;i++)b[i]=(uint8_t)(rand()&0xff);}
#else
    /* bare-metal / Windows: no /dev/urandom — fall back to the PRNG */
    for(int i=0;i<16;i++)b[i]=(uint8_t)(rand()&0xff);
#endif
    b[6]=(b[6]&0x0f)|0x40; b[8]=(b[8]&0x3f)|0x80;
    char* out=(char*)TAURARO_ALLOC(37); if(!out) return NULL;
    snprintf(out,37,"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15]);
    return out;
}

/* ── MD5 (compact, for legacy use) ─────────────────────────────────────── */
static inline char* _tr_md5_hex(char* s) {
    /* Minimal MD5; message expanded inline. */
    static const uint32_t T[64]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
        0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
        0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
        0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
        0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
        0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
    };
    static const int S[64]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
                             5, 9,14,20,5, 9,14,20,5, 9,14,20,5, 9,14,20,
                             4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
                             6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
    size_t ilen = s ? strlen(s) : 0;
    size_t padlen = ((ilen+8)/64+1)*64;
    uint8_t* msg = (uint8_t*)TAURARO_CALLOC(1,padlen);
    if(!msg) return _tr_strdup("00000000000000000000000000000000");
    if(s) memcpy(msg,s,ilen);
    msg[ilen]=0x80;
    uint64_t bits=(uint64_t)ilen*8;
    for(int i=0;i<8;i++) msg[padlen-8+i]=(uint8_t)(bits>>(uint64_t)(i*8));
    uint32_t a=0x67452301,b=0xefcdab89,cc=0x98badcfe,d=0x10325476;
    for(size_t off=0;off<padlen;off+=64){
        uint32_t M[16],A=a,B=b,C=cc,D=d;
        for(int i=0;i<16;i++) M[i]=((uint32_t)msg[off+i*4])|((uint32_t)msg[off+i*4+1]<<8)|((uint32_t)msg[off+i*4+2]<<16)|((uint32_t)msg[off+i*4+3]<<24);
        for(int i=0;i<64;i++){
            uint32_t F,g2;
            if(i<16){F=(_TR_CH(B,C,D));g2=(uint32_t)i;}
            else if(i<32){F=(D^(B&(C^D)));g2=(uint32_t)(5*i+1)%16;}
            else if(i<48){F=(B^C^D);g2=(uint32_t)(3*i+5)%16;}
            else{F=(C^(B|(~D)));g2=(uint32_t)(7*i)%16;}
            F=F+A+T[i]+M[g2];
            A=D;D=C;C=B;B=B+((F<<S[i])|(F>>(32-S[i])));
        }
        a+=A;b+=B;cc+=C;d+=D;
    }
    TAURARO_FREE(msg);
    char* out=(char*)TAURARO_ALLOC(33); if(!out) return _tr_strdup("00000000000000000000000000000000");
    uint32_t r[4]={a,b,cc,d};
    for(int i=0;i<4;i++) for(int j=0;j<4;j++){
        uint8_t byte=(uint8_t)(r[i]>>(j*8));
        out[i*8+j*2]=_tr_hex_lc[byte>>4]; out[i*8+j*2+1]=_tr_hex_lc[byte&15];
    }
    out[32]='\0'; return out;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TLS/HTTPS — OpenSSL (opt-in: -DTAURARO_TLS_OPENSSL -lssl -lcrypto).
 * ═══════════════════════════════════════════════════════════════════════════ */
#ifdef TAURARO_TLS_OPENSSL
#  include <openssl/ssl.h>
#  include <openssl/err.h>
typedef struct { SSL_CTX* ctx; SSL* ssl; int fd; } _TrTLSConn;
#  ifdef _WIN32
#    define _TR_SOCK_CLOSE(fd) closesocket(fd)
#  else
#    define _TR_SOCK_CLOSE(fd) close(fd)
#  endif
_TR_XLINK char* _tr_tls_connect(char* host, int port) {
    static _Atomic int _tr_ssl_once = 0;
    if (atomic_fetch_add(&_tr_ssl_once,1)==0){SSL_library_init();SSL_load_error_strings();OpenSSL_add_all_algorithms();}
    struct addrinfo hints={0},*res=NULL;
    hints.ai_family=AF_UNSPEC;hints.ai_socktype=SOCK_STREAM;
    char pbuf[16]; snprintf(pbuf,sizeof(pbuf),"%d",port);
    if(getaddrinfo(host,pbuf,&hints,&res)!=0) return NULL;
    int fd=(int)socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(fd<0){freeaddrinfo(res);return NULL;}
    if(connect(fd,res->ai_addr,(int)res->ai_addrlen)!=0){freeaddrinfo(res);_TR_SOCK_CLOSE(fd);return NULL;}
    freeaddrinfo(res);
    SSL_CTX* ctx=SSL_CTX_new(TLS_client_method());
    if(!ctx){_TR_SOCK_CLOSE(fd);return NULL;}
    SSL* ssl=SSL_new(ctx); SSL_set_fd(ssl,fd); SSL_set_tlsext_host_name(ssl,host);
    if(SSL_connect(ssl)!=1){SSL_free(ssl);SSL_CTX_free(ctx);_TR_SOCK_CLOSE(fd);return NULL;}
    _TrTLSConn* c=(_TrTLSConn*)TAURARO_ALLOC(sizeof(_TrTLSConn));
    if(!c){SSL_free(ssl);SSL_CTX_free(ctx);_TR_SOCK_CLOSE(fd);return NULL;}
    c->ctx=ctx;c->ssl=ssl;c->fd=fd; return (char*)c;
}
_TR_XLINK int _tr_tls_send(char* h, char* d) { if(!h||!d) return -1; return SSL_write(((_TrTLSConn*)h)->ssl,d,(int)strlen(d)); }
_TR_XLINK char* _tr_tls_recv(char* h, int cap) {
    if(!h||cap<=0) return _tr_strdup("");
    char* buf=(char*)TAURARO_ALLOC((size_t)cap+1); if(!buf) return _tr_strdup("");
    int n=SSL_read(((_TrTLSConn*)h)->ssl,buf,cap);
    if(n<=0){TAURARO_FREE(buf);return _tr_strdup("");}
    buf[n]='\0'; return buf;
}
_TR_XLINK void _tr_tls_close(char* h) {
    if(!h) return; _TrTLSConn* c=(_TrTLSConn*)h;
    SSL_shutdown(c->ssl);SSL_free(c->ssl);SSL_CTX_free(c->ctx);_TR_SOCK_CLOSE(c->fd);TAURARO_FREE(c);
}
/* ── Server side: one SSL_CTX (cert+key), one _TrTLSConn per accepted fd ──
 * SSL_accept/read/write are blocking, so server TLS is for the thread-per-
 * connection model (listen_tls), where blocking a worker thread is fine. */
_TR_XLINK char* _tr_tls_server_new(char* cert, char* key) {
    static _Atomic int _tr_ssl_once_s = 0;
    if (atomic_fetch_add(&_tr_ssl_once_s,1)==0){SSL_library_init();SSL_load_error_strings();OpenSSL_add_all_algorithms();}
    SSL_CTX* ctx=SSL_CTX_new(TLS_server_method());
    if(!ctx) return NULL;
    if(SSL_CTX_use_certificate_chain_file(ctx,cert)<=0){SSL_CTX_free(ctx);return NULL;}
    if(SSL_CTX_use_PrivateKey_file(ctx,key,SSL_FILETYPE_PEM)<=0){SSL_CTX_free(ctx);return NULL;}
    return (char*)ctx;
}
_TR_XLINK char* _tr_tls_accept(char* ctxh, int fd) {
    if(!ctxh) return NULL;
    SSL* ssl=SSL_new((SSL_CTX*)ctxh); if(!ssl) return NULL;
    SSL_set_fd(ssl,fd);
    if(SSL_accept(ssl)!=1){SSL_free(ssl);return NULL;}
    _TrTLSConn* c=(_TrTLSConn*)TAURARO_ALLOC(sizeof(_TrTLSConn));
    if(!c){SSL_free(ssl);return NULL;}
    c->ctx=NULL; c->ssl=ssl; c->fd=fd;   /* ctx is shared/server-owned, not freed per-conn */
    return (char*)c;
}
_TR_XLINK void _tr_tls_server_free(char* ctxh) { if(ctxh) SSL_CTX_free((SSL_CTX*)ctxh); }
#else
_TR_XLINK char* _tr_tls_connect(char* h, int p) { (void)h;(void)p; return NULL; }
_TR_XLINK int _tr_tls_send(char* h, char* d)  { (void)h;(void)d; return -1; }
_TR_XLINK char* _tr_tls_recv(char* h, int c)    { (void)h;(void)c; return _tr_strdup(""); }
_TR_XLINK void _tr_tls_close(char* h)          { (void)h; }
_TR_XLINK char* _tr_tls_server_new(char* c, char* k) { (void)c;(void)k; return NULL; }
_TR_XLINK char* _tr_tls_accept(char* x, int fd) { (void)x;(void)fd; return NULL; }
_TR_XLINK void _tr_tls_server_free(char* x) { (void)x; }
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * COMPRESS — zlib (opt-in: -DTAURARO_COMPRESS_ZLIB -lz).
 * ═══════════════════════════════════════════════════════════════════════════ */
#ifdef TAURARO_COMPRESS_ZLIB
#  include <zlib.h>
static inline char* _tr_zlib_compress(char* input, int ilen, int* out_len) {
    if(!input||ilen<=0){if(out_len)*out_len=0;return NULL;}
    uLong bound=compressBound((uLong)ilen);
    char* out=(char*)TAURARO_ALLOC(bound);if(!out){if(out_len)*out_len=0;return NULL;}
    uLong dlen=bound;
    if(compress2((Bytef*)out,&dlen,(const Bytef*)input,(uLong)ilen,Z_BEST_COMPRESSION)!=Z_OK){TAURARO_FREE(out);if(out_len)*out_len=0;return NULL;}
    if(out_len)*out_len=(int)dlen; return out;
}
static inline char* _tr_zlib_decompress(char* input, int ilen, int max_out) {
    if(!input||ilen<=0) return _tr_strdup("");
    char* out=(char*)TAURARO_ALLOC((size_t)max_out+1);if(!out) return _tr_strdup("");
    uLong dlen=(uLong)max_out;
    if(uncompress((Bytef*)out,&dlen,(const Bytef*)input,(uLong)ilen)!=Z_OK){TAURARO_FREE(out);return _tr_strdup("");}
    out[dlen]='\0'; return out;
}
static inline char* _tr_deflate(char* input, int ilen, int* out_len) {
    if(!input||ilen<=0){if(out_len)*out_len=0;return NULL;}
    z_stream s={0}; deflateInit2(&s,Z_BEST_COMPRESSION,Z_DEFLATED,-15,8,Z_DEFAULT_STRATEGY);
    uLong bound=deflateBound(&s,(uLong)ilen);
    char* out=(char*)TAURARO_ALLOC(bound);if(!out){deflateEnd(&s);if(out_len)*out_len=0;return NULL;}
    s.next_in=(Bytef*)input;s.avail_in=(uInt)ilen;s.next_out=(Bytef*)out;s.avail_out=(uInt)bound;
    deflate(&s,Z_FINISH);if(out_len)*out_len=(int)s.total_out;deflateEnd(&s);return out;
}
static inline char* _tr_inflate(char* input, int ilen, int max_out) {
    if(!input||ilen<=0) return _tr_strdup("");
    z_stream s={0};inflateInit2(&s,-15);
    char* out=(char*)TAURARO_ALLOC((size_t)max_out+1);if(!out){inflateEnd(&s);return _tr_strdup("");}
    s.next_in=(Bytef*)input;s.avail_in=(uInt)ilen;s.next_out=(Bytef*)out;s.avail_out=(uInt)max_out;
    inflate(&s,Z_FINISH);int n=(int)s.total_out;inflateEnd(&s);out[n]='\0';return out;
}
#else
static inline char* _tr_zlib_compress(char* i, int il, int* n) { (void)i;(void)il;if(n)*n=0;return NULL; }
static inline char* _tr_zlib_decompress(char* i, int il, int m) { (void)i;(void)il;(void)m;return _tr_strdup(""); }
static inline char* _tr_deflate(char* i, int il, int* n) { (void)i;(void)il;if(n)*n=0;return NULL; }
static inline char* _tr_inflate(char* i, int il, int m) { (void)i;(void)il;(void)m;return _tr_strdup(""); }
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * UNICODE / UTF-8 — pure C, no external dependencies.
 * ═══════════════════════════════════════════════════════════════════════════ */
static inline uint32_t _tr_utf8_next(const char** pp) {
    const uint8_t* p=(const uint8_t*)*pp; uint32_t cp;
    if(!*p){return 0;}
    if(p[0]<0x80){cp=p[0];*pp=(const char*)(p+1);return cp;}
    if((p[0]&0xe0)==0xc0&&(p[1]&0xc0)==0x80){cp=((p[0]&0x1f)<<6)|(p[1]&0x3f);*pp=(const char*)(p+2);return cp;}
    if((p[0]&0xf0)==0xe0&&(p[1]&0xc0)==0x80&&(p[2]&0xc0)==0x80){cp=((p[0]&0x0f)<<12)|((p[1]&0x3f)<<6)|(p[2]&0x3f);*pp=(const char*)(p+3);return cp;}
    if((p[0]&0xf8)==0xf0&&(p[1]&0xc0)==0x80&&(p[2]&0xc0)==0x80&&(p[3]&0xc0)==0x80){cp=((p[0]&0x07)<<18)|((p[1]&0x3f)<<12)|((p[2]&0x3f)<<6)|(p[3]&0x3f);*pp=(const char*)(p+4);return cp;}
    *pp=(const char*)(p+1);return 0xFFFD;
}
static inline int _tr_utf8_encode_cp(uint32_t cp, char* buf) {
    if(cp<0x80){buf[0]=(char)cp;return 1;}
    if(cp<0x800){buf[0]=(char)(0xc0|(cp>>6));buf[1]=(char)(0x80|(cp&0x3f));return 2;}
    if(cp<0x10000){buf[0]=(char)(0xe0|(cp>>12));buf[1]=(char)(0x80|((cp>>6)&0x3f));buf[2]=(char)(0x80|(cp&0x3f));return 3;}
    buf[0]=(char)(0xf0|(cp>>18));buf[1]=(char)(0x80|((cp>>12)&0x3f));buf[2]=(char)(0x80|((cp>>6)&0x3f));buf[3]=(char)(0x80|(cp&0x3f));return 4;
}
static inline int _tr_utf8_len(char* s) {
    if(!s) return 0; int n=0; const char* p=s; while(*p){_tr_utf8_next(&p);n++;} return n;
}
static inline bool _tr_utf8_valid(char* s) {
    if(!s) return false;
    const uint8_t* p=(const uint8_t*)s;
    while(*p){
        int seq;
        if(p[0]<0x80){p++;continue;}
        else if((p[0]&0xe0)==0xc0)seq=2;
        else if((p[0]&0xf0)==0xe0)seq=3;
        else if((p[0]&0xf8)==0xf0)seq=4;
        else return false;
        for(int i=1;i<seq;i++) if((p[i]&0xc0)!=0x80) return false;
        p+=seq;
    }
    return true;
}
static inline int _tr_utf8_char_at(char* s, int idx) {
    if(!s) return -1; const char* p=s; int n=0;
    while(*p){uint32_t cp=_tr_utf8_next(&p);if(n==idx)return(int)cp;n++;}
    return -1;
}
static inline char* _tr_utf8_slice(char* s, int start, int end_) {
    if(!s||start>=end_) return _tr_strdup("");
    const char* p=s; int n=0;
    while(*p&&n<start){_tr_utf8_next(&p);n++;}
    const char* begin=p;
    while(*p&&n<end_){_tr_utf8_next(&p);n++;}
    size_t len=(size_t)(p-begin);
    char* out=(char*)TAURARO_ALLOC(len+1);if(!out) return _tr_strdup("");
    memcpy(out,begin,len);out[len]='\0';return out;
}
static inline bool _tr_unicode_is_letter(int cp) {
    if((cp>=65&&cp<=90)||(cp>=97&&cp<=122)) return true;
    if(cp>=0xC0&&cp<=0x2AF) return true;
    if(cp>=0x4E00&&cp<=0x9FFF) return true;
    if(cp>=0x0400&&cp<=0x04FF) return true;
    if(cp>=0x0600&&cp<=0x06FF) return true;
    if(cp>=0xAC00&&cp<=0xD7AF) return true;
    return false;
}
static inline bool _tr_unicode_is_digit(int cp) {
    return (cp>=48&&cp<=57)||(cp>=0x0660&&cp<=0x0669)||(cp>=0x06F0&&cp<=0x06F9);
}
static inline int _tr_unicode_to_upper(int cp) {
    if(cp>=97&&cp<=122) return cp-32;
    if(cp>=0xE0&&cp<=0xFE&&cp!=0xF7) return cp-32;
    return cp;
}
static inline int _tr_unicode_to_lower(int cp) {
    if(cp>=65&&cp<=90) return cp+32;
    if(cp>=0xC0&&cp<=0xDE&&cp!=0xD7) return cp+32;
    return cp;
}
static inline char* _tr_utf8_to_upper(char* s) {
    if(!s) return _tr_strdup("");
    size_t cap=strlen(s)*4+4; char* out=(char*)TAURARO_ALLOC(cap);if(!out) return _tr_strdup("");
    const char* p=s; char* q=out; char tmp[5];
    while(*p){uint32_t cp=_tr_utf8_next(&p);int n=_tr_utf8_encode_cp((uint32_t)_tr_unicode_to_upper((int)cp),tmp);memcpy(q,tmp,(size_t)n);q+=n;}
    *q='\0'; return out;
}
static inline char* _tr_utf8_to_lower(char* s) {
    if(!s) return _tr_strdup("");
    size_t cap=strlen(s)*4+4; char* out=(char*)TAURARO_ALLOC(cap);if(!out) return _tr_strdup("");
    const char* p=s; char* q=out; char tmp[5];
    while(*p){uint32_t cp=_tr_utf8_next(&p);int n=_tr_utf8_encode_cp((uint32_t)_tr_unicode_to_lower((int)cp),tmp);memcpy(q,tmp,(size_t)n);q+=n;}
    *q='\0'; return out;
}
/* Return the codepoint category string: "L"=letter, "N"=digit, "Z"=space, "C"=other */
static inline char* _tr_unicode_category(int cp) {
    if(_tr_unicode_is_letter(cp)) return _tr_strdup("L");
    if(_tr_unicode_is_digit(cp))  return _tr_strdup("N");
    if(cp==32||cp==9||cp==10||cp==13) return _tr_strdup("Z");
    return _tr_strdup("C");
}

/* ── std.gpu helpers ─────────────────────────────────────────────────────── */
static inline bool _tr_gpu_is_openmp(void) {
#ifdef _OPENMP
    return true;
#else
    return false;
#endif
}

static inline int64_t _tr_gpu_thread_id(void) {
#ifdef _OPENMP
    return (int64_t)omp_get_thread_num();
#else
    return 0;
#endif
}

static inline int64_t _tr_gpu_num_threads(void) {
#ifdef _OPENMP
    return (int64_t)omp_get_num_threads();
#else
    return 1;
#endif
}

static inline void _tr_gpu_openmp_parallel_i64(int64_t n, void* fn_ptr) {
    typedef void (*_tr_gpu_fn)(int64_t);
    _tr_gpu_fn fn = (_tr_gpu_fn)fn_ptr;
#ifdef _OPENMP
    #pragma omp parallel for schedule(static)
    for (int64_t _i = 0; _i < n; _i++) { fn(_i); }
#else
    for (int64_t _i = 0; _i < n; _i++) { fn(_i); }
#endif
}

/* ── v0.0.5: Map.update / Set[T] / List helpers ──────────────────────────────
 * All placed here so TrMap, List_str, List_i64 etc. are defined above.   */

/* Map.update / Map.clear */
static void _tr_dict_update(TrMap* dst, TrMap* src) {
    if (!dst || !src) return;
    List_TrStr* ks = _tr_dict_keys(src);
    if (!ks) return;
    for (int64_t i = 0; i < (int64_t)ks->len; i++) {
        char* k = ks->data[i].data; _tr_dict_set(dst, k, _tr_dict_get(src, k));
    }
    List_TrStr_free(ks);
}
static void _tr_idict_update(TrMap* dst, TrMap* src) { _tr_dict_update(dst, src); }
static void _tr_dict_clear(TrMap* m) { Dict_clear_entries(m); }
static void _tr_idict_clear(TrIDict* m) {
    if (!m) return;
    for (size_t i=0;i<m->cap;i++) {
        _TrIDictNode* n=m->buckets[i];
        while(n){ _TrIDictNode* nx=n->next; _tr_free(n); n=nx; }
        m->buckets[i]=NULL;
    }
    m->len=0;
}
/* Free a Map[int,V]/Dict[int,V] (TrIDict) entirely - nodes, buckets array and
   the struct itself. Mirrors Dict_free() but for the int-keyed node layout
   (whose key is a long long, not a heap string). */
static void _tr_idict_free(TrIDict* d) {
    if (!d) return;
    for (size_t i=0;i<d->cap;i++) {
        _TrIDictNode* n=d->buckets[i];
        while(n){ _TrIDictNode* nx=n->next; _tr_free(n); n=nx; }
    }
    _tr_free(d->buckets); _tr_free(d);
}
/* Like _tr_idict_free(), but for Dict[int,str]/Map[int,str] whose values are
   _tr_str_box(TrStr)-allocated boxes (#54): unbox+release the TrStr, then
   free the box itself, before freeing the node/buckets/struct. */
static void _tr_idict_free_strval(TrIDict* d) {
    if (!d) return;
    for (size_t i=0;i<d->cap;i++) {
        _TrIDictNode* n=d->buckets[i];
        while(n){
            _TrIDictNode* nx=n->next;
            if(n->value) { _tr_str_release(*(TrStr*)n->value); _tr_free(n->value); }
            _tr_free(n);
            n=nx;
        }
    }
    _tr_free(d->buckets); _tr_free(d);
}
/* Dict[int, HeapClass]: values are owned refcounted instances stored directly as
   void* (the dict co-owns each via insert-retain) — release each before teardown. */
static void _tr_idict_free_objval(TrIDict* d, void(*drop)(void*)) {
    if (!d) return;
    for (size_t i=0;i<d->cap;i++) {
        _TrIDictNode* n=d->buckets[i];
        while(n){ _TrIDictNode* nx=n->next; _tr_obj_release(n->value, drop); _tr_free(n); n=nx; }
    }
    _tr_free(d->buckets); _tr_free(d);
}

/* Set[HeapClass] — hash set of heap instances keyed by POINTER IDENTITY. The set
   OWNS each element (add retains); _tr_pset_free_obj releases them on teardown. */
typedef struct _TrPSetNode { void* elem; struct _TrPSetNode* next; } _TrPSetNode;
typedef struct { _TrPSetNode** buckets; size_t cap; size_t len; } _TrPtrSet;
static inline _TrPtrSet* _tr_pset_new(int64_t cap) {
    _TrPtrSet* s=(_TrPtrSet*)_tr_checked_alloc(sizeof(_TrPtrSet));
    s->cap = cap>0?(size_t)cap:16; s->len=0;
    s->buckets=(_TrPSetNode**)_tr_checked_alloc(sizeof(_TrPSetNode*)*s->cap);
    for(size_t i=0;i<s->cap;i++) s->buckets[i]=NULL;
    return s;
}
static inline size_t _tr_pset_hash(_TrPtrSet* s, void* e) { return ((size_t)(uintptr_t)e >> 4) % s->cap; }
static inline int64_t _tr_pset_contains(_TrPtrSet* s, void* e) {
    if(!s) return 0; _TrPSetNode* n=s->buckets[_tr_pset_hash(s,e)];
    while(n){ if(n->elem==e) return 1; n=n->next; } return 0;
}
static inline void _tr_pset_add(_TrPtrSet* s, void* e) {
    if(!s||_tr_pset_contains(s,e)) return;
    size_t h=_tr_pset_hash(s,e);
    _TrPSetNode* n=(_TrPSetNode*)_tr_checked_alloc(sizeof(_TrPSetNode));
    n->elem=e; n->next=s->buckets[h]; s->buckets[h]=n; s->len++;
}
static inline void _tr_pset_remove(_TrPtrSet* s, void* e) {
    if(!s) return; size_t h=_tr_pset_hash(s,e);
    _TrPSetNode* n=s->buckets[h]; _TrPSetNode* p=NULL;
    while(n){ if(n->elem==e){ if(p)p->next=n->next; else s->buckets[h]=n->next; _tr_free(n); if(s->len)s->len--; return; } p=n; n=n->next; }
}
static inline int64_t _tr_pset_len(_TrPtrSet* s) { return s?(int64_t)s->len:0; }
static inline void _tr_pset_clear(_TrPtrSet* s) {
    if(!s) return;
    for(size_t i=0;i<s->cap;i++){ _TrPSetNode* n=s->buckets[i]; while(n){ _TrPSetNode* nx=n->next; _tr_free(n); n=nx; } s->buckets[i]=NULL; }
    s->len=0;
}
static inline void _tr_pset_free(_TrPtrSet* s) {
    if(!s) return;
    for(size_t i=0;i<s->cap;i++){ _TrPSetNode* n=s->buckets[i]; while(n){ _TrPSetNode* nx=n->next; _tr_free(n); n=nx; } }
    _tr_free(s->buckets); _tr_free(s);
}
static inline void _tr_pset_free_obj(_TrPtrSet* s, void(*drop)(void*)) {
    if(!s) return;
    for(size_t i=0;i<s->cap;i++){ _TrPSetNode* n=s->buckets[i]; while(n){ _TrPSetNode* nx=n->next; _tr_obj_release(n->elem, drop); _tr_free(n); n=nx; } }
    _tr_free(s->buckets); _tr_free(s);
}
static inline List_ptr* _tr_pset_to_list(_TrPtrSet* s) {
    List_ptr* l=List_ptr_new();
    if(s){ for(size_t i=0;i<s->cap;i++){ _TrPSetNode* n=s->buckets[i]; while(n){ List_ptr_append(l,n->elem); n=n->next; } } }
    return l;
}

/* Set[T] — hash set backed by TrMap */
typedef TrMap _TrSet;
static _TrSet* _tr_set_new(int64_t cap) { return _tr_dict_new(cap > 0 ? cap : 16); }
static void    _tr_set_add(_TrSet* s, char* e)      { _tr_dict_set(s, e, (void*)1); }
static int64_t _tr_set_contains(_TrSet* s, char* e) { return (int64_t)_tr_dict_contains(s, e); }
static void    _tr_set_remove(_TrSet* s, char* e)   { _tr_dict_remove(s, e); }
static int64_t _tr_set_len(_TrSet* s)               { return _tr_dict_len(s); }
static void    _tr_set_clear(_TrSet* s)             { _tr_dict_clear(s); }
static List_TrStr* _tr_set_to_list(_TrSet* s)       { return _tr_dict_keys(s); }
static _TrSet* _tr_set_union(_TrSet* a, _TrSet* b) {
    _TrSet* r=_tr_set_new(16);
    List_TrStr* ka=_tr_dict_keys(a); if(ka){ for(int64_t i=0;i<(int64_t)ka->len;i++) _tr_set_add(r,_tr_strz(ka->data[i])); List_TrStr_free(ka); }
    List_TrStr* kb=_tr_dict_keys(b); if(kb){ for(int64_t i=0;i<(int64_t)kb->len;i++) _tr_set_add(r,_tr_strz(kb->data[i])); List_TrStr_free(kb); }
    return r;
}
static _TrSet* _tr_set_intersection(_TrSet* a, _TrSet* b) {
    _TrSet* r=_tr_set_new(16);
    List_TrStr* ka=_tr_dict_keys(a);
    if(ka){ for(int64_t i=0;i<(int64_t)ka->len;i++) if(_tr_set_contains(b,_tr_strz(ka->data[i]))) _tr_set_add(r,_tr_strz(ka->data[i])); List_TrStr_free(ka); }
    return r;
}
static _TrSet* _tr_set_difference(_TrSet* a, _TrSet* b) {
    _TrSet* r=_tr_set_new(16);
    List_TrStr* ka=_tr_dict_keys(a);
    if(ka){ for(int64_t i=0;i<(int64_t)ka->len;i++) if(!_tr_set_contains(b,_tr_strz(ka->data[i]))) _tr_set_add(r,_tr_strz(ka->data[i])); List_TrStr_free(ka); }
    return r;
}
static int64_t _tr_set_is_subset(_TrSet* a, _TrSet* b) {
    List_TrStr* ka=_tr_dict_keys(a);
    if(!ka) return 1LL;
    int64_t result = 1LL;
    for(int64_t i=0;i<(int64_t)ka->len;i++) if(!_tr_set_contains(b,_tr_strz(ka->data[i]))) { result = 0LL; break; }
    List_TrStr_free(ka);
    return result;
}

/* ── Int-keyed Set: Set[int] backed by TrIDict ─────────────────────────────── */
typedef TrIDict _TrISet;
static _TrISet* _tr_iset_new(int64_t cap) { return _tr_idict_new(cap > 0 ? cap : 16); }
static void     _tr_iset_add(_TrISet* s, int64_t e)      { _tr_idict_set(s, e, (void*)1); }
static int64_t  _tr_iset_contains(_TrISet* s, int64_t e) { return (int64_t)_tr_idict_contains(s, e); }
static void     _tr_iset_remove(_TrISet* s, int64_t e)   { _tr_idict_remove(s, e); }
static int64_t  _tr_iset_len(_TrISet* s)                 { return _tr_idict_len(s); }
static void     _tr_iset_clear(_TrISet* s)               { if(s){ for(size_t i=0;i<s->cap;i++){ _TrIDictNode* n=s->buckets[i]; while(n){ _TrIDictNode* nx=n->next; free(n); n=nx; } s->buckets[i]=NULL; } s->len=0; } }
static List_i64* _tr_iset_to_list(_TrISet* s)            { return _tr_idict_keys(s); }
static _TrISet* _tr_iset_union(_TrISet* a, _TrISet* b) {
    _TrISet* r=_tr_iset_new(16);
    List_i64* ka=_tr_idict_keys(a); if(ka) for(int64_t i=0;i<(int64_t)ka->len;i++) _tr_iset_add(r,ka->data[i]);
    List_i64* kb=_tr_idict_keys(b); if(kb) for(int64_t i=0;i<(int64_t)kb->len;i++) _tr_iset_add(r,kb->data[i]);
    return r;
}
static _TrISet* _tr_iset_intersection(_TrISet* a, _TrISet* b) {
    _TrISet* r=_tr_iset_new(16);
    List_i64* ka=_tr_idict_keys(a);
    if(ka) for(int64_t i=0;i<(int64_t)ka->len;i++) if(_tr_iset_contains(b,ka->data[i])) _tr_iset_add(r,ka->data[i]);
    return r;
}
static _TrISet* _tr_iset_difference(_TrISet* a, _TrISet* b) {
    _TrISet* r=_tr_iset_new(16);
    List_i64* ka=_tr_idict_keys(a);
    if(ka) for(int64_t i=0;i<(int64_t)ka->len;i++) if(!_tr_iset_contains(b,ka->data[i])) _tr_iset_add(r,ka->data[i]);
    return r;
}
static int64_t _tr_iset_is_subset(_TrISet* a, _TrISet* b) {
    List_i64* ka=_tr_idict_keys(a);
    if(!ka) return 1LL;
    for(int64_t i=0;i<(int64_t)ka->len;i++) if(!_tr_iset_contains(b,ka->data[i])) return 0LL;
    return 1LL;
}

/* ── Generic collection-to-string for print()/f-strings (List/Set/Dict repr) ──
 * All List_T / Set_T headers share the same {data; size_t len; size_t cap}
 * layout, so a generic header view + element-size + per-element formatter
 * is enough to render "[1, 2, 3]" / "{'a': 1}" style output for any T. */
typedef struct { void* data; size_t len; size_t capacity; } _TrListHdr;
typedef char* (*_TrElemFmt)(const void* elem);

static char* _tr_sb_init(size_t* cap) { *cap = 64; char* b = (char*)_tr_checked_alloc(*cap); b[0] = '\0'; return b; }
static char* _tr_sb_append(char* buf, size_t* len, size_t* cap, const char* s) {
    size_t sl = strlen(s);
    if (*len + sl + 1 > *cap) {
        while (*len + sl + 1 > *cap) *cap *= 2;
        char* nb = (char*)_tr_checked_alloc(*cap);
        memcpy(nb, buf, *len + 1);
        buf = nb;
    }
    memcpy(buf + *len, s, sl);
    *len += sl;
    buf[*len] = '\0';
    return buf;
}
static char* _tr_collection_to_str(const void* data, size_t len, size_t elem_size, _TrElemFmt fmt, const char* open, const char* close, const char* sep) {
    size_t cap, blen = 0;
    char* buf = _tr_sb_init(&cap);
    buf = _tr_sb_append(buf, &blen, &cap, open);
    for (size_t i = 0; i < len; i++) {
        if (i > 0) buf = _tr_sb_append(buf, &blen, &cap, sep);
        const char* ep = (const char*)data + i * elem_size;
        buf = _tr_sb_append(buf, &blen, &cap, fmt(ep));
    }
    buf = _tr_sb_append(buf, &blen, &cap, close);
    return buf;
}
/* Build "{k1: v1, k2: v2}" from parallel key/value List headers. */
static char* _tr_dict_to_str(const void* kdata, const void* vdata, size_t len, size_t vsize, _TrElemFmt kfmt, _TrElemFmt vfmt) {
    size_t cap, blen = 0;
    char* buf = _tr_sb_init(&cap);
    buf = _tr_sb_append(buf, &blen, &cap, "{");
    for (size_t i = 0; i < len; i++) {
        if (i > 0) buf = _tr_sb_append(buf, &blen, &cap, ", ");
        const char* kp = (const char*)kdata + i * sizeof(TrStr);
        const char* vp = (const char*)vdata + i * vsize;
        buf = _tr_sb_append(buf, &blen, &cap, kfmt(kp));
        buf = _tr_sb_append(buf, &blen, &cap, ": ");
        buf = _tr_sb_append(buf, &blen, &cap, vfmt(vp));
    }
    buf = _tr_sb_append(buf, &blen, &cap, "}");
    return buf;
}
/* Like _tr_dict_to_str but with an explicit KEY stride (int-keyed dicts store i64 keys,
   not sizeof(TrStr) — #27b: the fixed-stride version read garbage past the first key). */
static char* _tr_dict_to_str_ks(const void* kdata, const void* vdata, size_t len, size_t ksize, size_t vsize, _TrElemFmt kfmt, _TrElemFmt vfmt) {
    size_t cap, blen = 0;
    char* buf = _tr_sb_init(&cap);
    buf = _tr_sb_append(buf, &blen, &cap, "{");
    for (size_t i = 0; i < len; i++) {
        if (i > 0) buf = _tr_sb_append(buf, &blen, &cap, ", ");
        const char* kp = (const char*)kdata + i * ksize;
        const char* vp = (const char*)vdata + i * vsize;
        buf = _tr_sb_append(buf, &blen, &cap, kfmt(kp));
        buf = _tr_sb_append(buf, &blen, &cap, ": ");
        buf = _tr_sb_append(buf, &blen, &cap, vfmt(vp));
    }
    buf = _tr_sb_append(buf, &blen, &cap, "}");
    return buf;
}
/* Element formatters for primitive List/Set element types. */
static char* _tr_fmt_i64(const void* p)  { return _tr_int_to_str(*(const long long*)p); }
static char* _tr_fmt_i32(const void* p)  { return _tr_int_to_str((long long)*(const int32_t*)p); }
static char* _tr_fmt_i16(const void* p)  { return _tr_int_to_str((long long)*(const int16_t*)p); }
static char* _tr_fmt_i8(const void* p)   { return _tr_int_to_str((long long)*(const int8_t*)p); }
static char* _tr_fmt_u8(const void* p)   { return _tr_int_to_str((long long)*(const uint8_t*)p); }
static char* _tr_fmt_u16(const void* p)  { return _tr_int_to_str((long long)*(const uint16_t*)p); }
static char* _tr_fmt_u32(const void* p)  { return _tr_int_to_str((long long)*(const uint32_t*)p); }
static char* _tr_fmt_u64(const void* p)  { return _tr_int_to_str((long long)*(const uint64_t*)p); }
static char* _tr_fmt_f64(const void* p)  { return _tr_float_to_str(*(const double*)p); }
static char* _tr_fmt_bool(const void* p) { return _tr_bool_to_str(*(const _Bool*)p); }
static char* _tr_fmt_char(const void* p) {
    char c = *(const char*)p;
    char* b = (char*)_tr_checked_alloc(4);
    b[0] = '\''; b[1] = c; b[2] = '\''; b[3] = '\0';
    return b;
}
/* String element/key, quoted Python-repr style: 'text' */
static char* _tr_fmt_str(const void* p) {
    const char* s = *(const char* const*)p;
    if (!s) s = "";
    size_t n = strlen(s);
    char* b = (char*)_tr_checked_alloc(n + 3);
    b[0] = '\'';
    memcpy(b + 1, s, n);
    b[n + 1] = '\'';
    b[n + 2] = '\0';
    return b;
}
/* Boxed-string Dict/Map value, quoted Python-repr style: 'text'.
   Dict values are stored as void* boxes (_tr_str_box -> TrStr*), so the slot
   is a TrStr*, not a char*. Unbox it before quoting (plain _tr_fmt_str would
   read the TrStr* address as a char* and print garbage). */
static char* _tr_fmt_str_box(const void* p) {
    TrStr* box = *(TrStr* const*)p;
    const char* s = (box && box->data) ? box->data : "";
    size_t n = strlen(s);
    char* b = (char*)_tr_checked_alloc(n + 3);
    b[0] = '\'';
    memcpy(b + 1, s, n);
    b[n + 1] = '\'';
    b[n + 2] = '\0';
    return b;
}
/* String element, unquoted (used for stringified Set[non-str] keys). */
static char* _tr_fmt_str_raw(const void* p) {
    const char* s = *(const char* const*)p;
    if (!s) s = "";
    size_t n = strlen(s);
    char* b = (char*)_tr_checked_alloc(n + 1);
    memcpy(b, s, n);
    b[n] = '\0';
    return b;
}
/* Default repr for objects without __str__/__repr__: "ClassName.obj at 0xADDR" */
static char* _tr_default_obj_str(const char* cls_name, const void* obj) {
    char* b = (char*)_tr_checked_alloc(64);
    snprintf(b, 64, "%s.obj at 0x%llx", cls_name, (unsigned long long)(uintptr_t)obj);
    return b;
}

/* ── v0.0.5: List sort / aggregate / functional helpers ──────────────────────
 * All List_T typedefs are defined above.                                  */
static int _tr_cmp_str_asc (const void* a, const void* b) { return strcmp(*(char**)a, *(char**)b); }
static int _tr_cmp_str_desc(const void* a, const void* b) { return strcmp(*(char**)b, *(char**)a); }
static int _tr_cmp_i64_asc (const void* a, const void* b) { int64_t x=*(int64_t*)a,y=*(int64_t*)b; return (x>y)-(x<y); }
static int _tr_cmp_i64_desc(const void* a, const void* b) { int64_t x=*(int64_t*)a,y=*(int64_t*)b; return (x<y)-(x>y); }
static int _tr_cmp_f64_asc (const void* a, const void* b) { double x=*(double*)a,y=*(double*)b; return (x>y)-(x<y); }
static int _tr_cmp_f64_desc(const void* a, const void* b) { double x=*(double*)a,y=*(double*)b; return (x<y)-(x>y); }
static void _tr_list_sort_str(List_str* l, int dir) { if(l&&l->len>1) qsort(l->data,(size_t)l->len,sizeof(char*),dir>0?_tr_cmp_str_asc:_tr_cmp_str_desc); }
static int _tr_cmp_trstr_asc (const void* a, const void* b) { return strcmp(((const TrStr*)a)->data, ((const TrStr*)b)->data); }
static int _tr_cmp_trstr_desc(const void* a, const void* b) { return strcmp(((const TrStr*)b)->data, ((const TrStr*)a)->data); }
static void _tr_list_sort_TrStr(List_TrStr* l, int dir) { if(l&&l->len>1) qsort(l->data,(size_t)l->len,sizeof(TrStr),dir>0?_tr_cmp_trstr_asc:_tr_cmp_trstr_desc); }
static void _tr_list_sort_i64(List_i64* l, int dir) { if(l&&l->len>1) qsort(l->data,(size_t)l->len,sizeof(int64_t),dir>0?_tr_cmp_i64_asc:_tr_cmp_i64_desc); }
static void _tr_list_sort_f64(List_f64* l, int dir) { if(l&&l->len>1) qsort(l->data,(size_t)l->len,sizeof(double),dir>0?_tr_cmp_f64_asc:_tr_cmp_f64_desc); }
static void _tr_list_sort_ptr(List_ptr* l, int dir) { (void)l; (void)dir; }
static int64_t _tr_list_sum_i64(List_i64* l) { int64_t s=0; if(l) for(int64_t i=0;i<(int64_t)l->len;i++) s+=l->data[i]; return s; }
static double  _tr_list_sum_f64(List_f64* l) { double  s=0; if(l) for(int64_t i=0;i<(int64_t)l->len;i++) s+=l->data[i]; return s; }
static int64_t _tr_list_min_i64(List_i64* l) { if(!l||l->len==0) return 0LL; int64_t m=l->data[0]; for(int64_t i=1;i<(int64_t)l->len;i++) if(l->data[i]<m) m=l->data[i]; return m; }
static int64_t _tr_list_max_i64(List_i64* l) { if(!l||l->len==0) return 0LL; int64_t m=l->data[0]; for(int64_t i=1;i<(int64_t)l->len;i++) if(l->data[i]>m) m=l->data[i]; return m; }
static double  _tr_list_min_f64(List_f64* l) { if(!l||l->len==0) return 0.0;  double  m=l->data[0]; for(int64_t i=1;i<(int64_t)l->len;i++) if(l->data[i]<m) m=l->data[i]; return m; }
static double  _tr_list_max_f64(List_f64* l) { if(!l||l->len==0) return 0.0;  double  m=l->data[0]; for(int64_t i=1;i<(int64_t)l->len;i++) if(l->data[i]>m) m=l->data[i]; return m; }
static int64_t _tr_list_sum_ptr(List_ptr* l) { return 0LL; }
static int64_t _tr_list_min_ptr(List_ptr* l) { return 0LL; }
static int64_t _tr_list_max_ptr(List_ptr* l) { return 0LL; }
static List_i64* _tr_list_reversed_i64(List_i64* l) {
    List_i64* r=List_i64_new(); if(l) for(int64_t i=(int64_t)l->len-1;i>=0;i--) List_i64_append(r,l->data[i]); return r;
}
static List_ptr* _tr_list_reversed_ptr(List_ptr* l) {
    List_ptr* r=List_ptr_new(); if(l) for(int64_t i=(int64_t)l->len-1;i>=0;i--) List_ptr_append(r,l->data[i]); return r;
}
static List_f64* _tr_list_reversed_f64(List_f64* l) {
    List_f64* r=List_f64_new(); if(l) for(int64_t i=(int64_t)l->len-1;i>=0;i--) List_f64_append(r,l->data[i]); return r;
}
static List_TrStr* _tr_list_reversed_TrStr(List_TrStr* l) {
    List_TrStr* r=List_TrStr_new(); if(l) for(int64_t i=(int64_t)l->len-1;i>=0;i--) List_TrStr_append(r,l->data[i]); return r;
}
/* In-place reverse */
static void _tr_list_reverse_i64(List_i64* l){ if(!l)return; for(int64_t i=0,j=(int64_t)l->len-1;i<j;i++,j--){int64_t t=l->data[i];l->data[i]=l->data[j];l->data[j]=t;} }
static void _tr_list_reverse_f64(List_f64* l){ if(!l)return; for(int64_t i=0,j=(int64_t)l->len-1;i<j;i++,j--){double t=l->data[i];l->data[i]=l->data[j];l->data[j]=t;} }
static void _tr_list_reverse_ptr(List_ptr* l){ if(!l)return; for(int64_t i=0,j=(int64_t)l->len-1;i<j;i++,j--){void* t=l->data[i];l->data[i]=l->data[j];l->data[j]=t;} }
static void _tr_list_reverse_str(List_str* l){ if(!l)return; for(int64_t i=0,j=(int64_t)l->len-1;i<j;i++,j--){char* t=l->data[i];l->data[i]=l->data[j];l->data[j]=t;} }
static void _tr_list_reverse_TrStr(List_TrStr* l){ if(!l)return; for(int64_t i=0,j=(int64_t)l->len-1;i<j;i++,j--){TrStr t=l->data[i];l->data[i]=l->data[j];l->data[j]=t;} }
/* Shallow clone (new backing buffer, same elements) */
static List_i64* _tr_list_clone_i64(List_i64* l){ List_i64* r=List_i64_new(); if(l) for(int64_t i=0;i<(int64_t)l->len;i++) List_i64_append(r,l->data[i]); return r; }
static List_f64* _tr_list_clone_f64(List_f64* l){ List_f64* r=List_f64_new(); if(l) for(int64_t i=0;i<(int64_t)l->len;i++) List_f64_append(r,l->data[i]); return r; }
static List_ptr* _tr_list_clone_ptr(List_ptr* l){ List_ptr* r=List_ptr_new(); if(l) for(int64_t i=0;i<(int64_t)l->len;i++) List_ptr_append(r,l->data[i]); return r; }
static List_str* _tr_list_clone_str(List_str* l){ List_str* r=List_str_new(); if(l) for(int64_t i=0;i<(int64_t)l->len;i++) List_str_append(r,l->data[i]); return r; }
static List_TrStr* _tr_list_clone_TrStr(List_TrStr* l){ List_TrStr* r=List_TrStr_new(); if(l) for(int64_t i=0;i<(int64_t)l->len;i++) List_TrStr_append(r,l->data[i]); return r; }
typedef int64_t (*_tr_pred_fn)(void*);
static int64_t _tr_list_any_ptr(List_ptr* l, _tr_pred_fn p) { if(!l) return 0LL; for(int64_t i=0;i<(int64_t)l->len;i++) if(p(l->data[i])) return 1LL; return 0LL; }
static int64_t _tr_list_all_ptr(List_ptr* l, _tr_pred_fn p) { if(!l) return 1LL; for(int64_t i=0;i<(int64_t)l->len;i++) if(!p(l->data[i])) return 0LL; return 1LL; }
static int64_t _tr_list_any_i64(List_i64* l, _tr_pred_fn p) { return _tr_list_any_ptr((List_ptr*)l, p); }
static int64_t _tr_list_all_i64(List_i64* l, _tr_pred_fn p) { return _tr_list_all_ptr((List_ptr*)l, p); }
static int64_t _tr_list_any_f64(List_f64* l, _tr_pred_fn p) { return _tr_list_any_ptr((List_ptr*)l, p); }
static int64_t _tr_list_all_f64(List_f64* l, _tr_pred_fn p) { return _tr_list_all_ptr((List_ptr*)l, p); }

/* ══════════════════════════════════════════════════════════════════════════
 *  std.gpu — Portable GPU compute runtime
 *
 *  ZERO build/link dependency on any GPU SDK. The CUDA Driver API and the
 *  OpenCL API are stable C ABIs; we redeclare the minimal subset we use and
 *  load the vendor driver library at RUNTIME (LoadLibrary / dlopen). No
 *  -lcuda / -lOpenCL, no CUDA/OpenCL headers required to build.
 *
 *  Backend selection order:  CUDA -> OpenCL -> CPU.
 *
 *  On a machine with no GPU driver the backend degrades to CPU: device
 *  buffers become host allocations and H2D/D2H copies become memcpy, so
 *  buffer programs stay CORRECT everywhere. Kernel launch requires a real
 *  GPU and reports an error string via _tr_gpu_last_error() on the CPU path.
 * ════════════════════════════════════════════════════════════════════════ */
#if !defined(TAURARO_NO_OS) && !defined(TAURARO_KERNEL) && !defined(__wasi__)

#if defined(_WIN32)
  /* windows.h already included above */
  typedef HMODULE _TrDl;
  static _TrDl _tr_gpu_dlopen(const char* n){ return LoadLibraryA(n); }
  static void* _tr_gpu_dlsym(_TrDl h, const char* s){ return (void*)(intptr_t)GetProcAddress(h, s); }
#else
  #include <dlfcn.h>
  typedef void* _TrDl;
  static _TrDl _tr_gpu_dlopen(const char* n){ return dlopen(n, RTLD_NOW | RTLD_LOCAL); }
  static void* _tr_gpu_dlsym(_TrDl h, const char* s){ return dlsym(h, s); }
#endif

enum { TR_GPU_CPU = 0, TR_GPU_CUDA = 1, TR_GPU_OPENCL = 2 };
#define TR_GPU_MAX_ARGS 32

/* ── CUDA Driver API (redeclared; loaded at runtime) ───────────────────────── */
typedef int                 _CUresult;
typedef int                 _CUdevice;
typedef unsigned long long  _CUdeviceptr;
typedef void*               _CUcontext;
typedef void*               _CUmodule;
typedef void*               _CUfunction;
typedef void*               _CUstream;
/* cuDeviceGetAttribute indices */
#define TR_CU_ATTR_CC_MAJOR 75
#define TR_CU_ATTR_CC_MINOR 76

typedef _CUresult (*_pcuInit)(unsigned int);
typedef _CUresult (*_pcuDeviceGetCount)(int*);
typedef _CUresult (*_pcuDeviceGet)(_CUdevice*, int);
typedef _CUresult (*_pcuDeviceGetName)(char*, int, _CUdevice);
typedef _CUresult (*_pcuDeviceGetAttribute)(int*, int, _CUdevice);
typedef _CUresult (*_pcuDeviceTotalMem)(size_t*, _CUdevice);
typedef _CUresult (*_pcuCtxCreate)(_CUcontext*, unsigned int, _CUdevice);
typedef _CUresult (*_pcuCtxDestroy)(_CUcontext);
typedef _CUresult (*_pcuCtxSynchronize)(void);
typedef _CUresult (*_pcuMemAlloc)(_CUdeviceptr*, size_t);
typedef _CUresult (*_pcuMemFree)(_CUdeviceptr);
typedef _CUresult (*_pcuMemcpyHtoD)(_CUdeviceptr, const void*, size_t);
typedef _CUresult (*_pcuMemcpyDtoH)(void*, _CUdeviceptr, size_t);
typedef _CUresult (*_pcuMemsetD8)(_CUdeviceptr, unsigned char, size_t);
typedef _CUresult (*_pcuModuleLoadData)(_CUmodule*, const void*);
typedef _CUresult (*_pcuModuleUnload)(_CUmodule);
typedef _CUresult (*_pcuModuleGetFunction)(_CUfunction*, _CUmodule, const char*);
typedef _CUresult (*_pcuLaunchKernel)(_CUfunction, unsigned,unsigned,unsigned,
                                      unsigned,unsigned,unsigned, unsigned,
                                      _CUstream, void**, void**);

/* ── OpenCL API (redeclared; loaded at runtime) ────────────────────────────── */
typedef int                 _cl_int;
typedef unsigned int        _cl_uint;
typedef unsigned long long  _cl_bitfield;
typedef void* _cl_platform_id; typedef void* _cl_device_id;  typedef void* _cl_context;
typedef void* _cl_command_queue; typedef void* _cl_mem; typedef void* _cl_program; typedef void* _cl_kernel;
#define TR_CL_DEVICE_TYPE_GPU     (1<<2)
#define TR_CL_DEVICE_TYPE_ALL     0xFFFFFFFF
#define TR_CL_DEVICE_NAME         0x102B
#define TR_CL_DEVICE_GLOBAL_MEM   0x101F
#define TR_CL_MEM_READ_WRITE      (1<<0)
#define TR_CL_TRUE                1
#define TR_CL_PROGRAM_BUILD_LOG   0x1183

typedef _cl_int (*_pclGetPlatformIDs)(_cl_uint, _cl_platform_id*, _cl_uint*);
typedef _cl_int (*_pclGetDeviceIDs)(_cl_platform_id, _cl_bitfield, _cl_uint, _cl_device_id*, _cl_uint*);
typedef _cl_int (*_pclGetDeviceInfo)(_cl_device_id, _cl_uint, size_t, void*, size_t*);
typedef _cl_context (*_pclCreateContext)(const intptr_t*, _cl_uint, const _cl_device_id*, void*, void*, _cl_int*);
typedef _cl_command_queue (*_pclCreateCommandQueue)(_cl_context, _cl_device_id, _cl_bitfield, _cl_int*);
typedef _cl_mem (*_pclCreateBuffer)(_cl_context, _cl_bitfield, size_t, void*, _cl_int*);
typedef _cl_int (*_pclEnqueueWriteBuffer)(_cl_command_queue, _cl_mem, _cl_uint, size_t, size_t, const void*, _cl_uint, const void*, void*);
typedef _cl_int (*_pclEnqueueReadBuffer)(_cl_command_queue, _cl_mem, _cl_uint, size_t, size_t, void*, _cl_uint, const void*, void*);
typedef _cl_int (*_pclReleaseMemObject)(_cl_mem);
typedef _cl_program (*_pclCreateProgramWithSource)(_cl_context, _cl_uint, const char**, const size_t*, _cl_int*);
typedef _cl_program (*_pclCreateProgramWithIL)(_cl_context, const void*, size_t, _cl_int*);
typedef _cl_int (*_pclBuildProgram)(_cl_program, _cl_uint, const _cl_device_id*, const char*, void*, void*);
typedef _cl_int (*_pclGetProgramBuildInfo)(_cl_program, _cl_device_id, _cl_uint, size_t, void*, size_t*);
typedef _cl_kernel (*_pclCreateKernel)(_cl_program, const char*, _cl_int*);
typedef _cl_int (*_pclSetKernelArg)(_cl_kernel, _cl_uint, size_t, const void*);
typedef _cl_int (*_pclEnqueueNDRangeKernel)(_cl_command_queue, _cl_kernel, _cl_uint, const size_t*, const size_t*, const size_t*, _cl_uint, const void*, void*);
typedef _cl_int (*_pclFinish)(_cl_command_queue);
typedef _cl_int (*_pclReleaseKernel)(_cl_kernel);
typedef _cl_int (*_pclReleaseProgram)(_cl_program);
typedef _cl_int (*_pclReleaseMemQueue)(_cl_command_queue);

typedef struct {
    int   inited;
    int   backend;
    int   dev_count;
    int   cc_major, cc_minor;
    long long total_mem;
    char  name[256];
    char  err[512];
    /* CUDA */
    _TrDl cu_lib; _CUcontext cu_ctx; _CUdevice cu_dev;
    _pcuMemAlloc cuMemAlloc; _pcuMemFree cuMemFree;
    _pcuMemcpyHtoD cuMemcpyHtoD; _pcuMemcpyDtoH cuMemcpyDtoH; _pcuMemsetD8 cuMemsetD8;
    _pcuModuleLoadData cuModuleLoadData; _pcuModuleUnload cuModuleUnload;
    _pcuModuleGetFunction cuModuleGetFunction; _pcuLaunchKernel cuLaunchKernel;
    _pcuCtxSynchronize cuCtxSynchronize;
    /* OpenCL */
    _TrDl cl_lib; _cl_platform_id cl_plat; _cl_device_id cl_dev;
    _cl_context cl_ctx; _cl_command_queue cl_q;
    _pclCreateBuffer clCreateBuffer; _pclEnqueueWriteBuffer clEnqueueWriteBuffer;
    _pclEnqueueReadBuffer clEnqueueReadBuffer; _pclReleaseMemObject clReleaseMemObject;
    _pclCreateProgramWithSource clCreateProgramWithSource; _pclCreateProgramWithIL clCreateProgramWithIL; _pclBuildProgram clBuildProgram;
    _pclGetProgramBuildInfo clGetProgramBuildInfo;
    _pclCreateKernel clCreateKernel; _pclSetKernelArg clSetKernelArg;
    _pclEnqueueNDRangeKernel clEnqueueNDRangeKernel; _pclFinish clFinish;
    _pclReleaseKernel clReleaseKernel; _pclReleaseProgram clReleaseProgram;
} _TrGpuState;
/* SINGLE shared instance across all translation units. If this were `static`,
 * each emitted module (buffer.c, kernel.c, main.c, …) would get its OWN GPU
 * context/queue — a buffer created in one module could not be seen by a kernel
 * launched from another, silently producing no result. The primary TU
 * (_TR_MAIN) owns the definition; every other TU sees it via `extern`. */
#ifdef _TR_MAIN
_TrGpuState _tr_gpu = {0};
#else
extern _TrGpuState _tr_gpu;
#endif

typedef struct { int backend; size_t size; void* host; _CUdeviceptr cu_ptr; _cl_mem cl_mem; } _TrGpuBuf;
typedef struct { int backend; _CUmodule cu_mod; _cl_program cl_prog; } _TrGpuMod;
typedef struct {
    int backend; _CUfunction cu_fn; _cl_kernel cl_k; int nargs;
    unsigned char argbuf[TR_GPU_MAX_ARGS][16];
    void* argptr[TR_GPU_MAX_ARGS];
} _TrGpuKern;

static void _tr_gpu_seterr(const char* m){ snprintf(_tr_gpu.err, sizeof(_tr_gpu.err), "%s", m ? m : ""); }

/* Try `base` then `base_v2` (CUDA versioned symbols). */
static void* _tr_gpu_cusym(_TrDl h, const char* base){
    char buf[128]; void* p;
    snprintf(buf, sizeof(buf), "%s_v2", base);
    p = _tr_gpu_dlsym(h, buf);
    if (!p) p = _tr_gpu_dlsym(h, base);
    return p;
}

static int _tr_gpu_try_cuda(void){
    static const char* libs[] = {
#if defined(_WIN32)
        "nvcuda.dll",
#elif defined(__APPLE__)
        "/usr/local/cuda/lib/libcuda.dylib", "libcuda.dylib",
#else
        "libcuda.so.1", "libcuda.so",
#endif
        0 };
    _TrDl h = 0; int i;
    for (i=0; libs[i]; i++){ h = _tr_gpu_dlopen(libs[i]); if (h) break; }
    if (!h) return 0;
    _pcuInit             pInit  = (_pcuInit)_tr_gpu_dlsym(h, "cuInit");
    _pcuDeviceGetCount   pCnt   = (_pcuDeviceGetCount)_tr_gpu_dlsym(h, "cuDeviceGetCount");
    _pcuDeviceGet        pGet   = (_pcuDeviceGet)_tr_gpu_dlsym(h, "cuDeviceGet");
    _pcuDeviceGetName    pName  = (_pcuDeviceGetName)_tr_gpu_dlsym(h, "cuDeviceGetName");
    _pcuDeviceGetAttribute pAttr= (_pcuDeviceGetAttribute)_tr_gpu_dlsym(h, "cuDeviceGetAttribute");
    _pcuDeviceTotalMem   pMem   = (_pcuDeviceTotalMem)_tr_gpu_cusym(h, "cuDeviceTotalMem");
    _pcuCtxCreate        pCtx   = (_pcuCtxCreate)_tr_gpu_cusym(h, "cuCtxCreate");
    if (!pInit || !pCnt || !pGet || !pCtx) return 0;
    if (pInit(0) != 0) return 0;
    int cnt = 0;
    if (pCnt(&cnt) != 0 || cnt <= 0) return 0;
    _CUdevice dev = 0;
    if (pGet(&dev, 0) != 0) return 0;
    _CUcontext ctx = 0;
    if (pCtx(&ctx, 0, dev) != 0) return 0;
    _tr_gpu.cu_lib = h; _tr_gpu.cu_ctx = ctx; _tr_gpu.cu_dev = dev; _tr_gpu.dev_count = cnt;
    if (pName) pName(_tr_gpu.name, sizeof(_tr_gpu.name), dev); else snprintf(_tr_gpu.name,sizeof(_tr_gpu.name),"CUDA device");
    if (pAttr){ int mj=0,mn=0; pAttr(&mj,TR_CU_ATTR_CC_MAJOR,dev); pAttr(&mn,TR_CU_ATTR_CC_MINOR,dev); _tr_gpu.cc_major=mj; _tr_gpu.cc_minor=mn; }
    if (pMem){ size_t tm=0; pMem(&tm, dev); _tr_gpu.total_mem=(long long)tm; }
    _tr_gpu.cuMemAlloc=(_pcuMemAlloc)_tr_gpu_cusym(h,"cuMemAlloc");
    _tr_gpu.cuMemFree=(_pcuMemFree)_tr_gpu_cusym(h,"cuMemFree");
    _tr_gpu.cuMemcpyHtoD=(_pcuMemcpyHtoD)_tr_gpu_cusym(h,"cuMemcpyHtoD");
    _tr_gpu.cuMemcpyDtoH=(_pcuMemcpyDtoH)_tr_gpu_cusym(h,"cuMemcpyDtoH");
    _tr_gpu.cuMemsetD8=(_pcuMemsetD8)_tr_gpu_cusym(h,"cuMemsetD8");
    _tr_gpu.cuModuleLoadData=(_pcuModuleLoadData)_tr_gpu_dlsym(h,"cuModuleLoadData");
    _tr_gpu.cuModuleUnload=(_pcuModuleUnload)_tr_gpu_dlsym(h,"cuModuleUnload");
    _tr_gpu.cuModuleGetFunction=(_pcuModuleGetFunction)_tr_gpu_dlsym(h,"cuModuleGetFunction");
    _tr_gpu.cuLaunchKernel=(_pcuLaunchKernel)_tr_gpu_dlsym(h,"cuLaunchKernel");
    _tr_gpu.cuCtxSynchronize=(_pcuCtxSynchronize)_tr_gpu_dlsym(h,"cuCtxSynchronize");
    _tr_gpu.backend = TR_GPU_CUDA;
    return 1;
}

static int _tr_gpu_try_opencl(void){
    static const char* libs[] = {
#if defined(_WIN32)
        "OpenCL.dll",
#elif defined(__APPLE__)
        "/System/Library/Frameworks/OpenCL.framework/OpenCL", "libOpenCL.so",
#else
        "libOpenCL.so.1", "libOpenCL.so",
#endif
        0 };
    _TrDl h = 0; int i;
    for (i=0; libs[i]; i++){ h = _tr_gpu_dlopen(libs[i]); if (h) break; }
    if (!h) return 0;
    _pclGetPlatformIDs pPlat = (_pclGetPlatformIDs)_tr_gpu_dlsym(h,"clGetPlatformIDs");
    _pclGetDeviceIDs   pDev  = (_pclGetDeviceIDs)_tr_gpu_dlsym(h,"clGetDeviceIDs");
    _pclGetDeviceInfo  pInfo = (_pclGetDeviceInfo)_tr_gpu_dlsym(h,"clGetDeviceInfo");
    _pclCreateContext  pCtx  = (_pclCreateContext)_tr_gpu_dlsym(h,"clCreateContext");
    _pclCreateCommandQueue pQ= (_pclCreateCommandQueue)_tr_gpu_dlsym(h,"clCreateCommandQueue");
    if (!pPlat || !pDev || !pCtx || !pQ) return 0;
    _cl_uint nplat = 0;
    if (pPlat(0, 0, &nplat) != 0 || nplat == 0) return 0;
    _cl_platform_id plats[8]; if (nplat > 8) nplat = 8;
    if (pPlat(nplat, plats, 0) != 0) return 0;
    _cl_device_id dev = 0; _cl_platform_id plat = 0; _cl_uint pi;
    for (pi = 0; pi < nplat; pi++){
        _cl_uint nd = 0;
        if (pDev(plats[pi], TR_CL_DEVICE_TYPE_GPU, 1, &dev, &nd) == 0 && nd > 0){ plat = plats[pi]; break; }
    }
    if (!dev){ /* fall back to any device type */
        for (pi = 0; pi < nplat; pi++){
            _cl_uint nd = 0;
            if (pDev(plats[pi], TR_CL_DEVICE_TYPE_ALL, 1, &dev, &nd) == 0 && nd > 0){ plat = plats[pi]; break; }
        }
    }
    if (!dev) return 0;
    _cl_int err = 0;
    _cl_context ctx = pCtx(0, 1, &dev, 0, 0, &err);
    if (!ctx || err != 0) return 0;
    _cl_command_queue q = pQ(ctx, dev, 0, &err);
    if (!q || err != 0) return 0;
    _tr_gpu.cl_lib=h; _tr_gpu.cl_plat=plat; _tr_gpu.cl_dev=dev; _tr_gpu.cl_ctx=ctx; _tr_gpu.cl_q=q; _tr_gpu.dev_count=1;
    if (pInfo){ pInfo(dev, TR_CL_DEVICE_NAME, sizeof(_tr_gpu.name), _tr_gpu.name, 0);
                _cl_bitfield gm=0; pInfo(dev, TR_CL_DEVICE_GLOBAL_MEM, sizeof(gm), &gm, 0); _tr_gpu.total_mem=(long long)gm; }
    else snprintf(_tr_gpu.name,sizeof(_tr_gpu.name),"OpenCL device");
    _tr_gpu.clCreateBuffer=(_pclCreateBuffer)_tr_gpu_dlsym(h,"clCreateBuffer");
    _tr_gpu.clEnqueueWriteBuffer=(_pclEnqueueWriteBuffer)_tr_gpu_dlsym(h,"clEnqueueWriteBuffer");
    _tr_gpu.clEnqueueReadBuffer=(_pclEnqueueReadBuffer)_tr_gpu_dlsym(h,"clEnqueueReadBuffer");
    _tr_gpu.clReleaseMemObject=(_pclReleaseMemObject)_tr_gpu_dlsym(h,"clReleaseMemObject");
    _tr_gpu.clCreateProgramWithSource=(_pclCreateProgramWithSource)_tr_gpu_dlsym(h,"clCreateProgramWithSource");
    _tr_gpu.clCreateProgramWithIL=(_pclCreateProgramWithIL)_tr_gpu_dlsym(h,"clCreateProgramWithIL");
    _tr_gpu.clBuildProgram=(_pclBuildProgram)_tr_gpu_dlsym(h,"clBuildProgram");
    _tr_gpu.clGetProgramBuildInfo=(_pclGetProgramBuildInfo)_tr_gpu_dlsym(h,"clGetProgramBuildInfo");
    _tr_gpu.clCreateKernel=(_pclCreateKernel)_tr_gpu_dlsym(h,"clCreateKernel");
    _tr_gpu.clSetKernelArg=(_pclSetKernelArg)_tr_gpu_dlsym(h,"clSetKernelArg");
    _tr_gpu.clEnqueueNDRangeKernel=(_pclEnqueueNDRangeKernel)_tr_gpu_dlsym(h,"clEnqueueNDRangeKernel");
    _tr_gpu.clFinish=(_pclFinish)_tr_gpu_dlsym(h,"clFinish");
    _tr_gpu.clReleaseKernel=(_pclReleaseKernel)_tr_gpu_dlsym(h,"clReleaseKernel");
    _tr_gpu.clReleaseProgram=(_pclReleaseProgram)_tr_gpu_dlsym(h,"clReleaseProgram");
    _tr_gpu.backend = TR_GPU_OPENCL;
    /* Warm up the launch path once. Some OpenCL drivers (notably older Intel
     * Gen graphics) defer/miss the *first* kernel enqueue in a process even
     * though clFinish reports success; a prior throwaway launch makes every
     * subsequent user launch reliable. Costs microseconds, runs once. */
    if (_tr_gpu.clCreateProgramWithSource && _tr_gpu.clBuildProgram && _tr_gpu.clCreateKernel &&
        _tr_gpu.clCreateBuffer && _tr_gpu.clSetKernelArg && _tr_gpu.clEnqueueNDRangeKernel &&
        _tr_gpu.clFinish && _tr_gpu.clReleaseMemObject && _tr_gpu.clReleaseKernel && _tr_gpu.clReleaseProgram &&
        _tr_gpu.clEnqueueWriteBuffer){
        /* Prime BOTH an fp32 and an fp64 compute path: some drivers JIT/prime
         * per element-type, so a real double-typed launch must run once. */
        const char* ws = "#ifdef cl_khr_fp64\n#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n__kernel void _tr_warm(__global double* a,double k,int n){int i=get_global_id(0); if(i<n) a[i]=a[i]*k;}\n#else\n__kernel void _tr_warm(__global float* a,float k,int n){int i=get_global_id(0); if(i<n) a[i]=a[i]*k;}\n#endif";
        size_t wl = strlen(ws); _cl_int we=0;
        _cl_program wp = _tr_gpu.clCreateProgramWithSource(_tr_gpu.cl_ctx,1,&ws,&wl,&we);
        if (wp && we==0 && _tr_gpu.clBuildProgram(wp,1,&_tr_gpu.cl_dev,0,0,0)==0){
            _cl_kernel wk = _tr_gpu.clCreateKernel(wp,"_tr_warm",&we);
            _cl_mem wb = _tr_gpu.clCreateBuffer(_tr_gpu.cl_ctx,TR_CL_MEM_READ_WRITE,8,0,&we);
            if (wk && wb){
                double init=1.0; _tr_gpu.clEnqueueWriteBuffer(_tr_gpu.cl_q,wb,TR_CL_TRUE,0,8,&init,0,0,0);
                double kk=2.0; int nn=1;
                _tr_gpu.clSetKernelArg(wk,0,sizeof(_cl_mem),&wb);
                _tr_gpu.clSetKernelArg(wk,1,8,&kk);
                _tr_gpu.clSetKernelArg(wk,2,4,&nn);
                size_t wg=1; _tr_gpu.clEnqueueNDRangeKernel(_tr_gpu.cl_q,wk,1,0,&wg,0,0,0,0);
                _tr_gpu.clFinish(_tr_gpu.cl_q);
            }
            if (wb) _tr_gpu.clReleaseMemObject(wb);
            if (wk) _tr_gpu.clReleaseKernel(wk);
            _tr_gpu.clReleaseProgram(wp);
        } else if (wp) { _tr_gpu.clReleaseProgram(wp); }
    }
    return 1;
}

static int64_t _tr_gpu_init(void){
    if (_tr_gpu.inited) return (int64_t)_tr_gpu.backend;
    _tr_gpu.inited = 1;
    _tr_gpu.backend = TR_GPU_CPU;
    snprintf(_tr_gpu.name, sizeof(_tr_gpu.name), "CPU (no GPU backend)");
    if (_tr_gpu_try_cuda()) { _tr_gpu_seterr(""); return (int64_t)_tr_gpu.backend; }
    if (_tr_gpu_try_opencl()) { _tr_gpu_seterr(""); return (int64_t)_tr_gpu.backend; }
    _tr_gpu_seterr("no CUDA or OpenCL device found; using CPU fallback");
    return (int64_t)_tr_gpu.backend;
}

static int64_t _tr_gpu_backend(void){ _tr_gpu_init(); return (int64_t)_tr_gpu.backend; }
static char* _tr_gpu_backend_name(void){
    _tr_gpu_init();
    const char* n = _tr_gpu.backend==TR_GPU_CUDA ? "CUDA" : (_tr_gpu.backend==TR_GPU_OPENCL ? "OpenCL" : "CPU");
    return _tr_strdup((char*)n);
}
static char* _tr_gpu_device_name(void){ _tr_gpu_init(); return _tr_strdup(_tr_gpu.name); }
static int64_t _tr_gpu_device_count(void){ _tr_gpu_init(); return (int64_t)_tr_gpu.dev_count; }
static char* _tr_gpu_last_error(void){ return _tr_strdup(_tr_gpu.err); }
static int64_t _tr_gpu_compute_capability(void){ _tr_gpu_init(); return (int64_t)(_tr_gpu.cc_major*10 + _tr_gpu.cc_minor); }
static int64_t _tr_gpu_total_mem(void){ _tr_gpu_init(); return (int64_t)_tr_gpu.total_mem; }
static int64_t _tr_gpu_has_device(void){ return _tr_gpu_init() != TR_GPU_CPU ? 1 : 0; }

/* ── Buffers ───────────────────────────────────────────────────────────────── */
static void* _tr_gpu_malloc(int64_t size){
    _tr_gpu_init();
    if (size <= 0) size = 1;
    _TrGpuBuf* b = (_TrGpuBuf*)TAURARO_ALLOC(sizeof(_TrGpuBuf));
    if (!b){ _tr_gpu_seterr("gpu_malloc: out of host memory"); return 0; }
    memset(b, 0, sizeof(*b)); b->backend=_tr_gpu.backend; b->size=(size_t)size;
    if (_tr_gpu.backend==TR_GPU_CUDA && _tr_gpu.cuMemAlloc){
        if (_tr_gpu.cuMemAlloc(&b->cu_ptr, b->size)!=0){ _tr_gpu_seterr("cuMemAlloc failed"); TAURARO_FREE(b); return 0; }
    } else if (_tr_gpu.backend==TR_GPU_OPENCL && _tr_gpu.clCreateBuffer){
        _cl_int e=0; b->cl_mem=_tr_gpu.clCreateBuffer(_tr_gpu.cl_ctx, TR_CL_MEM_READ_WRITE, b->size, 0, &e);
        if (!b->cl_mem || e!=0){ _tr_gpu_seterr("clCreateBuffer failed"); TAURARO_FREE(b); return 0; }
    } else {
        b->host = TAURARO_ALLOC(b->size);
        if (!b->host){ _tr_gpu_seterr("gpu_malloc: out of host memory"); TAURARO_FREE(b); return 0; }
        memset(b->host, 0, b->size);
    }
    return b;
}
static void _tr_gpu_free(void* handle){
    _TrGpuBuf* b=(_TrGpuBuf*)handle; if (!b) return;
    if (b->backend==TR_GPU_CUDA && _tr_gpu.cuMemFree && b->cu_ptr) _tr_gpu.cuMemFree(b->cu_ptr);
    else if (b->backend==TR_GPU_OPENCL && _tr_gpu.clReleaseMemObject && b->cl_mem) _tr_gpu.clReleaseMemObject(b->cl_mem);
    else if (b->host) TAURARO_FREE(b->host);
    TAURARO_FREE(b);
}
static int64_t _tr_gpu_buffer_size(void* handle){ _TrGpuBuf* b=(_TrGpuBuf*)handle; return b?(int64_t)b->size:0; }
static int64_t _tr_gpu_h2d(void* handle, void* src, int64_t n){
    _TrGpuBuf* b=(_TrGpuBuf*)handle; if (!b||!src) return -1;
    size_t sz=(size_t)n; if (sz>b->size) sz=b->size;
    if (b->backend==TR_GPU_CUDA){ if(_tr_gpu.cuMemcpyHtoD(b->cu_ptr,src,sz)!=0){_tr_gpu_seterr("cuMemcpyHtoD failed");return -1;} }
    else if (b->backend==TR_GPU_OPENCL){ if(_tr_gpu.clEnqueueWriteBuffer(_tr_gpu.cl_q,b->cl_mem,TR_CL_TRUE,0,sz,src,0,0,0)!=0){_tr_gpu_seterr("clEnqueueWriteBuffer failed");return -1;} }
    else memcpy(b->host, src, sz);
    return 0;
}
static int64_t _tr_gpu_d2h(void* handle, void* dst, int64_t n){
    _TrGpuBuf* b=(_TrGpuBuf*)handle; if (!b||!dst) return -1;
    size_t sz=(size_t)n; if (sz>b->size) sz=b->size;
    if (b->backend==TR_GPU_CUDA){ if(_tr_gpu.cuMemcpyDtoH(dst,b->cu_ptr,sz)!=0){_tr_gpu_seterr("cuMemcpyDtoH failed");return -1;} }
    else if (b->backend==TR_GPU_OPENCL){ if(_tr_gpu.clEnqueueReadBuffer(_tr_gpu.cl_q,b->cl_mem,TR_CL_TRUE,0,sz,dst,0,0,0)!=0){_tr_gpu_seterr("clEnqueueReadBuffer failed");return -1;} }
    else memcpy(dst, b->host, sz);
    return 0;
}
static int64_t _tr_gpu_memset(void* handle, int64_t val, int64_t n){
    _TrGpuBuf* b=(_TrGpuBuf*)handle; if (!b) return -1;
    size_t sz=(size_t)n; if (sz>b->size) sz=b->size;
    if (b->backend==TR_GPU_CUDA){ if(_tr_gpu.cuMemsetD8(b->cu_ptr,(unsigned char)val,sz)!=0){_tr_gpu_seterr("cuMemsetD8 failed");return -1;} }
    else if (b->backend==TR_GPU_OPENCL){ /* no direct fill in 1.x subset: stage on host */
        void* tmp=TAURARO_ALLOC(sz); if(!tmp) return -1; memset(tmp,(int)val,sz);
        int64_t r=_tr_gpu_h2d(handle,tmp,(int64_t)sz); TAURARO_FREE(tmp); return r; }
    else memset(b->host,(int)val,sz);
    return 0;
}

/* ── Modules & kernels ─────────────────────────────────────────────────────── */
static void* _tr_gpu_module_load_ptx(char* ptx){
    _tr_gpu_init();
    if (_tr_gpu.backend!=TR_GPU_CUDA || !_tr_gpu.cuModuleLoadData){ _tr_gpu_seterr("PTX modules require the CUDA backend"); return 0; }
    _TrGpuMod* m=(_TrGpuMod*)TAURARO_ALLOC(sizeof(_TrGpuMod)); if(!m) return 0; memset(m,0,sizeof(*m)); m->backend=TR_GPU_CUDA;
    if (_tr_gpu.cuModuleLoadData(&m->cu_mod, ptx)!=0){ _tr_gpu_seterr("cuModuleLoadData failed (bad PTX?)"); TAURARO_FREE(m); return 0; }
    return m;
}
static void* _tr_gpu_module_load_source(char* src){
    _tr_gpu_init();
    if (_tr_gpu.backend!=TR_GPU_OPENCL || !_tr_gpu.clCreateProgramWithSource){ _tr_gpu_seterr("source kernels require the OpenCL backend (use load_ptx for CUDA)"); return 0; }
    _TrGpuMod* m=(_TrGpuMod*)TAURARO_ALLOC(sizeof(_TrGpuMod)); if(!m) return 0; memset(m,0,sizeof(*m)); m->backend=TR_GPU_OPENCL;
    _cl_int e=0; const char* s=src; size_t len=strlen(src);
    m->cl_prog=_tr_gpu.clCreateProgramWithSource(_tr_gpu.cl_ctx,1,&s,&len,&e);
    if (!m->cl_prog||e!=0){ _tr_gpu_seterr("clCreateProgramWithSource failed"); TAURARO_FREE(m); return 0; }
    if (_tr_gpu.clBuildProgram(m->cl_prog,1,&_tr_gpu.cl_dev,0,0,0)!=0){
        char log[2048]; log[0]=0;
        if (_tr_gpu.clGetProgramBuildInfo) _tr_gpu.clGetProgramBuildInfo(m->cl_prog,_tr_gpu.cl_dev,TR_CL_PROGRAM_BUILD_LOG,sizeof(log),log,0);
        char msg[2176]; snprintf(msg,sizeof(msg),"clBuildProgram failed: %s",log); _tr_gpu_seterr(msg);
        if (_tr_gpu.clReleaseProgram) _tr_gpu.clReleaseProgram(m->cl_prog);
        TAURARO_FREE(m); return 0;
    }
    return m;
}
/* Load a binary intermediate-language module: SPIR-V for OpenCL (via
 * clCreateProgramWithIL, needs an OpenCL 2.1+ / SPIR-V-capable driver), or a
 * PTX/cubin image for CUDA (same path as load_ptx). `il` points to `len` bytes. */
static void* _tr_gpu_module_load_il(void* il, int64_t len){
    _tr_gpu_init();
    if (_tr_gpu.backend==TR_GPU_CUDA && _tr_gpu.cuModuleLoadData){
        _TrGpuMod* m=(_TrGpuMod*)TAURARO_ALLOC(sizeof(_TrGpuMod)); if(!m) return 0; memset(m,0,sizeof(*m)); m->backend=TR_GPU_CUDA;
        if (_tr_gpu.cuModuleLoadData(&m->cu_mod, il)!=0){ _tr_gpu_seterr("cuModuleLoadData failed (bad PTX/cubin?)"); TAURARO_FREE(m); return 0; }
        return m;
    }
    if (_tr_gpu.backend==TR_GPU_OPENCL){
        if (!_tr_gpu.clCreateProgramWithIL){ _tr_gpu_seterr("SPIR-V modules require an OpenCL 2.1+ driver (clCreateProgramWithIL unavailable)"); return 0; }
        /* Guard: clCreateProgramWithIL expects SPIR-V; feeding it a PTX blob (wrong
         * magic) can CRASH some drivers. Verify the SPIR-V magic (0x07230203) first. */
        {
            const unsigned char* _b=(const unsigned char*)il;
            if (len<4 || !(_b[0]==0x03 && _b[1]==0x02 && _b[2]==0x23 && _b[3]==0x07)){
                _tr_gpu_seterr("module is not SPIR-V (rebuild with --gpu-embed spirv / --gpu-target spirv for the OpenCL backend)");
                return 0;
            }
        }
        _TrGpuMod* m=(_TrGpuMod*)TAURARO_ALLOC(sizeof(_TrGpuMod)); if(!m) return 0; memset(m,0,sizeof(*m)); m->backend=TR_GPU_OPENCL;
        _cl_int e=0;
        m->cl_prog=_tr_gpu.clCreateProgramWithIL(_tr_gpu.cl_ctx, il, (size_t)len, &e);
        if (!m->cl_prog||e!=0){ char msg[96]; snprintf(msg,sizeof(msg),"clCreateProgramWithIL failed: %d",(int)e); _tr_gpu_seterr(msg); TAURARO_FREE(m); return 0; }
        if (_tr_gpu.clBuildProgram(m->cl_prog,1,&_tr_gpu.cl_dev,0,0,0)!=0){
            char log[2048]; log[0]=0;
            if (_tr_gpu.clGetProgramBuildInfo) _tr_gpu.clGetProgramBuildInfo(m->cl_prog,_tr_gpu.cl_dev,TR_CL_PROGRAM_BUILD_LOG,sizeof(log),log,0);
            char msg[2176]; snprintf(msg,sizeof(msg),"clBuildProgram(SPIR-V) failed: %s",log); _tr_gpu_seterr(msg);
            if (_tr_gpu.clReleaseProgram) _tr_gpu.clReleaseProgram(m->cl_prog);
            TAURARO_FREE(m); return 0;
        }
        return m;
    }
    _tr_gpu_seterr("IL modules require a CUDA or OpenCL backend");
    return 0;
}
static void _tr_gpu_module_free(void* handle){
    _TrGpuMod* m=(_TrGpuMod*)handle; if(!m) return;
    if (m->backend==TR_GPU_CUDA && _tr_gpu.cuModuleUnload && m->cu_mod) _tr_gpu.cuModuleUnload(m->cu_mod);
    else if (m->backend==TR_GPU_OPENCL && _tr_gpu.clReleaseProgram && m->cl_prog) _tr_gpu.clReleaseProgram(m->cl_prog);
    TAURARO_FREE(m);
}
static void* _tr_gpu_kernel_get(void* modh, char* name){
    _TrGpuMod* m=(_TrGpuMod*)modh; if(!m||!name){ _tr_gpu_seterr("kernel_get: null module"); return 0; }
    _TrGpuKern* k=(_TrGpuKern*)TAURARO_ALLOC(sizeof(_TrGpuKern)); if(!k) return 0; memset(k,0,sizeof(*k)); k->backend=m->backend;
    if (m->backend==TR_GPU_CUDA){
        if (_tr_gpu.cuModuleGetFunction(&k->cu_fn,m->cu_mod,name)!=0){ _tr_gpu_seterr("cuModuleGetFunction: kernel not found"); TAURARO_FREE(k); return 0; }
    } else if (m->backend==TR_GPU_OPENCL){
        _cl_int e=0; k->cl_k=_tr_gpu.clCreateKernel(m->cl_prog,name,&e);
        if (!k->cl_k||e!=0){ _tr_gpu_seterr("clCreateKernel: kernel not found"); TAURARO_FREE(k); return 0; }
    } else { _tr_gpu_seterr("kernel_get: no GPU backend"); TAURARO_FREE(k); return 0; }
    return k;
}
static void _tr_gpu_kernel_free(void* handle){
    _TrGpuKern* k=(_TrGpuKern*)handle; if(!k) return;
    if (k->backend==TR_GPU_OPENCL && _tr_gpu.clReleaseKernel && k->cl_k) _tr_gpu.clReleaseKernel(k->cl_k);
    TAURARO_FREE(k);
}
static void _tr_gpu_kern_stage(_TrGpuKern* k, int idx, const void* data, size_t sz){
    if (!k || idx<0||idx>=TR_GPU_MAX_ARGS) return;   /* NULL kernel (failed load) → no-op */
    if (sz>16) sz=16;
    memcpy(k->argbuf[idx], data, sz);
    k->argptr[idx]=k->argbuf[idx];
    if (idx+1>k->nargs) k->nargs=idx+1;
    if (k->backend==TR_GPU_OPENCL && _tr_gpu.clSetKernelArg){
        _cl_int e = _tr_gpu.clSetKernelArg(k->cl_k,(_cl_uint)idx,sz,k->argbuf[idx]);
        if (e != 0){ char m[96]; snprintf(m,sizeof(m),"clSetKernelArg[%d] failed: %d", idx, (int)e); _tr_gpu_seterr(m); }
    }
}
static void _tr_gpu_kernel_set_arg_buf(void* handle, int64_t idx, void* bufh){
    _TrGpuKern* k=(_TrGpuKern*)handle; _TrGpuBuf* b=(_TrGpuBuf*)bufh; if(!k||!b) return;
    if (k->backend==TR_GPU_CUDA) _tr_gpu_kern_stage(k,(int)idx,&b->cu_ptr,sizeof(b->cu_ptr));
    else _tr_gpu_kern_stage(k,(int)idx,&b->cl_mem,sizeof(b->cl_mem));
}
static void _tr_gpu_kernel_set_arg_i32(void* h,int64_t idx,int64_t v){ int x=(int)v; _tr_gpu_kern_stage((_TrGpuKern*)h,(int)idx,&x,sizeof(x)); }
static void _tr_gpu_kernel_set_arg_i64(void* h,int64_t idx,int64_t v){ _tr_gpu_kern_stage((_TrGpuKern*)h,(int)idx,&v,sizeof(v)); }
static void _tr_gpu_kernel_set_arg_f32(void* h,int64_t idx,double v){ float f=(float)v; _tr_gpu_kern_stage((_TrGpuKern*)h,(int)idx,&f,sizeof(f)); }
static void _tr_gpu_kernel_set_arg_f64(void* h,int64_t idx,double v){ _tr_gpu_kern_stage((_TrGpuKern*)h,(int)idx,&v,sizeof(v)); }

static int64_t _tr_gpu_launch(void* handle, int64_t gx,int64_t gy,int64_t gz, int64_t bx,int64_t by,int64_t bz){
    _TrGpuKern* k=(_TrGpuKern*)handle; if(!k){ _tr_gpu_seterr("launch: null kernel"); return -1; }
    if (gx<1)gx=1; if(gy<1)gy=1; if(gz<1)gz=1; if(bx<1)bx=1; if(by<1)by=1; if(bz<1)bz=1;
    if (k->backend==TR_GPU_CUDA){
        void* params[TR_GPU_MAX_ARGS]; int i; for(i=0;i<k->nargs;i++) params[i]=k->argptr[i];
        if (_tr_gpu.cuLaunchKernel(k->cu_fn,(unsigned)gx,(unsigned)gy,(unsigned)gz,(unsigned)bx,(unsigned)by,(unsigned)bz,0,0,params,0)!=0){ _tr_gpu_seterr("cuLaunchKernel failed"); return -1; }
        return 0;
    } else if (k->backend==TR_GPU_OPENCL){
        size_t global[3]={(size_t)(gx*bx),(size_t)(gy*by),(size_t)(gz*bz)};
        size_t local[3]={(size_t)bx,(size_t)by,(size_t)bz};
        _cl_uint dims = (gz*bz>1)?3:((gy*by>1)?2:1);
        /* Pass local=NULL: let the driver choose a valid work-group size. This
         * avoids CL_INVALID_WORK_GROUP_SIZE when the requested block does not
         * divide the global size or exceeds the kernel's device maximum. */
        _cl_int e = _tr_gpu.clEnqueueNDRangeKernel(_tr_gpu.cl_q,k->cl_k,dims,0,global,0,0,0,0);
        if (e!=0){ char m[96]; snprintf(m,sizeof(m),"clEnqueueNDRangeKernel failed: %d",(int)e); _tr_gpu_seterr(m); return -1; }
        return 0;
    }
    _tr_gpu_seterr("launch: no GPU backend (kernels require CUDA or OpenCL)");
    return -1;
}
/* Read a whole file as raw bytes (binary-safe, unlike read_file which stops at NUL) for
 * loading a precompiled kernel module (.spv is binary, .ptx is text). Returns the buffer;
 * the byte length is retrieved via _tr_gpu_read_file_len() immediately after (both calls
 * run in the same std.gpu translation unit, so the static length stays consistent). */
static int64_t _tr_gpu_last_file_len = 0;
static void* _tr_gpu_read_file(char* path){
    _tr_gpu_last_file_len = 0;
    if (!path) return 0;
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END); long sz = ftell(f); rewind(f);
    if (sz < 0){ fclose(f); return 0; }
    void* buf = TAURARO_ALLOC((size_t)sz + 1);
    if (!buf){ fclose(f); return 0; }
    size_t rd = fread(buf, 1, (size_t)sz, f); fclose(f);
    ((char*)buf)[rd] = 0;
    _tr_gpu_last_file_len = (int64_t)rd;
    return buf;
}
static int64_t _tr_gpu_read_file_len(void){ return _tr_gpu_last_file_len; }

/* Embedded kernel module accessors. When a program is built with `--gpu-embed`,
 * the compiler emits a `_tr_gpu_blob.c` with STRONG definitions of these (the
 * compiled PTX/SPIR-V bytes). These WEAK defaults (empty) let Module.embedded()
 * link and degrade gracefully when nothing was embedded. The prototypes are
 * visible to EVERY TU (std.gpu's kernel.c calls them); only _TR_MAIN provides
 * the weak definitions. */
char*     _tr_gpu_embedded_blob(void);
long long _tr_gpu_embedded_len(void);
#ifdef _TR_MAIN
#if defined(__GNUC__)
__attribute__((weak)) char*     _tr_gpu_embedded_blob(void){ return 0; }
__attribute__((weak)) long long _tr_gpu_embedded_len(void){ return 0; }
#endif
#endif
static int64_t _tr_gpu_synchronize(void){
    _tr_gpu_init();
    if (_tr_gpu.backend==TR_GPU_CUDA && _tr_gpu.cuCtxSynchronize) return _tr_gpu.cuCtxSynchronize()==0?0:-1;
    if (_tr_gpu.backend==TR_GPU_OPENCL && _tr_gpu.clFinish) return _tr_gpu.clFinish(_tr_gpu.cl_q)==0?0:-1;
    return 0;
}


#endif /* !TAURARO_NO_OS && !TAURARO_KERNEL && !__wasi__ */

#endif /* TAURARO_RT_H */
