# Compilacao local com GCC em C11. Executaveis ficam fora do versionamento.
param([string]$Compilador = "gcc")
$ErrorActionPreference = "Stop"
Push-Location $PSScriptRoot
try {
    New-Item -ItemType Directory -Force -Path "output/testes" | Out-Null
    $opcoes = @("-std=c11", "-Wall", "-Wextra", "-Wpedantic", "-Werror")
    foreach ($modulo in @(
        @("01-pilhas/pilha.c", "01-pilhas/main.c", "-o", "output/pilha.exe"),
        @("02-filas/fila.c", "02-filas/main.c", "-o", "output/fila.exe"),
        @("03-listas/lista.c", "03-listas/main.c", "-o", "output/lista.exe"),
        @("04-grafos/grafo.c", "04-grafos/caminho.c", "04-grafos/algoritmos.c", "04-grafos/main.c", "-o", "output/grafos.exe"),
        @("testes/test_tads.c", "01-pilhas/pilha.c", "02-filas/fila.c", "03-listas/lista.c", "04-grafos/grafo.c", "-o", "output/testes/test_tads.exe")
    )) {
        & $Compilador @opcoes @modulo
        if ($LASTEXITCODE -ne 0) { throw "Falha na compilacao: $($modulo -join ' ')" }
    }
    Write-Output "Compilacao concluida sem avisos."
} finally {
    Pop-Location
}
