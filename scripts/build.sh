#!/usr/bin/env bash
set -euo pipefail

# Bootstrap binary — set by CI via BOOTSTRAP_BIN, or fall back to tauraroc on PATH.
BOOTSTRAP="${BOOTSTRAP_BIN:-tauraroc}"

if [ ! -x "$BOOTSTRAP" ] && ! command -v "$BOOTSTRAP" &>/dev/null; then
    echo "ERROR: bootstrap binary not found: $BOOTSTRAP"
    echo "Set BOOTSTRAP_BIN to the path of a tauraroc binary, or put tauraroc on PATH."
    exit 1
fi

# Remove the ARM64 musl wrapper - use native glibc GCC instead
# (musl lacks ucontext functions needed by the coroutine scheduler)
STATIC_FLAG=""
# Opt-in: TAURARO_STATIC=1 statically links the produced tauraroc binary
# itself (real glibc, NOT musl — musl lacks ucontext, see above) via
# tauraroc's own `--static` flag. Used by the Termux/Android CI job: a
# static binary has no ELF PT_INTERP, so it has no dynamic-linker-path
# dependency at all, sidestepping the "wrong ld.so path" class of failure
# a dynamically-linked build hits on Termux's non-standard filesystem
# layout (everything under /data/data/com.termux/files/usr).
if [ -n "${TAURARO_STATIC:-}" ]; then
    STATIC_FLAG="--static"
fi

# For Linux, ensure -std=gnu11 and -D_GNU_SOURCE for ucontext
if [ "$(uname -s)" = "Linux" ]; then
    # Create a GCC wrapper that always uses -std=gnu11 -D_GNU_SOURCE
    mkdir -p "$HOME/.local/bin"
    if [ ! -f "$HOME/.local/bin/gcc" ]; then
        printf '#!/bin/sh\nexec /usr/bin/gcc -std=gnu11 -D_GNU_SOURCE "$@"\n' > "$HOME/.local/bin/gcc"
        chmod +x "$HOME/.local/bin/gcc"
    fi
    export PATH="$HOME/.local/bin:$PATH"
    if [ -n "${TAURARO_STATIC:-}" ]; then
        # Termux/Android static build only: respect the CI job's own
        # CC/CFLAGS/LDFLAGS (it sets _XOPEN_SOURCE=700 and -lrt) instead of
        # the hardcoded defaults below. Scoped to this opt-in path so every
        # OTHER job's behavior (which relied on the unconditional overwrite
        # below, even though it also sets its own env: block) is completely
        # unchanged.
        : "${CC:=gcc}"
        : "${CFLAGS:=-O2 -std=gnu11 -D_GNU_SOURCE -D_XOPEN_SOURCE=700}"
        : "${LDFLAGS:=-lm -lpthread -lrt}"
        export CC CFLAGS LDFLAGS
    else
        # Use native glibc GCC on all Linux platforms.
        export CC="gcc"
        export CFLAGS="-O2 -std=gnu11 -D_GNU_SOURCE"
        export LDFLAGS="-lm -lpthread"
    fi
fi

echo "==> Compiling src/main.tr → ./tauraroc"
# The in-process LLVM shim provides _tr_llvm_emit_object so the `--backend llvm` drive links.
# DEFAULT: --link the plain .c (stub returning -1 → the compiler uses the bundled-llc
# subprocess, Stage 1). No libLLVM dependency.
LLVM_SHIM="runtime/tauraro_llvm.c"
# STAGE 2 opt-in: TAURARO_LLVM_INPROC=1 builds the shim against libLLVM (via llvm-config) so
# `--backend llvm` emits objects IN-PROCESS (no llc subprocess). Requires libLLVM dev files.
if [ -n "${TAURARO_LLVM_INPROC:-}" ]; then
    LC="${LLVM_CONFIG:-llvm-config}"
    echo "==> Stage 2: in-process libLLVM emission ($("$LC" --version))"
    "${CC:-gcc}" -DTAURARO_LLVM_INPROC -O2 -c runtime/tauraro_llvm.c \
        -I"$("$LC" --includedir)" -o build_tauraro_llvm.o
    LNAME="$("$LC" --libs | tr -d ' ' | sed 's/^-l//')"            # e.g. LLVM-21
    LDIR="$("$LC" --libdir)"
    LIMP="$LDIR/lib$LNAME.dll.a"                                   # Windows import lib
    [ -f "$LIMP" ] || LIMP="$LDIR/lib$LNAME.so"                    # Linux shared
    [ -f "$LIMP" ] || LIMP="$LDIR/lib$LNAME.dylib"                 # macOS shared
    LLVM_SHIM="build_tauraro_llvm.o --link $LIMP"
fi
"$BOOTSTRAP" src/main.tr -o tauraroc --link $LLVM_SHIM $STATIC_FLAG

# New bootstrap (v0.0.4+): binary lands in CWD as ./tauraroc
# Old bootstrap (≤v0.0.3): binary lands in src/build/tauraroc
# Normalise: move from old location to CWD if needed.
if [ ! -f "./tauraroc" ] && [ -f "./src/build/tauraroc" ]; then
    echo "==> Moving src/build/tauraroc → ./tauraroc (old bootstrap compat)"
    mv "./src/build/tauraroc" "./tauraroc"
fi

if [ ! -f "./tauraroc" ]; then
    echo "ERROR: tauraroc not produced — compilation failed"
    exit 1
fi

echo "==> Done: ./tauraroc"