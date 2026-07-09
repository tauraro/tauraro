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
#  include <setjmp.h>
#  include <ctype.h>
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
     /* Bare-metal (non-Linux-kernel, e.g. arm-none-eabi/newlib): setjmp.h and
      * stdarg.h are freestanding headers. setjmp is needed by the bare-metal panic
      * buffer (_tr_thread_panic_jmpbuf); stdarg by the libc-lite vsnprintf. */
#    include <setjmp.h>
#    include <stdarg.h>
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
/* sprintf intentionally omitted: use snprintf with an explicit buffer size */
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

// Wrappers for core library to avoid signature conflicts
static inline void* _tr_c_malloc(size_t size) {
    void* p = TAURARO_ALLOC(size);
    if (p) _TR_MEMCOUNT_INC();
    return p;
}
static inline void* _tr_c_calloc(size_t count, size_t size) {
    void* p = TAURARO_CALLOC(count, size);
    if (!p && count * size > 0) { _TR_OOM_ABORT(); }
    if (p) _TR_MEMCOUNT_INC();
    return p;
}
static inline void _tr_free(void* p) {
    if (p) { _TR_MEMCOUNT_DEC(); TAURARO_FREE(p); }
}
static inline void _tr_c_free(void* ptr) { _tr_free(ptr); }

#ifndef TAURARO_KERNEL
static inline void _tr_print(char* s) { printf("%s\n", s); }
static inline void _tr_print_raw(char* s) { printf("%s", s); fflush(stdout); }
static inline void _tr_eprint(char* s) { _TR_DIAG("%s\n", s); fflush(stderr); }
#else
#ifndef _TR_WRITE
#  define _TR_WRITE(s) ((void)(s))   /* freestanding sink: redefine to UART/semihosting */
#endif
static inline void _tr_print(char* s) { _TR_WRITE(s); _TR_WRITE("\n"); }
static inline void _tr_print_raw(char* s) { _TR_WRITE(s); }
static inline void _tr_eprint(char* s) { _TR_WRITE(s); _TR_WRITE("\n"); }
#endif

static inline void* _tr_c_realloc(void* ptr, size_t size) {
    void* p = TAURARO_REALLOC(ptr, size);
    /* realloc(NULL, n) acts as malloc -> a new live block; realloc of an
     * existing block frees the old internally (no _tr_free) and keeps the
     * same logical block, so only the fresh-allocation case is counted. */
    if (!ptr && p) _TR_MEMCOUNT_INC();
    return p;
}
static inline void* _tr_checked_alloc(size_t sz) {
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
 * idiom in std/*.tr) - that free() must see a real malloc base pointer.
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

static inline void* _tr_c_memcpy(void* dst, void* src, size_t n) { return memcpy(dst, src, n); }
static inline void* _tr_c_memset(void* ptr, int val, size_t n) { return memset(ptr, val, n); }
static inline void* _tr_c_memmove(void* dst, void* src, size_t n) { return memmove(dst, src, n); }
/* File I/O + env: std-tier only (need <stdio.h>'s FILE / getenv). Gated so a
 * freestanding (TAURARO_BARE) build parses past here — leaving these ungated was
 * the 'FILE undeclared' early-header-failure that made everything after look
 * implicit on bare-metal. */
#ifndef TAURARO_BARE
static inline void* _tr_c_fopen(const char* path, const char* mode) { return (void*)fopen(path, mode); }
static inline int _tr_c_fclose(void* fp) { return fclose((FILE*)fp); }
static inline size_t _tr_c_fread(void* ptr, size_t size, size_t nmemb, void* fp) { return fread(ptr, size, nmemb, (FILE*)fp); }
static inline size_t _tr_c_fwrite(const void* ptr, size_t size, size_t nmemb, void* fp) { return fwrite(ptr, size, nmemb, (FILE*)fp); }
static inline int _tr_c_fseek(void* fp, long offset, int whence) { return fseek((FILE*)fp, offset, whence); }
static inline long _tr_c_ftell(void* fp) { return ftell((FILE*)fp); }
static inline char* _tr_getenv(const char* name) { char* v = getenv(name); return v ? v : ""; }
#else
static inline char* _tr_getenv(const char* name) { (void)name; return (char*)""; }
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
#ifdef TAURARO_BARE
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
#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
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

#ifndef _WIN32
/* Debug helper: prints current process memory usage to stderr, tagged with
 * `label`. Used to bisect memory growth across checkpoints during
 * leak-hunting; not called by normal runtime code. No-op on non-Windows. */
static inline void _tr_report_mem(const char* label) { (void)label; }
#endif

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

/* Debug helper: prints current process working-set size to stderr, tagged
 * with `label`. Used to bisect memory growth across checkpoints during
 * leak-hunting; not called by normal runtime code. */
static inline void _tr_report_mem(const char* label) {
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
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    _TrWin32StartArg* s = (_TrWin32StartArg*)malloc(sizeof(_TrWin32StartArg));
    s->fn = fn; s->arg = arg; s->result = NULL;
    SIZE_T ss = (SIZE_T)TAURARO_THREAD_STACK_SIZE;
    return CreateThread(NULL, ss, _tr_thread_start_trampoline, s, 0, NULL);
}
static void _tr_thread_detach(_TrThread t) { CloseHandle(t); }
static void _tr_thread_join_wait(_TrThread t) { WaitForSingleObject(t, INFINITE); CloseHandle(t); }

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
static void _tr_sleep_ms(long ms) { Sleep((DWORD)(ms < 0 ? 0 : ms)); }
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

#elif defined(TAURARO_BARE)
/* ── BARE/WASM: single-threaded primitive stubs ──────────────────────── */
typedef int _TrThread;
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) { fn(arg); return 0; }
static void _tr_thread_detach(_TrThread t)      { (void)t; }
static void _tr_thread_join_wait(_TrThread t)   { (void)t; }

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
static void _tr_sleep_ms(long ms) { (void)ms; }

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
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    _TrPosixStartArg* s = (_TrPosixStartArg*)malloc(sizeof(_TrPosixStartArg));
    s->fn = fn; s->arg = arg; s->result = NULL;
    pthread_attr_t attr; pthread_attr_init(&attr);
    if (TAURARO_THREAD_STACK_SIZE > 0)
        pthread_attr_setstacksize(&attr, (size_t)TAURARO_THREAD_STACK_SIZE);
    pthread_attr_setguardsize(&attr, 4096);  /* one-page guard against overflow */
    pthread_t t; pthread_create(&t, &attr, _tr_posix_thread_trampoline, s);
    pthread_attr_destroy(&attr); return t;
}
static void _tr_thread_detach(_TrThread t) { pthread_detach(t); }
static void _tr_thread_join_wait(_TrThread t) { pthread_join(t, NULL); }

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
static void _tr_sleep_ms(long ms) {
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
static _TrChan* _tr_chan_new(long long cap) {
    if (cap < 1) cap = 1;
    _TrChan* c = (_TrChan*)calloc(1, sizeof(_TrChan));
    c->buf = (long long*)TAURARO_CALLOC((size_t)cap, sizeof(long long)); c->cap = cap;
    InitializeCriticalSection(&c->mu);
    InitializeConditionVariable(&c->not_empty);
    InitializeConditionVariable(&c->not_full);
    return c;
}
static void _tr_chan_send(_TrChan* c, long long val) {
    EnterCriticalSection(&c->mu);
    while (c->count >= c->cap && !c->closed)
        SleepConditionVariableCS(&c->not_full, &c->mu, INFINITE);
    if (!c->closed) {
        c->buf[c->tail] = val; c->tail = (c->tail+1)%c->cap; c->count++;
        WakeConditionVariable(&c->not_empty);
    }
    LeaveCriticalSection(&c->mu);
}
static long long _tr_chan_recv(_TrChan* c) {
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
static bool _tr_chan_try_send(_TrChan* c, long long val) {
    EnterCriticalSection(&c->mu);
    bool ok = !c->closed && c->count < c->cap;
    if (ok) { c->buf[c->tail]=val; c->tail=(c->tail+1)%c->cap; c->count++; WakeConditionVariable(&c->not_empty); }
    LeaveCriticalSection(&c->mu); return ok;
}
static long long _tr_chan_try_recv_val(_TrChan* c) {
    EnterCriticalSection(&c->mu);
    long long v = LLONG_MIN;
    if (c->count > 0) { v=c->buf[c->head]; c->head=(c->head+1)%c->cap; c->count--; WakeConditionVariable(&c->not_full); }
    LeaveCriticalSection(&c->mu); return v;
}
static bool _tr_chan_send_timeout(_TrChan* c, long long val, long long ms) {
    EnterCriticalSection(&c->mu);
    ULONGLONG dl = GetTickCount64()+(ULONGLONG)ms; bool ok=true;
    while (c->count>=c->cap && !c->closed) {
        ULONGLONG now=GetTickCount64();
        if (now>=dl||!SleepConditionVariableCS(&c->not_full,&c->mu,(DWORD)(dl-now))){ok=false;break;}
    }
    if (ok&&!c->closed&&c->count<c->cap){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;WakeConditionVariable(&c->not_empty);}else ok=false;
    LeaveCriticalSection(&c->mu); return ok;
}
static long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms) {
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
static void _tr_chan_close(_TrChan* c) {
    EnterCriticalSection(&c->mu); c->closed=1;
    WakeAllConditionVariable(&c->not_empty); WakeAllConditionVariable(&c->not_full);
    LeaveCriticalSection(&c->mu);
}
static bool   _tr_chan_is_closed(_TrChan* c) { EnterCriticalSection(&c->mu); bool r=c->closed!=0; LeaveCriticalSection(&c->mu); return r; }
static long long _tr_chan_len(_TrChan* c)    { EnterCriticalSection(&c->mu); long long n=c->count; LeaveCriticalSection(&c->mu); return n; }
static long long _tr_chan_cap(_TrChan* c)    { return c?c->cap:0; }
static void   _tr_chan_free(_TrChan* c)      { if(!c)return; DeleteCriticalSection(&c->mu); _tr_free(c->buf); _tr_free(c); }
static long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
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
static inline _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)malloc(sizeof(_TrTLS));
    t->key = TlsAlloc();
    TlsSetValue(t->key, (LPVOID)(uintptr_t)(unsigned long long)init);
    return t;
}
static inline long long _tr_tls_get(_TrTLS* t) {
    return t ? (long long)(uintptr_t)TlsGetValue(t->key) : 0LL;
}
static inline void _tr_tls_set(_TrTLS* t, long long v) {
    if (t) TlsSetValue(t->key, (LPVOID)(uintptr_t)(unsigned long long)v);
}
static inline void _tr_tls_free(_TrTLS* t) { if (!t) return; TlsFree(t->key); free(t); }

#elif defined(TAURARO_BARE)
/* ═══════════════════════════════════════════════════════════════════════════
 * BARE/WASM: single-threaded async stubs — no locking, no blocking.
 * Channels are lock-free ring buffers; mutexes/semaphores are no-ops.
 * ═══════════════════════════════════════════════════════════════════════════*/

typedef struct {
    long long* buf; long long head, tail, count, cap; volatile int closed;
} _TrChan;
static _TrChan* _tr_chan_new(long long cap) {
    if (cap < 1) cap = 1;
    _TrChan* c = (_TrChan*)TAURARO_CALLOC(1, sizeof(_TrChan));
    c->buf = (long long*)TAURARO_CALLOC((size_t)cap, sizeof(long long)); c->cap = cap;
    return c;
}
static void _tr_chan_send(_TrChan* c, long long val) {
    if (!c || c->closed || c->count >= c->cap) return;
    c->buf[c->tail] = val; c->tail = (c->tail+1)%c->cap; c->count++;
}
static long long _tr_chan_recv(_TrChan* c) {
    if (!c || c->count == 0) return 0LL;
    long long v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
    return v;
}
static bool _tr_chan_try_send(_TrChan* c, long long val) {
    if (!c || c->closed || c->count >= c->cap) return false;
    c->buf[c->tail]=val; c->tail=(c->tail+1)%c->cap; c->count++; return true;
}
static long long _tr_chan_try_recv_val(_TrChan* c) {
    if (!c || c->count == 0) return LLONG_MIN;
    long long v=c->buf[c->head]; c->head=(c->head+1)%c->cap; c->count--; return v;
}
static bool  _tr_chan_send_timeout(_TrChan* c, long long val, long long ms)  { (void)ms; return _tr_chan_try_send(c, val); }
static long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms)         { (void)ms; return _tr_chan_try_recv_val(c); }
static void  _tr_chan_close(_TrChan* c)          { if (c) c->closed = 1; }
static bool  _tr_chan_is_closed(_TrChan* c)      { return c && c->closed; }
static long long _tr_chan_len(_TrChan* c)         { return c ? c->count : 0LL; }
static long long _tr_chan_cap(_TrChan* c)         { return c ? c->cap : 0LL; }
static void  _tr_chan_free(_TrChan* c)            { if (!c) return; TAURARO_FREE(c->buf); TAURARO_FREE(c); }
static long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
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
static inline _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)TAURARO_ALLOC(sizeof(_TrTLS)); t->val = init; return t;
}
static inline long long _tr_tls_get(_TrTLS* t) { return t ? t->val : 0LL; }
static inline void _tr_tls_set(_TrTLS* t, long long v) { if (t) t->val = v; }
static inline void _tr_tls_free(_TrTLS* t) { if (t) TAURARO_FREE(t); }

/* ── BARE ThreadPool: runs jobs synchronously (no OS threads) ─────────── */
typedef struct { int _dummy; } _TrThreadPool;
static inline long long _tr_threadpool_auto_n(void)  { return 1LL; }
static inline _TrThreadPool* _tr_threadpool_new(long long n)  { (void)n; return (_TrThreadPool*)TAURARO_CALLOC(1,sizeof(_TrThreadPool)); }
static inline _TrThreadPool* _tr_threadpool_auto(void)        { return _tr_threadpool_new(1LL); }
static inline void _tr_threadpool_spawn(_TrThreadPool* p, void*(*fn)(void*), void* arg) { (void)p; fn(arg); }
static inline void _tr_threadpool_wait(_TrThreadPool* p)      { (void)p; }
static inline void _tr_threadpool_free(_TrThreadPool* p)      { if(p)TAURARO_FREE(p); }

#else /* POSIX ─────────────────────────────────────────────────────────── */

typedef struct {
    long long* buf; long long head,tail,count,cap; volatile int closed;
    pthread_mutex_t mu; pthread_cond_t not_empty, not_full;
} _TrChan;
static _TrChan* _tr_chan_new(long long cap) {
    if(cap<1)cap=1; _TrChan* c=(_TrChan*)calloc(1,sizeof(_TrChan));
    c->buf=(long long*)TAURARO_CALLOC((size_t)cap,sizeof(long long)); c->cap=cap;
    pthread_mutex_init(&c->mu,NULL); pthread_cond_init(&c->not_empty,NULL); pthread_cond_init(&c->not_full,NULL); return c;
}
static void _tr_chan_send(_TrChan* c, long long val) {
    pthread_mutex_lock(&c->mu);
    while(c->count>=c->cap&&!c->closed) pthread_cond_wait(&c->not_full,&c->mu);
    if(!c->closed){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}
    pthread_mutex_unlock(&c->mu);
}
static long long _tr_chan_recv(_TrChan* c) {
    pthread_mutex_lock(&c->mu);
    while(c->count==0&&!c->closed) pthread_cond_wait(&c->not_empty,&c->mu);
    long long v=0;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
static bool _tr_chan_try_send(_TrChan* c, long long val) {
    pthread_mutex_lock(&c->mu); bool ok=!c->closed&&c->count<c->cap;
    if(ok){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}
    pthread_mutex_unlock(&c->mu); return ok;
}
static long long _tr_chan_try_recv_val(_TrChan* c) {
    pthread_mutex_lock(&c->mu); long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
static bool _tr_chan_send_timeout(_TrChan* c, long long val, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&c->mu); bool ok=true;
    while(c->count>=c->cap&&!c->closed) if(pthread_cond_timedwait(&c->not_full,&c->mu,&ts)){ok=false;break;}
    if(ok&&!c->closed&&c->count<c->cap){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}else ok=false;
    pthread_mutex_unlock(&c->mu); return ok;
}
static long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&c->mu);
    while(c->count==0&&!c->closed) if(pthread_cond_timedwait(&c->not_empty,&c->mu,&ts)){pthread_mutex_unlock(&c->mu);return LLONG_MIN;}
    long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
static void _tr_chan_close(_TrChan* c) {
    pthread_mutex_lock(&c->mu); c->closed=1;
    pthread_cond_broadcast(&c->not_empty); pthread_cond_broadcast(&c->not_full); pthread_mutex_unlock(&c->mu);
}
static bool   _tr_chan_is_closed(_TrChan* c) { pthread_mutex_lock(&c->mu); bool r=c->closed!=0; pthread_mutex_unlock(&c->mu); return r; }
static long long _tr_chan_len(_TrChan* c)    { pthread_mutex_lock(&c->mu); long long n=c->count; pthread_mutex_unlock(&c->mu); return n; }
static long long _tr_chan_cap(_TrChan* c)    { return c?c->cap:0; }
static void   _tr_chan_free(_TrChan* c)      { if(!c)return; pthread_mutex_destroy(&c->mu); pthread_cond_destroy(&c->not_empty); pthread_cond_destroy(&c->not_full); _tr_free(c->buf); _tr_free(c); }
static long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
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
static inline _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)malloc(sizeof(_TrTLS));
    pthread_key_create(&t->key, NULL);
    pthread_setspecific(t->key, (void*)(uintptr_t)(unsigned long long)init);
    return t;
}
static inline long long _tr_tls_get(_TrTLS* t) {
    return t ? (long long)(uintptr_t)pthread_getspecific(t->key) : 0LL;
}
static inline void _tr_tls_set(_TrTLS* t, long long v) {
    if (t) pthread_setspecific(t->key, (void*)(uintptr_t)(unsigned long long)v);
}
static inline void _tr_tls_free(_TrTLS* t) {
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
typedef struct { _TrMutexH* mu; long long data; _Atomic int rc; } _TrMutexBox;
static inline _TrMutexBox* _tr_mutexbox_new(long long init) {
    _TrMutexBox* b = (_TrMutexBox*)TAURARO_ALLOC(sizeof(_TrMutexBox));
    b->mu = _tr_mutex_new(); b->data = init;
    atomic_store(&b->rc, 1); return b;
}
static inline long long _tr_mutexbox_lock_get(_TrMutexBox* b) {
    _tr_mutex_hlock(b->mu); _tr_lstack_push(&_tr_tl_mu_stk, b); return b->data;
}
static inline void _tr_mutexbox_set_unlock(_TrMutexBox* b, long long v) {
    b->data = v; _tr_lstack_pop(&_tr_tl_mu_stk, b); _tr_mutex_hunlock(b->mu);
}
static inline void _tr_mutexbox_unlock(_TrMutexBox* b) { _tr_lstack_pop(&_tr_tl_mu_stk, b); _tr_mutex_hunlock(b->mu); }
static inline void _tr_mutexbox_free(_TrMutexBox* b) {
    if (!b) return; _tr_mutex_hfree(b->mu); TAURARO_FREE(b);
}
static inline _TrMutexBox* _tr_mutexbox_clone(_TrMutexBox* b) {
    if (b) atomic_fetch_add(&b->rc, 1); return b;
}
static inline void _tr_mutexbox_drop(_TrMutexBox* b) {
    if (!b || atomic_fetch_sub(&b->rc, 1) > 1) return; _tr_mutexbox_free(b);
}
/* Auto-unlock cleanup — used by __attribute__((cleanup)) RAII guard in codegen.
 * Fires when the guard variable goes out of scope. No-op if already unlocked
 * (set_unlock/unlock already popped the box from the TLS stack). */
static inline void _tr_mutexbox_cleanup(_TrMutexBox** bp) {
    if (bp && *bp && _tr_lstack_pop(&_tr_tl_mu_stk, *bp)) _tr_mutex_hunlock((*bp)->mu);
}

/* ── RwLockBox<T>: reader-writer guarded single value ────────────────── */
typedef struct { _TrRWL* rw; long long data; _Atomic int rc; } _TrRWLBox;
static inline _TrRWLBox* _tr_rwlbox_new(long long init) {
    _TrRWLBox* b = (_TrRWLBox*)TAURARO_ALLOC(sizeof(_TrRWLBox));
    b->rw = _tr_rwl_new(); b->data = init;
    atomic_store(&b->rc, 1); return b;
}
static inline long long _tr_rwlbox_read_get(_TrRWLBox* b) {
    _tr_rwl_read_lock(b->rw); _tr_lstack_push(&_tr_tl_rwl_r_stk, b); return b->data;
}
static inline void _tr_rwlbox_read_unlock(_TrRWLBox* b) { _tr_lstack_pop(&_tr_tl_rwl_r_stk, b); _tr_rwl_read_unlock(b->rw); }
static inline long long _tr_rwlbox_write_get(_TrRWLBox* b) {
    _tr_rwl_write_lock(b->rw); _tr_lstack_push(&_tr_tl_rwl_w_stk, b); return b->data;
}
static inline void _tr_rwlbox_write_set_unlock(_TrRWLBox* b, long long v) {
    b->data = v; _tr_lstack_pop(&_tr_tl_rwl_w_stk, b); _tr_rwl_write_unlock(b->rw);
}
static inline void _tr_rwlbox_free(_TrRWLBox* b) {
    if (!b) return; _tr_rwl_free(b->rw); TAURARO_FREE(b);
}
static inline _TrRWLBox* _tr_rwlbox_clone(_TrRWLBox* b) {
    if (b) atomic_fetch_add(&b->rc, 1); return b;
}
static inline void _tr_rwlbox_drop(_TrRWLBox* b) {
    if (!b || atomic_fetch_sub(&b->rc, 1) > 1) return; _tr_rwlbox_free(b);
}
/* Auto-unlock cleanup for read/write guards. */
static inline void _tr_rwlbox_cleanup_r(_TrRWLBox** bp) {
    if (bp && *bp && _tr_lstack_pop(&_tr_tl_rwl_r_stk, *bp)) _tr_rwl_read_unlock((*bp)->rw);
}
static inline void _tr_rwlbox_cleanup_w(_TrRWLBox** bp) {
    if (bp && *bp && _tr_lstack_pop(&_tr_tl_rwl_w_stk, *bp)) _tr_rwl_write_unlock((*bp)->rw);
}

/* ── ThreadPool: fixed-N worker pool with a channel work queue ────────── */
/* BARE stub is defined inside the BARE platform block above. */
#ifndef TAURARO_BARE
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
static inline long long _tr_threadpool_auto_n(void) {
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
static inline _TrThreadPool* _tr_threadpool_new(long long n) {
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
static inline _TrThreadPool* _tr_threadpool_auto(void) {
    return _tr_threadpool_new(_tr_threadpool_auto_n());
}
static inline void _tr_threadpool_spawn(_TrThreadPool* p, void*(*fn)(void*), void* arg) {
    _TrPoolItem* item = (_TrPoolItem*)TAURARO_ALLOC(sizeof(_TrPoolItem));
    item->fn = fn; item->arg = arg;
    _tr_wg_add(p->wg, 1);
    /* uintptr_t cast: safe on 32-bit and 64-bit; avoids sign-extension of intptr_t */
    _tr_chan_send(p->queue, (long long)(uintptr_t)(void*)item);
}
static inline void _tr_threadpool_wait(_TrThreadPool* p) { _tr_wg_wait(p->wg); }
static inline void _tr_threadpool_free(_TrThreadPool* p) {
    if (!p) return;
    _tr_chan_close(p->queue);
    for (int i = 0; i < p->n_workers; i++) _tr_thread_join_wait(p->workers[i]);
    _tr_chan_free(p->queue); _tr_wg_free(p->wg);
    TAURARO_FREE(p->workers); TAURARO_FREE(p);
}
#endif /* !TAURARO_BARE */

/* Global async pool — submits work to the thread pool; falls back to sync if pool is NULL */
static inline void _tr_async_pool_submit(_TrThreadPool* p, void*(*fn)(void*), void* arg) {
    if (p) _tr_threadpool_spawn(p, fn, arg);
    else fn(arg); /* synchronous fallback (BARE / pre-init) */
}

/* ── Atomic[T]: lock-free atomic integer (C11 _Atomic) ───────────────── */
typedef struct { _Atomic long long val; } _TrAtomic;
static inline _TrAtomic* _tr_atomic_new(long long init) {
    _TrAtomic* a = (_TrAtomic*)TAURARO_ALLOC(sizeof(_TrAtomic));
    atomic_init(&a->val, init); return a;
}
/* Hot-path ops: null-check removed — codegen never emits NULL _TrAtomic* */
static inline long long _tr_atomic_load(_TrAtomic* a)               { return atomic_load(&a->val); }
static inline void      _tr_atomic_store(_TrAtomic* a, long long v)  { atomic_store(&a->val, v); }
static inline long long _tr_atomic_add(_TrAtomic* a, long long v)    { return atomic_fetch_add(&a->val, v); }
static inline long long _tr_atomic_sub(_TrAtomic* a, long long v)    { return atomic_fetch_sub(&a->val, v); }
static inline long long _tr_atomic_swap(_TrAtomic* a, long long v)   { return atomic_exchange(&a->val, v); }
static inline bool _tr_atomic_cas(_TrAtomic* a, long long expected, long long desired) {
    return atomic_compare_exchange_strong(&a->val, &expected, desired);
}
static inline void _tr_atomic_free(_TrAtomic* a) { if (a) TAURARO_FREE(a); }

/* Atomic[T]: explicit memory-order variants (C11 stdatomic) */
static inline long long _tr_atomic_load_relaxed(_TrAtomic* a) { return atomic_load_explicit(&a->val, memory_order_relaxed); }
static inline long long _tr_atomic_load_acquire(_TrAtomic* a) { return atomic_load_explicit(&a->val, memory_order_acquire); }
static inline long long _tr_atomic_load_seqcst(_TrAtomic* a)  { return atomic_load_explicit(&a->val, memory_order_seq_cst); }
static inline void _tr_atomic_store_relaxed(_TrAtomic* a, long long v) { atomic_store_explicit(&a->val, v, memory_order_relaxed); }
static inline void _tr_atomic_store_release(_TrAtomic* a, long long v) { atomic_store_explicit(&a->val, v, memory_order_release); }
static inline void _tr_atomic_store_seqcst(_TrAtomic* a, long long v)  { atomic_store_explicit(&a->val, v, memory_order_seq_cst); }
static inline long long _tr_atomic_add_relaxed(_TrAtomic* a, long long v) { return atomic_fetch_add_explicit(&a->val, v, memory_order_relaxed); }
static inline long long _tr_atomic_add_release(_TrAtomic* a, long long v) { return atomic_fetch_add_explicit(&a->val, v, memory_order_release); }
static inline long long _tr_atomic_add_acqrel(_TrAtomic* a, long long v)  { return atomic_fetch_add_explicit(&a->val, v, memory_order_acq_rel); }
static inline long long _tr_atomic_sub_relaxed(_TrAtomic* a, long long v) { return atomic_fetch_sub_explicit(&a->val, v, memory_order_relaxed); }
static inline long long _tr_atomic_sub_release(_TrAtomic* a, long long v) { return atomic_fetch_sub_explicit(&a->val, v, memory_order_release); }
static inline bool _tr_atomic_cas_weak(_TrAtomic* a, long long exp, long long des)   { return atomic_compare_exchange_weak(&a->val, &exp, des); }
static inline bool _tr_atomic_cas_acqrel(_TrAtomic* a, long long exp, long long des) {
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
#elif !defined(TAURARO_BARE)
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

static inline _TrThreadObj* _tr_threadobj_spawn(void*(*fn)(void*), void* arg) {
    _TrThreadObj* t = (_TrThreadObj*)TAURARO_CALLOC(1, sizeof(_TrThreadObj));
    t->result.panicked = 0; t->result.panic_msg = NULL;
    t->handle = _tr_thread_start_result(fn, arg, &t->result);
    return t;
}
static inline void _tr_threadobj_join(_TrThreadObj* t) {
    if (!t || t->done) return; t->done = 1; _tr_thread_join_wait(t->handle);
}
/* Re-raise the thread's panic in the calling thread after join */
static inline bool _tr_threadobj_panicked(_TrThreadObj* t) {
    return t && t->result.panicked;
}
static inline char* _tr_threadobj_panic_msg(_TrThreadObj* t) {
    return (t && t->result.panic_msg) ? t->result.panic_msg : "";
}
static inline void _tr_threadobj_detach(_TrThreadObj* t) {
    if (!t || t->done) return; t->done = 1; _tr_thread_detach(t->handle);
}
static inline void _tr_threadobj_free(_TrThreadObj* t) { if (t) TAURARO_FREE(t); }

/* ── Thread utilities: current-thread ID and sleep ───────────────────── */
#ifdef _WIN32
static inline long long _tr_thread_current_id(void) { return (long long)(uintptr_t)GetCurrentThreadId(); }
#elif defined(TAURARO_BARE)
static inline long long _tr_thread_current_id(void) { return 0LL; }
#else
static inline long long _tr_thread_current_id(void) { return (long long)(uintptr_t)pthread_self(); }
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
static inline char* _tr_chan_new_h(long long cap)                              { return (char*)_tr_chan_new(cap); }
static inline void  _tr_chan_send_h(char* c, long long v)                      { _tr_chan_send((_TrChan*)c, v); }
static inline long long _tr_chan_recv_h(char* c)                               { return _tr_chan_recv((_TrChan*)c); }
static inline bool  _tr_chan_try_send_h(char* c, long long v)                  { return _tr_chan_try_send((_TrChan*)c, v); }
static inline long long _tr_chan_try_recv_val_h(char* c)                       { return _tr_chan_try_recv_val((_TrChan*)c); }
static inline bool  _tr_chan_send_timeout_h(char* c, long long v, long long ms){ return _tr_chan_send_timeout((_TrChan*)c, v, ms); }
static inline long long _tr_chan_recv_timeout_val_h(char* c, long long ms)     { return _tr_chan_recv_timeout_val((_TrChan*)c, ms); }
static inline void  _tr_chan_close_h(char* c)                                  { _tr_chan_close((_TrChan*)c); }
static inline bool  _tr_chan_is_closed_h(char* c)                              { return _tr_chan_is_closed((_TrChan*)c); }
static inline long long _tr_chan_len_h(char* c)                                { return _tr_chan_len((_TrChan*)c); }
static inline long long _tr_chan_cap_h(char* c)                                { return _tr_chan_cap((_TrChan*)c); }
static inline void  _tr_chan_free_h(char* c)                                   { _tr_chan_free((_TrChan*)c); }

/* Task / Future */
static inline char* _tr_task_new_h(void)                                       { return (char*)_tr_task_new(); }
static inline void  _tr_task_complete_h(char* t, long long r)                  { _tr_task_complete((_TrTaskState*)t, r); }
static inline void  _tr_task_complete_err_h(char* t, char* msg)                { _tr_task_complete_err((_TrTaskState*)t, msg); }
static inline void  _tr_task_cancel_h(char* t)                                 { _tr_task_cancel((_TrTaskState*)t); }
static inline long long _tr_task_await_h(char* t)                              { return _tr_task_await((_TrTaskState*)t); }
static inline bool  _tr_task_await_timeout_h(char* t, long long ms)            { return _tr_task_await_timeout_ok((_TrTaskState*)t, ms); }
static inline bool  _tr_task_is_done_h(char* t)                                { return _tr_task_is_done((_TrTaskState*)t); }
static inline bool  _tr_task_is_cancelled_h(char* t)                           { return _tr_task_is_cancelled((_TrTaskState*)t); }
static inline bool  _tr_task_has_error_h(char* t)                              { return _tr_task_has_error((_TrTaskState*)t); }
static inline char* _tr_task_get_error_h(char* t)                              { return _tr_task_get_error((_TrTaskState*)t); }
static inline void  _tr_task_free_h(char* t)                                   { _tr_task_free((_TrTaskState*)t); }

/* Mutex / RWLock */
static inline char* _tr_mutex_new_h(void)                                      { return (char*)_tr_mutex_new(); }
static inline void  _tr_mutex_lock_h(char* m)                                  { _tr_mutex_hlock((_TrMutexH*)m); }
static inline void  _tr_mutex_unlock_h(char* m)                                { _tr_mutex_hunlock((_TrMutexH*)m); }
static inline bool  _tr_mutex_trylock_h(char* m)                               { return _tr_mutex_htrylock((_TrMutexH*)m); }
static inline void  _tr_mutex_free_h(char* m)                                  { _tr_mutex_hfree((_TrMutexH*)m); }
static inline char* _tr_rwl_new_h(void)                                        { return (char*)_tr_rwl_new(); }
static inline void  _tr_rwl_read_lock_h(char* r)                               { _tr_rwl_read_lock((_TrRWL*)r); }
static inline void  _tr_rwl_read_unlock_h(char* r)                             { _tr_rwl_read_unlock((_TrRWL*)r); }
static inline void  _tr_rwl_write_lock_h(char* r)                              { _tr_rwl_write_lock((_TrRWL*)r); }
static inline void  _tr_rwl_write_unlock_h(char* r)                            { _tr_rwl_write_unlock((_TrRWL*)r); }
static inline void  _tr_rwl_free_h(char* r)                                    { _tr_rwl_free((_TrRWL*)r); }

/* Semaphore */
static inline char* _tr_sema_new_h(long long init, long long maxv)             { return (char*)_tr_sema_new(init, maxv); }
static inline void  _tr_sema_acquire_h(char* s)                                { _tr_sema_acquire((_TrSema*)s); }
static inline bool  _tr_sema_try_acquire_h(char* s)                            { return _tr_sema_try_acquire((_TrSema*)s); }
static inline bool  _tr_sema_acquire_timeout_h(char* s, long long ms)          { return _tr_sema_acquire_timeout((_TrSema*)s, ms); }
static inline void  _tr_sema_release_h(char* s)                                { _tr_sema_release((_TrSema*)s); }
static inline void  _tr_sema_free_h(char* s)                                   { _tr_sema_free((_TrSema*)s); }

/* WaitGroup */
static inline char* _tr_wg_new_h(void)                                         { return (char*)_tr_wg_new(); }
static inline void  _tr_wg_add_h(char* w, long long n)                         { _tr_wg_add((_TrWG*)w, n); }
static inline void  _tr_wg_done_h(char* w)                                     { _tr_wg_done((_TrWG*)w); }
static inline void  _tr_wg_wait_h(char* w)                                     { _tr_wg_wait((_TrWG*)w); }
static inline bool  _tr_wg_wait_timeout_h(char* w, long long ms)               { return _tr_wg_wait_timeout((_TrWG*)w, ms); }
static inline void  _tr_wg_free_h(char* w)                                     { _tr_wg_free((_TrWG*)w); }

/* Barrier */
static inline char* _tr_barrier_new_h(long long n)                             { return (char*)_tr_barrier_new(n); }
static inline void  _tr_barrier_wait_h(char* b)                                { _tr_barrier_wait((_TrBarrier*)b); }
static inline void  _tr_barrier_free_h(char* b)                                { _tr_barrier_free((_TrBarrier*)b); }

/* Once */
static inline char* _tr_once_new_h(void)                                       { return (char*)_tr_once_new(); }
static inline bool  _tr_once_do_h(char* o)                                     { return _tr_once_do((_TrOnce*)o); }
static inline void  _tr_once_free_h(char* o)                                   { _tr_once_free((_TrOnce*)o); }

/* Timer / Ticker */
static inline char* _tr_timer_new_h(long long ms, char* ch)                    { return (char*)_tr_timer_new(ms, (_TrChan*)ch); }
static inline char* _tr_ticker_new_h(long long ms, char* ch)                   { return (char*)_tr_ticker_new(ms, (_TrChan*)ch); }
static inline void  _tr_timer_stop_h(char* s)                                  { _tr_timer_stop((_TrTimerState*)s); }

/* Thread object (joinable handle) */
typedef void*(*_TrThreadFn)(void*);
static inline char* _tr_threadobj_spawn_h(char* fn, char* arg)                 { return (char*)_tr_threadobj_spawn((_TrThreadFn)(uintptr_t)fn, (void*)arg); }
static inline void  _tr_threadobj_join_h(char* t)                              { _tr_threadobj_join((_TrThreadObj*)t); }
static inline void  _tr_threadobj_detach_h(char* t)                            { _tr_threadobj_detach((_TrThreadObj*)t); }
static inline void  _tr_threadobj_free_h(char* t)                              { _tr_threadobj_free((_TrThreadObj*)t); }
static inline bool  _tr_threadobj_panicked_h(char* t)                          { return _tr_threadobj_panicked((_TrThreadObj*)t); }
static inline char* _tr_threadobj_panic_msg_h(char* t)                         { return _tr_str_dup_owned(_tr_threadobj_panic_msg((_TrThreadObj*)t)); }
static inline long long _tr_thread_current_id_h(void)                          { return _tr_thread_current_id(); }
static inline void  _tr_thread_sleep_ms_h(long long ms)                        { _tr_thread_sleep_ms(ms); }

/* Atomic[T]: lock-free integer */
static inline char* _tr_atomic_new_h(long long init)                           { return (char*)_tr_atomic_new(init); }
static inline long long _tr_atomic_load_h(char* a)                             { return _tr_atomic_load((_TrAtomic*)a); }
static inline void  _tr_atomic_store_h(char* a, long long v)                   { _tr_atomic_store((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_h(char* a, long long v)                 { return _tr_atomic_add((_TrAtomic*)a, v); }
static inline long long _tr_atomic_sub_h(char* a, long long v)                 { return _tr_atomic_sub((_TrAtomic*)a, v); }
static inline long long _tr_atomic_swap_h(char* a, long long v)                { return _tr_atomic_swap((_TrAtomic*)a, v); }
static inline bool  _tr_atomic_cas_h(char* a, long long expected, long long desired) { return _tr_atomic_cas((_TrAtomic*)a, expected, desired); }
static inline void  _tr_atomic_free_h(char* a)                                 { _tr_atomic_free((_TrAtomic*)a); }
static inline long long _tr_atomic_load_relaxed_h(char* a)                     { return _tr_atomic_load_relaxed((_TrAtomic*)a); }
static inline long long _tr_atomic_load_acquire_h(char* a)                     { return _tr_atomic_load_acquire((_TrAtomic*)a); }
static inline long long _tr_atomic_load_seqcst_h(char* a)                      { return _tr_atomic_load_seqcst((_TrAtomic*)a); }
static inline void  _tr_atomic_store_relaxed_h(char* a, long long v)           { _tr_atomic_store_relaxed((_TrAtomic*)a, v); }
static inline void  _tr_atomic_store_release_h(char* a, long long v)           { _tr_atomic_store_release((_TrAtomic*)a, v); }
static inline void  _tr_atomic_store_seqcst_h(char* a, long long v)            { _tr_atomic_store_seqcst((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_relaxed_h(char* a, long long v)         { return _tr_atomic_add_relaxed((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_release_h(char* a, long long v)         { return _tr_atomic_add_release((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_acqrel_h(char* a, long long v)          { return _tr_atomic_add_acqrel((_TrAtomic*)a, v); }
static inline long long _tr_atomic_sub_relaxed_h(char* a, long long v)         { return _tr_atomic_sub_relaxed((_TrAtomic*)a, v); }
static inline long long _tr_atomic_sub_release_h(char* a, long long v)         { return _tr_atomic_sub_release((_TrAtomic*)a, v); }
static inline bool  _tr_atomic_cas_weak_h(char* a, long long exp, long long des)   { return _tr_atomic_cas_weak((_TrAtomic*)a, exp, des); }
static inline bool  _tr_atomic_cas_acqrel_h(char* a, long long exp, long long des) { return _tr_atomic_cas_acqrel((_TrAtomic*)a, exp, des); }

/* ThreadLocal[T]: per-thread storage */
static inline char* _tr_tls_new_h(long long init)                              { return (char*)_tr_tls_new(init); }
static inline long long _tr_tls_get_h(char* t)                                 { return _tr_tls_get((_TrTLS*)t); }
static inline void  _tr_tls_set_h(char* t, long long v)                        { _tr_tls_set((_TrTLS*)t, v); }
static inline void  _tr_tls_free_h(char* t)                                    { _tr_tls_free((_TrTLS*)t); }

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
#ifdef _WIN32
#  ifndef _INC_IO
#    include <io.h>     /* _isatty, _fileno on Windows */
#  endif
#else
#  ifndef _UNISTD_H
#    include <unistd.h> /* isatty, STDIN_FILENO on Unix */
#  endif
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
static int64_t _tr_env_set(const char* name) {
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
static int64_t _tr_env_set(const char* name) { (void)name; return 0; }
#endif

/* The ESC control byte (0x1b) as an owned string. Lets the diagnostics module
 * build ANSI sequences without depending on core.string (StringBuilder).     */
static char* _tr_ansi_esc(void) { return _tr_str_dup_owned("\x1b"); }


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

static inline void _tr_exit(long long code) { exit((int)code); }

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

#include <time.h>
static inline long long _tr_timestamp(void) { return (long long)time(NULL); }

/* High-resolution millisecond wall-clock: QueryPerformanceCounter on Windows,
   CLOCK_MONOTONIC on POSIX.  Used by std.sys.time.time_ms / elapsed_ms. */
static inline long long _tr_time_ms(void) {
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
static int64_t _tr_stdout_supports_ansi(void) {
#if defined(TAURARO_BARE)
    return 0;   /* no console on bare-metal */
#elif defined(_WIN32)
    if (!_isatty(_fileno(stdout))) return 0;
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
static inline _TrIOPoll* _tr_iopoll_create(void) {
    return (_TrIOPoll*)TAURARO_CALLOC(1, sizeof(_TrIOPoll));
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) { if (p) TAURARO_FREE(p); }
static inline int  _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { (void)p;(void)fd;(void)ev;(void)ud; return 0; }
static inline int  _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { (void)p;(void)fd;(void)ev;(void)ud; return 0; }
static inline int  _tr_iopoll_del(_TrIOPoll* p, int fd)
    { (void)p;(void)fd; return 0; }
static inline int  _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* ev, int maxev, int timeout_ms)
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
static inline _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    if (!p) return NULL;
    p->cap = 64;
    p->pfds = (WSAPOLLFD*)TAURARO_CALLOC((size_t)p->cap, sizeof(WSAPOLLFD));
    p->userdata = (void**)TAURARO_CALLOC((size_t)p->cap, sizeof(void*));
    if (!p->pfds || !p->userdata) { _tr_free(p->pfds); _tr_free(p->userdata); _tr_free(p); return NULL; }
    return p;
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (p) { _tr_free(p->pfds); _tr_free(p->userdata); _tr_free(p); }
}
static inline SHORT _tr_poll_events(uint32_t ev) {
    SHORT e = 0;
    if (ev & TAURARO_POLLIN)  e |= POLLRDNORM;
    if (ev & TAURARO_POLLOUT) e |= POLLWRNORM;
    return e;
}
static inline int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
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
static inline int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { return _tr_iopoll_add(p, fd, ev, ud); }
static inline int _tr_iopoll_del(_TrIOPoll* p, int fd) {
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
static inline int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
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
static inline _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    p->epfd = epoll_create1(EPOLL_CLOEXEC);
    return p;
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (!p) return; if (p->epfd >= 0) close(p->epfd); free(p);
}
static inline int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct epoll_event e = {0};
    if (ev & TAURARO_POLLIN)  e.events |= EPOLLIN;
    if (ev & TAURARO_POLLOUT) e.events |= EPOLLOUT;
    e.data.ptr = ud;
    return epoll_ctl(p->epfd, EPOLL_CTL_ADD, fd, &e);
}
static inline int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct epoll_event e = {0};
    if (ev & TAURARO_POLLIN)  e.events |= EPOLLIN;
    if (ev & TAURARO_POLLOUT) e.events |= EPOLLOUT;
    e.data.ptr = ud;
    return epoll_ctl(p->epfd, EPOLL_CTL_MOD, fd, &e);
}
static inline int _tr_iopoll_del(_TrIOPoll* p, int fd) {
    if (!p) return -1;
    return epoll_ctl(p->epfd, EPOLL_CTL_DEL, fd, NULL);
}
static inline int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
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
static inline _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    p->kqfd = kqueue(); return p;
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (!p) return; if (p->kqfd >= 0) close(p->kqfd); free(p);
}
static inline int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct kevent changes[2]; int n = 0;
    if (ev & TAURARO_POLLIN)
        EV_SET(&changes[n++], (uintptr_t)fd, EVFILT_READ,  EV_ADD|EV_ENABLE, 0, 0, ud);
    if (ev & TAURARO_POLLOUT)
        EV_SET(&changes[n++], (uintptr_t)fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, ud);
    return kevent(p->kqfd, changes, n, NULL, 0, NULL);
}
static inline int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { return _tr_iopoll_add(p, fd, ev, ud); }
static inline int _tr_iopoll_del(_TrIOPoll* p, int fd) {
    if (!p) return -1;
    struct kevent changes[2];
    EV_SET(&changes[0], (uintptr_t)fd, EVFILT_READ,  EV_DELETE, 0, 0, NULL);
    EV_SET(&changes[1], (uintptr_t)fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    return kevent(p->kqfd, changes, 2, NULL, 0, NULL);
}
static inline int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
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
static inline _TrIOPoll* _tr_iopoll_create(void) { return (_TrIOPoll*)calloc(1,sizeof(_TrIOPoll)); }
static inline void _tr_iopoll_destroy(_TrIOPoll* p) { if(p) free(p); }
static inline int  _tr_iopoll_add(_TrIOPoll* p,int fd,uint32_t ev,void* ud){(void)p;(void)fd;(void)ev;(void)ud;return -1;}
static inline int  _tr_iopoll_mod(_TrIOPoll* p,int fd,uint32_t ev,void* ud){(void)p;(void)fd;(void)ev;(void)ud;return -1;}
static inline int  _tr_iopoll_del(_TrIOPoll* p,int fd){(void)p;(void)fd;return -1;}
static inline int  _tr_iopoll_wait(_TrIOPoll* p,_TrIOEvent* ev,int m,int t){(void)p;(void)ev;(void)m;(void)t;return 0;}
#endif /* _TrIOPoll platform backends */

/* _tr_iopoll_wait_raw: Tauraro-callable version.
 * out_buf is a caller-allocated byte array; each slot is sizeof(_TrIOEvent).
 * Returns number of events written.  Tauraro code reads fd/events/userdata
 * at offsets 0/4/8 within each 16-byte slot. */
static inline int _tr_iopoll_wait_raw(char* p_raw, char* out_buf, int maxev, int timeout_ms) {
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
static inline char* _tr_iopoll_create_h(void)
    { return (char*)_tr_iopoll_create(); }
static inline void  _tr_iopoll_destroy_h(char* p)
    { _tr_iopoll_destroy((_TrIOPoll*)p); }
static inline int   _tr_iopoll_add_h(char* p, long long fd, long long ev, long long ud)
    { return _tr_iopoll_add((_TrIOPoll*)p,(int)fd,(uint32_t)ev,(void*)(uintptr_t)(unsigned long long)ud); }
static inline int   _tr_iopoll_mod_h(char* p, long long fd, long long ev, long long ud)
    { return _tr_iopoll_mod((_TrIOPoll*)p,(int)fd,(uint32_t)ev,(void*)(uintptr_t)(unsigned long long)ud); }
static inline int   _tr_iopoll_del_h(char* p, long long fd)
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

/* Await another coroutine's completion and return its result. Works both
 * inside a coro (cooperative suspend) and from the top level (pumps the
 * scheduler until the target finishes). */
static long long _tr_co_await(_TrCoro* target) {
    if (!target) return 0;
    if (_tr_g.current) {
        if (target->state != _TRC_DONE) {
            target->joiner = _tr_g.current;
            _tr_g.current->state = _TRC_SUSP;
            _tr_co_to_sched(_tr_g.current);
        }
        return target->result;
    }
    while (target->state != _TRC_DONE) {
        if (!_tr_sched_step()) break;
    }
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
    while (target->state != _TRC_DONE) {
        if (_tr_mono_ms() >= deadline) { if (out) *out = 0; return 0; }
        if (!_tr_sched_step()) break;
    }
    if (out) *out = target->result;
    return target->state == _TRC_DONE;
}

/* Tauraro-callable handle-based wrappers - extern "C" decls in std/async. */
static char*     _tr_co_go_h(void* fn, void* arg) { return (char*)_tr_co_go((_tr_coro_fn)fn, arg); }
static void       _tr_co_spawn_h(void* fn, void* arg) { _tr_co_spawn((_tr_coro_fn)fn, arg); }
static long long  _tr_co_await_h(char* c)          { return _tr_co_await((_TrCoro*)c); }
static void       _tr_co_free_h(char* c)           { _tr_co_free((_TrCoro*)c); }
static void       _tr_co_yield_h(void)             { _tr_co_yield(); }
static void       _tr_co_sleep_h(long long ms)     { _tr_co_sleep_ms(ms); }
static int        _tr_co_await_fd_h(long long fd, long long ev) { return _tr_co_await_fd((int)fd, (unsigned int)ev); }
static void       _tr_co_run_h(void)               { _tr_sched_run(); }
static int        _tr_co_done_h(char* c)           { return _tr_co_done((_TrCoro*)c); }

#endif /* green-thread scheduler */

/* ── TCP socket helpers ─────────────────────────────────────────────── */
#if defined(TAURARO_BARE) || defined(TAURARO_WASM)
/* No networking on bare WASM or freestanding targets */
static inline int _tr_net_init(void)                                              { return -1; }
static inline int _tr_tcp_connect(const char* h, int p)                           { (void)h;(void)p; return -1; }
static inline int _tr_tcp_send(int fd, const char* d, int l)                      { (void)fd;(void)d;(void)l; return -1; }
static inline int _tr_tcp_recv(int fd, char* b, int c)                            { (void)fd;(void)b;(void)c; return -1; }
static inline void _tr_tcp_close(int fd)                                           { (void)fd; }
static inline int _tr_tcp_listen(const char* h, int p, int bl)                    { (void)h;(void)p;(void)bl; return -1; }
static inline int _tr_tcp_accept(int s)                                            { (void)s; return -1; }
static inline char* _tr_tcp_peer_addr(int fd)                                      { (void)fd; return (char*)""; }
static inline int _tr_udp_socket(void)                                             { return -1; }
static inline int _tr_udp_bind(int fd, int p)                                      { (void)fd;(void)p; return -1; }
static inline int _tr_udp_send_to(int fd, const char* d, int l, const char* h, int p) { (void)fd;(void)d;(void)l;(void)h;(void)p; return -1; }
static inline int _tr_udp_recv_from(int fd, char* b, int c, char* src)            { (void)fd;(void)b;(void)c;(void)src; return -1; }
static inline void _tr_udp_close(int fd)                                           { (void)fd; }
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
static inline int _tr_tcp_connect(const char* host, int port) {
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
static inline int  _tr_tcp_send(int fd, const char* data, int len) { return send((SOCKET)fd, data, len, 0); }
static inline int  _tr_tcp_recv(int fd, char* buf, int cap)        { return recv((SOCKET)fd, buf, cap, 0); }
static inline void _tr_tcp_close(int fd)                           { closesocket((SOCKET)fd); }

#else  /* POSIX */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

static inline int _tr_net_init(void) { return 0; }
static inline int _tr_tcp_connect(const char* host, int port) {
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
static inline int  _tr_tcp_send(int fd, const char* data, int len) { return (int)send(fd, data, (size_t)len, 0); }
static inline int  _tr_tcp_recv(int fd, char* buf, int cap)        { return (int)recv(fd, buf, (size_t)cap, 0); }
static inline void _tr_tcp_close(int fd)                           { close(fd); }
#endif

/* ── Platform detection ──────────────────────────────────────────────── */
static inline bool _tr_is_windows(void) {
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
static inline bool  _tr_dir_exists(const char* p){ (void)p; return false; }
static inline bool  _tr_is_dir(const char* p)    { (void)p; return false; }
static inline bool  _tr_is_file(const char* p)   { (void)p; return false; }
static inline void* _tr_opendir(const char* p)   { (void)p; return NULL; }
static inline char* _tr_readdir(void* h)         { (void)h; return strdup(""); }
static inline void  _tr_closedir(void* h)        { (void)h; }
#elif defined(_WIN32)
static inline int  _tr_mkdir(const char* path)     { return CreateDirectoryA(path, NULL) ? 0 : -1; }
static inline int  _tr_rmdir(const char* path)     { return RemoveDirectoryA(path) ? 0 : -1; }
static inline bool _tr_dir_exists(const char* path) {
    if (!path) return false;
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY));
}
static inline bool _tr_is_dir(const char* path)  { return _tr_dir_exists(path); }
static inline bool _tr_is_file(const char* path) {
    if (!path) return false;
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}
typedef struct { HANDLE h; WIN32_FIND_DATAA ffd; int first; } _TrDir;
static inline void* _tr_opendir(const char* path) {
    if (!path) return NULL;
    _TrDir* d = (_TrDir*)malloc(sizeof(_TrDir));
    char pat[4096]; snprintf(pat, sizeof(pat), "%s\\*", path);
    d->h = FindFirstFileA(pat, &d->ffd); d->first = 1;
    if (d->h == INVALID_HANDLE_VALUE) { free(d); return NULL; }
    return (void*)d;
}
static inline char* _tr_readdir(void* handle) {
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
static inline void _tr_closedir(void* handle) {
    _TrDir* d = (_TrDir*)handle;
    if (d) { if (d->h != INVALID_HANDLE_VALUE) FindClose(d->h); free(d); }
}
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
static inline int  _tr_mkdir(const char* path)     { return mkdir(path, 0755) == 0 ? 0 : -1; }
static inline int  _tr_rmdir(const char* path)     { return rmdir(path) == 0 ? 0 : -1; }
static inline bool _tr_dir_exists(const char* path) {
    if (!path) return false;
    struct stat st; return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}
static inline bool _tr_is_dir(const char* path)  { return _tr_dir_exists(path); }
static inline bool _tr_is_file(const char* path) {
    if (!path) return false;
    struct stat st; return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}
static inline void* _tr_opendir(const char* path)  { return (void*)opendir(path); }
static inline char* _tr_readdir(void* handle) {
    DIR* d = (DIR*)handle;
    /* Always return OWNED heap (codegen frees it); strdup("") at end-of-dir,
     * never a string literal. */
    if (!d) return strdup("");
    struct dirent* e = readdir(d);
    return e ? strdup(e->d_name) : strdup("");
}
static inline void _tr_closedir(void* handle)       { if (handle) closedir((DIR*)handle); }
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

/* argc/argv made available to std.sys.env at runtime. */
_TR_GLOBAL int    _tr_argc;
_TR_GLOBAL char** _tr_argv;

static inline long long _tr_get_argc(void)       { return (long long)_tr_argc; }
static inline char*     _tr_get_arg(long long n) { return (_tr_argv && n >= 0 && (int)n < _tr_argc) ? _tr_argv[(int)n] : (char*)""; }

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
static inline void _tr_tg_begin(void) {
    _tr_tg.cap = 16; _tr_tg.count = 0;
    _tr_tg.ths = (_TrThread*)TAURARO_ALLOC((size_t)_tr_tg.cap * sizeof(_TrThread));
}
static inline void _tr_tg_push(_TrThread t) {
    if (_tr_tg.count >= _tr_tg.cap) {
        _tr_tg.cap *= 2;
        _tr_tg.ths = (_TrThread*)TAURARO_REALLOC(_tr_tg.ths, (size_t)_tr_tg.cap * sizeof(_TrThread));
    }
    _tr_tg.ths[_tr_tg.count++] = t;
}
static inline void _tr_taskgroup_wait(void) {
    for (int i = 0; i < _tr_tg.count; i++) _tr_thread_join_wait(_tr_tg.ths[i]);
    if (_tr_tg.ths) { TAURARO_FREE(_tr_tg.ths); _tr_tg.ths = NULL; }
    _tr_tg.count = 0; _tr_tg.cap = 0;
}

/* ── Per-thread panic state (storage definitions for _TR_MAIN TU) ─── */
#if !defined(TAURARO_BARE) && !defined(TAURARO_KERNEL)
_TR_GLOBAL _TR_THREAD_LOCAL int     _tr_thread_has_panic_buf;
_TR_GLOBAL _TR_THREAD_LOCAL jmp_buf _tr_thread_panic_jmpbuf;
_TR_GLOBAL _TR_THREAD_LOCAL char*   _tr_thread_panic_message;
#endif

/* ── Exception stack (setjmp/longjmp based, per-thread) ─────────────── */

#define _TR_MAX_EXC 64
_TR_GLOBAL _TR_THREAD_LOCAL jmp_buf*  _tr_exc_bufs[_TR_MAX_EXC];
_TR_GLOBAL _TR_THREAD_LOCAL char**    _tr_exc_msgs[_TR_MAX_EXC];
_TR_GLOBAL _TR_THREAD_LOCAL int       _tr_exc_sp;

static void _tr_exc_push(jmp_buf* b, char** m) {
    if (_tr_exc_sp < _TR_MAX_EXC) {
        _tr_exc_bufs[_tr_exc_sp] = b;
        _tr_exc_msgs[_tr_exc_sp] = m;
        _tr_exc_sp++;
    }
}
static void _tr_exc_pop(void)  { if (_tr_exc_sp > 0) _tr_exc_sp--; }
static void _tr_exc_raise(char* msg) {
    if (_tr_exc_sp > 0) {
        _tr_exc_sp--;
        *_tr_exc_msgs[_tr_exc_sp] = msg;
        longjmp(*_tr_exc_bufs[_tr_exc_sp], 1);
    }
    /* No user try-handler: escalate to thread panic handler if in a spawned thread */
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
static bool _tr_str_contains(const char* s, const char* sub) {
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
static char* _tr_int_to_str(long long n)   { char* b=(char*)TAURARO_ALLOC(32); snprintf(b,32,"%lld",n); return b; }
static char* _tr_float_to_str(double n)    { char* b=(char*)TAURARO_ALLOC(32); snprintf(b,32,"%g",n);   return b; }
static char* _tr_float_to_c_lit(double n) {
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
static long long _tr_strlen(char* s)     { return s ? (long long)strlen(s) : 0LL; }

/* ── String equality ─────────────────────────────────────────────────── */
static inline bool _tr_str_eq(const char* a, const char* b) {
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
static inline char* _tr_str_slice(const char* s, long long start, long long end) {
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
static inline int _tr_system(const char* cmd) { (void)cmd; return -1; }
#else
static inline int _tr_system(const char* cmd) { return system(cmd); }
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
static bool      Dict_has(Dict* d, char* key) { return Dict_get(d,key)!=NULL; }
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

/* ── Built-in Tuple (up to 8 elements, all stored as long long) ────────── */
typedef struct { long long data[8]; } TrTuple;

/* List_TrTuple: vector of builtin tuples (Vec[Tuple]). Predefined here so the
   codegen needn't lazily emit it (which races the types-header global decls). */
typedef struct { TrTuple* data; size_t len; size_t capacity; } List_TrTuple;
static inline List_TrTuple* List_TrTuple_new(void) { List_TrTuple* l=(List_TrTuple*)malloc(sizeof(List_TrTuple)); l->data=(TrTuple*)calloc(8,sizeof(TrTuple)); l->len=0; l->capacity=8; return l; }
static inline void List_TrTuple_append(List_TrTuple* l, TrTuple val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(TrTuple*)realloc(l->data,sizeof(TrTuple)*l->capacity); } l->data[l->len++]=val; }
static inline TrTuple List_TrTuple_get(List_TrTuple* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline TrTuple List_TrTuple_pop(List_TrTuple* l) { if(!l||l->len==0) return (TrTuple){0}; l->len--; return l->data[l->len]; }
static inline void List_TrTuple_set(List_TrTuple* l, long long i, TrTuple v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline void List_TrTuple_free(List_TrTuple* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── List types (bootstrap phase) ─────────────────────────────────── */

typedef struct { long long* __restrict__ data; size_t len; size_t capacity; } List_i64;
static inline List_i64* List_i64_new(void) { List_i64* l=(List_i64*)malloc(sizeof(List_i64)); l->data=(long long*)calloc(8,sizeof(long long)); l->len=0; l->capacity=8; return l; }
static inline void List_i64_append(List_i64* l, long long val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(long long*)TAURARO_REALLOC(l->data,sizeof(long long)*l->capacity); } l->data[l->len++]=val; }
static inline bool List_i64_contains(List_i64* l, long long val) { for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return true; } return false; }
static inline long long List_i64_pop(List_i64* l) { if(!l||l->len==0) return 0LL; l->len--; return l->data[l->len]; }
static inline void List_i64_set(List_i64* l, long long i, long long v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline long long List_i64_get(List_i64* l, long long i) { if(l&&(size_t)i<l->len) return l->data[i]; return 0LL; }
static inline void List_i64_free(List_i64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { double* __restrict__ data; size_t len; size_t capacity; } List_f64;
static inline List_f64* List_f64_new(void) { List_f64* l=(List_f64*)malloc(sizeof(List_f64)); l->data=(double*)calloc(8,sizeof(double)); l->len=0; l->capacity=8; return l; }
static inline void List_f64_append(List_f64* l, double val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(double*)realloc(l->data,sizeof(double)*l->capacity); } l->data[l->len++]=val; }
static inline double List_f64_pop(List_f64* l) { if(!l||l->len==0) return 0.0; l->len--; return l->data[l->len]; }
static inline void List_f64_free(List_f64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* Undefine the forward-declaration macros before the real typedef */
#undef List_str
#undef List_i64
#undef List_f64
#undef List_ptr
typedef struct { char** data; size_t len; size_t capacity; } List_str;
static inline List_str* List_str_new(void) { List_str* l=(List_str*)malloc(sizeof(List_str)); l->data=(char**)calloc(8,sizeof(char*)); l->len=0; l->capacity=8; return l; }
static inline void List_str_append(List_str* l, char* val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(char**)TAURARO_REALLOC(l->data,sizeof(char*)*l->capacity); } l->data[l->len++]=val; }
static inline char* List_str_pop(List_str* l) { if(!l||l->len==0) return NULL; l->len--; return l->data[l->len]; }
static inline void List_str_free(List_str* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── List_TrStr: refcounted-string element container (Phase 2 target) ──
 * Parallel to List_str (char**); element is the 16-byte TrStr fat
 * pointer. append() retains, free() releases every element. */
typedef struct { TrStr* data; size_t len; size_t capacity; } List_TrStr;
static inline List_TrStr* List_TrStr_new(void) { List_TrStr* l=(List_TrStr*)malloc(sizeof(List_TrStr)); _TR_MEMCOUNT_INC(); _TR_MEMCOUNT_LIST_INC(); l->data=(TrStr*)calloc(8,sizeof(TrStr)); _TR_MEMCOUNT_INC(); l->len=0; l->capacity=8; return l; }
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
static inline List_ptr* List_ptr_new(void) { List_ptr* l=(List_ptr*)malloc(sizeof(List_ptr)); _TR_MEMCOUNT_INC(); _TR_MEMCOUNT_LIST_INC(); l->data=(void**)calloc(8,sizeof(void*)); _TR_MEMCOUNT_INC(); l->len=0; l->capacity=8; return l; }
static inline void List_ptr_append(List_ptr* l, void* val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(void**)TAURARO_REALLOC(l->data,sizeof(void*)*l->capacity); } l->data[l->len++]=val; }
static inline void* List_ptr_pop(List_ptr* l) { if(!l||l->len==0) return NULL; l->len--; return l->data[l->len]; }
static inline void List_ptr_free(List_ptr* l) { if(l){ _TR_MEMCOUNT_LIST_DEC(); _tr_free(l->data); _tr_free(l); } }
/* Free a List_ptr whose elements are owned refcounted heap-class instances:
   release each element (via its _trdrop_T) before freeing the buffer. The list
   co-owns each element (append retains), so this balances the count. */
static inline void List_ptr_free_obj(List_ptr* l, void(*drop)(void*)) { if(l){ for(size_t _i=0;_i<l->len;_i++){ _tr_obj_release(l->data[_i], drop); } _TR_MEMCOUNT_LIST_DEC(); _tr_free(l->data); _tr_free(l); } }

typedef struct { _Bool* data; size_t len; size_t capacity; } List_bool;
static inline List_bool* List_bool_new(void) { List_bool* l=(List_bool*)malloc(sizeof(List_bool)); l->data=(_Bool*)calloc(8,sizeof(_Bool)); l->len=0; l->capacity=8; return l; }
static inline void List_bool_append(List_bool* l, _Bool val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(_Bool*)realloc(l->data,sizeof(_Bool)*l->capacity); } l->data[l->len++]=val; }
static inline _Bool List_bool_get(List_bool* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_bool_set(List_bool* l, long long i, _Bool v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_bool_free(List_bool* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { int8_t* data; size_t len; size_t capacity; } List_i8;
static inline List_i8* List_i8_new(void) { List_i8* l=(List_i8*)malloc(sizeof(List_i8)); l->data=(int8_t*)calloc(8,sizeof(int8_t)); l->len=0; l->capacity=8; return l; }
static inline void List_i8_append(List_i8* l, int8_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(int8_t*)realloc(l->data,sizeof(int8_t)*l->capacity); } l->data[l->len++]=val; }
static inline int8_t List_i8_get(List_i8* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_i8_set(List_i8* l, long long i, int8_t v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_i8_free(List_i8* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { int* data; size_t len; size_t capacity; } List_i32;
static inline List_i32* List_i32_new(void) { List_i32* l=(List_i32*)malloc(sizeof(List_i32)); l->data=(int*)calloc(8,sizeof(int)); l->len=0; l->capacity=8; return l; }
static inline void List_i32_append(List_i32* l, int val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(int*)realloc(l->data,sizeof(int)*l->capacity); } l->data[l->len++]=val; }
static inline int List_i32_get(List_i32* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_i32_set(List_i32* l, long long i, int v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_i32_free(List_i32* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { char* data; size_t len; size_t capacity; } List_char;
static inline List_char* List_char_new(void) { List_char* l=(List_char*)malloc(sizeof(List_char)); l->data=(char*)calloc(8,sizeof(char)); l->len=0; l->capacity=8; return l; }
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
static inline List_u8* List_u8_new(void) { List_u8* l=(List_u8*)malloc(sizeof(List_u8)); l->data=(uint8_t*)calloc(8,sizeof(uint8_t)); l->len=0; l->capacity=8; return l; }
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
static inline List_u32* List_u32_new(void) { List_u32* l=(List_u32*)malloc(sizeof(List_u32)); l->data=(uint32_t*)calloc(8,sizeof(uint32_t)); l->len=0; l->capacity=8; return l; }
static inline void List_u32_append(List_u32* l, uint32_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(uint32_t*)realloc(l->data,sizeof(uint32_t)*l->capacity); } l->data[l->len++]=val; }
static inline void List_u32_free(List_u32* l) { if(l){ _tr_free(l->data); _tr_free(l); } }
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
static inline Set_i64* Set_i64_new(void) { Set_i64* l=(Set_i64*)malloc(sizeof(Set_i64)); l->data=(long long*)calloc(8,sizeof(long long)); l->len=0; l->capacity=8; return l; }
static inline void Set_i64_add(Set_i64* l, long long val) { 
    for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(long long*)TAURARO_REALLOC(l->data,sizeof(long long)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_i64_free(Set_i64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { void** data; size_t len; size_t capacity; } Set_ptr;
static inline Set_ptr* Set_ptr_new(void) { Set_ptr* l=(Set_ptr*)malloc(sizeof(Set_ptr)); l->data=(void**)calloc(8,sizeof(void*)); l->len=0; l->capacity=8; return l; }
static inline void Set_ptr_add(Set_ptr* l, void* val) { 
    for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(void**)TAURARO_REALLOC(l->data,sizeof(void*)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_ptr_free(Set_ptr* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { char** data; size_t len; size_t capacity; } Set_str;
static inline Set_str* Set_str_new(void) { Set_str* l=(Set_str*)malloc(sizeof(Set_str)); l->data=(char**)calloc(8,sizeof(char*)); l->len=0; l->capacity=8; return l; }
static inline void Set_str_add(Set_str* l, char* val) { 
    for (size_t i = 0; i < l->len; i++) { if (strcmp(l->data[i], val) == 0) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(char**)TAURARO_REALLOC(l->data,sizeof(char*)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_str_free(Set_str* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

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

static inline List_TrStr* _tr_str_split(const char* s, const char* sep) {
    List_TrStr* l=List_TrStr_new(); if(!s||!sep||!*sep) return l;
    char* cp=(char*)malloc(strlen(s)+1); if(cp){ size_t _n=strlen(s)+1; memcpy(cp,s,_n); }
    char* tok=strtok(cp,(char*)sep);
    while(tok){ List_TrStr_append_owned(l,_tr_str_wrap(strdup(tok))); tok=strtok(NULL,(char*)sep); }
    _tr_free(cp); return l;
}
static inline List_TrStr* _tr_str_lines(const char* s) { return _tr_str_split(s, "\n"); }
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

/* -- OS / System helpers (platform-specific) ------------------------------- */
#if defined(TAURARO_BARE) && !defined(__wasi__)
/* Bare / freestanding: no OS services */
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
static inline int _tr_tcp_listen(const char* host,int port,int backlog) {
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
static inline void _tr_tcp_set_nodelay(int fd) {
    int one = 1;
    setsockopt((SOCKET)fd, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(one));
}
static inline int   _tr_tcp_accept(int srv) { SOCKET c=accept((SOCKET)srv,NULL,NULL); if(c!=INVALID_SOCKET) _tr_tcp_set_nodelay((int)c); return (c==INVALID_SOCKET)?-1:(int)c; }
static inline char* _tr_tcp_peer_addr(int fd) {
    struct sockaddr_in a; int al=sizeof(a);
    if(getpeername((SOCKET)fd,(struct sockaddr*)&a,&al)!=0) return _tr_empty_heap_str();
    char* buf=(char*)_tr_c_malloc(64); char ip[32];
    inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));
    _snprintf(buf,63,"%s:%d",ip,(int)ntohs(a.sin_port)); return buf;
}
static inline int  _tr_udp_socket(void) { _tr_net_init(); SOCKET s=socket(AF_INET,SOCK_DGRAM,0); return (s==INVALID_SOCKET)?-1:(int)s; }
static inline int  _tr_udp_bind(int fd,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    return bind((SOCKET)fd,(struct sockaddr*)&a,sizeof(a))==0?0:-1;
}
static inline int  _tr_udp_send_to(int fd,const char* data,int len,const char* host,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=inet_addr(host);
    return (int)sendto((SOCKET)fd,data,len,0,(struct sockaddr*)&a,sizeof(a));
}
static inline int  _tr_udp_recv_from(int fd,char* buf,int cap,char* src) {
    struct sockaddr_in a; int al=sizeof(a);
    int n=(int)recvfrom((SOCKET)fd,buf,cap,0,(struct sockaddr*)&a,&al);
    if(n>0&&src){char ip[32];inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));_snprintf(src,63,"%s:%d",ip,(int)ntohs(a.sin_port));}
    return n;
}
static inline void _tr_udp_close(int fd) { closesocket((SOCKET)fd); }
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
static inline int _tr_tcp_listen(const char* host,int port,int backlog) {
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
static inline void _tr_tcp_set_nodelay(int fd) {
    int one = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
}
static inline int   _tr_tcp_accept(int srv) { int c=accept(srv,NULL,NULL); if(c>=0) _tr_tcp_set_nodelay(c); return c; }
static inline char* _tr_tcp_peer_addr(int fd) {
    struct sockaddr_in a; socklen_t al=sizeof(a);
    if(getpeername(fd,(struct sockaddr*)&a,&al)<0) return _tr_empty_heap_str();
    char* buf=(char*)_tr_c_malloc(64); char ip[32];
    inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));
    snprintf(buf,63,"%s:%d",ip,(int)ntohs(a.sin_port)); return buf;
}
static inline int  _tr_udp_socket(void) { return socket(AF_INET,SOCK_DGRAM,0); }
static inline int  _tr_udp_bind(int fd,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    return bind(fd,(struct sockaddr*)&a,sizeof(a))==0?0:-1;
}
static inline int  _tr_udp_send_to(int fd,const char* data,int len,const char* host,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=inet_addr(host);
    return (int)sendto(fd,data,len,0,(struct sockaddr*)&a,sizeof(a));
}
static inline int  _tr_udp_recv_from(int fd,char* buf,int cap,char* src) {
    struct sockaddr_in a; socklen_t al=sizeof(a);
    int n=(int)recvfrom(fd,buf,cap,0,(struct sockaddr*)&a,&al);
    if(n>0&&src){char ip[32];inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));snprintf(src,63,"%s:%d",ip,(int)ntohs(a.sin_port));}
    return n;
}
static inline void _tr_udp_close(int fd) { close(fd); }
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

#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
static inline int  _tr_tcp_set_nonblocking(int fd)                    { (void)fd; return -1; }
static inline int  _tr_tcp_recv_nb(int fd, char* b, int c)            { (void)fd;(void)b;(void)c; return -1; }
static inline int  _tr_tcp_send_nb(int fd, const char* d, int l)      { (void)fd;(void)d;(void)l; return -1; }
static inline int  _tr_tcp_accept_nb(int fd)                          { (void)fd; return TAURARO_WOULD_BLOCK; }
static inline int  _tr_tcp_connect_nb(const char* h, int p)           { (void)h;(void)p; return -1; }

#elif defined(_WIN32)
#ifndef _TR_NET_INCLUDED
#define _TR_NET_INCLUDED
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
static inline int _tr_tcp_set_nonblocking(int fd) {
    u_long mode = 1;
    return ioctlsocket((SOCKET)fd, FIONBIO, &mode) == 0 ? 0 : -1;
}
static inline int _tr_tcp_recv_nb(int fd, char* buf, int cap) {
    int n = recv((SOCKET)fd, buf, cap, 0);
    if (n < 0 && WSAGetLastError() == WSAEWOULDBLOCK) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_send_nb(int fd, const char* data, int len) {
    int n = send((SOCKET)fd, data, len, 0);
    if (n < 0 && WSAGetLastError() == WSAEWOULDBLOCK) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_accept_nb(int server_fd) {
    SOCKET s = accept((SOCKET)server_fd, NULL, NULL);
    if (s == INVALID_SOCKET) {
        return (WSAGetLastError() == WSAEWOULDBLOCK) ? TAURARO_WOULD_BLOCK : -1;
    }
    _tr_tcp_set_nodelay((int)s);
    return (int)s;
}
static inline int _tr_tcp_connect_nb(const char* host, int port) {
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
static inline int _tr_tcp_set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0 ? 0 : -1;
}
static inline int _tr_tcp_recv_nb(int fd, char* buf, int cap) {
    int n = (int)recv(fd, buf, (size_t)cap, 0);
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_send_nb(int fd, const char* data, int len) {
    int n = (int)send(fd, data, (size_t)len, 0);
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_accept_nb(int server_fd) {
    int fd = accept(server_fd, NULL, NULL);
    if (fd < 0) return (errno == EAGAIN || errno == EWOULDBLOCK) ? TAURARO_WOULD_BLOCK : -1;
    _tr_tcp_set_nodelay(fd);
    return fd;
}
static inline int _tr_tcp_connect_nb(const char* host, int port) {
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
static inline int _tr_tcp_send_raw(int fd, char* buf, int len) { return _tr_tcp_send_nb(fd, buf, len); }

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
    char fmt[16]; int d = decimals < 0 ? 6 : decimals;
    snprintf(fmt, sizeof(fmt), "%%.%df", d);
    char* buf = (char*)_tr_c_malloc(64); if(!buf) return _tr_empty_heap_str();
    snprintf(buf, sizeof(buf), fmt, f); return buf;
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
static inline char* _tr_exe_dir(void) {
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
    if(!realpath(tmp,buf)){buf[0]='.';buf[1]='\0';return buf;}
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
#ifndef TAURARO_BARE
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
static inline char* _tr_tls_connect(char* host, int port) {
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
static inline int   _tr_tls_send(char* h, char* d) { if(!h||!d) return -1; return SSL_write(((_TrTLSConn*)h)->ssl,d,(int)strlen(d)); }
static inline char* _tr_tls_recv(char* h, int cap) {
    if(!h||cap<=0) return _tr_strdup("");
    char* buf=(char*)TAURARO_ALLOC((size_t)cap+1); if(!buf) return _tr_strdup("");
    int n=SSL_read(((_TrTLSConn*)h)->ssl,buf,cap);
    if(n<=0){TAURARO_FREE(buf);return _tr_strdup("");}
    buf[n]='\0'; return buf;
}
static inline void _tr_tls_close(char* h) {
    if(!h) return; _TrTLSConn* c=(_TrTLSConn*)h;
    SSL_shutdown(c->ssl);SSL_free(c->ssl);SSL_CTX_free(c->ctx);_TR_SOCK_CLOSE(c->fd);TAURARO_FREE(c);
}
/* ── Server side: one SSL_CTX (cert+key), one _TrTLSConn per accepted fd ──
 * SSL_accept/read/write are blocking, so server TLS is for the thread-per-
 * connection model (listen_tls), where blocking a worker thread is fine. */
static inline char* _tr_tls_server_new(char* cert, char* key) {
    static _Atomic int _tr_ssl_once_s = 0;
    if (atomic_fetch_add(&_tr_ssl_once_s,1)==0){SSL_library_init();SSL_load_error_strings();OpenSSL_add_all_algorithms();}
    SSL_CTX* ctx=SSL_CTX_new(TLS_server_method());
    if(!ctx) return NULL;
    if(SSL_CTX_use_certificate_chain_file(ctx,cert)<=0){SSL_CTX_free(ctx);return NULL;}
    if(SSL_CTX_use_PrivateKey_file(ctx,key,SSL_FILETYPE_PEM)<=0){SSL_CTX_free(ctx);return NULL;}
    return (char*)ctx;
}
static inline char* _tr_tls_accept(char* ctxh, int fd) {
    if(!ctxh) return NULL;
    SSL* ssl=SSL_new((SSL_CTX*)ctxh); if(!ssl) return NULL;
    SSL_set_fd(ssl,fd);
    if(SSL_accept(ssl)!=1){SSL_free(ssl);return NULL;}
    _TrTLSConn* c=(_TrTLSConn*)TAURARO_ALLOC(sizeof(_TrTLSConn));
    if(!c){SSL_free(ssl);return NULL;}
    c->ctx=NULL; c->ssl=ssl; c->fd=fd;   /* ctx is shared/server-owned, not freed per-conn */
    return (char*)c;
}
static inline void _tr_tls_server_free(char* ctxh) { if(ctxh) SSL_CTX_free((SSL_CTX*)ctxh); }
#else
static inline char* _tr_tls_connect(char* h, int p) { (void)h;(void)p; return NULL; }
static inline int   _tr_tls_send(char* h, char* d)  { (void)h;(void)d; return -1; }
static inline char* _tr_tls_recv(char* h, int c)    { (void)h;(void)c; return _tr_strdup(""); }
static inline void  _tr_tls_close(char* h)          { (void)h; }
static inline char* _tr_tls_server_new(char* c, char* k) { (void)c;(void)k; return NULL; }
static inline char* _tr_tls_accept(char* x, int fd) { (void)x;(void)fd; return NULL; }
static inline void  _tr_tls_server_free(char* x) { (void)x; }
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

#endif /* TAURARO_RT_H */
