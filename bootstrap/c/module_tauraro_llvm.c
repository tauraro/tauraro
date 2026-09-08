/* tauraro_llvm.c — Stage 2 of the LLVM backend toolchain: emit an object file from
 * textual LLVM IR IN-PROCESS via the llvm-c C API, replacing the `llc` subprocess.
 *
 * The Tauraro compiler keeps EMITTING textual `.ll` (so the differential oracle — C vs
 * LLVM byte-for-byte output — stays intact); this shim just parses that text and runs the
 * optimizer + codegen through libLLVM's stable C ABI, then writes `out.o`. Linking still
 * reuses the system C compiler (the C backend already needs one), exactly like Stage 1.
 *
 * Built ONLY when TAURARO_LLVM_INPROC is defined (and the tauraroc link adds `-lLLVM-<N>`).
 * Otherwise `_tr_llvm_emit_object` is a stub returning -1, and the compiler falls back to
 * the bundled `llc` subprocess (Stage 1). So libLLVM is an OPT-IN build dependency, never
 * required for a plain `gcc`-only build of the compiler. This file is ALWAYS linked in (the
 * prototype lives in tauraro_rt.h); only the body changes with the flag.
 *
 * Returns 0 on success; >0 on an LLVM error (message to stderr); -1 when not compiled in.
 * Set TAURARO_LLVM_VERBOSE=1 to log which path emitted the object.
 */

#ifdef TAURARO_LLVM_INPROC
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Transforms/PassBuilder.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int _tr_llvm_emit_object(const char* ll_path, const char* out_path, const char* triple) {
    /* Host arch codegen + object emission (X86 on x64, AArch64 on arm64). The requested
     * `triple` (e.g. x86_64-pc-windows-gnu) drives the object's ABI; the host target only
     * needs to cover the host arch, which InitializeNativeTarget does. */
    if (LLVMInitializeNativeTarget())      { fprintf(stderr, "tauraro-llvm: no native target\n");      return 5; }
    if (LLVMInitializeNativeAsmPrinter())  { fprintf(stderr, "tauraro-llvm: no native asm printer\n"); return 6; }

    LLVMContextRef ctx = LLVMContextCreate();
    LLVMMemoryBufferRef buf = NULL;
    char* err = NULL;
    if (LLVMCreateMemoryBufferWithContentsOfFile(ll_path, &buf, &err)) {
        fprintf(stderr, "tauraro-llvm: cannot read '%s': %s\n", ll_path, err ? err : "(none)");
        if (err) LLVMDisposeMessage(err);
        LLVMContextDispose(ctx);
        return 7;
    }
    LLVMModuleRef mod = NULL;

    /* LLVMParseIRInContext consumes `buf`. Non-zero return = parse failure. */
    if (LLVMParseIRInContext(ctx, buf, &mod, &err)) {
        fprintf(stderr, "tauraro-llvm: IR parse error: %s\n", err ? err : "(none)");
        if (err) LLVMDisposeMessage(err);
        LLVMContextDispose(ctx);
        return 1;
    }

    char* def_tri = NULL;
    const char* tri = triple;
    if (!tri || !tri[0]) { def_tri = LLVMGetDefaultTargetTriple(); tri = def_tri; }

    LLVMTargetRef target = NULL;
    if (LLVMGetTargetFromTriple(tri, &target, &err)) {
        fprintf(stderr, "tauraro-llvm: no target for '%s': %s\n", tri, err ? err : "(none)");
        if (err) LLVMDisposeMessage(err);
        if (def_tri) LLVMDisposeMessage(def_tri);
        LLVMDisposeModule(mod); LLVMContextDispose(ctx);
        return 2;
    }

    LLVMTargetMachineRef tm = LLVMCreateTargetMachine(
        target, tri, "generic", "",
        LLVMCodeGenLevelAggressive, LLVMRelocDefault, LLVMCodeModelDefault);

    /* Run the -O2 mid-level pipeline (new pass manager). This is what clang runs and what
     * our alloca-per-vreg IR needs (mem2reg etc.) — llc alone does not. */
    LLVMPassBuilderOptionsRef opts = LLVMCreatePassBuilderOptions();
    LLVMErrorRef perr = LLVMRunPasses(mod, "default<O2>", tm, opts);
    LLVMDisposePassBuilderOptions(opts);
    if (perr) {
        char* pm = LLVMGetErrorMessage(perr);
        fprintf(stderr, "tauraro-llvm: optimization error: %s\n", pm ? pm : "(none)");
        if (pm) LLVMDisposeErrorMessage(pm);
        LLVMDisposeTargetMachine(tm);
        if (def_tri) LLVMDisposeMessage(def_tri);
        LLVMDisposeModule(mod); LLVMContextDispose(ctx);
        return 3;
    }

    int rc = 0;
    if (LLVMTargetMachineEmitToFile(tm, mod, (char*)out_path, LLVMObjectFile, &err)) {
        fprintf(stderr, "tauraro-llvm: object emit error: %s\n", err ? err : "(none)");
        if (err) LLVMDisposeMessage(err);
        rc = 4;
    } else if (getenv("TAURARO_LLVM_VERBOSE")) {
        fprintf(stderr, "tauraro-llvm: in-process object emit OK -> %s\n", out_path);
    }

    LLVMDisposeTargetMachine(tm);
    if (def_tri) LLVMDisposeMessage(def_tri);
    LLVMDisposeModule(mod);
    LLVMContextDispose(ctx);
    return rc;
}

#else  /* !TAURARO_LLVM_INPROC — the default: no libLLVM dependency, stub returns -1 so the
        * compiler falls back to the bundled-llc subprocess (Stage 1). This file is ALWAYS
        * linked into the compiler (via the build's --link / the bootstrap/c copy), so the
        * `_tr_llvm_emit_object` symbol always resolves; only its body changes with the flag. */

int _tr_llvm_emit_object(const char* ll_path, const char* out_path, const char* triple) {
    (void)ll_path; (void)out_path; (void)triple;
    return -1;
}

#endif
