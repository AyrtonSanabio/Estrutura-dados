/* Nome: Ayrton Sanabio | Matricula: 0117925 | Data: 19/10/2026 */
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caminho.h"

void GliberaCaminho(Caminho *caminho) {
    if (!caminho) return;
    free(caminho->vertices);
    free(caminho->arestas);
    memset(caminho, 0, sizeof(*caminho));
}

/* Dijkstra com selecao linear: nao exige heap nem biblioteca externa. */
int GcalculaCaminho(Grafo g, const float *pesos, int partida, int chegada,
                   int dirigido, Caminho *resultado) {
    int capacidade = GInumeroVerticesMax(g), v, u, a, tamanho, status = -1;
    double *distancia = NULL;
    int *anterior = NULL, *aresta_anterior = NULL, *fechado = NULL;
    if (!resultado) return -1;
    memset(resultado, 0, sizeof(*resultado));
    if (!GBexisteIdVertice(g, partida) || !GBexisteIdVertice(g, chegada)) return -1;
    for (a = GAprimeiraAresta(g); a; a = GAproximaAresta(g, a))
        if (pesos && (!isfinite(pesos[a]) || pesos[a] < 0)) return -1;
    tamanho = capacidade + 1;
    distancia = calloc((size_t)tamanho, sizeof(*distancia));
    anterior = calloc((size_t)tamanho, sizeof(*anterior));
    aresta_anterior = calloc((size_t)tamanho, sizeof(*aresta_anterior));
    fechado = calloc((size_t)tamanho, sizeof(*fechado));
    resultado->vertices = calloc((size_t)tamanho, sizeof(*resultado->vertices));
    resultado->arestas = calloc((size_t)tamanho, sizeof(*resultado->arestas));
    if (!distancia || !anterior || !aresta_anterior || !fechado ||
        !resultado->vertices || !resultado->arestas) goto fim;
    for (v = GVprimeiroVertice(g); v; v = GVproximoVertice(g, v)) distancia[v] = DBL_MAX;
    distancia[partida] = 0;
    for (;;) {
        double menor = DBL_MAX;
        u = 0;
        for (v = GVprimeiroVertice(g); v; v = GVproximoVertice(g, v))
            if (!fechado[v] && distancia[v] < menor) { menor = distancia[v]; u = v; }
        if (!u || u == chegada) break;
        fechado[u] = 1;
        a = dirigido ? GAprimaSaida(g, u) : GAprimaAresta(g, u);
        while (a) {
            double peso = pesos ? pesos[a] : 1.0;
            double candidato = distancia[u] + peso;
            v = dirigido ? GVomega(g, a) : GVvizinho(g, a, u);
            if (!fechado[v] && candidato < distancia[v]) {
                distancia[v] = candidato;
                anterior[v] = u;
                aresta_anterior[v] = a;
            }
            a = dirigido ? GAproxSaida(g, u, a) : GAproxAresta(g, u, a);
        }
    }
    if (distancia[chegada] == DBL_MAX) { status = 0; goto fim; }
    /* Primeiro monta destino -> origem; depois inverte vertices e arestas. */
    for (v = chegada; v; v = anterior[v]) {
        int i = resultado->quantidade++;
        resultado->vertices[i] = v;
        resultado->arestas[i] = aresta_anterior[v];
    }
    for (int i = 0; i < resultado->quantidade / 2; ++i) {
        int j = resultado->quantidade - i - 1, temporario = resultado->vertices[i];
        resultado->vertices[i] = resultado->vertices[j];
        resultado->vertices[j] = temporario;
    }
    /* arestas reversas: [aresta do destino, ..., aresta do primeiro filho, 0]. */
    for (int i = 0; i < (resultado->quantidade - 1) / 2; ++i) {
        int j = resultado->quantidade - i - 2, temporario = resultado->arestas[i];
        resultado->arestas[i] = resultado->arestas[j];
        resultado->arestas[j] = temporario;
    }
    resultado->distancia = distancia[chegada];
    status = 1;
fim:
    free(distancia); free(anterior); free(aresta_anterior); free(fechado);
    if (status != 1) GliberaCaminho(resultado);
    return status;
}

void Gcaminho(Grafo grafo, float *pesos, int partida, int chegada) {
    Caminho caminho;
    int status = GcalculaCaminho(grafo, pesos, partida, chegada, 1, &caminho);
    if (status < 0) { fputs("Caminho invalido: vertices, pesos ou memoria.\n", stderr); return; }
    if (!status) { printf("%d\n", partida); return; }
    for (int i = 0; i < caminho.quantidade; ++i)
        printf(i ? " -> %d" : "%d", caminho.vertices[i]);
    putchar('\n');
    GliberaCaminho(&caminho);
}
