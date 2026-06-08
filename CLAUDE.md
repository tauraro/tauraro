# Tauraro — AI Contribution Guide

## Project Overview

Self-hosted compiled language with Python syntax + bilingual (English/Hausa) keywords. Compiles to C → native via GCC/Clang. ~13,700 LOC compiler, ~8,000 LOC stdlib. Version 0.0.5.

**Key constraint:** All compiler source is written in Tauraro itself (`.tr` files). The bootstrap chain is: `bootstrap/c/main.c` (portable C seed) → stage0 → `src/main.tr` → final `tauraroc` binary.

## Compiler Pipeline

```
source.tr → Lexer (src/lexer.tr, 677L)
         → Parser (src/parser.tr, 2,259L)
         → Sema  (src/sema.tr, 2,498L)     ← type-checking + ownership inference
         → HIR   (src/hir.tr, 296L)        ← typed intermediate representation
         → Codegen (src/codegen/c.tr, 5,697L)  ← emits per-module .c files
         → GCC/Clang → native binary
```

## Source Map

| File | LOC | Role |
|------|-----|------|
| `main.tr` | 830 | CLI driver, arg parsing, pipeline orchestration, C linking |
| `token.tr` | 359 | Token enum (185 variants) — literals, keywords (EN+HA), operators, delimiters, indentation |
| `lexer.tr` | 677 | Tokenizer with indentation tracking, bilingual keyword mapping |
| `parser.tr` | 2,259 | Recursive-descent parser — all features (async, generics, GPU, unsafe, closures, comprehensions) |
| `ast.tr` | 483 | AST nodes — Decl (11 variants), Expr (40+), Stmt (25+), Pattern |
| `sema.tr` | 2,498 | Semantic analysis — type checking, ownership inference (M-1..M-7 rules), scope management, HIR lowering |
| `hir.tr` | 296 | HIR types — HirExpr (45 variants), HirStmt (26 variants), HirProgram |
| `resolver.tr` | 240 | Unity-build module resolver — search paths, pub/private filtering |
| `codegen/c.tr` | 5,697 | C backend — per-module .c generation, monomorphization, runtime header |
| `codegen/llvm.tr` | 362 | LLVM IR backend (experimental, text-IR skeleton) |

## Key Architecture Decisions

1. **Unity-build model:** All imports resolved into a single flat declaration list by the resolver. No separate compilation units for Tauraro modules — each module gets its own `.c` file, but the C compiler links all at once.

2. **Automatic ownership:** Sema infers Own/Borrow/Move/Shared/Stack for every variable and injects `free()` at scope exit. No lifetime annotations needed (except `from` for special pointer-return cases).

3. **Zero-cost abstractions:** Classes → C structs, methods → `ClassName_method()` functions, interfaces → vtable structs (one deref per virtual call), generics → monomorphized per type combo.

4. **Bilingual keywords:** Every keyword has EN and HA forms (e.g. `def`/`aiki`, `class`/`aji`, `if`/`idan`). Lexer's `keyword_to_token()` maps both to the same token variant. Programs can mix freely.

5. **`@copy` annotation:** Arena-allocated types (like `AstType`) annotated with `@copy` to opt out of move semantics — they're never individually freed.

## Coding Conventions

- Files start with `# comment` header describing purpose
- Classes: `pub class Name:`
- Extend blocks: `extend Name:` for methods (separated from field declarations)
- Methods: `pub def method_name(self, param: Type) -> ReturnType:`
- Boxing: `box_expr()`, `box_stmt()`, `box_decl()`, `box_asttype()` — heap-allocates wrappers for AST/HIR nodes
- Enum variants: PascalCase (`ELitInt`, `SLet`, `DClass`)
- Variables/functions: snake_case
- Constants: PascalCase or UPPER_CASE (inconsistent — follow existing pattern in file)
- Match exhaustiveness: always include `case _: pass` as default unless all variants handled
- Null pointer sentinel: `Pointer[T](0)` for zeroed pointers

## Ownership Rules (Compiler Error Codes)

- **M-1:** Heap allocation not tracked — always bind `alloc()` to named variable
- **M-2:** Use after move — access before consuming or use `clone()`
- **M-3:** No double-free — don't manually free Own variables
- **M-4:** No dangling pointers — don't return refs to locals
- **M-5:** No aliased mutation — don't mutate while borrow active
- **M-6:** Immutable by default — add `mut` to reassign
- **M-7:** None requires Option[T] or pointer type
- **T-1 through T-5:** Type rules — use explicit `as` casts, match types
- **F-1/F-2/F-3:** Function rules — arg count, undefined calls, missing returns
- **U-1:** alloc/dealloc outside `unsafe:` (only with `--strict`)

## Build System

```bash
# Bootstrap build (requires existing tauraroc binary):
export BOOTSTRAP_BIN=/path/to/tauraroc
bash scripts/build.sh

# Or with tauraroc on PATH:
bash scripts/build.sh

# Output: ./tauraroc

# CI equivalent: .github/workflows/build.yml
# Builds on ubuntu/windows/macos via scripts/build.sh / build.ps1
```

CI artifacts: binary + `src/`, `std/`, `runtime/`, `examples/`, `benchmarks/` in release zips.

## Cross-Compilation Targets

Shorthands: android-arm64, android-arm32, ios, ios-sim, linux-arm64/arm32/x86_64/riscv64, windows-x64/arm64, macos-arm64/x86_64, embedded-arm/arm64/riscv32/riscv64, wasm, wasm-wasi. Raw LLVM triples also accepted.

## Standard Library Structure

`std/` — 16 subdirectories, ~8,000 LOC total. Opaque handle pattern (`handle: Pointer[char]` → heap-allocated C struct). Import via `from std.module.submodule import Name`.

Key modules: core (vec, map, string, alloc, io), async (channel, task, mutex, semaphore, barrier, group, poll, event_loop), collections (stack, queue, deque, set, heap, graph, list, dict), io (file, dir, path, console, buffer), net (tcp, udp, dns, url, http, https, server), math (int, float, stat, random, complex), sys (env, fs, process, time, os), string (str, fmt), encoding (json, base64, hex), crypto (sha256, hmac, md5, uuid), regex, test, unicode, compress (zlib), gpu.

## Bilingual Keyword Table

EN keyword(s) → Hausa counterpart(s):
def→aiki, class→aji, struct→tsari, if→idan, elif→koidan, else→sai, for→ga, while→yayinda, return→dawo, break→tsaya, continue→ci_gaba, pass→wuce, match→duba, case→hali, try→gwada, except→kama, finally→karshe, raise→jefa, assert→tabbatar, with→tare, async→ba_jira, await→jira, yield→bayar, import→shigo, from→daga, as→kamar, in→a_cikin/ciki, and→da, or→ko, not→ba, print→buga, pub→fito, lambda→dan_aiki, true→gaskiya, false→karya, none→babu.

## Common Contribution Patterns

- **Add a new AST node:** Add variant to `Expr`/`Stmt`/`Decl` enum in `ast.tr`, add parsing in `parser.tr`, add lowering in `sema.tr`, add HIR variant in `hir.tr`, add codegen in `codegen/c.tr`.
- **Add a new keyword:** Add to `keyword_to_token()` in `lexer.tr`, add token variant in `token.tr` (both EN and HA forms), handle in parser, add debug() output.
- **Add a type:** Add type keyword tokens, handle in parser's type parsing, add type checking in `sema.tr`, add C type mapping in `codegen/c.tr`.
- **Fix ownership bug:** Look at `sema.tr` — ownership inference happens during HIR lowering. Rules M-1 through M-7 enforced there.
- **Fix codegen bug:** Look at `codegen/c.tr` — check the specific expression/statement handler.
- **New backend:** Add to `codegen/` directory, register in `codegen/mod.tr`, add `--backend` flag handling in `main.tr`.

## Testing

Tests use `std.test` framework. Example programs in `examples/` (29 annotated examples) serve as integration tests. Benchmarks in `benchmarks/` validate correctness via output comparison. Run `tauraroc --check file.tr` for semantic-only validation.
