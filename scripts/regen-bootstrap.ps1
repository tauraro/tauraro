# Regenerate the portable C bootstrap (bootstrap/c/) from a working tauraroc.
# Run this whenever src/main.tr starts using a construct the committed-C
# bootstrap can no longer compile. Commit the updated bootstrap/c/ tree.
#
#   ./scripts/regen-bootstrap.ps1 [-Compiler .\tauraroc.exe]
param([string]$Compiler = ".\tauraroc.exe")
$ErrorActionPreference = "Stop"

if (-not (Test-Path $Compiler)) {
    $c = Get-Command tauraroc.exe -ErrorAction SilentlyContinue
    if ($c) { $Compiler = $c.Source }
    else { Write-Error "No tauraroc.exe found. Pass -Compiler <path> to a working build."; exit 1 }
}

Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
& $Compiler src/main.tr --emit c
if (-not (Test-Path build/main.c)) { Write-Error "emit failed: build/main.c missing"; exit 1 }

Remove-Item -Recurse -Force bootstrap/c -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force bootstrap | Out-Null
Copy-Item -Recurse build bootstrap/c

# The in-process LLVM shim (stub by default) must be in the C seed so the stage0 build
# resolves _tr_llvm_emit_object. The stage0 command globs bootstrap/c/module_*.c, so copy
# it in under a module_* name to be picked up without changing the seed compile line.
Copy-Item runtime/tauraro_llvm.c bootstrap/c/module_tauraro_llvm.c

$n = (Get-ChildItem -Recurse -File bootstrap/c).Count
Write-Host "==> Regenerated bootstrap/c/ ($n files). Review + commit the tree."
