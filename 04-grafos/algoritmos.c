/* Nome: Ayrton Sanabio | Matricula: 0117925 | Data: 19/10/2026 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"
#include "caminho.h"

static int primeira(Grafo g, int v, int dirigido) {
    return dirigido ? GAprimaSaida(g, v) : GAprimaAresta(g, v);
}
static int proxima(Grafo g, int v, int a, int dirigido) {
    return dirigido ? GAproxSaida(g, v, a) : GAproxAresta(g, v, a);
}
static int vizinho(Grafo g, int a, int v, int dirigido) {
    return dirigido ? GVomega(g, a) : GVvizinho(g, a, v);
}

void GBuscaLarguraModo(Grafo g, int partida, int dirigido) {
    int *visitado, *fila, inicio = 0, fim = 0;
    size_t n = (size_t)GInumeroVerticesMax(g) + 1;
    if (!GBexisteIdVertice(g, partida)) return;
    visitado = calloc(n, sizeof(*visitado));
    fila = calloc(n, sizeof(*fila));
    if (!visitado || !fila) {
        free(visitado); free(fila);
        fputs("Memoria insuficiente para BFS.\n", stderr); return;
    }
    visitado[partida] = 1;
    fila[fim++] = partida;
    printf("v%d", partida);
    while (inicio < fim) {
        int u = fila[inicio++], a;
        for (a = primeira(g, u, dirigido); a; a = proxima(g, u, a, dirigido)) {
            int v = vizinho(g, a, u, dirigido);
            if (!visitado[v]) {
                visitado[v] = 1;
                fila[fim++] = v;
                printf(",a%d,v%d", a, v);
            }
        }
    }
    putchar('\n');
    free(visitado); free(fila);
}
void GBuscaLargura(Grafo g, int partida) { GBuscaLarguraModo(g, partida, 1); }

/* Cada quadro guarda o vertice e a proxima aresta, simulando a DFS recursiva. */
void GBuscaProfundidadeModo(Grafo g, int partida, int dirigido) {
    int *visitado, *pilha, *proxima_aresta, topo = 0;
    size_t n = (size_t)GInumeroVerticesMax(g) + 1;
    if (!GBexisteIdVertice(g, partida)) return;
    visitado = calloc(n, sizeof(*visitado));
    pilha = calloc(n, sizeof(*pilha));
    proxima_aresta = calloc(n, sizeof(*proxima_aresta));
    if (!visitado || !pilha || !proxima_aresta) {
        free(visitado); free(pilha); free(proxima_aresta);
        fputs("Memoria insuficiente para DFS.\n", stderr); return;
    }
    visitado[partida] = 1;
    pilha[0] = partida;
    proxima_aresta[0] = primeira(g, partida, dirigido);
    printf("v%d", partida);
    while (topo >= 0) {
        int u = pilha[topo], a = proxima_aresta[topo], v;
        if (!a) { --topo; continue; }
        proxima_aresta[topo] = proxima(g, u, a, dirigido);
        v = vizinho(g, a, u, dirigido);
        if (!visitado[v]) {
            visitado[v] = 1;
            printf(",a%d,v%d", a, v);
            pilha[++topo] = v;
            proxima_aresta[topo] = primeira(g, v, dirigido);
        }
    }
    putchar('\n');
    free(visitado); free(pilha); free(proxima_aresta);
}
void GBuscaProfundidade(Grafo g, int partida) { GBuscaProfundidadeModo(g, partida, 1); }

void GComponentesConexos(Grafo g) {
    size_t n = (size_t)GInumeroVerticesMax(g) + 1;
    int *visitado, *fila, raiz, primeiro = 1;
    if (!g) return;
    visitado = calloc(n, sizeof(*visitado)); fila = calloc(n, sizeof(*fila));
    if (!visitado || !fila) {
        free(visitado); free(fila);
        fputs("Memoria insuficiente para componentes.\n", stderr); return;
    }
    for (raiz = GVprimeiroVertice(g); raiz; raiz = GVproximoVertice(g, raiz)) {
        int inicio = 0, fim = 0;
        if (visitado[raiz]) continue;
        printf(primeiro ? "{%d" : " {%d", raiz); primeiro = 0;
        visitado[raiz] = 1; fila[fim++] = raiz;
        while (inicio < fim) {
            int u = fila[inicio++], a;
            for (a = GAprimaAresta(g, u); a; a = GAproxAresta(g, u, a)) {
                int v = GVvizinho(g, a, u);
                if (!visitado[v]) {
                    visitado[v] = 1; fila[fim++] = v; printf(",%d", v);
                }
            }
        }
        putchar('}');
    }
    if (primeiro) fputs("{}", stdout);
    putchar('\n'); free(visitado); free(fila);
}

typedef struct { int id, u, v; float peso; } ArestaOrdenada;
static int compara_arestas(const void *x, const void *y) {
    const ArestaOrdenada *a = x, *b = y;
    if (a->peso < b->peso) return -1;
    if (a->peso > b->peso) return 1;
    return (a->id > b->id) - (a->id < b->id);
}
static int representante(int *pais, int v) {
    while (pais[v] != v) {
        pais[v] = pais[pais[v]];
        v = pais[v];
    }
    return v;
}

void GArvoreGeradoraMinimaComPesos(Grafo g, const float *pesos) {
    size_t n = (size_t)GInumeroVerticesMax(g) + 1;
    int quantidade = GInumeroArestas(g), *pais, *tamanhos, *escolhidas;
    int a, v, i = 0, usadas = 0;
    ArestaOrdenada *arestas;
    if (!g) return;
    arestas = calloc((size_t)quantidade + 1, sizeof(*arestas));
    escolhidas = calloc((size_t)GInumeroVertices(g) + 1, sizeof(*escolhidas));
    pais = calloc(n, sizeof(*pais)); tamanhos = calloc(n, sizeof(*tamanhos));
    if (!arestas || !escolhidas || !pais || !tamanhos) {
        fputs("Memoria insuficiente para AGM.\n", stderr); goto fim;
    }
    for (v = GVprimeiroVertice(g); v; v = GVproximoVertice(g, v)) {
        pais[v] = v; tamanhos[v] = 1;
    }
    for (a = GAprimeiraAresta(g); a; a = GAproximaAresta(g, a)) {
        float peso = pesos ? pesos[a] : 1.0f;
        if (!isfinite(peso)) { fputs("Peso nao finito na AGM.\n", stderr); goto fim; }
        arestas[i++] = (ArestaOrdenada){a, GValfa(g, a), GVomega(g, a), peso};
    }
    qsort(arestas, (size_t)quantidade, sizeof(*arestas), compara_arestas);
    for (i = 0; i < quantidade; ++i) {
        int u = representante(pais, arestas[i].u), w = representante(pais, arestas[i].v);
        if (u == w) continue;
        if (tamanhos[u] < tamanhos[w]) { int t = u; u = w; w = t; }
        pais[w] = u; tamanhos[u] += tamanhos[w];
        escolhidas[usadas++] = arestas[i].id;
    }
    if (GInumeroVertices(g) > 0 && usadas != GInumeroVertices(g) - 1) {
        puts("Sem AGM: grafo desconexo."); goto fim;
    }
    for (i = 0; i < usadas; ++i) printf(i ? ",a%d" : "a%d", escolhidas[i]);
    putchar('\n');
fim:
    free(arestas); free(escolhidas); free(pais); free(tamanhos);
}
void GArvoreGeradoraMinima(Grafo g) { GArvoreGeradoraMinimaComPesos(g, NULL); }

void GCaminhoMaisCurtoComPesos(Grafo g, const float *pesos, int partida,
                             int chegada, int dirigido) {
    Caminho caminho;
    int status = GcalculaCaminho(g, pesos, partida, chegada, dirigido, &caminho);
    if (status < 0) { fputs("Caminho invalido: vertices, pesos ou memoria.\n", stderr); return; }
    if (!status) { puts("Sem caminho."); return; }
    printf("v%d", caminho.vertices[0]);
    for (int i = 1; i < caminho.quantidade; ++i)
        printf(",a%d,v%d", caminho.arestas[i - 1], caminho.vertices[i]);
    printf("\nDistancia: %.9g\n", caminho.distancia);
    GliberaCaminho(&caminho);
}
void GCaminhoMaisCurto(Grafo g, int partida, int chegada) {
    GCaminhoMaisCurtoComPesos(g, NULL, partida, chegada, 1);
}
