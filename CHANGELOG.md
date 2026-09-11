# Changelog

All notable changes to the Tauraro language and compiler (`tauraroc`) are
documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).
Versioning follows the pre-0.1 policy described in
[docs/dev/00_versioning_policy.md](docs/dev/00_versioning_policy.md): in the
current `0.0.x` phase, **any** version bump may contain breaking changes, and
there is no deprecation period.

## [Unreleased]

Work in progress on the production-readiness roadmap (formal ownership/safety
spec, automated regression suite + CI, incremental compilation, and a batch of
codegen-safety / diagnostics / stdlib / tooling improvements). Entries will be
added here as each phase lands.

### Fixed
- **F-1 collection-push leak closed.** `v.push(Box.init(k))` for a `Vec[HeapClass]`
  RETAINED the fresh constructor result (rc 1→2) instead of MOVING it, leaking the
  temporary's reference (the container releases only one). Static constructors
  `Class.init(...)`/`Class.new(...)` are now recognised as fresh owned values that
  TRANSFER on store (`_obj_store_needs_retain`/`_obj_expr_owns_ref`), like the
  `Class(...)` form. `f_vec_box` `LIVE 0`.
- **F-4 Mutex-owned-collection leak closed.** A `Mutex[Coll[..]]` wrapping a FRESH
  collection (e.g. `Mutex[Map[str, Box]].init(Map[str, Box].init(8))`) never freed
  the collection (or its owned elements) on drop. `_TrMutexBox` gained a `cdrop`
  slot; codegen emits `_tr_mutexbox_new_owned_coll(..., _mtxcd_*)`, where `_mtxcd_*`
  is a generated `static inline` wrapper (recorded by the mono scan, emitted into the
  shared header) calling the collection's typed free. Borrowed payloads stay
  non-owning (no double-free). `f_mutex_map` `LIVE 0`. With this, **all four fuzz
  findings F-1..F-4 are leak-free and promoted to the CORE regression gate.**
- **Latent runtime bug in `Dict`/`Map[K,bool]`/sets:** `Dict_has` reported key
  presence as `Dict_get(d,key) != NULL`, so any key stored with a `false`/`NULL`
  value was wrongly reported **absent**. This silently broke every false-valued
  dict/set — and, crucially, made the compiler's own interprocedural
  `consumes(fn,i)` ownership summary (`fn_param_consumes`, a `Map[str,bool]`)
  invisible. Fixed to walk the bucket chain by key (`runtime/tauraro_rt.h`). The
  compiler's other `Map[str,bool]` were unaffected because they only ever store
  `true`, which is why this went unnoticed.
- **F-2 borrow-vs-consume leak closed.** With `consumes(fn,i)` finally visible, a
  new post-pass `apply_borrow_drops` (`src/sema.tr`) reclaims a fresh owned local
  that is passed only to functions that BORROW it — the auto-drop the conservative
  class-arg escape used to suppress (a leak). It is additive and strictly
  conservative (adds a drop only for a local proven not-consumed), a `.free()` call
  is now detected as consuming its receiver (so a consumed local is never
  double-dropped), and a pure-scalar return (`return x.score()`) is correctly read
  as a borrow. Verified: `f_owned_use` `LIVE 0`, soundness 18/18+8/8, gen2≡gen3
  fixpoint. Promoted to the `CORE` fuzz gate.
- A method call on a receiver that names **no defined type or value** (e.g. a
  renamed/removed type used as a static receiver, `JsonValue.init_object()` after
  `std.encoding.json` dropped `JsonValue`) silently emitted a bogus free
  `init_object(JsonValue)` call, producing an `implicit declaration` C compile
  error with no source-level diagnostic. Such calls are now a hard **`[E-1]`**
  error naming the unresolved receiver (`src/sema.tr`, new `_is_known_type_name`
  guard). Module-statics (`Str`/`Clock`/`OS`), module globals, instances, generic
  parameters, and all builtin generics are unaffected (self-host `--check` clean,
  fixpoint holds). Regression: `tests/soundness/reject/unknown_receiver_method.tr`.
- The static constructors `Option.some(x)` / `Result.ok(x)` / `Result.err(x)`
  dropped their payload type when assigned to an **unannotated** local, so
  `mut o = Option.some(5)` inferred a bare `Option` (not `Option[int]`) and a
  following `o.unwrap()` mis-resolved to a bogus free `unwrap(o)` call. The
  lowercase constructors now carry the concrete payload type (`src/sema.tr`,
  guarded by a static receiver so the instance `.ok()`/`.err()` accessors are
  unaffected). Regression: `tests/regression/option_result_ctor_infer.tr`.
- Whole-number float literals (e.g. `7.0`) were emitted into generated C
  without a `.`/exponent marker (`%.17g` of `7.0` is `"7"`), so expressions
  like `7.0 / 2.0` silently became integer division (`3` instead of `3.5`).
  Fixed in `_tr_float_to_c_lit` (`runtime/tauraro_rt.h`) by appending `.0`
  when no float marker is present.
- Fixed broken escaped-quote f-strings (`f"... \"...\" ..."`) in
  `std/test/mod.tr` that caused 39 cascading parse errors and made the
  `TestRunner` helper unusable.

- Escape analysis (`mark_escaped_str_args` / `mark_escaped_coll_args` in
  `src/sema.tr`) previously had a silent wildcard `case _: pass` that skipped
  several `HirExpr` variants, so `str`/collection locals referenced only
  through an f-string interpolation, a closure capture, a list/generator
  comprehension, a slice, or a `super` method call were not excluded from
  auto-drop — a potential use-after-free / double-free. All such variants are
  now walked explicitly, and the catch-all is replaced by an exhaustive list
  of leaf/literal variants so a future new `HirExpr` variant is a compile
  error rather than a silent miss.
- Storing the same `str` local under two keys of a `Dict[K,str]`/`Map[K,str]`
  (or storing one that is later auto-dropped) double-freed / use-after-freed
  at dict teardown: `dict_val_arg` boxed the value with `_tr_str_box` but no
  `_tr_str_retain`, so multiple boxes aliased one reference. Dict/Map string
  values now retain on insert (`_tr_str_box(_tr_str_retain(...))`), mirroring
  `List_TrStr_append`, and `mark_escaped_str_args` no longer excludes the
  value arg from auto-drop. Fresh string values (concat/call/method results)
  are hoisted to a temp and released after the set so reference counts stay
  balanced.
- `extern` (`_tr_*` runtime-helper) calls passing a fresh `str` argument
  (concat/call/method result) leaked the temporary `TrStr`: `gen_args_extern`
  extracted `.data` via a bare `_tr_strz()` and discarded the struct. Fresh
  string arguments are now hoisted to a temp and released by the enclosing
  statement's `flush_wraps`, matching `gen_args` for normal calls.
- **Parser hang/crash on a generic bound carrying its own type argument**
  (`[C: DbConnection[RS], RS: DbResultSet]`). `parse_generic_bound`
  (`src/parser.tr`) consumed a bound's name with a bare `consume_ident()`,
  which doesn't understand `Iface[Arg]` syntax; the parser stalled on the `[`
  and looped forever without advancing. Fixed by reusing the existing
  `parse_type()` parser (already used for the same `Name[Args]` shape
  elsewhere), which also covers the identical class-generic-bound case.
- **No dynamic-dispatch boxing for a bare interface return type.** A function
  declared `-> SomeInterface` (the interface used directly as a concrete
  return type, not a generic bound) type-checked but failed C codegen —
  `incompatible types when returning type 'X *' but 'Y_obj' was expected` —
  because `return` never boxed the concrete class pointer into the
  interface's `{vtable, data}` dispatch object the way `let`/call-argument
  sites already did. Added `cur_ret_ty` tracking and an `iface_wrap_to`
  helper (`src/codegen/c.tr`) invoked at every `return` site; verified the
  generated C now performs real vtable dispatch (`rs.vtable->next(rs.data)`).
- **Interface-typed field/local assignment had the same missing box.**
  `obj.field = ConcreteClass()` for a field declared with an interface type
  hit the identical `incompatible types when assigning to type 'Iface_obj'
  from type 'Class *'` error, since plain assignment-statement codegen never
  called the boxing helper above. `iface_wrap_return` was generalized into
  `iface_wrap_to(target_ty, expr, rhs)` and wired into the general
  assignment path (`src/codegen/c.tr`), so any `lhs = rhs` where `lhs`'s
  declared type is an interface and `rhs` is a concrete class now boxes
  correctly, consistent with every other interface-assignment context.
- **Method calls on a concrete type returned from a generic-bound receiver
  mis-codegen'd as bare, unqualified C calls.** Inside a generic function
  body `[C: DbConnection]`, calling a method on the result of
  `conn.execute()` (`conn: C`) resolved to `void` during sema — return-type
  inference only handled a receiver whose *own* declared type was literally
  the interface name, not a generic type parameter bound to it — so codegen
  fell back to `next(rs)` instead of `QueryResultSet_next(rs)`, colliding
  with unrelated C symbols. Added `current_func_constraints` tracking and
  `_resolve_generic_bound_method_ret` (`src/sema.tr`) to resolve the call
  through the bound interface before falling through to the generic
  return-type-inference chain.
- **Explicit multi-type-argument generic function calls
  (`fname[T1, T2](args)`) were mis-parsed as an index/tuple expression**
  instead of a generic call, emitting a bogus `def_get_index(...)` — the
  existing explicit-call handling in `Expr.ECall` lowering only matched a
  single `Expr.EIdent` type argument, silently doing nothing for
  `Expr.ETuple` (2+ args). Sema now mangles the callee to
  `fname__MONO_T1_T2` for the multi-arg case too, and `ensure_mono_func`
  (`src/codegen/c.tr`) was generalized into `ensure_mono_func_n` to
  monomorphize a free function over any number of type parameters (the
  mono-scan's `__MONO_` name-splitting was updated to match).
- **A non-generic class implementing a generic interface with concrete
  arguments silently lost those arguments**, so its `_as_Iface` vtable wrap
  could never be correctly generated. `class MyConn implements
  DbConnection[MyRS]:` — the parser recognized `DbConnection` but discarded
  the `[MyRS]` entirely (never stored anywhere). Added `iface_targs`
  (parallel to `iface_names`) through `ClassDef` → `HirClass`
  (`src/ast.tr`, `src/hir.tr`, `src/parser.tr`, `src/sema.tr`);
  `gen_one_iface_wrap` (`src/codegen/c.tr`) now monomorphizes the interface's
  vtable/obj typedefs per concrete instantiation (`DbConnection_MyRS`)
  instead of assuming a non-generic interface.
- **Implicit (non-bracketed) calls to a generic free function with 2+ type
  parameters were never monomorphized**, emitting an `implicit declaration
  of function` C error — the call-site inference path only handled exactly
  one generic parameter. Added `infer_generic_targs_multi`
  (`src/codegen/c.tr`): generics that appear directly as a parameter's own
  type are inferred as before; any remaining generic (e.g. `RS` in
  `[C: DbConnection[RS], RS: DbResultSet]`, which never appears as a
  parameter's own type) is cross-referenced through the now-resolved
  concrete argument class's own `implements Iface[Concrete]` declaration
  (via the `iface_targs` above), iterated to a fixed point for chained
  bounds.
- **Module-level `pub mut`/`mut` globals had to be declared textually
  before any function in the same file that referenced them**, or name
  resolution failed with `[N-3] name 'X' is not defined` — misreported
  against whichever call site happened to trigger it first, not the
  declaration. `Sema.analyze()` processes declarations in one sequential
  pass that both registers a global's symbol and lowers function bodies in
  file order, so a function positioned earlier saw an as-yet-unregistered
  symbol. Added a pre-registration pass (`src/sema.tr`, mirroring the
  existing class/actor pre-registration) that declares every top-level
  `SLet` global's name and type — from its explicit annotation, or a
  side-effect-free literal-only inference when unannotated — before any
  function body is lowered.
- **`Dict[K,V].init()` / `.new()` (and the `Map[K,V]` alias) called with no
  arguments failed to compile**, forwarding zero arguments straight into
  `_tr_dict_new`/`_tr_idict_new`, which both require a capacity-hint
  parameter (`too few arguments to function`). Defaulted to `16`
  (`src/codegen/c.tr`), matching the identical no-arg default `Set[T].init()`
  already had two branches above it in the same function.
- **`await` on a `throws`-declared function tried to cast a `Result` struct
  to/from a scalar integer** (`aggregate value used where an integer was
  expected` / `conversion to non-scalar type requested`), since a `Result`/
  `Option`/`Tuple` return value is an aggregate, not a pointer- or
  integer-sized scalar, and the generic `void*`-channel the coroutine
  wrapper uses to pass a task's result back assumed the latter.
  `emit_async_wrapper_for_call` and `gen_await_call` (`src/codegen/c.tr`)
  now heap-allocate room for the struct, copy the return value into it, and
  hand back the pointer through that same channel, dereferencing and
  freeing it on the await side — the same pattern every other return type
  on that channel already used.
- **`fname[T](args)` (a single explicit type argument) never looked up the
  called function's actual declared return type**, instead blindly setting
  the call's result type to the type ARGUMENT itself — correct only when
  the function's return type genuinely is its own generic parameter
  (`id[T](x: T) -> T`), and silently wrong for anything else
  (`get_value[T](f: T) -> int`, always `int` regardless of `T`): a plain
  sync call like `get_value[Foo](foo)` mis-inferred `Foo` as the result
  type (an immediate, loud C compile error), while the same call reached
  through `await` corrupted the value silently instead (the mismatch never
  surfaced as a type error, since the coroutine result channel is an
  untyped `void*`). The multi-type-argument sibling case
  (`fname[T1,T2](args)`) already resolved this correctly; the single-arg
  case in `sema.tr`'s generic free-function call handling just never had
  the matching logic. Fixed by looking up the function's declared return
  type and only substituting the explicit type argument when it actually
  matches the function's own (first) generic parameter, mirroring the
  multi-arg case.
- **`async def` methods declared inside an `extend` block were not
  recognized as async at all.** `parse_extend_decl`'s method-parsing loop
  (`src/parser.tr`) had cases for `KwDef`/`KwPass`/`Dedent|Eof` but none for
  `KwAsync` — an `async` token there fell into the catch-all
  `case _: self.pos += 1`, silently discarding it; the method then parsed
  as an ordinary synchronous method (`is_async` never set on it). This was
  invisible unless the method's own body called `await` internally, in
  which case it failed with `[C-4] 'await' used outside an async function`
  on a method plainly declared `async def`. **Async methods were entirely
  unsupported before this fix — only async free functions worked.** Fixed
  by adding a `Token.KwAsync` case mirroring the top-level declaration
  parser's own handling of the same keyword.
- **`Map`/`Dict` `.get()` / `.get_or()` / `.set()` / `.free()` checked the
  raw, un-substituted generic type-PARAMETER name (e.g. literally `"T"`)
  against the str/float value-type checks instead of resolving it through
  the active monomorphization substitution first.** Inside a monomorphized
  method where a field's value type resolves concretely for that
  instantiation (e.g. `Map[str, T]` with `T=str`), the raw-name check
  always missed, and `.get()` produced `(TrStr)(uintptr_t)ptr` — casting a
  pointer directly to a non-scalar STRUCT type (`conversion to non-scalar
  type requested`). Fixed 4 call sites in `codegen/c.tr` to resolve the
  value type name via `resolve_generic_prim(...)` first, the same helper
  the adjacent float-value check at each site already (inconsistently)
  used. A pre-existing SLet-level compensating recovery for a different,
  genuinely distinct case — a bare `Dict` annotation with no value-type
  argument at all (`mut config: Dict = {...}`), which has no type
  information to resolve in the first place — was updated to use the same
  resolved check rather than the raw one, so the two mechanisms agree on
  when unboxing has already happened and a value is never double-unboxed.
- **A macro-generated plain top-level declaration's function BODY was
  never emitted into any generated `.c` file**, producing an undefined-
  reference link error despite `--check` passing cleanly and the
  prototype being correctly declared and referenced. Root cause:
  `main.tr` builds the per-module (and main-module) function/class sets
  used to decide which generated `.c` file's body-emission loop a given
  declaration belongs to from `resolver.all_decls`/`all_decl_modules`,
  snapshotted by `resolve_main` **before** `expand_macros` runs; a
  macro-spliced declaration is appended to the program's declaration list
  during macro expansion but those resolver-owned bookkeeping lists never
  learn about it, so it is absent from every module's function set and
  its body-emission is silently skipped everywhere. Fixed via a new
  `expand_macros_tracked(prog, all_decls, all_decl_modules)` (`macros.tr`)
  that pushes each newly-generated declaration into the same resolver
  lists, attributed to its triggering declaration's own module — sound
  because `prog.decls[i]` and `resolver.all_decls[i]` are the same
  pointers in the same order at this pass's entry point (`resolve_main`
  copies one into the other verbatim), so the loop index doubles as a
  valid lookup key into `all_decl_modules`. `main.tr` updated to call the
  tracked variant (the untracked `expand_macros` is kept as a thin
  wrapper for any other caller).

### Fixed (build/CI)
- **The portable-C bootstrap seed (`bootstrap/c/`) was missing the
  in-process-LLVM shim**, so every CI platform's stage0 build failed to
  link with `undefined reference to '_tr_llvm_emit_object'`.
  `runtime/tauraro_llvm.c` provides that symbol and must be copied into
  `bootstrap/c/` as `module_tauraro_llvm.c` (picked up by the stage0
  build's `module_*.c` glob) — `scripts/regen-bootstrap.sh` already did
  this, but `scripts/regen-bootstrap.ps1` never did, so a bootstrap
  regenerated on Windows silently omitted it. Added the missing file and
  brought the PowerShell script to parity with the shell one.

### Changed
- **Async/await is now a green-thread runtime.** `async def` / `await` no
  longer spawn an OS thread per `await` and block the caller. Instead each task
  is a lightweight **stackful coroutine** (Windows Fibers / POSIX `ucontext`)
  with its own small stack, and `await` is a cheap context switch driven by a
  cooperative scheduler. A single OS thread runs many tasks; `await` suspends
  the caller and resumes it with the callee's result without blocking. The
  scheduler integrates the `_TrIOPoll` reactor (epoll / IOCP-select / kqueue),
  so an `await` on a socket parks the task on fd-readiness and yields the worker
  — true non-blocking I/O, no thread blocked per connection. Nested `await`
  chains, cooperative `Coro.sleep_ms` (timer-parked) and `Coro.yield_now`, and
  reactor-based `Coro.await_readable`/`await_writable` are provided via the new
  `std/async/coro` module. (`spawn`/`task_group` keep OS-thread parallelism for
  CPU-bound work; multi-core green-thread scheduling is future work.)
- Debug builds (`--debug`) now emit C `#line N "source.tr"` directives mapping
  each generated statement back to its original Tauraro source file and line,
  so GCC diagnostics and GDB backtraces reference the `.tr` source rather than
  the generated C. Line directives are off by default (non-debug output is
  unchanged). Paths are emitted with forward slashes for cross-platform C
  string compatibility.
- Incremental compilation: the build now compiles each module's generated C to
  its own object file (`gcc -c`) and links the objects, reusing the cached `.o`
  for any module whose generated C is byte-identical to the previous build (and
  whose shared headers / compile flags are unchanged). A clean rebuild after
  editing a single module now recompiles only the affected module(s) instead of
  the whole program. The `build/` directory is kept populated between builds as
  the object cache (it is no longer wiped when `-o` is given). Verified
  correctness-preserving: an all-cache-reuse build is byte-identical to a
  from-scratch build.
- Parser diagnostics now report a precise column and a source-code snippet
  with a caret, in the style `file:line:col: error: <msg>` followed by the
  offending source line and a `^` under the exact column, then the `FIX:`
  hint. The lexer tracks per-token columns (`token_cols`) alongside lines;
  the parser is given the source text and renders the snippet. (Column info
  is diagnostic-only and never affects codegen.)

### Added
- Bidirectional FFI / library export: an `export def` function is given C-ABI
  external linkage (`__declspec(dllexport)` on Windows, default visibility on
  ELF/Mach-O via the new `TR_EXPORT` macro). The new `tauraroc --lib` mode
  builds a shared library (`.dll`/`.so`) instead of an executable (compiling
  every module `-fPIC` and linking `-shared`) and emits a self-contained C
  header declaring the exported functions, so C/Rust/etc. consumers can call
  into Tauraro code. Verified end-to-end (and in CI) by a C program linking the
  generated library through the generated header. (Exports with primitive /
  pointer signatures are fully self-contained; signatures using Tauraro struct
  types like `TrStr`/`Result` still require the runtime header — a documented
  v1 limitation.)
- `std/encoding/toml.tr`: a TOML parser and serializer (`TomlValue` tagged
  tree + `Toml.parse`/`Toml.stringify`), modeled on `std/encoding/json.tr`.
  Supports comments, bare/quoted keys, basic & literal strings, integers
  (with `_` separators) / floats / booleans, arrays, inline tables, and
  `[table]` / `[a.b.c]` nested table headers; round-trips. (YAML, a regex
  backtracking engine, and other encodings remain deferred.)
- `tauraroc fmt [-w] <file>`: a source formatter that re-emits canonically
  formatted Tauraro, **preserving comments** (the lexer records them out-of-band
  so the parser/codegen are unaffected). Standalone comments stay on their own
  line; trailing comments stay attached to their code line. Binary/unary
  sub-expressions are conservatively parenthesized so the output always
  re-parses to the same AST. The formatter is idempotent (`fmt(fmt(x)) ==
  fmt(x)`, verified on all formattable example files and in CI) and refuses to
  rewrite a file containing a construct it cannot yet render (rather than
  risk corrupting it).
- `tauraroc lint <file>`: runs module resolution + semantic analysis and
  reports warnings/errors without producing an executable.
- CI / `scripts/run_tests.*` now include a formatter idempotency check.
- `tests/lang/` and `tests/regression/` regression test suites (9 files,
  190 assertions) covering core language, collections, strings, classes &
  enums, error handling, concurrency, the #52/#54 string-collection fixes,
  the Phase 4 escape-walker variants, and the Phase 5 Dict-value retain fix,
  using `std/test`'s `TestRunner`.
- `scripts/run_tests.sh` / `scripts/run_tests.ps1` to run the suite via
  `tauraroc --run`.
- CI now runs the regression suite on every platform after building
  `tauraroc` (`.github/workflows/build.yml`).

## [0.0.8]

**Tauraro becomes a batteries-included, cross-platform toolchain.** Full notes in
[`note.md`](note.md).

### Added
- **Bundled toolchain (zig).** The release SDK ships a `zig/` folder beside the compiler
  (clang code generator + `lld` linker + a libc for every target), auto-detected at
  startup. `--backend llvm` and `--target` now work with **no system compiler, no LLVM,
  and no per-target sysroot** to install.
- **Cross-compilation — `--target <name|triple>`, on both the C and LLVM backends.**
  Linux (x86_64/arm64/arm32/riscv64), Windows (x64/arm64), macOS, Android, iOS,
  WebAssembly (`wasm`, `wasm-wasi`), and bare-metal (`embedded-arm/arm64/riscv32/riscv64`),
  or a raw LLVM triple. Hosted targets need no sysroot (zig bundles the libc). `--static`
  produces fully static binaries (musl on Linux).
- **WebAssembly is a first-class target.** `--backend llvm --target wasm-wasi` emits a
  runnable `.wasm`; the runtime gained a WASI profile (single-threaded, no sockets/subprocess).
- **`--no-heap`** — a compile-time zero-heap guarantee for bare metal: the compiler rejects
  every heap-allocating construct with `[H-1]`. New `examples/freestanding/zero_heap/`.
- **`-d <dir>`** to choose the output directory (default: the current working directory,
  consistent across all backends).

### Changed
- **LLVM backend self-hosts and is faster than C on most benchmarks** (a `getelementptr`
  codegen fix unblocked auto-vectorization: MatMul ~4×, Collatz ~3×). It compiles the whole
  compiler and emits byte-identical C to the reference build.
- **Bare-metal works via the bundled zig** (`zig cc -target thumb-freestanding-eabi …`) —
  no dedicated `arm-none-eabi-gcc` required. `TAURARO_BARE` was split into orthogonal
  `NO_LIBC`/`NO_THREADS`/`NO_NET`/`NO_SUBPROCESS` switches (bare-metal behaves identically).
- Host builds prefer a fast system `gcc`/`clang` and fall back to bundled `zig cc`.

### Fixed
- Three C-backend bugs found via the `LLVM ≡ C` differential oracle (interface retain,
  missing `_trdrop` for `free()`-owning classes, undefined `_tr_rt_str_new`); wildcard-match
  over-release in the shared LIR lowering; host-vs-target link flags (Winsock/output
  extension); the WASI `__main_void` entry glue.

### Docs
- New [Compiling, Backends & Cross-Compilation](docs/lang/22_compiling_and_cross_compilation.md)
  guide; updated intro, README, and the bare-metal guide.

## [0.0.5] - in development

### Memory model (TrStr migration, items #48-#58)
- Migrated `str`/`String` to a refcounted `TrStr` (`.data`/`.rc`) representation
  across literals, call sites, FFI/extern boundaries, and the standard library
  (`Str`, `StringBuilder`, `Fmt`, JSON/Base64/Hex/URL/DateTime).
- `List[str]` / `Dict[K,str]` / `Set[str]` specializations migrated to
  `List_TrStr` / `Dict_free_strval`, fixing use-after-free and double-free bugs
  in string-valued collections.
- Added collection escape-analysis (`coll_escaped`) and string escape-analysis
  (`str_escaped`) passes in `src/sema.tr` to drive auto-drop (RAII-lite) of
  non-escaping `List`/`Vec`/`Dict`/`Map`/`Set`/`str` locals.
- Added block-id (block-stack) tracking for correct per-branch drop insertion.
- Fixed numerous wrap-hoist leaks (`wrapstr`/`flush_wraps`/`strz`) for fresh
  TrStr temporaries in concat, method calls, and return/assign/condition
  positions.
- Added `.free()`/`.dispose()` to `Pair`, `StrPair`, `Triple`, `HttpHeader`,
  `Url`, `TimeDelta`, `DateTime`, `Date`, `Time`, `HttpConn`, `HttpResponse`,
  `JsonValue`.
- Net effect on the watax HTTP benchmark: residual per-request allocation
  dropped from multi-KB to roughly 50-300B/req with no crashes at 25k+
  requests / concurrency=500.

### Compiler / safety
- Implemented `extern "CPP":` as an ABI synonym for `extern "C":` (real C++
  interop still crosses a C boundary).
- Added `inspect(T)` compile-time reflection (types, functions, classes,
  enums, interfaces, docstrings).
- Added variadic function parameters (`args...`).
- Implemented nested (Java-style) `class`/`def`/`enum`/`interface`/`extend`
  declarations inside `main()`.
- Enforced `unsafe:` blocks around raw-pointer `.write()` calls across
  `std/` (json, map, path, collections/list, resolver, codegen).
- Improved parser error reporting with current-file tracking.

### Ecosystem
- watax (Axum-style web framework): reactor-based concurrency
  (`IOPoll`/epoll/IOCP/kqueue), thread-per-connection and reactor-pool
  listeners, templa template engine with `{% extends %}`/`{% block %}`
  inheritance, static file serving, JSON API helpers, CORS middleware,
  graceful shutdown, `TestClient`.
- taupkg package manager: semver + lockfile, local/git dependencies,
  `cp_r` packaging fixes.
- Full rewrite of `docs/lang/*`, `docs/std/*`, and new `docs/dev/*`
  contributor/internals guide.

### Build
- Fixed bootstrap binary output location (`build.sh`/`build.ps1` now move
  `src/build/tauraroc[.exe]` to the working directory if the compiler still
  places it there).
- Self-locating compiler (`_tr_exe_dir()`) so bare-name PATH invocation finds
  `runtime/`/`std/` relative to the executable.

## [0.0.2] - tagged

- Tagged early bootstrap milestone (see `git tag v0.0.2`).

## [0.0.1] - tagged

- Initial tagged bootstrap milestone (see `git tag v0.0.1`).

[Unreleased]: https://github.com/Yusee-Programmer/tauraro/compare/v0.0.2...HEAD
[0.0.5]: https://github.com/Yusee-Programmer/tauraro/compare/v0.0.2...HEAD
[0.0.2]: https://github.com/Yusee-Programmer/tauraro/releases/tag/v0.0.2
[0.0.1]: https://github.com/Yusee-Programmer/tauraro/releases/tag/v0.0.1
