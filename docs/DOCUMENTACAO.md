# Documentação dos trabalhos de Estruturas de Dados

Autor: Ayrton Sanabio | Matrícula: 0117925 | Atualização: 19/10/2026.

## 1. Escopo e requisitos

O TP básico exige pilha, fila e lista dinâmica duplamente encadeada, cada qual com
interface .h, implementação .c e programa demonstrativo. As implementações
existentes foram mantidas e validadas.

O TP 1 exige as 31 operações de grafo.h, carregamento e gravação de texto,
Gcaminho(Grafo, float *, int, int) e relatório PDF. As operações adicionais
removeVertice e removeAresta vêm da publicação do professor no Classroom.

O TP 2 exige AGM, busca em largura, busca em profundidade, caminho mais curto e
componentes conexos. Entrada e saída são textuais e há leitura de grafo por arquivo.
Os algoritmos consultam somente a interface da TAD, sem acessar struct grafo.

05-arvores não é requisito dos enunciados lidos e permanece como esqueleto de ABB.
Prim, ordenação topológica e componentes fortemente conexos pertencem ao conteúdo
de estudo, mas não são funções adicionais implementadas nesta entrega. A AGM
pedida é resolvida por Kruskal.

## 2. Organização e execução

- grafo.c/.h: armazenamento, existência, iteração, capacidades, graus, estrelas,
  extremos, arquivos e remoções.
- caminho.c/.h: Dijkstra, resultado com vértices/arestas/distância e saída do TP 1.
- algoritmos.c/.h: algoritmos e saídas do TP 2.
- main.c: validação de argumentos, leitura de pesos e seleção da operação.
- compilar.ps1 e testar.ps1: compilação rigorosa e verificação reproduzível.

Execute os scripts na raiz do projeto. Para outro compilador ou Python:

~~~powershell
.\testar.ps1 -Compilador "gcc" -Python "python"
~~~

O executável C não depende de Python, Node, Graphviz ou internet. Python é usado
somente nos testes; ReportLab é opcional para regenerar o PDF.

## 3. Estrutura do grafo

Dois vetores são alocados na criação: vértices e arestas, com capacidades fixas.
Cada posição tem uma marca de atividade. As arestas guardam seus extremos e
elos para as estrelas de entrada, saída e incidência. Os elos são IDs inteiros,
não alocações separadas; cada estrela fica ordenada por ID crescente.

~~~text
Grafo
  vertices[id] -> ativo, primeiras/ultimas arestas das tres estrelas
  arestas[id]  -> ativa, alfa, omega, anteriores/proximas em cada estrela
~~~

Uma aresta pode ser consultada como dirigida ou não dirigida. Não existe uma flag
de orientação no armazenamento. Laços e arestas paralelas possuem IDs próprios.
Um laço aparece uma vez na estrela de incidência e conta duas vezes no grau total,
uma pela entrada e outra pela saída.

GGcriaGrafo exige capacidades positivas. Arquivos com zero vértices ou zero arestas
são carregados reservando ao menos uma posição útil de capacidade. O identificador
zero é sempre reservado para "não existe".

GGdestroiGrafo retorna NULL, de acordo com a saída indicada no PDF. Utilize
g = GGdestroiGrafo(g) para não manter um ponteiro pendente.

### Remoções e persistência

removeAresta retira a aresta das três estrelas e a marca como inativa.
removeVertice remove antes todas as suas arestas incidentes. Ambas retornam 1
em sucesso e 0 para ID inexistente.

Os IDs dos elementos restantes ficam estáveis; IDs removidos não são reutilizados.
Assim, a capacidade limita também o maior ID já criado. Após esgotar essa sequência,
uma nova inserção falha mesmo que existam posições removidas. Essa convenção evita
que referências externas ou vetores de pesos passem a apontar para outra entidade.
O enunciado original não especifica reciclagem de IDs após as remoções adicionais.

O formato original de arquivo só representa IDs contíguos. Ao salvar depois de
remoções, vértices ativos são renumerados de 1 a V, pela ordem dos IDs antigos,
e arestas são gravadas por ID antigo crescente. A TAD em memória não muda.
Ao recarregar, os IDs correspondem à nova ordem compactada; pesos externos não são
gravados automaticamente e devem acompanhar a mesma ordem das arestas.

## 4. Formato de entrada e operações de terminal

Grafo (exemplo com um vértice isolado):

~~~text
4 2
1 2
2 3
~~~

Cada linha de aresta tem exatamente dois inteiros válidos. Quantidades negativas,
extremos inexistentes, truncamento, números fora do intervalo e conteúdo extra são
rejeitados. Linhas vazias entre arestas não fazem parte do formato. Espaço em branco
após a última aresta é aceito.

Pesos opcionais: exatamente um número finito por aresta, separado por espaços ou
quebras de linha. O vetor possui GInumeroArestasMax(g) + 1 posições e é indexado
pelo ID. Isso continua seguro se houver lacunas de remoções. Índice zero não é usado.
Sem pesos, o custo de cada aresta é 1.

~~~powershell
.\output\grafos.exe exemplos/grafo.txt arestas
.\output\grafos.exe exemplos/grafo.txt bfs 1
.\output\grafos.exe exemplos/grafo.txt dfs 1 --nao-dirigido
.\output\grafos.exe exemplos/grafo.txt caminho 1 6 --pesos exemplos/pesos.txt
.\output\grafos.exe exemplos/grafo.txt tp1 1 6 --pesos exemplos/pesos.txt
.\output\grafos.exe exemplos/grafo.txt agm --pesos exemplos/pesos.txt
.\output\grafos.exe exemplos/grafo.txt componentes
.\output\grafos.exe exemplos/grafo.txt salvar output/copia.txt
.\output\grafos.exe exemplos/grafo.txt remover-vertice 2 output/sem-vertice.txt
.\output\grafos.exe exemplos/grafo.txt remover-aresta 1 output/sem-aresta.txt
~~~

Arquivos de saída são os destinos explicitamente fornecidos no comando. A operação
salvar e as remoções persistem apenas a topologia.

## 5. Algoritmos e convenções

### BFS e DFS

As interfaces originais GBuscaLargura e GBuscaProfundidade respeitam a direção
das arestas. As variantes com sufixo Modo permitem ignorar a direção.

BFS utiliza fila de IDs em vetor e marca na descoberta. DFS usa quadros de uma
pilha explícita para preservar a ordem da DFS recursiva sem depender da pilha do
processo. Ambas visitam somente os vértices alcançáveis da partida.

A saída registra descobertas: v1,a1,v2,... Cada vértice aparece uma vez e a
aresta anterior a ele é a usada para descobri-lo. Essa sequência é um histórico,
não necessariamente um passeio contínuo no grafo. Vizinhos são explorados por ID
de aresta crescente; arestas para vértices já descobertos e retornos da DFS não
são impressos.

### Caminho mínimo

Dijkstra mantém distância, predecessor, aresta predecessora e marca de finalização.
A escolha da menor distância é linear; não usa heap. Distâncias acumuladas usam
double, embora o vetor de entrada seja de float. Pesos negativos e não finitos
são rejeitados. Laços de peso zero e paralelas são tratados sem formar ciclos no
encadeamento dos predecessores.

Gcaminho imprime 1 -> 3 -> 4; se não há trajeto, imprime somente a partida,
conforme TP 1. GCaminhoMaisCurto imprime IDs intercalados de vértices e arestas
e Distancia: valor; destino inalcançável produz Sem caminho.

As interfaces originais do TP 2 não recebem pesos: usam peso unitário. As variantes
GArvoreGeradoraMinimaComPesos e GCaminhoMaisCurtoComPesos recebem o vetor externo.
O terminal usa essas variantes quando se fornece --pesos.

GcalculaCaminho retorna 1 para caminho encontrado, 0 para inalcançável e -1 para
erro. Recebe uma estrutura de saída nova, que deve ser liberada com
GliberaCaminho. Origem igual ao destino retorna um vértice e distância zero.

### AGM

Kruskal ordena por peso, desempata pelo ID da aresta e usa Union-Find com
compressão de caminhos e união por tamanho. Aceita pesos negativos finitos e
ignora laços por não unirem componentes diferentes.

Imprime a lista de IDs escolhidos, por exemplo a2,a3,a4,a8,a5. Em grafo desconexo
imprime Sem AGM: grafo desconexo. Para grafo vazio ou de um vértice, imprime
uma linha vazia. A AGM consulta as ligações como não dirigidas.

### Componentes

Executa BFS a partir de cada vértice ainda não marcado, imprimindo {1,2} {3}.
A direção é ignorada: em dígrafos, isso corresponde aos componentes fracamente
conexos. Componentes fortemente conexos não fazem parte desta função.
Grafo vazio produz {}. Vértices isolados formam componentes de um elemento.

### Erros

Consultas/alterações inválidas da TAD retornam zero ou NULL e não imprimem.
Algoritmos com assinatura void informam falhas de memória em stderr; BFS/DFS
com origem inexistente retornam sem imprimir. O terminal valida origens antes da
chamada e retorna código 2 para arquivos/argumentos inválidos. Ausência de caminho
ou de AGM é resultado normal e retorna código zero.

## 6. Custos reais desta implementação

Considere V e E ativos, Cv e Ce capacidades reservadas, e d(v) o grau.
Remoções podem criar lacunas: varrer IDs globais custa até O(Cv) ou O(Ce).

| Operação | Tempo |
|---|---|
| Existência por ID, alfa/ômega, vizinho, contagens/capacidades | O(1) |
| Criar vértice/aresta com espaço disponível | O(1) |
| Remover aresta | O(1) |
| Remover vértice | O(d(v)) |
| Grau, buscar uma aresta pelo par de extremos | O(d(v)) |
| Próxima aresta de uma estrela a partir de um membro ativo | O(1) |
| Próxima de uma estrela a partir de ID externo/removido | O(d(v)) |
| BFS/DFS da origem | O(Cv + Vr + Er), com Vr/Er alcançáveis |
| Componentes | O(Cv + E) |
| Dijkstra com seleção linear | O(V * Cv + E + Ce) |
| Kruskal | O(Cv + Ce + E log E), ordenação usual da biblioteca |
| Salvar/carregar | O(Cv + Ce) na topologia compactada correspondente |

Espaço permanente da TAD: O(Cv + Ce). BFS, DFS e Dijkstra reservam O(Cv) auxiliar.
Kruskal reserva O(Cv + E). Sem lacunas e com capacidades proporcionais ao tamanho,
os percursos completos ficam O(V + E), Dijkstra O(V² + E) e Kruskal O(V + E log E).
Esses custos descrevem o código entregue, e não versões com heap que não foram
implementadas.

Pilha e fila têm operações usuais O(1), tamanho O(1) e destruição O(n).
Na lista, consulta/localização/remoção e inserção no meio são O(n); inserções nas
extremidades são O(1). Pcheia, Fcheia e Lcheia sondam a alocação de um nó:
esse resultado não garante que uma alocação posterior terá sucesso.

## 7. Validação

test_tads.c cobre as operações de pilha, fila e lista, as 31 operações de grafo,
capacidades, laços, paralelas, persistência e remoções. Compara estrelas e graus
contra um modelo independente em 540 estados de remoção.

test_grafos.py contém dez grupos de regressão, incluindo saída exata do exemplo
do TP 1, direção, desconexão, vazio, isolado, laços, paralelas, pesos zero e negativos,
arquivos/argumentos inválidos e DFS com cadeia de 4.000 vértices.
Em 25 grafos aleatórios determinísticos, compara BFS/DFS com referências Python,
distâncias com Bellman-Ford e o custo da AGM com enumeração exaustiva de subconjuntos.
Também verifica se cada aresta do caminho corresponde aos vértices impressos.

A execução em 19/10/2026 passou com GCC 6.3.0, C11 e todos os avisos tratados como
erros. A suite não simula esgotamento de memória nem comprova todos os tamanhos
possíveis. A pasta adicional de árvores não é compilada por não integrar os TPs.

## 8. Referências e elaboração

- Walace de Almeida Rodrigues. TP - TADs basicas.pdf. Enunciado da disciplina.
- Walace de Almeida Rodrigues. TP - TAD Grafo estatico.pdf. Especificação, pp. 3-10.
- Walace de Almeida Rodrigues. TP - algoritmos.pdf. Requisitos do TP 2,
  consultados anteriormente nesta tarefa no Classroom.
- Publicação complementar do TP 1: remoção de vértices e arestas.

Código e documentação foram concluídos com assistência de IA nesta tarefa. O
relatório descreve decisões e verificações técnicas; a avaliação pessoal da
experiência e a preparação para a apresentação oral cabem ao aluno.
