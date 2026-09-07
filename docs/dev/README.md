# Tauraro Developer & Contributor Documentation

This section is for people who want to **work on the Tauraro compiler itself**,
**contribute fixes to `src/`/`std/`/`runtime/`**, or **build and publish their
own libraries** with `taupkg`. If you just want to *use* the language, see the
[Language Documentation](../lang/README.md) and [Standard Library
Documentation](../std/README.md) instead.

---

## Contents

| # | File | Topics |
|---|------|--------|
| 00 | [Versioning & Stability Policy](00_versioning_policy.md) | Pre-1.0 versioning rules, where the version string lives, CHANGELOG.md conventions, error-code stability |
| 01 | [How the Compiler Works](01_architecture.md) | Lexer → Parser → AST → Resolver → Sema/HIR → CGenerator pipeline, ownership/escape analysis, generic monomorphization, how to add a new keyword/statement |
| 02 | [Building and Contributing](02_contributing.md) | Self-hosting bootstrap (gen1→gen2→gen3), "never patch generated C", runtime header sync, blessing checklist, commit conventions |
| 03 | [Memory Model Internals](03_memory_model_internals.md) | `TrStr` refcounting, auto-drop/escape analysis, wrap-hoist, `List_TrStr`/`Dict_free_strval`, best practices for stdlib authors |
| 04 | [Codegen Guide](04_codegen_guide.md) | How specific features lower to C: generics, enums/match, closures, interfaces/vtables, Sendable/concurrency, channels/select, `ETypeArg` |
| 05 | [Building Libraries](05_building_libraries.md) | `taupkg` manifests, package layout conventions, `local:` deps, `TAURARO_PATH`, worked example, publishing |
| 06 | [Best Practices & Pitfalls](06_best_practices_pitfalls.md) | Curated list of hard-won lessons: memory/string pitfalls, codegen correctness, parser/lexer traps, build/bootstrap traps |
| 07 | [MIR Ownership Plan](07_mir_ownership_plan.md) | The sound interprocedural consumes-fixpoint ownership analysis (retiring heuristics) |
| 08 | [Runtime Tiers & Freestanding](08_runtime_tiers_and_freestanding.md) | `TAURARO_KERNEL`/`NO_LIBC`/`NO_THREADS`/`NO_NET` tiers, bare-metal builds |
| 09 | [GPU Kernel Backend](09_gpu_backend.md) | `@kernel` → LLVM IR → PTX/SPIR-V (`src/codegen/gpu`), the host→GPU deltas, runtime loading, validation |
| 10 | [Wrapping C/C++ Libraries](10_wrapping_c_cpp_libraries.md) | Zero-manual-link packages around an existing C/C++ library — vendoring the source vs. binding a system-installed library via `pkg-config`, then distributing either with `taupkg` |

---

## Suggested Reading Order

**New compiler contributor:** 01 → 02 → 03 → 04 → 06
**Library author (taupkg):** 05 → 03 (for memory rules) → 06
**Wrapping a C/C++ library:** 10 → [24 — Bindgen](../lang/24_bindgen.md) → 05
**Fixing a specific bug:** jump to 06 first to check if it's a known pitfall, then 01/03/04 for the relevant subsystem

---

Next: [How the Compiler Works →](01_architecture.md)
