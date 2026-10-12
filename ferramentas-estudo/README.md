# Laboratório de grafos e árvores

Ferramentas de apoio separadas de `04-grafos/grafo.c`, `04-grafos/algoritmos.c` e `05-arvores/arvore.c`. Nenhum TODO acadêmico é implementado aqui. O contrato atual é JSON: seu programa em C poderá **emitir um arquivo** nesse formato quando as operações correspondentes estiverem prontas. Não é necessário alterar a TAD para usar os exemplos. A página mostra os formatos de grafo, árvore, rastro e resultado, com modelos que podem ser abertos, copiados e baixados.

## Navegar e importar

A primeira aba, **Laboratório e importação**, tem o botão **Importar meu arquivo .json** logo no início. Ele aceita grafos (`kind: "graph"`) e árvores (`kind: "binary_tree"`) salvos no computador. O visualizador também possui um seletor de arquivos e botões para abrir exemplos. Modelos JSON ficam recolhidos em “Ver formato JSON”; não é preciso editá-los para usar o site.

As demais abas trazem 25 conceitos com exemplos, uma galeria de três redes ilustrativas grandes, links para redes reais, três projetos públicos de empresas, seis aplicações e nove visualizações passo a passo de ordenação e busca. Na aba **Algoritmos**, o estado dos dados e a linha de código aparecem juntos. As visualizações de BFS e DFS usam um grafo didático fixo; não são saídas das funções acadêmicas em C.

As imagens da galeria são sintéticas e geradas no próprio navegador. Para dados reais, a página aponta para os conjuntos [Facebook Social Circles](https://snap.stanford.edu/data/egonets-Facebook.html) e [California road network](https://snap.stanford.edu/data/roadNet-CA.html) da Stanford SNAP. A aba **Código público** aponta para [Uber H3](https://github.com/uber/h3), [Google OR-Tools](https://github.com/google/or-tools) e [Meta Faiss](https://github.com/facebookresearch/faiss). Os pequenos trechos exibidos ali são exemplos didáticos de uso das APIs; os links “Ver código real” levam aos arquivos públicos. H3 é um índice espacial com vizinhança entre células, não o código privado de despacho da Uber.

## Executar no Windows

Para estudar no navegador com Live Server, execute `npm ci` nesta pasta e abra `index.html` pelo servidor local. O visualizador usa Graphviz compilado em WebAssembly para organizar grafos e árvores; o rastro, comparador e verificador também funcionam no navegador, sem Python nem `dot.exe` global. Os botões "Arquivos prontos da pasta exemplos" abrem os JSONs do projeto diretamente. Para arquivos de outras pastas, use o seletor do navegador. O navegador não permite apontar esse seletor automaticamente para um caminho absoluto local.

Para os comandos de terminal e o servidor Python, use Python 3.10+ e uma das opções abaixo:

1. Instalar [Graphviz para Windows](https://graphviz.org/download/) e adicionar a pasta de `dot.exe` ao PATH. Confirme com `dot -V` em um terminal novo.
2. Instalar Node.js e, nesta pasta, executar `npm ci`. Essa opção usa [Graphviz compilado em WebAssembly](https://github.com/hpcc-systems/hpcc-js-wasm) pelo pacote `@hpcc-js/wasm`; não exige `dot.exe` global.

No PowerShell, a partir de `A:\Projetos\estrutura-dados\ferramentas-estudo`:

```powershell
python .\study.py render .\exemplos\grafo.json
python .\study.py render .\exemplos\arvore.json
python .\study.py serve --port 8765
```

Abra `http://127.0.0.1:8765` no navegador. Os dois primeiros comandos geram `output/ferramentas-estudo/grafo.svg`, `grafo.dot`, `arvore.svg` e `arvore.dot` na pasta de saídas já existente do projeto. Para apontar para um `dot.exe` fora do PATH, use `--dot-exe "C:\caminho\Graphviz\bin\dot.exe"` no comando `render`. `node_modules/` é ignorado pelo Git.

Para **compilar os esqueletos atuais em C** com GCC, a partir da raiz do projeto:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic .\04-grafos\main.c .\04-grafos\grafo.c .\04-grafos\caminho.c .\04-grafos\algoritmos.c -o .\output\ferramentas-estudo\grafos.exe
gcc -std=c11 -Wall -Wextra -Wpedantic .\05-arvores\main.c .\05-arvores\arvore.c -o .\output\ferramentas-estudo\arvores.exe
```

Eles compilam, mas seus `main.c` e operações ainda são esqueletos; executar esses `.exe` não produz grafos nem árvores até você implementar o trabalho acadêmico. A interface de estudo roda via `study.py` e exemplos JSON.

## Contrato de intercâmbio v1

Todos os IDs são textos únicos. Use, por exemplo, `"1"` e `"a1"` para os IDs inteiros da TAD. Vértices são declarados separadamente, inclusive os isolados. Arestas têm IDs próprios para preservar laços e arestas paralelas. `weight` e `label` são opcionais. O sentido global vem de `directed`.

```json
{"version":1,"kind":"graph","directed":true,
 "vertices":[{"id":"1"},{"id":"2"},{"id":"3"}],
 "edges":[{"id":"a1","from":"1","to":"2","weight":2}]}
```

Árvore binária como estrutura de dados usa **outro** `kind`. `left` e `right` indicam posições, não arestas arbitrárias; `root` marca a raiz. Uma árvore geradora de um grafo permanece um resultado sobre `kind: "graph"`.

```json
{"version":1,"kind":"binary_tree","root":"8",
 "nodes":[{"id":"8","left":"3","right":"10"},{"id":"3"},{"id":"10"}]}
```

O rastro contém instantâneos completos e ordenados. Isso torna avanço e retorno determinísticos: voltar mostra o evento anterior sem executar algoritmo algum. `queue`, `stack`, `visited`, `highlight_vertices` e `highlight_edges` são opcionais; omitir indica que a informação não foi registrada. `graph` é o nome informativo do arquivo correspondente; na interface, importe primeiro esse grafo. O exemplo é fictício e **não** é saída da implementação acadêmica.

```json
{"version":1,"kind":"trace","graph":"grafo.json","algorithm":"BFS",
 "events":[{"label":"Início","current":"1","queue":["1"],
 "visited":["1"],"highlight_vertices":["1"],"highlight_edges":[]}]}
```

O comparador mostra IDs e pesos na matriz e na lista. Para grafo não dirigido, cada aresta aparece nas duas incidências; laço aparece uma vez por célula/linha. Uma matriz simples de 0/1 ou de um único peso não distingue paralelas, por isso a interface lista os IDs dentro da célula. Seleção por vértice ou aresta destaca a mesma entidade nas três vistas.

## Verificador

No terminal:

```powershell
python .\study.py verify .\exemplos\grafo.json .\exemplos\resultado-caminho-valido.json
python .\study.py verify .\exemplos\grafo.json .\exemplos\resultado-caminho-invalido.json
python .\study.py verify .\exemplos\grafo.json .\exemplos\resultado-busca-valida.json
python .\study.py verify .\exemplos\grafo.json .\exemplos\resultado-busca-invalida.json
python .\study.py verify .\exemplos\grafo-nao-dirigido.json .\exemplos\resultado-arvore-geradora-valida.json
python .\study.py verify .\exemplos\grafo-nao-dirigido.json .\exemplos\resultado-arvore-geradora-invalida.json
```

Saída `ACEITO` retorna código 0; `REJEITADO` retorna 1. `path` verifica um caminho simples (sem repetição de vértices), existência de cada transição e, se fornecidos, os IDs das arestas. `search` verifica início, unicidade e alcançabilidade; com `complete:true`, exige todos os alcançáveis. `spanning_tree` aceita somente grafo não dirigido e verifica que as arestas escolhidas existem, não formam ciclo e abrangem os vértices. Em caso de falha, a mensagem identifica passo, vértice ou aresta quando possível.

Essas são **checagens estruturais**. Não demonstram que um caminho é ótimo, que uma busca seguiu a ordem exata de BFS/DFS, nem que uma árvore geradora tem peso mínimo. Uma busca parcial pode omitir vértices quando `complete` está ausente ou falso.

## Integração futura com C

Quando `GVprimeiroVertice`/`GVproximoVertice`, `GAprimeiraAresta`/`GAproximaAresta`, `GValfa`/`GVomega` e demais consultas estiverem funcionando, um adaptador separado poderá percorrer a TAD e escrever `graph` v1. Para rastro de busca, só adicione emissão de eventos após sua implementação de BFS/DFS estar funcional e testada; cada evento deve registrar o estado necessário para retroceder. A ferramenta atual não chama nem substitui as operações acadêmicas.
