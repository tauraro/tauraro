# 10 — Wrapping a C/C++ Library as a Zero-Link Tauraro Package

---

## What This Covers

You have an existing C or C++ library and want Tauraro code to call it —
and you want a **consumer** of your package to never type `--link`,
`-lfoo`, or an `-I`/`-L` flag. They just add your package as a dependency
and `import`.

There are two starting situations, and each has a different zero-link
strategy:

| Situation | Strategy | Section |
|---|---|---|
| You want the library **bundled inside your package** — no system install required on the consumer's machine | Vendor the library's source, bindgen it, auto-link the vendored source | [Scenario A](#scenario-a-vendoring-bundling-the-library) |
| The library is (or will be) **already installed on the OS** — a system package, `apt`/`brew`/`pacman`-installed, etc. | bindgen against the installed headers, auto-link via `pkg-config` (or a hand-pinned link flag) | [Scenario B](#scenario-b-a-system-installed-library) |

Both end the same way: a normal [taupkg library](05_building_libraries.md)
that a consumer depends on and imports, with **zero manual linking** at
every step. This guide assumes you're already familiar with
[24 — Bindgen](../lang/24_bindgen.md); it doesn't repeat bindgen's full
flag reference, just how to point its output at the right auto-link
strategy for each situation.

---

## The auto-link mechanism, in one paragraph

Every `.tr` file bindgen writes carries machine-readable pragma comments in
its header:

```
# tauraro-cpp-shim: foo_shim.cpp        # a shim/vendored source to compile
# tauraro-cpp-lib: stdc++               # library name(s) to link (-l<name>)
# tauraro-cpp-linkflags: -L… -lfoo      # raw linker flags, appended verbatim
# tauraro-cpp-cflags: -std=c++17 -I…    # flags to compile the shim
```

When `tauraroc` compiles a program that imports such a module, it scans
every loaded module for these pragmas (`auto_link_cpp_shims` in
`src/main.tr`), compiles any referenced shim/source **once** (cached by
content), and adds everything to the link line automatically. A `.c` file
with no *unconditional* `extern "C"` block is compiled with the plain C
driver (no libstdc++ needed); anything else goes through the C++ driver.
This is the one mechanism both scenarios below build on — Scenario A points
`tauraro-cpp-shim` at a **vendored source file**, Scenario B leaves it empty
and relies on `tauraro-cpp-linkflags` alone.

---

## Scenario A: Vendoring (Bundling) the Library

Use this when you want your package to be self-contained — no system
install, no version-mismatch risk, works identically on every machine that
can run a C compiler. This is how a package like a SQLite or zlib wrapper
should ship: nobody should need `apt install libsqlite3-dev` just to use
your Tauraro binding.

### 1. Vendor the library's source

Drop the library's own source into your package, unmodified, next to (not
inside) your Tauraro sources:

```
mylib/
  headers/
    thelib.h          # the library's public header, unpatched upstream
    thelib.c           # the library's own source (or a full amalgamation)
    VERSION.md           # pinned upstream version, download URL, date
  src/
    mylib.tr              # re-export hub
    raw.tr                  # bindgen output lives here (next section)
```

A single-file "amalgamation" (SQLite, some zlib forks, many small C
libraries) is the easiest case — one `.c` to vendor and auto-link. A
multi-file library needs a bit more (see
[Multi-file vendored sources](#multi-file-vendored-sources) below).

### 2. Generate the raw bindings

```sh
tauraroc bindgen headers/thelib.h -o src/raw.tr --cc gcc
```

Plain **C mode** (no `-h cpp`) for a C library. For a C++ library, add
`-h cpp` (and see [24 — Bindgen's C++ section](../lang/24_bindgen.md#binding-c-headers--h-cpp)
for the libclang-shim path) — bindgen already emits a `tauraro-cpp-shim`
pragma pointing at *its own* generated shim in that case. Skip `--pkg` here
— you're not depending on anything system-installed, so there's nothing for
pkg-config to discover.

### 3. Point the auto-link pragma at your vendored source

This is the one step bindgen's CLI has no flag for — add two lines to the
top of `src/raw.tr` (right after the `# Auto-generated FFI bindings...`
line bindgen already wrote):

```
# tauraro-cpp-shim: ../headers/thelib.c
# tauraro-cpp-cflags: -DTHELIB_SOME_FEATURE=1
```

The path is relative to `raw.tr`'s own directory. `tauraro-cpp-cflags` is
optional — only needed if the library has compile-time feature flags you
want baked in.

**Why this works for a plain C library specifically:** `auto_link_cpp_shims`
classifies a referenced `.c` file as C-only (compiled with the plain C
driver, no C++ runtime pulled in) unless it contains an *unconditional*
`extern "C" { ... }` block. A library written to be usable from both C and
C++ typically guards that block with `#ifdef __cplusplus`, which does
**not** count as unconditional — so a normal, portable C library vendors and
auto-links cleanly with zero extra work here.

### 4. Verify before building anything on top

Compile a throwaway program against just `raw.tr` with a **plain**
`tauraroc app.tr -o app` — no `--link`, no flags — and confirm the vendored
source actually compiles and links:

```python
from raw import *

def main():
    # call one real function from the library here
    print(thelib_version() as str)
```

This is the highest-risk step (the first proof the vendored source and its
pragma are wired correctly) — worth confirming before writing any
higher-level API on top of `raw.tr`.

### 5. Build the high-level API, then package with taupkg

Wrap the raw 1:1 bindings in an idiomatic API (see
[05 — Building Libraries](05_building_libraries.md) for the package layout:
a `src/<pkgname>.tr` re-export hub, `taupkg.toml` with `bin =
"src/<pkgname>.tr"`). A consumer's whole integration is:

```toml
# consumer's taupkg.toml
[deps]
mylib = "github:you/mylib"
```
```python
from mylib import Thing   # taupkg build compiles + auto-links thelib.c — nothing else to do
```

### Multi-file vendored sources

If the library isn't a single-file amalgamation, either produce one (many
C libraries ship an amalgamation build or one is easy to script), or vendor
each `.c` file and add one `tauraro-cpp-shim` pragma line per file — the
pragma is repeatable, and each referenced file is compiled and cached
independently:

```
# tauraro-cpp-shim: ../headers/thelib_core.c
# tauraro-cpp-shim: ../headers/thelib_util.c
```

### Keeping the vendored copy upgradable

Write a `scripts/regen-bindings.ps1`/`.sh` that re-downloads the vendored
source, re-runs `bindgen`, and re-applies the pragma patch in one command —
the patch step is the only part that doesn't survive a `bindgen` re-run, so
scripting it now saves a manual re-edit on every future version bump.

---

## Scenario B: A System-Installed Library

Use this when the library is expected to already be present on the
consumer's machine — a system package most users already have (OpenSSL,
libcurl, a database client library, ...), or one they're expected to
install via their platform's package manager. Rather than vendoring
megabytes of someone else's source, you bind against the **installed**
headers and auto-link against the **installed** library.

### The cross-platform way: `--pkg`

Most system C/C++ libraries ship a [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)
`.pc` file. `--pkg <name>` queries it for both the compile flags (parses
correctly without you hand-passing `-I`/`-D`) and the link flags — which get
recorded in the generated binding and auto-linked, with no `--pkg` needed
again at the consumer's build:

```sh
tauraroc bindgen /usr/include/curl/curl.h -o curl.tr --pkg libcurl
```

This is the **one command** that does everything: parses the header
correctly, AND writes the `# tauraro-cpp-linkflags: -L… -lcurl` pragma so
every future build of every consumer auto-links it. Nothing else to do —
skip straight to packaging with taupkg.

`pkg-config` itself (the tool, not the library you're binding) needs to be
on `PATH` on whichever machine **generates** the binding — this is a
one-time, author-side step, not something your consumers need:

| Platform | Install `pkg-config` |
|---|---|
| Debian/Ubuntu | `apt install pkg-config` |
| Fedora | `dnf install pkgconf-pkg-config` |
| macOS (Homebrew) | `brew install pkg-config` |
| Windows (MSYS2) | `pacman -S pkg-config` |

Consumers only need the **library itself** installed (its headers aren't
needed at all post-bindgen; its shared library/import lib is needed at link
time, same as any C program depending on it) — `pkg-config` is not a
runtime or build dependency of *their* project, only of the one-time
binding-generation step you ran.

### No `.pc` file available

Some libraries (more common on Windows, or older/smaller C libraries) don't
ship pkg-config metadata. Two options, in order of preference:

**1. The library resolves via the linker's default search path everywhere
you target.** Bind normally with explicit `-I`/`-D` (no `--pkg`), then
hand-patch a link-flags pragma the same way Scenario A hand-patches a shim
path:

```sh
tauraroc bindgen /usr/include/thelib.h -o thelib.tr -I/opt/thelib/include
```
```
# tauraro-cpp-linkflags: -lthelib
```

`-lfoo` alone (no `-L`) works when the library lives in a directory the
linker already searches by default — true for most Linux distro packages
(`/usr/lib`), Homebrew's linked prefix, and MSYS2's `/mingw64/lib`. This
covers a real majority of system libraries in practice, even without
`pkg-config`.

**2. The install location genuinely varies by platform/package manager**
(a fixed, non-default path — common for Windows libraries installed via
vcpkg, or a library with no consistent packaging story). The pragma is
static text with no per-platform conditional syntax, so there is no way to
make *this specific case* zero-link across every platform from one binding.
Prefer **[Scenario A](#scenario-a-vendoring-bundling-the-library)** instead
if the library is small enough to vendor — it sidesteps the problem
entirely and is the more robust choice whenever the library's own build is
simple enough to bundle. If vendoring isn't practical (a very large
library), document the required `-L<path>` for each platform you support
and accept that those consumers pass it via `--link`/their own build flags
until they set up pkg-config for it locally.

### Verify, then package

Same as Scenario A steps 4–5: prove the plain `tauraroc app.tr -o app`
(no flags) actually links against the installed library, then wrap the raw
bindings in an idiomatic API and package with taupkg.

---

## Using `taupkg` to Distribute the Result

Either scenario above ends at the same place: an ordinary Tauraro package.
Nothing about the auto-link pragmas is taupkg-specific — they work whether
a consumer compiles directly with `tauraroc` or through `taupkg build`.
What taupkg adds is dependency resolution and vendoring, so a consumer
never touches your package's internals at all:

```toml
# your package's taupkg.toml
[package]
name    = "mylib"
version = "0.1.0"
bin     = "src/mylib.tr"   # pure library — see 05_building_libraries.md
```

```toml
# a consumer's taupkg.toml
[deps]
mylib = "github:you/mylib"        # or "local:../mylib" while developing together
```

```sh
taupkg install   # fetches/vendors mylib (and, for Scenario A, its bundled C/C++ source)
taupkg build     # compiles the consumer AND auto-links mylib's shim/vendored source
```

No `--link`, no `-lfoo`, no `-I`, at any layer, for the consumer — exactly
the property both scenarios above were built to guarantee end to end.

See the [taupkg repository](https://github.com/tauraro/taupkg) for the full
CLI reference, the manifest format, and how to publish a package to the
registry. [05 — Building Libraries](05_building_libraries.md) covers the
taupkg package-layout conventions this guide assumes (the `src/<pkgname>.tr`
hub, `local:`/`github:` dependency sources, and the `TAURARO_PATH` escape
hatch for ad-hoc compiles outside a taupkg sandbox).

---

Previous: [GPU Kernel Backend ←](09_gpu_backend.md)
