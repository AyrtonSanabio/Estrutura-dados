# Estado dos trabalhos de Estruturas de Dados

## TP — TADs básicas (11/09/2026)

O PDF exige bibliotecas `.c` e `.h` para pilha, fila e lista, além de um programa principal demonstrativo. A lista deve ser dinâmica e duplamente encadeada, com início, fim e tamanho, conforme a figura da página 5.

### Pilha — `06-pilhas/pilha.c`

| Operação do PDF | Estado atual |
| --- | --- |
| `Pcria()` | Falta. `inicializar_pilha` apenas inicializa uma estrutura recebida; não cria e retorna uma pilha. |
| `Pdestroi(pilha)` | Falta formalizar. `limpar_pilha` já libera todos os nós, mas a criação/destruição da própria estrutura ainda precisa ser definida. |
| `Pvazia(pilha)` | Implementada; antes chamada `esta_vazia`. |
| `Pcheia(pilha)` | Falta; definir o significado de cheia na representação dinâmica. |
| `Ptamanho(pilha)` | Implementada; antes chamada `quantidade_elementos`. |
| `Pexamina(pilha)` | Lógica implementada; antes `consultar_topo`. Ainda recebe ponteiro de saída e retorna sucesso, enquanto o PDF pede retorno do elemento. |
| `Pinsere(pilha, elemento)` | Implementada; antes `empilhar`. Ainda retorna inteiro; o PDF pede `void`. |
| `Premove(pilha)` | Remoção implementada; antes `desempilhar`. Ainda retorna sucesso/falha, e não o elemento removido exigido. |

Também existem impressão, contagem de ocorrências, busca de valor, maior e menor valor. Soma, média e igualdade aparecem apenas como declarações comentadas e não são exigidas no PDF.

### Lista duplamente encadeada — `04-listas-duplamente-encadeadas/lista-duplamente-encadeada.c`

Já implementado: estrutura com início, fim e quantidade; inicialização; inserção no início e no fim. O tipo dos ponteiros dos nós foi corrigido.

Faltam as operações da interface: `Lcria`, `Ldestroi`, `Lvazia`, `Lcheia`, `Ltamanho`, `Lexamina`, `Llocaliza`, `Linsere` e `Lremove`.

As inserções nas extremidades podem ser reaproveitadas em `Linsere`, mas ainda não equivalem à inserção em qualquer posição exigida. A inicialização também não equivale à criação e retorno da lista. Por isso essas funções mantiveram seus nomes. Declarações dentro de comentários não são implementações.

### Fila — `07-filas`

Sem implementação. Faltam `Fcria`, `Fdestroi`, `Fvazia`, `Fcheia`, `Ftamanho`, `Fexamina`, `Finsere` e `Fremove`.

### Lista simplesmente encadeada — material de estudo

Arquivo: `03-listas-simplesmente-encadeadas/lista-simplesmente-encadeada.c`.

Já existem inicialização, verificação de vazia, inserções no início/fim/posição, impressão, limpeza, busca de valor, contagem de ocorrências, remoção do início e consulta por posição.

Renomeações: `esta_vazia` → `Lvazia`; `inserir_posicao` → `Linsere`; `buscar_posicao` → `Lexamina` (essa função consulta o valor de uma posição, não localiza a posição de um valor).

`Linsere` ainda recebe `(lista, posicao, valor)` e retorna inteiro; o PDF pede `(lista, elemento, posicao)` e retorno `void`. `Lexamina` ainda entrega o elemento por ponteiro de saída. Os índices atuais começam em zero.

Esta representação não substitui a lista duplamente encadeada exigida. As demais operações listadas em comentários continuam sem implementação. Há uma variável local `contador` não utilizada na consulta por posição, mantida por estar fora do escopo da correção de ponteiros.

### Lista estática — material de estudo

Arquivo: `lista-estática/lista.c`.

Já existem inicialização, inserção no fim, remoção por valor, verificação de vazia/cheia, limpeza, impressão, quantidade, localização de elemento, contagem de ocorrências e inserção por posição.

Renomeações: `estaVazia` → `Lvazia`; `estaCheia` → `Lcheia`; `total_elementos` → `Ltamanho`; `posicao_elemento` → `Llocaliza`; `inserir_posicao` → `Linsere`.

`Linsere` preserva a ordem `(lista, posicao, valor)` e o retorno inteiro; ambos ainda diferem do PDF. A remoção por valor não foi renomeada para `Lremove`, pois o PDF exige remoção por posição com retorno do elemento. `limparLista` permanece declarada como `int` sem retornar valor: pendência fora do escopo de ponteiros/nomes.

Esta representação estática não atende ao modelo de lista pedido.

### Pendências gerais

- Criar os cabeçalhos e separar as bibliotecas do programa principal.
- Adequar assinaturas e retornos às interfaces do enunciado.
- Definir tratamento de estrutura vazia, falta de memória e posições inválidas.
- Criar demonstração das operações; os `main` atuais apenas retornam zero.
- Validar inserções e remoções, estruturas vazias e limites de posição.
- As implementações de listas são alternativas de estudo e têm nomes repetidos; não devem ser ligadas todas como uma única biblioteca.

## TP — TAD Grafo estático (30/09/2026)

A pasta `14-grafos` ainda não tem implementação. Faltam:

- `grafo.h` e `grafo.c`, com `typedef struct grafo *Grafo` e capacidade máxima prevista para vértices e arestas.
- As 31 operações abaixo, com os nomes e contratos do PDF.
- `caminho.h` e `caminho.c`, com `void Gcaminho(Grafo g, float *pesos, int a, int b)`, utilizando a TAD para o caminho mais curto.
- Programa principal, arquivos de entrada e validação das saídas.
- Relatório PDF, sugerido entre 2 e 4 páginas: introdução, implementação, validação, conclusão e bibliografia.
- Nome, matrícula e data nos cabeçalhos dos arquivos-fonte; identificação dos autores no relatório; apresentação oral individual.

Operações da TAD:

| Grupo | Operações |
| --- | --- |
| Criação/destruição | `GGcriaGrafo`, `GGdestroiGrafo`, `GVcriaVertice`, `GAcriaAresta` |
| Existência | `GBexisteIdVertice`, `GBexisteIdAresta`, `GBexisteArestaDir`, `GBexisteAresta` |
| Localização | `GApegaArestaDir`, `GApegaAresta` |
| Percurso global | `GVprimeiroVertice`, `GVproximoVertice`, `GAprimeiraAresta`, `GAproximaAresta` |
| Quantidades/capacidades | `GInumeroVertices`, `GInumeroVerticesMax`, `GInumeroArestas`, `GInumeroArestasMax` |
| Arquivos | `GGcarregaGrafo`, `GBsalvaGrafo` |
| Grau/estrelas | `GIpegaGrau`, `GAprimaAresta`, `GAproxAresta`, `GAprimaEntrada`, `GAproxEntrada`, `GAprimaSaida`, `GAproxSaida` |
| Arestas/extremos | `GBarestaLaco`, `GValfa`, `GVomega`, `GVvizinho` |

O grafo deve suportar consultas dirigidas e não dirigidas, laços e arestas múltiplas. Identificadores começam em 1; zero indica inexistência. Os pesos ficam em um vetor separado, com a posição zero não utilizada. O arquivo textual começa com quantidade de vértices e arestas, seguido de um par de extremos por aresta. Quando não houver caminho, `Gcaminho` imprime apenas o vértice inicial. Decisões sobre detalhes omissos devem ser documentadas no relatório.
