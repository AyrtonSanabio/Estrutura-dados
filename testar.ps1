param([string]$Compilador = "gcc", [string]$Python = "python")
$ErrorActionPreference = "Stop"
Push-Location $PSScriptRoot
try {
    & ./compilar.ps1 -Compilador $Compilador
    & ./output/testes/test_tads.exe
    if ($LASTEXITCODE -ne 0) { throw "Falha nos testes das TADs." }
    & ./output/testes/test_arvore.exe
    if ($LASTEXITCODE -ne 0) { throw "Falha nos testes da arvore." }
    & $Python -m unittest discover -s testes -p "test_*.py" -v
    if ($LASTEXITCODE -ne 0) { throw "Falha nos testes de grafos ou arvores." }
} finally {
    Pop-Location
}
