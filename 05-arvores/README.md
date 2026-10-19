# Árvore binária de busca (ABB)

Inteiros distintos: valores menores ficam à esquerda; maiores, à direita.
A árvore não possui balanceamento automático. A implementação usa apenas C11 e
a biblioteca padrão e não altera os módulos de pilha, fila ou lista.

## Executar

Na raiz, compile com compilar.ps1 e execute:

~~~powershell
.\output\arvores.exe
~~~

O menu oferece inserção, busca, remoção, tamanho, altura, mínimo, máximo, os três
percursos e consultas de vazio/cheio.

Para uma demonstração sem menu:

~~~powershell
.\output\arvores.exe 8 3 10 1 6 14 4 7 13
~~~

Saída:

~~~text
Quantidade: 9
Altura: 3
Menor: 1
Maior: 14
Pre-ordem: 8 3 1 6 4 7 10 14 13
Em ordem: 1 3 4 6 7 8 10 13 14
Pos-ordem: 1 4 7 6 3 13 14 10 8
~~~

## Convenções

- Altura em arestas: árvore vazia tem -1 e folha tem 0.
- Repetidos não são inseridos; Ainsere retorna 0.
- Abusca, Amenor e Amaior preenchem o parâmetro de saída somente em sucesso.
- Remover um nó com dois filhos utiliza seu sucessor.
- Altura e percursos usam pilha explícita. Não há recursão nessas operações.
- Adestroi libera todos os nós sem recursão e sem nova alocação.
- Após Adestroi, o ponteiro do chamador não deve ser utilizado.
- Aaltura retorna -2 se a pilha auxiliar não puder ser alocada.
- Sem argumentos, EOF encerra o menu. Argumentos inválidos ou repetidos produzem
  código de saída 2 na demonstração.

## Custos

Com n nós e altura h: inserção, busca, remoção, mínimo e máximo são O(h).
Sem balanceamento, h pode ser n - 1. Tamanho/vazio são O(1). Altura e percursos
fazem O(n) operações e reservam O(n) memória auxiliar; destruir custa O(n) e
espaço auxiliar O(1).

Execute testar.ps1 para compilar e testar todas as estruturas.
