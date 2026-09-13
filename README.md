<div align="center">
  <img src="assets/tauraro-img.jpg" alt="Tauraro Logo" width="180" style="border-radius: 16px;"/>

  <h1>Tauraro</h1>
  <p><strong>Compiled · Statically Typed · Python Syntax · C Performance</strong></p>

  <p>
    <img src="https://img.shields.io/badge/license-MIT%20%2F%20Apache%202.0-blue?style=flat-square" alt="License"/>
    <img src="https://img.shields.io/badge/version-v0.0.8-brightgreen?style=flat-square" alt="Version"/>
    <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS%20%7C%20Android(termux)-lightgrey?style=flat-square" alt="Platform"/>
    <img src="https://img.shields.io/badge/backends-C%20%C2%B7%20LLVM%20%C2%B7%20WASM%20%C2%B7%20bare--metal-red?style=flat-square" alt="Backends"/>
    <img src="https://img.shields.io/badge/self--hosted-yes-purple?style=flat-square" alt="Self-hosted"/>
  </p>

  <p>
    <a href="docs/lang/README.md"><strong>Documentation</strong></a>
    &nbsp;·&nbsp;
    <a href="examples/"><strong>Examples</strong></a>
    &nbsp;·&nbsp;
    <a href="https://github.com/tauraro/tauraro/releases"><strong>Releases</strong></a>
    &nbsp;·&nbsp;
    <a href="https://github.com/tauraro/tauraro/issues"><strong>Issues</strong></a>
  </p>
</div>

---

## What Is Tauraro?

Tauraro is a compiled, statically-typed language with Python-style indentation syntax — Python's readability with performance close to hand-written C.

It ships as a **batteries-included, cross-platform toolchain**: one download compiles for your machine *and* cross-compiles to Linux/Windows/macOS, ARM/RISC-V, **WebAssembly**, and **bare-metal firmware** — with **nothing else to install**. Two code generators back it: the default **C** backend (widest coverage, `gcc`/`clang`) and an optimizing **LLVM** backend (auto-vectorizing, often faster than C). A bundled **zig** provides clang + `lld` + a libc for every target, so `--backend llvm` and `--target <anything>` work straight out of the box. See **[Compiling, Backends & Cross-Compilation](docs/lang/22_compiling_and_cross_compilation.md)**.

```python
def greet(name: str) -> str:
    return f"Hello, {name}!"

def main():
    print(greet("world"))
```

---

## Installation

Download the latest binary from the [Releases](https://github.com/tauraro/tauraro/releases) page:

<div align="center">

| Platform | File |
|----------|------|
| Windows (x64) | `tauraroc-windows-x64.zip` |
| Linux (x64) | `tauraroc-linux-x64.zip` |
| Linux (arm64) | `tauraroc-linux-arm64.zip` |
| macOS (arm64) | `tauraroc-macos-arm64.zip` |

</div>

Extract and place `tauraroc` (or `tauraroc.exe` on Windows) somewhere on your `PATH`.

**No prerequisites.** The release SDK bundles a full toolchain (a `zig/` folder beside the
binary = clang + `lld` + a libc for every target), auto-detected at runtime — so
`--backend llvm` and cross-compilation work with **nothing else installed**. A system
`gcc`/`clang`, if present, is preferred for faster *host* builds but isn't required.

Verify your installation:

```sh
tauraroc --version
# tauraroc v0.0.8
```

---

## Quick Start

**hello.tr**
```python
def main():
    print("Hello, world!")
```

```sh
tauraroc --run hello.tr
```

---

## Language Features

<div align="center">

| Feature | Description |
|---------|-------------|
| **Classes** | Method dispatch, inheritance (`extends`), interfaces, operator overloading |
| **Enums** | Tagged unions with pattern matching |
| **Generics** | Monomorphized at compile time — no boxing |
| **F-strings** | `f"result = {value}"` — zero overhead |
| **Ownership** | Automatic memory management, no GC |
| **Error handling** | `Result[T,E]`, `throws`, `?` operator |
| **Concurrency** | `spawn`, `task_group:`, `await_all`, `Thread.spawn`, `AsyncPool` (work-stealing, multi-core), `Chan[T]`, `Mutex[T]`, `Atomic[T]` |
| **Data race safety** | `Sendable` interface enforced at compile time on all spawn/thread boundaries |
| **Unsafe** | `unsafe:`, `Pointer[T]`, inline `asm()` |
| **GPU** | `gpu:` blocks → OpenMP parallel loops |
| **FFI** | `extern "C"` for calling C libraries |
| **Closures** | First-class anonymous functions with capture |

</div>

---

## CLI Reference

```
tauraroc <file.tr> [options]
tauraroc fmt [-w] <file.tr>     Format source (stdout, or -w in place)
tauraroc lint <file.tr>         Analyze and report warnings/errors

  --version              Print compiler version and exit
  --run                  Compile and immediately execute
  --check                Semantic analysis only, no output
  --backend c|llvm|native  Code generator (default: c)
  --target <name|triple> Cross-compile: linux-arm64, windows-x64, macos-arm64,
                         android-*, ios, wasm-wasi, embedded-arm/riscv*, … or a raw triple
  --static               Statically link (musl on Linux)
  --sysroot <path>       Override the cross toolchain's sysroot
  -o <path>              Exact output path (dir honored; overrides -d)
  -d <dir>               Output directory (default: current dir)
  --lib                  Build a shared library (.so/.dll) + a C header
  --emit c|ast|mir       Emit generated C / AST / MIR and stop
  --emit-ld <path>       Also write a linker script (bare-metal @entry)
  -O0/-O1/-O2/-O3        Optimization level (default: -O2)
  -Os                    Optimize for size
  --debug                AddressSanitizer + bounds-check assertions
  --strict               Alloc/dealloc outside `unsafe:` is an error [U-1]
  --no-heap              Reject all heap-allocating constructs [H-1]
  --no-std               `alloc` tier — pluggable allocator, no OS services
  --freestanding         `core` tier — no OS, no libc (bare metal)
  --link <path>          Link a file (.c/.o/.a/.dll/.so)
  -l<name>               Link a library by name (e.g. -luser32)
  --verbose              Show all pipeline phases
```

Full details, the target matrix, static binaries, WebAssembly, and bare-metal:
**[Compiling, Backends & Cross-Compilation](docs/lang/22_compiling_and_cross_compilation.md)**.

---

## Example Program

```python
class Counter:
    pub total: i64

extend Counter:
    pub def init(n: i64) -> Counter:
        mut c = Counter()
        c.total = n
        return c

    pub def add(self, n: i64) -> void:
        self.total = self.total + n

    pub def show(self) -> void:
        print(f"total = {self.total}")

def main():
    mut c = Counter.init(0)
    for i in range(10):
        c.add(i)
    c.show()    # total = 45
```

---

## Compiler Pipeline

```
.tr source
    │
    ▼
  Lexer          tokenize source text
    │
    ▼
  Parser         build AST
    │
    ▼
  Sema           type-check, resolve names
    │
    ▼
  HIR            typed intermediate representation
    │
    ├─────────────┬───────────────────┐
    ▼             ▼                   ▼
  C Codegen    LLVM Codegen        (native x86-64, WIP)
    │             │
    ▼             ▼
  gcc/clang/    clang/llc/          ← bundled zig provides all of these +
  zig cc        zig cc                a libc for every --target
    │             │
    └──────┬──────┘
           ▼
     Executable / .wasm / bare-metal firmware
```

All stages are written in Tauraro itself — the compiler is **fully self-hosted** (the
LLVM backend compiles the whole compiler and emits byte-identical C to the reference C build).

---

## Performance

Benchmarks run on Linux x86_64 with `gcc -O3` (C), `rustc -C opt-level=3 -C target-cpu=native` (Rust), and `tauraroc -O3` (Tauraro → C → `gcc -O3 -march=native -funroll-loops`). Wall-clock seconds, lower is better.

| Benchmark | C (s) | Rust (s) | Tauraro (s) | Tau/C | Tau/Rust |
|-----------|------:|---------:|------------:|------:|---------:|
| Fibonacci 1B | 0.313 | 0.311 | 0.311 | **0.99×** | **1.00×** |
| Float Multiply 1B | 0.933 | 0.934 | 0.934 | **1.00×** | **1.00×** |
| XOR Shift PRNG 1B | 1.866 | 1.867 | 1.870 | **1.00×** | **1.00×** |
| Newton Sqrt 1B | 6.063 | 6.046 | 6.053 | **1.00×** | **1.00×** |
| Mandelbrot 800×800 | 0.442 | 0.441 | 0.428 | **0.97×** | **0.97×** |
| N-Body 10M | 0.286 | 0.284 | 0.289 | **1.01×** | **1.02×** |
| Sieve 50M | 0.172 | 0.182 | 0.265 | 1.54× | 1.46× |
| Matrix Multiply 400×400 | 0.015 | 0.012 | 0.033 | 2.20× | 2.75× |

Tauraro runs at **C/Rust parity** on the scalar compute kernels (within ~3%), and stays leaner than Rust on memory across the board. Sieve and MatMul are still slower (cache/aliasing-bound). Full results, including peak memory, in [`benchmarks/README.md`](benchmarks/README.md).

---

## Documentation

The full language reference lives in [`docs/lang/`](docs/lang/):

| # | Topic |
|---|-------|
| 01 | [Introduction & CLI](docs/lang/01_intro.md) |
| 02 | [Variables & Types](docs/lang/02_variables_and_types.md) |
| 03 | [Operators](docs/lang/03_operators.md) |
| 04 | [Control Flow](docs/lang/04_control_flow.md) |
| 05 | [Functions & Closures](docs/lang/05_functions.md) |
| 06 | [Strings & F-Strings](docs/lang/06_strings.md) |
| 07 | [Collections](docs/lang/07_collections.md) |
| 08 | [Classes & Extend](docs/lang/08_classes.md) |
| 09 | [Enums](docs/lang/09_enums.md) |
| 10 | [Interfaces](docs/lang/10_interfaces.md) |
| 11 | [Generics](docs/lang/11_generics.md) |
| 12 | [Error Handling](docs/lang/12_error_handling.md) |
| 13 | [Memory & Ownership](docs/lang/13_memory_and_ownership.md) |
| 14 | [Unsafe & Pointers](docs/lang/14_unsafe_and_pointers.md) |
| 15 | [Modules](docs/lang/15_modules.md) |
| 16 | [Concurrency](docs/lang/16_concurrency.md) |
| 17 | [Extern & FFI](docs/lang/17_extern_and_ffi.md) |
| 18 | [GPU & Inline Assembly](docs/lang/18_gpu_and_asm.md) |
| 19 | [Compiler Error Reference](docs/lang/19_compiler_errors.md) |
| 20 | [Advanced Patterns](docs/lang/20_advanced_patterns.md) |
| 21 | [Operator Overloading](docs/lang/21_operator_overloading.md) |

---

## License

Tauraro is dual-licensed under your choice of:

- **MIT License** — see [`LICENSE-MIT`](LICENSE-MIT)
- **Apache License, Version 2.0** — see [`LICENSE-APACHE`](LICENSE-APACHE)

You may use, distribute, and modify Tauraro under the terms of either license.

<div align="center">

<sub>Built with ❤️ — Python syntax · C performance</sub>

</div>
