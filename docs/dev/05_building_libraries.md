# 05 — Building Libraries

---

## What This Covers

This guide is for **library authors**: people writing a Tauraro package that
other projects will depend on (a web framework, a templating engine, a data
structure library, etc.), as opposed to a standalone application.

It covers:
- The `taupkg.toml` manifest format
- The directory layout convention for a library package
- Local path (`local:`) dependencies for developing two projects together
- `TAURARO_PATH` for resolving imports outside the canonical layout
- A minimal worked example: building `mylib` and consuming it from another project
- The full `taupkg` CLI command reference

**When to use this:** Any time you're splitting reusable code out of an
application into its own package, or starting a new package that's meant to
be imported by name (`from mylib import Thing`) rather than run directly.

---

## The `taupkg.toml` Manifest

Every taupkg package has a `taupkg.toml` file at its root. `taupkg` walks up
from the current directory (up to 16 levels) looking for this file — see
`find_manifest` in `taupkg/src/manifest.tr`.

### Fields

| Section | Field | Required | Notes |
|---------|-------|----------|-------|
| `[package]` | `name` | Yes | Package name. Must be non-empty for `Manifest.is_valid()` to pass. |
| `[package]` | `version` | Yes | Version string (e.g. `"0.1.0"`). Also required for validity. |
| `[package]` | `desc` | No | One-line description. Defaults to `""`. |
| `[package]` | `license` | No | Defaults to `"MIT"` if omitted. |
| `[package]` | `bin` | No | Entry-point `.tr` file, relative to the project root. Defaults to `"src/main.tr"` if empty. **For a pure library, point this at the library module — `src/<pkgname>.tr` (e.g. `src/watax.tr`) — not a `main.tr`.** `taupkg build` then compile-checks that module (tauraroc synthesizes the program entry for a module that defines no `main`). |
| `[package]` | `authors` | No | Currently parsed as a single string value pushed into a `Vec[str]` (one entry, even if the TOML value is an array-looking string). |
| `[deps]` | `<name> = "<source>"` | No | Runtime dependencies. Key = package name, value = source URI (see below). |
| `[build-deps]` | `<name> = "<source>"` | No | Build-time-only dependencies. Same source URI forms as `[deps]`. |
| `[sources]` | `<name> = "<source>"` | No | Extra named sources (used alongside `[deps]`/`[build-deps]`). |

**`is_valid()`** only checks `name` and `version` are non-empty — everything
else is optional and defaults sensibly.

### Annotated Example: `watax/taupkg.toml`

```toml
[package]
name    = "watax"
version = "0.1.0"
desc    = "A fast, ergonomic web framework for Tauraro."
license = "MIT"
# A pure library's entry point IS its library module (watax.tr) — not a main.tr.
bin     = "src/watax.tr"

[deps]
templa = "github:Yusee-Programmer/templa"
```

- `name`/`version` — required identity of the package.
- `bin = "src/watax.tr"` — watax is a **pure library**: its entry point is the
  library module `watax.tr`, not a `main.tr`. Consumers write
  `from watax import App, ...` (which resolves to `src/watax.tr` — see "Directory
  Layout" below), and `taupkg build` compile-checks that module. A package only
  needs a `main.tr` when it also ships a runnable binary/CLI.
- `[deps] templa = "github:..."` — watax depends on the `templa` templating
  engine, fetched from GitHub. See "Source URI Forms" below.

### Source URI Forms (for `[deps]` / `[build-deps]` / `[sources]`)

Parsed by `parse_source` in `taupkg/src/source.tr`:

| Form | Example | Meaning |
|------|---------|---------|
| `local:<path>` | `local:C:\Users\me\templa` or `local:../templa` | Copy a local directory in as the dependency. |
| `github:user/repo[@ref]` | `github:taupkg-packages/templa@v1.0.0` | Clone from GitHub at a tag/branch (default branch if `@ref` omitted). |
| `git:<url>[@ref]` | `git:https://example.com/x.git@main` | Arbitrary git URL. |
| `https://github.com/...` | plain GitHub URL, `/tree/<ref>`, or `@<ref>` | Same as `github:`, parsed from a normal URL. |
| `https://.../archive/...` / `.tar.gz` / `.zip` / `/releases/download/...` | — | Downloaded and extracted as an archive (via `curl`/`tar`). |
| `archive:<url>` | `archive:https://example.com/pkg.tar.gz` | Explicit archive download. |
| anything else (e.g. `^1.2.0`, `1.0.0`) | `templa = "^1.0.0"` | Treated as a **registry** version constraint (see "Current Limitations"). |

---

## Directory Layout Convention

A Tauraro library follows the same module-naming rules as any project (see
[15 — Modules](../lang/15_modules.md)), but with one extra convention: a
**re-export hub file named after the package**, at `src/<pkgname>.tr`. This
is the equivalent of a `mod.tr` for the whole package — it's the file every
consumer imports from.

### Layout

```
mylib/
  taupkg.toml          # package manifest
  src/
    mylib.tr            # ← re-export hub: `from mylib import X` resolves here
    main.tr              # optional: a runnable demo/CLI for this package itself
    foo.tr               # implementation submodule
    bar.tr               # implementation submodule
  example/
    app/                 # a separate consumer project demonstrating usage
      taupkg.toml
      src/main.tr
  tests/
    test_foo.tr
```

### Why `src/<pkgname>.tr` and not `src/mod.tr`

`mod.tr` works for namespacing *within* a project (a directory `math/mod.tr`
becomes importable as `math`). For a taupkg package, the *package name* and
the *resolvable import path* need to line up once the package is vendored.
`taupkg build` copies a dependency's directory in as `<project>/<pkgname>/`
(containing that package's own `src/`), and the compiler's module resolver
specifically looks for `<pkgname>/src/<pkgname>.tr` to satisfy
`from <pkgname> import X` (see "Module Resolution" in
[15 — Modules](../lang/15_modules.md)). So:

```
from mylib import Thing
```

resolves to `<vendored>/mylib/src/mylib.tr`, which should `from mylib.foo
import Thing` (or define `Thing` directly) and re-export it.

### Worked Example: `watax`

watax's `src/watax.tr` is exactly this hub. It doesn't implement anything
itself — it just re-exports from its submodules:

```python
# watax/src/watax.tr
from templa import Context

from route import Route
from static_files import StaticMount, content_type_for, serve_static
from json_ext import Json
from template_ext import render_string
from http_ext import HttpRequest
from app import App, Router
from threaded import ConnJob
from reactor import ReactorWorker
from middleware import request_logger_after, set_cors_origin, cors_mw, ...
from config import Config
from test_client import TestClient
```

watax's full layout:

```
watax/
  taupkg.toml            # name="watax", deps: templa = "local:...\templa"
  src/
    watax.tr               # re-export hub — consumers `from watax import App, ...`
    app.tr                  # App/Router builder
    route.tr                # Route matching
    reactor.tr              # select()-based event loop
    threaded.tr             # thread-per-connection mode
    static_files.tr         # static file serving
    json_ext.tr             # JSON response helpers
    template_ext.tr         # templa integration
    http_ext.tr             # HttpRequest helpers
    middleware.tr           # logging/CORS/rate-limit middleware
    config.tr               # Config.from_env()
    test_client.tr          # in-process TestClient for tests
    main.tr                 # watax's own demo entry point (bin = "src/main.tr")
  example/
    app/                    # "watax-notes" — a full reference app
      taupkg.toml            # depends on watax AND templa via local: paths
      src/main.tr
      templates/              # templa templates (layout, index, about, 404)
      static/                 # css/js
```

The `example/app` directory is a **separate taupkg project** with its own
`taupkg.toml`, `src/`, and `.taupkg/` build sandbox. It depends on `watax`
itself via `local:`, exactly as an external consumer would — this is how a
library author dogfoods their own package.

---

## Local Path Dependencies (`local:`)

While developing a library alongside the project(s) that consume it, point
`[deps]` at the library's directory on disk with `local:`:

```toml
[deps]
templa = "local:C:\\Users\\Yusee Habibu\\tauProject\\templa"
```

- The path can be absolute (as above) or relative (`local:../templa`).
- On Windows, backslashes inside a TOML string must be escaped (`\\`), or use
  forward slashes (`local:C:/Users/me/templa`).

### What `taupkg install` / `taupkg build` do with a `local:` dep

From `taupkg/src/installer.tr`, for `kind == "local"`:

1. The dependency's directory is **copied** (`cp_r`) into
   `.taupkg/packages/<name>/` inside the consuming project.
2. Its `taupkg.toml` is read to record the installed version.
3. A checksum of that `taupkg.toml` is stored in the lockfile.

From `taupkg/src/builder.tr`, `taupkg build` then:

1. Wipes and recreates `.taupkg/build/`.
2. Copies the project's own `src/` to `.taupkg/build/src/`.
3. For each locked package (including local ones), copies
   `.taupkg/packages/<name>/` to `.taupkg/build/<name>/` — so the build
   sandbox looks like:

```
.taupkg/build/
  src/            # the project's own source
  templa/         # vendored dependency, including templa/src/
  watax/          # vendored dependency, including watax/src/
```

4. Invokes the compiler on `.taupkg/build/<bin>` (default
   `.taupkg/build/src/main.tr`).

This is the layout the module resolver's "vendored taupkg dependency" rule
matches: it scans the entry file's parent directory for subdirectories
containing a `src/` folder, and adds each `<pkgname>/src` as a search path —
so `from templa import Context` inside `watax/src/watax.tr` resolves to
`.taupkg/build/templa/src/templa.tr` automatically, with **no extra
configuration**.

**Important caveat:** because `local:` deps are *copied*, not symlinked,
`taupkg build` does not pick up source edits to the dependency until you
re-run `taupkg install` (or `taupkg build`, which re-vendors from
`.taupkg/packages/`) — re-copying happens on every `prepare_sandbox` call in
`build`, but `.taupkg/packages/<name>/` itself is only refreshed by
`install`/`add`/`update`. If you edit the library's source directly, re-run
`taupkg install` before `taupkg build` to pick up the changes.

---

## `TAURARO_PATH` for Ad-Hoc Layouts

The vendored-dependency auto-discovery above only kicks in for the
`taupkg build` sandbox layout (`<project>/<pkgname>/src/...` siblings next to
the entry file). If you're compiling a library's source directly with
`tauraroc` — outside of `taupkg build`, e.g. to run a quick test file that
sits next to `src/` but isn't part of a `.taupkg/build/` sandbox — you need
to tell the compiler where to find:

1. The library's **own** sibling modules (its `src/` directory), and
2. Any of **its dependencies'** `src/` directories (e.g. `templa/src/`).

Set `TAURARO_PATH` to a `;`-separated (Windows) or `:`-separated (POSIX) list
of extra search directories — see
[15 — Modules: `TAURARO_PATH`](../lang/15_modules.md#tauraro_path--extending-the-search-path).

```powershell
# PowerShell — correct: use $env:, ';' separator
$env:TAURARO_PATH = "C:\Users\me\tauProject\watax\src;C:\Users\me\tauProject\templa\src"
tauraroc test_foo.tr -o test_foo.exe
```

**Common mistake (PowerShell):** using `set` instead of `$env:` does **not**
export the variable to the child `tauraroc` process — `set` in PowerShell
only creates a session-scoped alias/variable in some shells, and is not the
same as setting an environment variable. Always use:

```powershell
$env:TAURARO_PATH = "..."
```

not:

```powershell
set TAURARO_PATH=...     # WRONG on PowerShell — this is cmd.exe syntax
```

**Best practice:** prefer the `taupkg build` sandbox (with `local:` deps in
`taupkg.toml`) for anything you intend to ship or test as a consumer would.
Reach for `TAURARO_PATH` only for one-off compiler invocations during library
development itself.

---

## Minimal Worked Example: `mylib`

### 1. Create the library

```
mylib/
  taupkg.toml
  src/
    mylib.tr
    greet.tr
```

```toml
# mylib/taupkg.toml
[package]
name    = "mylib"
version = "0.1.0"
desc    = "Example library"
license = "MIT"
bin     = "src/main.tr"

[deps]
# (none yet)
```

```python
# mylib/src/greet.tr
pub def greet(name: str) -> str:
    return "Hello, " + name + "!"
```

```python
# mylib/src/mylib.tr — re-export hub
from greet import greet
```

(`src/main.tr` is optional — only needed if `mylib` itself is runnable, e.g.
for its own examples/tests. A pure library can omit `bin` and `src/main.tr`
entirely; `taupkg build`/`run` simply won't have anything to compile, but
`taupkg install` from a consumer still works.)

### 2. Create a consumer project that depends on `mylib` via `local:`

```
myapp/
  taupkg.toml
  src/
    main.tr
```

```toml
# myapp/taupkg.toml
[package]
name    = "myapp"
version = "0.1.0"
desc    = ""
license = "MIT"
bin     = "src/main.tr"

[deps]
mylib = "local:../mylib"
```

```python
# myapp/src/main.tr
from mylib import greet

def main():
    print(greet("Tauraro"))
```

### 3. Install and build

```bash
cd myapp
taupkg install     # copies ../mylib into .taupkg/packages/mylib/
taupkg build       # vendors it into .taupkg/build/mylib/, compiles
./myapp.exe        # -> "Hello, Tauraro!"
```

`taupkg install` records `mylib` in `taupkg.lock` with its version
(`0.1.0`, read from `mylib/taupkg.toml`) and a checksum of that manifest.

---

## `taupkg` CLI Command Reference

From `taupkg/src/cli.tr`:

| Command | Description |
|---------|-------------|
| `init [name]` | Create a new `taupkg.toml` in the current directory. |
| `add <pkg>[@constraint]` | Add a dependency and install it. |
| `remove <pkg>` | Remove a dependency. |
| `install` | Install all deps from `taupkg.toml` (respects `taupkg.lock`). |
| `update [pkg]` | Update all (or one) dependency to the newest compatible version. |
| `build [--release] [-o <file>]` | Vendor deps into `.taupkg/build/` and compile. `--release` adds `-O3`. |
| `run [--release] [-- args...]` | Build, then run the resulting binary, forwarding `args` after `--`. |
| `test` | Build and run tests. |
| `search <query>` | Search the package registry. |
| `publish` | Publish this package to the registry. |
| `info <pkg>` | Show information about an installed or registry package. |
| `list` | List installed packages. |
| `clean` | Remove `.taupkg/build/`. |
| `version` | Print the taupkg version. |
| `install-tauraro [--version x.y.z] [--mirror]` | Download and install the Tauraro compiler itself. |

Global options: `--verbose`/`-v` (detailed output), `--release`, `-o <file>`.

---

## Current Limitations

These are accurate as of the current `taupkg` source — not aspirational:

- **No live registry.** `search`, `publish`, `info`, and any dependency
  declared as a bare version constraint (e.g. `mylib = "^1.0.0"`, which
  `parse_source` treats as `kind = "registry"`) point at
  `registry.taupkg.dev` (overridable via `TAUPKG_REGISTRY`). There is client
  code for this (`taupkg/src/registry.tr`) but no indication the service is
  live — **use `local:`, `github:`, or `git:` dependency sources today**, not
  bare version constraints.
- **`publish` requires `TAUPKG_TOKEN`** and uses `curl` for multipart upload
  (the built-in `HttpClient` doesn't support multipart yet) — another sign
  this path is not the primary workflow yet.
- **`local:` deps are copied, not symlinked.** Edits to a local dependency's
  source require re-running `taupkg install` (not just `taupkg build`) to be
  picked up — see "Local Path Dependencies" above.
- **`authors` is parsed as a single string**, not a TOML array, despite the
  field being typed `Vec[str]` in `Manifest` — only one author value will be
  captured even if you write a TOML array in `taupkg.toml`.

---

See [15 — Modules](../lang/15_modules.md) for the underlying module
resolution rules that make vendored dependencies and `TAURARO_PATH` work.
Packaging a library that wraps an existing C/C++ library (vendored source or
a system-installed one), with zero manual linking for consumers, is its own
guide: [10 — Wrapping C/C++ Libraries](10_wrapping_c_cpp_libraries.md).

Previous: [Codegen Guide ←](04_codegen_guide.md) · Next: [Best Practices & Pitfalls →](06_best_practices_pitfalls.md)
