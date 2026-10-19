/* Nome: Ayrton Sanabio | Matricula: 0117925 | Data: 19/10/2026 */
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmos.h"
#include "caminho.h"
#include "grafo.h"

static void uso(void) {
    fputs("Uso: grafos <grafo.txt> <operacao> [argumentos] [--pesos pesos.txt] [--nao-dirigido]\n"
          "Operacoes: arestas | bfs inicio | dfs inicio | componentes | agm\n"
          "           caminho inicio fim | tp1 inicio fim | salvar destino.txt\n"
          "           remover-vertice id destino.txt | remover-aresta id destino.txt\n"
          "Grafo: primeira linha V E; depois E linhas com origem destino.\n"
          "Pesos: um numero por aresta, na ordem do arquivo; padrao = 1.\n", stderr);
}
static int le_id(const char *texto, int *valor) {
    char *fim;
    long numero;
    errno = 0;
    numero = strtol(texto, &fim, 10);
    if (errno || fim == texto || *fim || numero <= 0 || numero >= INT_MAX) return 0;
    *valor = (int)numero;
    return 1;
}
static float *le_pesos(Grafo g, const char *nome) {
    float *pesos = calloc((size_t)GInumeroArestasMax(g) + 1, sizeof(*pesos));
    FILE *arquivo = NULL;
    char token[128], *fim;
    int a;
    if (!pesos) return NULL;
    if (nome && !(arquivo = fopen(nome, "r"))) { free(pesos); return NULL; }
    for (a = GAprimeiraAresta(g); a; a = GAproximaAresta(g, a)) {
        pesos[a] = 1;
        if (arquivo) {
            if (fscanf(arquivo, "%127s", token) != 1) goto erro;
            errno = 0;
            pesos[a] = strtof(token, &fim);
            if (errno || fim == token || *fim || !isfinite(pesos[a])) goto erro;
        }
    }
    if (arquivo) {
        if (fscanf(arquivo, "%127s", token) == 1 || ferror(arquivo)) goto erro;
        fclose(arquivo);
    }
    return pesos;
erro:
    if (arquivo) fclose(arquivo);
    free(pesos);
    return NULL;
}

int main(int argc, char **argv) {
    Grafo g;
    float *pesos;
    const char *operacao, *arquivo_pesos = NULL, *args[2];
    int nargs = 0, dirigido = 1, partida = 0, chegada = 0, status = 0;
    if (argc < 3) { uso(); return 2; }
    operacao = argv[2];
    for (int i = 3; i < argc; ++i) {
        if (!strcmp(argv[i], "--pesos")) {
            if (arquivo_pesos || i + 1 == argc) { uso(); return 2; }
            arquivo_pesos = argv[++i];
        } else if (!strcmp(argv[i], "--nao-dirigido")) dirigido = 0;
        else if (!strncmp(argv[i], "--", 2) || nargs == 2) { uso(); return 2; }
        else args[nargs++] = argv[i];
    }
    if (!strcmp(operacao, "arestas") || !strcmp(operacao, "componentes") || !strcmp(operacao, "agm")) {
        if (nargs) { uso(); return 2; }
    } else if (!strcmp(operacao, "bfs") || !strcmp(operacao, "dfs")) {
        if (nargs != 1 || !le_id(args[0], &partida)) { uso(); return 2; }
    } else if (!strcmp(operacao, "caminho") || !strcmp(operacao, "tp1")) {
        if (nargs != 2 || !le_id(args[0], &partida) || !le_id(args[1], &chegada) ||
            (!strcmp(operacao, "tp1") && !dirigido)) { uso(); return 2; }
    } else if (!strcmp(operacao, "salvar")) {
        if (nargs != 1) { uso(); return 2; }
    } else if (!strcmp(operacao, "remover-vertice") || !strcmp(operacao, "remover-aresta")) {
        if (nargs != 2 || !le_id(args[0], &partida)) { uso(); return 2; }
    } else { uso(); return 2; }
    g = GGcarregaGrafo(argv[1]);
    if (!g) { fputs("Arquivo de grafo invalido ou memoria insuficiente.\n", stderr); return 2; }
    pesos = le_pesos(g, arquivo_pesos);
    if (!pesos) { fputs("Arquivo de pesos invalido ou memoria insuficiente.\n", stderr); GGdestroiGrafo(g); return 2; }
    if (partida && strcmp(operacao, "remover-aresta") && !GBexisteIdVertice(g, partida)) {
        fputs("Vertice de partida inexistente.\n", stderr); status = 2; goto fim;
    }
    if (chegada && !GBexisteIdVertice(g, chegada)) {
        fputs("Vertice de chegada inexistente.\n", stderr); status = 2; goto fim;
    }
    if (!strcmp(operacao, "caminho") || !strcmp(operacao, "tp1")) {
        for (int a = GAprimeiraAresta(g); a; a = GAproximaAresta(g, a))
            if (pesos[a] < 0) { fputs("Dijkstra exige pesos nao negativos.\n", stderr); status = 2; goto fim; }
    }
    if (!strcmp(operacao, "arestas")) {
        for (int a = GAprimeiraAresta(g); a; a = GAproximaAresta(g, a))
            printf("a%d: v%d -> v%d\n", a, GValfa(g, a), GVomega(g, a));
    } else if (!strcmp(operacao, "bfs")) GBuscaLarguraModo(g, partida, dirigido);
    else if (!strcmp(operacao, "dfs")) GBuscaProfundidadeModo(g, partida, dirigido);
    else if (!strcmp(operacao, "componentes")) GComponentesConexos(g);
    else if (!strcmp(operacao, "agm")) GArvoreGeradoraMinimaComPesos(g, pesos);
    else if (!strcmp(operacao, "caminho")) GCaminhoMaisCurtoComPesos(g, pesos, partida, chegada, dirigido);
    else if (!strcmp(operacao, "tp1")) Gcaminho(g, pesos, partida, chegada);
    else if (!strcmp(operacao, "salvar")) {
        if (!GBsalvaGrafo(g, args[0])) { fputs("Nao foi possivel salvar.\n", stderr); status = 2; }
    } else {
        int removido = !strcmp(operacao, "remover-vertice") ?
            removeVertice(g, partida) : removeAresta(g, partida);
        if (!removido || !GBsalvaGrafo(g, args[1])) {
            fputs("Remocao ou gravacao invalida.\n", stderr); status = 2;
        }
    }
fim:
    free(pesos);
    GGdestroiGrafo(g);
    return status;
}
