/*
Nome: Ayrton Sanabio
Matricula: 0117925
*/

#include <stddef.h>
#include "grafo.h"

/*
TODO: defina aqui a estrutura `struct grafo`.
Escolha como armazenar vertices, arestas, capacidades e identificadores.
*/

Grafo GGcriaGrafo(int max_vertices, int max_arestas) {
    (void) max_vertices;
    (void) max_arestas;
    /* TODO: alocar, inicializar e retornar o grafo vazio. */
    return NULL;
}

void GGdestroiGrafo(Grafo grafo) {
    (void) grafo;
    /* TODO: liberar todos os vetores/estruturas e o proprio grafo. */
}

int GVcriaVertice(Grafo grafo) {
    (void) grafo;
    /* TODO: criar o proximo vertice e retornar seu identificador. */
    return 0;
}

int GAcriaAresta(Grafo grafo, int partida, int chegada) {
    (void) grafo;
    (void) partida;
    (void) chegada;
    /* TODO: criar a aresta e retornar seu identificador. */
    return 0;
}

int GBexisteIdVertice(Grafo grafo, int vertice) {
    (void) grafo;
    (void) vertice;
    /* TODO: retornar 1 se o vertice existir; 0 caso contrario. */
    return 0;
}

int GBexisteIdAresta(Grafo grafo, int aresta) {
    (void) grafo;
    (void) aresta;
    /* TODO: retornar 1 se a aresta existir; 0 caso contrario. */
    return 0;
}

int GBexisteArestaDir(Grafo grafo, int partida, int chegada) {
    (void) grafo;
    (void) partida;
    (void) chegada;
    /* TODO: verificar a aresta dirigida (partida, chegada). */
    return 0;
}

int GBexisteAresta(Grafo grafo, int vertice1, int vertice2) {
    (void) grafo;
    (void) vertice1;
    (void) vertice2;
    /* TODO: verificar a aresta nao dirigida {vertice1, vertice2}. */
    return 0;
}

int GApegaArestaDir(Grafo grafo, int partida, int chegada) {
    (void) grafo;
    (void) partida;
    (void) chegada;
    /* TODO: retornar o id da aresta dirigida; 0 se nao existir. */
    return 0;
}

int GApegaAresta(Grafo grafo, int vertice1, int vertice2) {
    (void) grafo;
    (void) vertice1;
    (void) vertice2;
    /* TODO: retornar o id da aresta nao dirigida; 0 se nao existir. */
    return 0;
}

int GVprimeiroVertice(Grafo grafo) {
    (void) grafo;
    /* TODO: retornar o menor id de vertice existente. */
    return 0;
}

int GVproximoVertice(Grafo grafo, int vertice) {
    (void) grafo;
    (void) vertice;
    /* TODO: retornar o menor id existente maior que vertice. */
    return 0;
}

int GAprimeiraAresta(Grafo grafo) {
    (void) grafo;
    /* TODO: retornar o menor id de aresta existente. */
    return 0;
}

int GAproximaAresta(Grafo grafo, int aresta) {
    (void) grafo;
    (void) aresta;
    /* TODO: retornar o menor id existente maior que aresta. */
    return 0;
}

int GInumeroVertices(Grafo grafo) {
    (void) grafo;
    /* TODO: retornar a quantidade atual de vertices. */
    return 0;
}

int GInumeroVerticesMax(Grafo grafo) {
    (void) grafo;
    /* TODO: retornar a capacidade maxima de vertices. */
    return 0;
}

int GInumeroArestas(Grafo grafo) {
    (void) grafo;
    /* TODO: retornar a quantidade atual de arestas. */
    return 0;
}

int GInumeroArestasMax(Grafo grafo) {
    (void) grafo;
    /* TODO: retornar a capacidade maxima de arestas. */
    return 0;
}

Grafo GGcarregaGrafo(const char *nome_arquivo) {
    (void) nome_arquivo;
    /* TODO: abrir, validar e carregar o arquivo de texto. */
    return NULL;
}

int GBsalvaGrafo(Grafo grafo, const char *nome_arquivo) {
    (void) grafo;
    (void) nome_arquivo;
    /* TODO: salvar vertices e arestas no arquivo de texto. */
    return 0;
}

int GIpegaGrau(Grafo grafo, int vertice) {
    (void) grafo;
    (void) vertice;
    /* TODO: retornar o grau total do vertice. */
    return 0;
}

int GAprimaAresta(Grafo grafo, int vertice) {
    (void) grafo;
    (void) vertice;
    /* TODO: retornar a primeira aresta da estrela nao dirigida. */
    return 0;
}

int GAproxAresta(Grafo grafo, int vertice, int aresta) {
    (void) grafo;
    (void) vertice;
    (void) aresta;
    /* TODO: retornar a proxima aresta da estrela nao dirigida. */
    return 0;
}

int GAprimaEntrada(Grafo grafo, int vertice) {
    (void) grafo;
    (void) vertice;
    /* TODO: retornar a primeira aresta que chega ao vertice. */
    return 0;
}

int GAproxEntrada(Grafo grafo, int vertice, int aresta) {
    (void) grafo;
    (void) vertice;
    (void) aresta;
    /* TODO: retornar a proxima aresta que chega ao vertice. */
    return 0;
}

int GAprimaSaida(Grafo grafo, int vertice) {
    (void) grafo;
    (void) vertice;
    /* TODO: retornar a primeira aresta que sai do vertice. */
    return 0;
}

int GAproxSaida(Grafo grafo, int vertice, int aresta) {
    (void) grafo;
    (void) vertice;
    (void) aresta;
    /* TODO: retornar a proxima aresta que sai do vertice. */
    return 0;
}

int GBarestaLaco(Grafo grafo, int aresta) {
    (void) grafo;
    (void) aresta;
    /* TODO: retornar 1 se alfa(aresta) for igual a omega(aresta). */
    return 0;
}

int GValfa(Grafo grafo, int aresta) {
    (void) grafo;
    (void) aresta;
    /* TODO: retornar o vertice de partida da aresta. */
    return 0;
}

int GVomega(Grafo grafo, int aresta) {
    (void) grafo;
    (void) aresta;
    /* TODO: retornar o vertice de chegada da aresta. */
    return 0;
}

int GVvizinho(Grafo grafo, int aresta, int vertice) {
    (void) grafo;
    (void) aresta;
    (void) vertice;
    /* TODO: retornar o outro extremo da aresta; 0 se for invalido. */
    return 0;
}
