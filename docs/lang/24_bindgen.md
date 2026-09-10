# 24 — Bindgen: automatic C and C++ bindings

`tauraroc bindgen` reads a C or C++ header and writes a ready-to-use Tauraro binding module — you
never hand-write `extern` declarations for a whole library. It has three generation modes plus an
option to bind function-like macros:

| Mode | Command | For |
|------|---------|-----|
| **C** (default) | `tauraroc bindgen foo.h -o foo.tr` | C headers — direct `extern "C"` bindings |
| **C++** | `tauraroc bindgen foo.hpp -o foo.tr -h cpp` | C++ headers — auto-generates a C++→C shim via libclang |
| **+ macros** | add `--macros` | also bind function-like `#define M(x) …` macros (compile-verified shims) |
| **export** | `tauraroc lib.tr --export-cpp` | the reverse: expose a *Tauraro* module to C++ |

The generated module records how to build and link the library, so once it exists you just
`import` it and compile your program — **no manual `--link`, no `-lstdc++`, no build script.**

---

## The whole workflow in four steps

```sh
# 1. Generate the binding (once; check it into source control if you like)
tauraroc bindgen /usr/include/zlib.h -o zlib.tr --pkg zlib

# 2. Import it and use it
cat > app.tr <<'EOF'
from zlib import *

def main():
    mut buf: [c_uchar; 3]
    buf[0] = 104 as c_uchar   # 'h'
    buf[1] = 105 as c_uchar   # 'i'
    buf[2] = 33 as c_uchar    # '!'
    print("crc =", crc32(0, buf.as_ptr(), 3))
EOF

# 3. Build — the shim + library are auto-linked (no --link, no -lz)
tauraroc app.tr -o app

# 4. Run
./app
```

**Bind once, then `import` + build.** Everything below is detail on each feature and how to use it well.

---

## Importing a binding — three equivalent forms

Tauraro uses a **unity build**: importing a module brings all of its `pub` declarations into scope
**unqualified**. These three forms are equivalent — pick whichever reads best:

```python
from zlib import *      # Python-style wildcard  (recommended, most explicit)
import zlib.*           # shorthand for the wildcard
import zlib             # also exposes every pub name unqualified
```

You can also import **specific** names or alias them:

```python
from zlib import crc32, compress
from zlib import crc32 as checksum
```

Packages nest to any depth — the dotted path mirrors the directory layout (`a/b/c.tr`):

```python
from graphics.render.mesh import *
import graphics.render.mesh.*
```

### Import gotchas (these produce `unexpected end of line in expression`)

- **Module names must be valid identifiers.** A file named `some-lib.tr` can't be imported as
  `from some-lib import *` (the `-` parses as minus). **Name your binding output with underscores:**
  `tauraroc bindgen … -o some_lib.tr`.
- **No qualified access via alias.** `import zlib as z` then `z.crc32(…)` is **not** supported — the
  unity build exposes names directly, so just write `crc32(…)`.

---

## Using a binding — the type-boundary rules

Bindings are ordinary Tauraro, but they cross a C ABI, so a few rules apply. All of these are
demonstrated end-to-end below.

**Strings.** A C `const char*` **parameter** takes a Tauraro string via a cast; a `const char*`
**return** is a borrowed view:

```python
PyRun_SimpleString("print('hi')" as Pointer[char])   # str  -> const char*  (pass)
mut v = zlibVersion() as str                          # const char* -> str   (borrowed, never freed)
```

**Pointers & buffers.** Get a pointer into a fixed array with `.as_ptr()`. **Writing** through a raw
pointer requires an `unsafe:` block (memory-safety by default):

```python
mut p = some_valarray_op_index(v, 0)   # -> Pointer[T]
unsafe:
    p.write(100)                       # raw write is gated behind unsafe:
```

**Scalars.** Use the `c_*` family at the boundary (`c_int`, `c_long`, `c_double`, `c_size_t`,
`c_intptr_t`, `c_char`, `c_uchar`, …) so the width always matches the platform C ABI — `int`/`long`
in C are platform-dependent.

**Objects / opaque handles.** A bound C++ class or a C `typedef struct X*` is an opaque handle:
construct with `<pfx>_new(…)`, call `<pfx>_method(obj, …)`, free with `<pfx>_delete(obj)`.

### Worked example — a C++ class (`std::complex<double>`)

```python
from stdcomplex import *

def main():
    mut z = std__complex_double__new_2(3.0, 4.0)     # constructor: 3 + 4i
    mut re = std__complex_double__real(z)
    mut im = std__complex_double__imag(z)
    print("|z|^2 =", re*re + im*im)                  # 25.0
    mut z2 = std__complex_double__op_imul(z, 2.0)    # operator*=  -> 6 + 8i
    print("2z =", std__complex_double__real(z2), std__complex_double__imag(z2))
```

The `stdcomplex_shim.cpp` is compiled and linked automatically; you just `import` and run.

---

## `--pkg`: auto-discover a library's flags

Most C/C++ libraries ship a `pkg-config` file. `--pkg <name>` queries it for the compile flags
(`-I`/`-D`, used to parse the header) and the link flags (`-L`/`-l`, recorded in the binding), so you
never hand-pass them:

```sh
tauraroc bindgen sqlite3.h -o sqlite3.tr --pkg sqlite3
tauraroc bindgen /usr/include/cairo/cairo.h -o cairo.tr -h cpp --pkg cairo
```

If `pkg-config` doesn't know the package, bindgen warns and you fall back to explicit flags:

```sh
tauraroc bindgen mylib.h -o mylib.tr -I/opt/mylib/include -DMYLIB_STATIC
```

`--pkg` works on **both** the C and C++ paths.

---

## Auto-linking (zero build step, zero-cost)

The generated `.tr` carries machine-readable header pragmas:

```python
# tauraro-cpp-shim: foo_shim.cpp        # (C++/--macros) the generated shim to compile
# tauraro-cpp-lib: stdc++               # libraries to link
# tauraro-cpp-linkflags: -L… -lfoo      # (--pkg) link flags, appended verbatim
# tauraro-cpp-cflags: -std=c++17 -I…    # flags to compile the shim
```

When you `import` such a module, `tauraroc` reads these, compiles the shim once (cached by content),
and adds everything to the link line for you. On a **gcc / `zig cc`** toolchain the whole program is
built with `-flto`, so the C++→C shim wrapper is **inlined away** — a call into C++ costs exactly what
the C++ call costs, with no shim indirection.

- Opt out with `--no-auto-cpp` (then link `foo_shim.o -lstdc++` yourself).
- On a bare `clang` host without lld, the shim is still auto-linked; only the LTO inlining is skipped.
- You still link the C++ *library itself* (via `--pkg`, `-lfoo`, or `--link libfoo.a`); only the
  generated shim is automatic.

---

## Binding C++ headers (`-h cpp`)

C++ has no stable C-callable ABI (name mangling, `this`, vtables, templates, RAII, exceptions), so
`-h cpp` uses **libclang** to walk the header and emit a `foo_shim.cpp` (`extern "C"` wrappers around
the real C++ API) next to `foo.tr`. libclang is required **only** for `-h cpp`; if it's missing, the
tool prints per-platform install guidance.

Point it at a **specific** header, not an umbrella, and on Windows/MSYS add `--cc clang++` so libclang
uses clang's own intrinsic headers (GCC's `ia32intrin.h` isn't parseable by libclang):

```sh
# GOOD — a concrete header, clang++ on Windows/MSYS
tauraroc bindgen /usr/include/wx-3.2/wx/button.h -o wxbutton.tr -h cpp --cc clang++ \
    -I/usr/lib/wx/include/gtk3-unicode-3.2 -I/usr/include/wx-3.2 -DWXUSINGDLL

# BAD — umbrella headers often parse to zero bindable declarations
tauraroc bindgen wx/wx.h -o wx.tr -h cpp …
```

### What the C++ binder covers

- **Classes** — opaque handles: construct `<pfx>_new(...)`, call `<pfx>_method(obj, ...)`, free `<pfx>_delete(obj)`.
- **Methods** (static + `const`), **constructors/destructors**, **inherited public methods** (transitive,
  base-qualified to defeat name-hiding and ambiguous multiple inheritance).
- **Virtual dispatch** — a factory returning `Base*` dispatches to the right override.
- **Operator overloads** — `operator+`/`==`/`[]`/`*`/`()`/… bind under clean names (`op_add`, `op_eq`,
  `op_index`, `op_imul`, …).
- **Templates** — used class-template instantiations are force-instantiated (`std::vector<int>`,
  `Box<int>`, `std::complex<double>`) and bind with all their members.
- **Enums** (with values), **namespaces**, **global variables** & **static class constants**.
- **Public data members** — read-accessors (makes `std::pair`/`std::tuple` fields usable).
- **Default arguments** — one wrapper per trailing-default arity.
- **Free-function templates** of a single type parameter — instantiated for `int`/`double`.
- **Exceptions** — every wrapper is exception-safe: a C++ exception is caught, recorded, and turned into
  a zero return. Read it with `tauraro_cpp_last_error() as str`, clear with `tauraro_cpp_clear_error()`.
  A throwing function never crashes the caller.
- **Expression templates** — GMP's `__gmp_expr` (lazy CRTP proxies, the hardest C++ to cross a C ABI —
  SWIG and cxx both stop here) **bind and run**: the value types (`mpz_class`) work end-to-end.

### Standard-library type mapping

| C++ | Tauraro binding side | Notes |
|-----|----------------------|-------|
| `std::string`, `std::string_view` | native `str` | auto-converts both directions |
| `std::vector<T>`, `std::deque<T>`, `std::array<T,N>` | element by index (`at`/`[]`/`.len`) | forced instantiation |
| `std::map`/`unordered_map<K,V>` | `_key_nth(i)` / `_val_nth(i)` enumeration | |
| `std::set`/`list`/`forward_list`/`unordered_set` | `_nth(i)` index accessor | via `begin()`+`advance` |
| `std::pair`, `std::tuple` | `_first`/`_second` / member accessors | |
| `std::shared_ptr`/`unique_ptr`/`weak_ptr<T>` | `get()` → `Pointer[T]` | pointee is usable |
| `std::function<R(Args)>` | `Pointer[void]` | pass a Tauraro `def` — see below |
| function pointer / callback + `void*` | `Pointer[void]` | see below |
| `std::complex`, `std::valarray` | full class binding | verified running from Tauraro |

Template-heavy real headers are verified: `std::complex`/`std::valarray` bind **and run**; LLVM's
`APInt`/`raw_ostream`/`StringRef` shims compile clean (linking those needs the LLVM libraries).

### Passing a callback / `std::function`

A `std::function<R(Args)>` (or a raw function-pointer) parameter binds as `Pointer[void]`. Pass a
**top-level** Tauraro `def` with `as Pointer[void]`:

```python
def dbl(x: int) -> int:
    return x * 2

g_apply(dbl as Pointer[void], 20)     # C++: int g_apply(std::function<int(int)> f, int x)
```

The shim casts the pointer to the matching function type, which C++ builds the `std::function` from.
(Capturing closures with user-data go through the raw callback + `void*` pattern — see
[Callback Functions](17_extern_and_ffi.md#callback-functions-function-pointers).)

---

## `--macros`: binding function-like C macros

A huge amount of a C API can live in **function-like macros** — `Py_INCREF(op)`, `PyList_GET_ITEM(l,i)`,
`Py_TYPE(o)`. These have no linkable symbol, so no binder can bind them as plain `extern` functions.
`--macros` solves it by generating a **compile-verified shim** — one real wrapper per macro that the C
compiler is asked to accept:

```sh
tauraroc bindgen /usr/include/python3.14/Python.h -o python.tr --macros -I/usr/include/python3.14
# bindgen: macro shims: 364 of 426 function-like macros bound … -> python_macros.c (m_<NAME>)
```

Each macro `M` becomes `m_M(...)`, and `python_macros.c` is auto-compiled + linked via the shim
pragma — you just `from python import *` and call it. Arguments are passed as `Pointer[void]`
(pointers) or `c_intptr_t` (integers); a value-returning macro returns `Pointer[void]`:

```python
from python import *

def main():
    Py_Initialize()
    mut obj = PyLong_FromLong(12345)
    print("refcount =", m_Py_REFCNT(obj as Pointer[void]) as int)   # value macro -> Pointer[void] -> int
    m_Py_INCREF(obj as Pointer[void])                               # void action macro
    mut lst = PyList_New(3)
    mut item = m_PyList_GET_ITEM(lst as c_intptr_t, 0)              # integer-index macro
    Py_Finalize()
```

**How it stays correct:** every candidate shim is compiled; anything the compiler rejects is dropped by
name (so one bad macro can't break the file). The binder tries, in order, value/void forms with
`void*` then `intptr_t` arguments, a bare-statement form for `do{…}while(0)` action macros, and — for a
macro that only casts a pointer, like `PyObject_New(type, …)` — **type-erasure**: it bakes a placeholder
type into the `(type*)` position (a pointer cast is runtime-identity) so the macro binds *without* the
type argument. Scalar casts and `sizeof(type)` are correctly *not* erased (they'd truncate).

**What can't be bound (and why it's a hard limit, not a gap):** macros with no runtime meaning —
`##` token-paste (`Py_LL(x)→x##LL`), X-macros (`FOREACH(V)` taking a macro), attribute/declaration
decorators (`PyAPI_DATA`), `static_assert`, `sizeof`-of-a-field, and context macros that use the
caller's locals (`Py_VISIT`). These denote compile-time or lexical operations; a runtime function can't
represent them. For those, call the underlying function directly (e.g. `PyMem_Malloc` instead of
`PyMem_New(type, n)`).

> **Best practice:** use `--macros` when you need the ergonomic/fast macro accessors of a C API
> (CPython, Vulkan, SDL). The *functions* are the capability; the macros are convenience on top.

---

## Case study: embedding CPython

A complete, runnable example of driving a real library — bind `Python.h`, then use the interpreter:

```sh
tauraroc bindgen /usr/include/python3.14/Python.h -o python.tr --macros -I/usr/include/python3.14
```
```python
from python import *

def main():
    Py_Initialize()
    # run Python code
    PyRun_SimpleString("import sys; print('Python', sys.version.split()[0])" as Pointer[char])
    # call a stdlib function with an argument, get a typed result back
    mut math = PyImport_ImportModule("math" as Pointer[char])
    mut sqrt = PyObject_GetAttrString(math, "sqrt" as Pointer[char])
    mut args = PyTuple_New(1)
    PyTuple_SetItem(args, 0, PyFloat_FromDouble(1252.09))
    print("sqrt =", PyFloat_AsDouble(PyObject_CallObject(sqrt, args)))
    # variadics work both directions
    mut o = Py_BuildValue("(is)" as Pointer[char], 7, "seven" as Pointer[char])   # build
    mut n: c_int = 0
    PyArg_ParseTuple(o, "is" as Pointer[char], (&n) as Pointer[void], (&args) as Pointer[void])  # parse via out-pointers
    Py_Finalize()
```
```sh
tauraroc app.tr --link -lpython3.14 -o app && ./app
```

**Variadic FFI works in both directions:** pass varargs *into* C (`Py_BuildValue("i", 42)`, correct C
ABI widths for `int`/`double`/`char*`) and receive results through *output pointers*
(`PyArg_ParseTuple(t, "i", (&x) as Pointer[void])`, using `c_int`/`c_double` locals so the write width
matches).

---

## Pure-C-ABI headers (GDExtension / Vulkan style)

Some headers are *all* typedefs, enums, and function-pointer tables with **no functions or classes** —
e.g. Godot's `gdextension_interface.h`, where the whole API is reached via a `get_proc_address` table.

- Use **C mode** (the default, no `-h cpp`) for these. The C-mode preprocessor path emits the type
  aliases, enum constants, opaque handle types, struct layouts, and function-pointer typedefs.
- Do **not** use `-h cpp` here — the libclang walker anchors on classes/functions, so a header with
  none binds nothing.

```sh
tauraroc bindgen core/extension/gdextension_interface.h -o gd.tr
# -> 278 type aliases, 95 enum constants, 16 structs
```
```python
from gd import *

def main():
    mut t: GDExtensionVariantType = GDEXTENSION_VARIANT_TYPE_INT
    print("variant INT =", t)
```

Function-pointer struct members (the vtable pattern, `void (*init)(void*, level)`) bind as
`Pointer[void]`; you cast a `get_proc_address` result to call it.

---

## Calling Tauraro from C++ (`--export-cpp`)

The mirror image: expose a Tauraro module to C++.

```sh
tauraroc mylib.tr --export-cpp -o mylib   # -> mylib.dll/.so + mylib.h (C) + mylib.hpp (C++)
```

The `.hpp` is **self-contained** (no runtime header needed). Each `export def` becomes an inline
wrapper in `namespace <libname>`; Tauraro classes become **C++ RAII wrappers** (ARC → copy=retain,
dtor=release); and collections marshal to the standard library:

| Tauraro | C++ |
|---------|-----|
| `str` | `std::string` |
| `List[T]` (scalar/str/class element) | `std::vector<T>` |
| `Dict[K,V]` (str/int key, int/float/bool/str value) | `std::map<K,V>` |
| `(T0, T1, …)` tuple | `std::tuple<…>` |
| a heap class | a C++ RAII wrapper class |

```cpp
#include "mylib.hpp"
mylib::Counter c(10);          // constructor (calls the static factory `init`)
c.bump(5);                     // method
auto v = mylib::nums();        // List[int] -> std::vector<long long>
```
```sh
g++ app.cpp mylib.dll -o app
```

Full detail in the [FFI chapter](17_extern_and_ffi.md#calling-from-c-export-cpp).

---

## Best practices (per feature)

1. **Prefer `--pkg`.** If the library has a pkg-config file, `--pkg <name>` removes all guesswork about
   include and link flags — and makes the binding portable to any machine with the library.
2. **Bind a specific header, not an umbrella.** `foo/foo_all.h` often expands to thousands of
   declarations and binds nothing useful; point at the concrete header you need.
3. **Pick the right *public* header.** Several "misses" are really "wrong header": `libtiff` via
   `tiffio.h` (not internal `tiff.h`), `freetype` via `freetype/freetype.h` (not the `ft2build.h` macro
   shim). For an incomplete-template error, include the header that *defines* the template.
4. **Name outputs as valid identifiers.** `-o some_lib.tr`, not `some-lib.tr` — you must be able to
   `from some_lib import *`.
5. **On Windows/MSYS use `--cc clang++`** for C++ headers so libclang gets clang's own resource headers.
6. **Use `--macros` for macro-heavy C APIs** (CPython, Vulkan, SDL); the generated `m_<NAME>` shims are
   compile-verified and auto-linked. Remember pointer args → `as Pointer[void]`, int args → `c_intptr_t`.
7. **Strings:** pass with `"…" as Pointer[char]`; a returned `const char*` is a **borrowed** `ptr as str`
   (never freed). If *you* own a freshly-malloc'd buffer, adopt it with `_tr_str_wrap(ptr)` instead.
8. **Handle C++ exceptions.** After any C++ wrapper that might throw, check
   `tauraro_cpp_last_error() as str` — a non-empty message means the call was caught and returned zero.
9. **Use `c_*` types at the ABI boundary** so widths always match the platform C ABI.
10. **Keep the *library* on the link line.** The generated shim is auto-linked, but the actual C/C++
    library is yours to provide (`--pkg`, `-lfoo`, `--link libfoo.a`).
11. **Regenerate, don't hand-edit.** Treat the `.tr`/`_shim.cpp`/`_macros.c` as build artifacts and
    re-run bindgen on library upgrades. Check the `.tr` into source control if you want reproducible
    builds without libclang on every machine.

---

## Troubleshooting

| Symptom | Cause / fix |
|---------|-------------|
| `unexpected end of line in expression` on an import | Invalid module name (a `-`/hyphen — rename the file to underscores), or you wrote `import mod as m; m.f()` — use unqualified `f()`. |
| `no bindable declarations found` | Umbrella header, missing `-I`/`-D`, or a pure-typedef/enum/fnptr header parsed with `-h cpp` — use **C mode** for those. |
| `libclang could not fully parse the header` | Missing include dirs/macros. The tool prints the diagnostics — add the `-I`/`-D` (or `--pkg`) it names. |
| C++ shim won't compile | A construct the binder can't express (an incomplete forward-declared template used by value, a non-copyable-by-value return, deep SFINAE/variadic templates). That method is **omitted** and the rest binds — bind a simpler entry point, include the defining header, or wrap it in your own small `extern "C"` shim. |
| a `--macros` macro missing | It's non-denotational (token-paste / X-macro / decorator / `sizeof` / context macro) — call the underlying function instead. |
| `undefined reference` at link time | The *library* isn't linked. Add `--pkg <name>`, `-lfoo`, or `--link libfoo.a`. |
| libclang not found | Install it (the tool prints per-platform guidance). Needed **only** for `-h cpp`. |

---

## Coverage & limitations (measured)

Bindgen is exercised against a broad set of real, installed library headers — see the published
**[stress-test matrix](../../tools/bindgen/STRESS_MATRIX.md)**: **70 libraries, all PASS, 37,000+
wrappers bound**, including GLib/GIO/GObject (8,400+), Pango, ATK, OpenSSL, SQLite, libcurl, libxml2,
Cairo, HarfBuzz, FreeType, libpng/webp/jpeg/tiff, GMP/MPFR, the C standard library, wxWidgets C++
widgets, `std::complex`/`std::valarray`, and LLVM ADT/Support. Additional validated targets: the full
**CPython C API** (1,301 functions + 364/426 function-like macros, imported and *running* the
interpreter), **Godot** GDExtension, and **gmpxx expression templates** (bignum round-trips through the
C ABI).

It is a generated-shim approach, so a few C++ constructs still can't cross a C ABI and are cleanly
**omitted** (the rest of the header still binds — a partial bind is always usable): an incompletely
forward-declared template used by value, a non-copyable type returned by value, deeply SFINAE/variadic
templates, `std::function` **returns** (params work), an unresolved callable's `operator()`, and nested
collection element types in `--export-cpp`. Function-like macros with no runtime denotation (see
`--macros` above) are likewise skipped by design.

---

See [10 — Wrapping C/C++ Libraries](../dev/10_wrapping_c_cpp_libraries.md) for
the end-to-end package-author workflow built on this auto-linking mechanism:
vendoring a library's source into your own taupkg package vs. binding one
that's already installed on the OS via `pkg-config`, either way with zero
manual linking for anyone who depends on the result.

Next: [Casting with `as` →](23_casting_with_as.md)
