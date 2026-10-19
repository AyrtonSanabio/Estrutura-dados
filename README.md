# Estruturas de Dados em C

Trabalho de Ayrton Sanabio, matrícula 0117925. Implementação dos TPs de TADs básicas,
grafo estático e algoritmos em grafos. Os programas usam C11 e a biblioteca padrão.

## Entregas

| Pasta | Conteúdo | Situação |
|---|---|---|
| 01-pilhas | Pilha encadeada, LIFO e demonstração | Implementado e testado |
| 02-filas | Fila encadeada, FIFO e demonstração | Implementado e testado |
| 03-listas | Lista dinâmica duplamente encadeada | Implementado e testado |
| 04-grafos | 31 operações, remoções, caminho TP 1 e cinco algoritmos TP 2 | Implementado e testado |
| 05-arvores | ABB com 14 operações, menu e percursos | Implementado e testado; estudo adicional |
| docs | Manual técnico e relatório em PDF | Documentado |
| testes | Testes C e regressões Python | Aprovados |

O laboratório web ferramentas-estudo foi removido a pedido. É recuperável pelo Git
no histórico anterior ao commit 5ed14c7.

## Compilar e testar no Windows

Na raiz do projeto, com GCC no PATH:

~~~powershell
.\compilar.ps1
.\testar.ps1
~~~

O segundo comando também requer Python 3.10+ e usa apenas sua biblioteca padrão.
Os executáveis ficam em output, ignorado pelo Git. A compilação usa
-std=c11 -Wall -Wextra -Wpedantic -Werror.

~~~powershell
.\output\pilha.exe
.\output\fila.exe
.\output\lista.exe
.\output\arvores.exe
.\output\arvores.exe 8 3 10 1 6 14 4 7 13
.\output\grafos.exe exemplos/grafo.txt bfs 1
.\output\grafos.exe exemplos/grafo.txt dfs 1
.\output\grafos.exe exemplos/grafo.txt componentes
.\output\grafos.exe exemplos/grafo.txt agm --pesos exemplos/pesos.txt
.\output\grafos.exe exemplos/grafo.txt caminho 1 6 --pesos exemplos/pesos.txt
.\output\grafos.exe exemplos/grafo.txt tp1 1 6 --pesos exemplos/pesos.txt
~~~

Acrescente --nao-dirigido a BFS, DFS ou caminho para consultar conexões nos dois
sentidos. A operação tp1 é dirigida, conforme o PDF.

Sem argumentos, arvores.exe abre o menu. Com inteiros, constrói a ABB e imprime
tamanho, altura, extremos e os três percursos. Valores repetidos são recusados.
Veja [as convenções da ABB](05-arvores/README.md).

Sem PowerShell, compile o programa de grafos com:

~~~text
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror 04-grafos/main.c 04-grafos/grafo.c 04-grafos/caminho.c 04-grafos/algoritmos.c -o grafos
~~~

## Arquivos e saídas

O arquivo de grafo começa com V E e tem uma linha origem destino por aresta.
Os pesos ficam em outro arquivo, um número por aresta, na mesma ordem. Sem arquivo
de pesos, cada aresta custa 1. Os IDs começam em 1; zero significa ausência.

O exemplo ponderado produz:

~~~text
BFS: v1,a1,v2,a2,v3,a3,v4,a6,v5,a8,v6
DFS: v1,a1,v2,a3,v4,a5,v5,a8,v6,a2,v3
Caminho: v1,a2,v3,a4,v4,a5,v5,a8,v6
Distancia: 8
AGM: a2,a3,a4,a8,a5
~~~

Consulte [a documentação técnica](docs/DOCUMENTACAO.md) para as interfaces,
custos reais, convenções de erro, limitações e testes. O relatório exigido está em
[docs/RELATORIO.pdf](docs/RELATORIO.pdf). Leia-o antes da entrega e ajuste qualquer
avaliação pessoal que queira acrescentar.
