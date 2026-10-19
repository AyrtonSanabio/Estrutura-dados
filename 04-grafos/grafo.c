/* Nome: Ayrton Sanabio | Matricula: 0117925 | Data: 19/10/2026 */
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

/* Listas de IDs crescentes em vetores de capacidade fixa. Zero indica ausencia. */
typedef struct {
    int ativo, primeira_saida, ultima_saida, primeira_entrada, ultima_entrada;
    int primeira_incidente, ultima_incidente;
} Vertice;
typedef struct {
    int ativa, alfa, omega;
    int proxima_saida, anterior_saida, proxima_entrada, anterior_entrada;
    int proxima_alfa, anterior_alfa, proxima_omega, anterior_omega;
} Aresta;
struct grafo {
    int max_vertices, max_arestas, ultimo_vertice, ultima_aresta;
    int numero_vertices, numero_arestas;
    Vertice *vertices;
    Aresta *arestas;
};

Grafo GGcriaGrafo(int max_vertices, int max_arestas) {
    Grafo g;
    if (max_vertices <= 0 || max_arestas <= 0 ||
        max_vertices == INT_MAX || max_arestas == INT_MAX ||
        (size_t)max_vertices + 1 > SIZE_MAX / sizeof(Vertice) ||
        (size_t)max_arestas + 1 > SIZE_MAX / sizeof(Aresta)) return NULL;
    g = calloc(1, sizeof(*g));
    if (!g) return NULL;
    g->vertices = calloc((size_t)max_vertices + 1, sizeof(*g->vertices));
    g->arestas = calloc((size_t)max_arestas + 1, sizeof(*g->arestas));
    if (!g->vertices || !g->arestas) return GGdestroiGrafo(g);
    g->max_vertices = max_vertices;
    g->max_arestas = max_arestas;
    return g;
}

Grafo GGdestroiGrafo(Grafo g) {
    if (g) {
        free(g->vertices);
        free(g->arestas);
        free(g);
    }
    return NULL;
}
int GBexisteIdVertice(Grafo g, int v) {
    return g && v > 0 && v <= g->ultimo_vertice && g->vertices[v].ativo;
}
int GBexisteIdAresta(Grafo g, int a) {
    return g && a > 0 && a <= g->ultima_aresta && g->arestas[a].ativa;
}
int GVcriaVertice(Grafo g) {
    int id;
    if (!g || g->ultimo_vertice == g->max_vertices) return 0;
    id = ++g->ultimo_vertice;
    g->vertices[id].ativo = 1;
    ++g->numero_vertices;
    return id;
}
static int *proxima_incidente(Grafo g, int a, int v) {
    return g->arestas[a].alfa == v ? &g->arestas[a].proxima_alfa : &g->arestas[a].proxima_omega;
}
static int *anterior_incidente(Grafo g, int a, int v) {
    return g->arestas[a].alfa == v ? &g->arestas[a].anterior_alfa : &g->arestas[a].anterior_omega;
}
static void liga_incidente(Grafo g, int a, int v) {
    int anterior = g->vertices[v].ultima_incidente;
    *anterior_incidente(g, a, v) = anterior;
    if (anterior) *proxima_incidente(g, anterior, v) = a;
    else g->vertices[v].primeira_incidente = a;
    g->vertices[v].ultima_incidente = a;
}
static void desliga_incidente(Grafo g, int a, int v) {
    int anterior = *anterior_incidente(g, a, v), proxima = *proxima_incidente(g, a, v);
    if (anterior) *proxima_incidente(g, anterior, v) = proxima;
    else g->vertices[v].primeira_incidente = proxima;
    if (proxima) *anterior_incidente(g, proxima, v) = anterior;
    else g->vertices[v].ultima_incidente = anterior;
}
int GAcriaAresta(Grafo g, int partida, int chegada) {
    int id, anterior;
    Aresta *a;
    if (!GBexisteIdVertice(g, partida) || !GBexisteIdVertice(g, chegada) ||
        g->ultima_aresta == g->max_arestas) return 0;
    id = ++g->ultima_aresta;
    a = &g->arestas[id];
    a->ativa = 1;
    a->alfa = partida;
    a->omega = chegada;
    anterior = g->vertices[partida].ultima_saida;
    a->anterior_saida = anterior;
    if (anterior) g->arestas[anterior].proxima_saida = id;
    else g->vertices[partida].primeira_saida = id;
    g->vertices[partida].ultima_saida = id;
    anterior = g->vertices[chegada].ultima_entrada;
    a->anterior_entrada = anterior;
    if (anterior) g->arestas[anterior].proxima_entrada = id;
    else g->vertices[chegada].primeira_entrada = id;
    g->vertices[chegada].ultima_entrada = id;
    liga_incidente(g, id, partida);
    if (partida != chegada) liga_incidente(g, id, chegada);
    ++g->numero_arestas;
    return id;
}

int removeAresta(Grafo g, int id) {
    Aresta *a;
    Vertice *origem, *destino;
    if (!GBexisteIdAresta(g, id)) return 0;
    a = &g->arestas[id];
    origem = &g->vertices[a->alfa];
    destino = &g->vertices[a->omega];
    if (a->anterior_saida) g->arestas[a->anterior_saida].proxima_saida = a->proxima_saida;
    else origem->primeira_saida = a->proxima_saida;
    if (a->proxima_saida) g->arestas[a->proxima_saida].anterior_saida = a->anterior_saida;
    else origem->ultima_saida = a->anterior_saida;
    if (a->anterior_entrada) g->arestas[a->anterior_entrada].proxima_entrada = a->proxima_entrada;
    else destino->primeira_entrada = a->proxima_entrada;
    if (a->proxima_entrada) g->arestas[a->proxima_entrada].anterior_entrada = a->anterior_entrada;
    else destino->ultima_entrada = a->anterior_entrada;
    desliga_incidente(g, id, a->alfa);
    if (a->alfa != a->omega) desliga_incidente(g, id, a->omega);
    memset(a, 0, sizeof(*a));
    --g->numero_arestas;
    return 1;
}
int removeVertice(Grafo g, int v) {
    Vertice *vertice;
    if (!GBexisteIdVertice(g, v)) return 0;
    vertice = &g->vertices[v];
    while (vertice->primeira_saida) removeAresta(g, vertice->primeira_saida);
    while (vertice->primeira_entrada) removeAresta(g, vertice->primeira_entrada);
    memset(vertice, 0, sizeof(*vertice));
    --g->numero_vertices;
    return 1;
}
int GVproximoVertice(Grafo g, int v) {
    if (!g || v >= g->ultimo_vertice) return 0;
    if (v < 0) v = 0;
    for (++v; v <= g->ultimo_vertice; ++v)
        if (g->vertices[v].ativo) return v;
    return 0;
}
int GVprimeiroVertice(Grafo g) { return GVproximoVertice(g, 0); }
int GAproximaAresta(Grafo g, int a) {
    if (!g || a >= g->ultima_aresta) return 0;
    if (a < 0) a = 0;
    for (++a; a <= g->ultima_aresta; ++a)
        if (g->arestas[a].ativa) return a;
    return 0;
}
int GAprimeiraAresta(Grafo g) { return GAproximaAresta(g, 0); }

/* Proximo em O(1) se a pertence a estrela; caso contrario, busca o primeiro ID > a. */
static int proxima_estrela(Grafo g, int v, int a, int entrada) {
    int id;
    if (!GBexisteIdVertice(g, v)) return 0;
    if (GBexisteIdAresta(g, a) &&
        (entrada ? g->arestas[a].omega : g->arestas[a].alfa) == v)
        return entrada ? g->arestas[a].proxima_entrada : g->arestas[a].proxima_saida;
    id = entrada ? g->vertices[v].primeira_entrada : g->vertices[v].primeira_saida;
    while (id && id <= a)
        id = entrada ? g->arestas[id].proxima_entrada : g->arestas[id].proxima_saida;
    return id;
}
int GAproxEntrada(Grafo g, int v, int a) { return proxima_estrela(g, v, a, 1); }
int GAprimaEntrada(Grafo g, int v) { return GAproxEntrada(g, v, 0); }
int GAproxSaida(Grafo g, int v, int a) { return proxima_estrela(g, v, a, 0); }
int GAprimaSaida(Grafo g, int v) { return GAproxSaida(g, v, 0); }
int GAproxAresta(Grafo g, int v, int a) {
    int id;
    if (!GBexisteIdVertice(g, v)) return 0;
    if (GBexisteIdAresta(g, a) && (GValfa(g, a) == v || GVomega(g, a) == v))
        return *proxima_incidente(g, a, v);
    id = g->vertices[v].primeira_incidente;
    while (id && id <= a) id = *proxima_incidente(g, id, v);
    return id;
}
int GAprimaAresta(Grafo g, int v) { return GAproxAresta(g, v, 0); }
int GValfa(Grafo g, int a) { return GBexisteIdAresta(g, a) ? g->arestas[a].alfa : 0; }
int GVomega(Grafo g, int a) { return GBexisteIdAresta(g, a) ? g->arestas[a].omega : 0; }
int GBarestaLaco(Grafo g, int a) {
    return GBexisteIdAresta(g, a) && g->arestas[a].alfa == g->arestas[a].omega;
}
int GVvizinho(Grafo g, int a, int v) {
    if (!GBexisteIdAresta(g, a)) return 0;
    if (g->arestas[a].alfa == v) return g->arestas[a].omega;
    if (g->arestas[a].omega == v) return g->arestas[a].alfa;
    return 0;
}
int GApegaArestaDir(Grafo g, int partida, int chegada) {
    int a;
    if (!GBexisteIdVertice(g, chegada)) return 0;
    for (a = GAprimaSaida(g, partida); a; a = GAproxSaida(g, partida, a))
        if (GVomega(g, a) == chegada) return a;
    return 0;
}
int GApegaAresta(Grafo g, int v1, int v2) {
    int a;
    if (!GBexisteIdVertice(g, v2)) return 0;
    for (a = GAprimaAresta(g, v1); a; a = GAproxAresta(g, v1, a))
        if (GVvizinho(g, a, v1) == v2) return a;
    return 0;
}
int GBexisteArestaDir(Grafo g, int v1, int v2) { return GApegaArestaDir(g, v1, v2) != 0; }
int GBexisteAresta(Grafo g, int v1, int v2) { return GApegaAresta(g, v1, v2) != 0; }
int GInumeroVertices(Grafo g) { return g ? g->numero_vertices : 0; }
int GInumeroVerticesMax(Grafo g) { return g ? g->max_vertices : 0; }
int GInumeroArestas(Grafo g) { return g ? g->numero_arestas : 0; }
int GInumeroArestasMax(Grafo g) { return g ? g->max_arestas : 0; }
int GIpegaGrau(Grafo g, int v) {
    int a, grau = 0;
    for (a = GAprimaEntrada(g, v); a; a = GAproxEntrada(g, v, a)) ++grau;
    for (a = GAprimaSaida(g, v); a; a = GAproxSaida(g, v, a)) ++grau;
    return grau;
}

/* Exatamente dois inteiros por linha; recusa lixo e overflow. */
static int le_par(FILE *arquivo, int *x, int *y) {
    char linha[256], *cursor, *fim;
    long valores[2];
    int i;
    if (!fgets(linha, sizeof(linha), arquivo)) return 0;
    if (!strchr(linha, '\n') && !feof(arquivo)) return 0;
    cursor = linha;
    for (i = 0; i < 2; ++i) {
        errno = 0;
        valores[i] = strtol(cursor, &fim, 10);
        if (fim == cursor || errno || valores[i] < 0 || valores[i] >= INT_MAX) return 0;
        if (i == 0 && !isspace((unsigned char)*fim)) return 0;
        cursor = fim;
    }
    while (isspace((unsigned char)*cursor)) ++cursor;
    if (*cursor) return 0;
    *x = (int)valores[0];
    *y = (int)valores[1];
    return 1;
}
Grafo GGcarregaGrafo(const char *nome_arquivo) {
    FILE *arquivo;
    Grafo g = NULL;
    int nv, na, i, origem, destino, c;
    if (!nome_arquivo || !(arquivo = fopen(nome_arquivo, "r"))) return NULL;
    if (!le_par(arquivo, &nv, &na) || (!nv && na)) goto fim;
    g = GGcriaGrafo(nv ? nv : 1, na ? na : 1);
    if (!g) goto fim;
    for (i = 0; i < nv; ++i) GVcriaVertice(g);
    for (i = 0; i < na; ++i) {
        if (!le_par(arquivo, &origem, &destino) || !GAcriaAresta(g, origem, destino)) {
            g = GGdestroiGrafo(g);
            goto fim;
        }
    }
    while ((c = fgetc(arquivo)) != EOF) {
        if (!isspace((unsigned char)c)) { g = GGdestroiGrafo(g); break; }
    }
    if (ferror(arquivo)) g = GGdestroiGrafo(g);
fim:
    fclose(arquivo);
    return g;
}
int GBsalvaGrafo(Grafo g, const char *nome_arquivo) {
    FILE *arquivo;
    int *mapa, v, a, novo = 0, ok;
    if (!g || !nome_arquivo) return 0;
    mapa = calloc((size_t)g->max_vertices + 1, sizeof(*mapa));
    if (!mapa) return 0;
    for (v = GVprimeiroVertice(g); v; v = GVproximoVertice(g, v)) mapa[v] = ++novo;
    arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) { free(mapa); return 0; }
    ok = fprintf(arquivo, "%d %d\n", g->numero_vertices, g->numero_arestas) >= 0;
    for (a = GAprimeiraAresta(g); a && ok; a = GAproximaAresta(g, a))
        ok = fprintf(arquivo, "%d %d\n", mapa[GValfa(g, a)], mapa[GVomega(g, a)]) >= 0;
    if (fclose(arquivo)) ok = 0;
    free(mapa);
    return ok;
}
