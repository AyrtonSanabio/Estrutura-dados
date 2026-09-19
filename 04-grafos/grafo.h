/*
Nome:
Matricula:
Data:
*/

#ifndef GRAFO_H
#define GRAFO_H

/* O grafo e representado por um ponteiro para uma estrutura interna. */
typedef struct grafo *Grafo;

/* Operacoes de criacao e destruicao. */
Grafo GGcriaGrafo(int max_vertices, int max_arestas);
void GGdestroiGrafo(Grafo grafo);
int GVcriaVertice(Grafo grafo);
int GAcriaAresta(Grafo grafo, int partida, int chegada);

/* Consultas de existencia e identificacao de arestas. */
int GBexisteIdVertice(Grafo grafo, int vertice);
int GBexisteIdAresta(Grafo grafo, int aresta);
int GBexisteArestaDir(Grafo grafo, int partida, int chegada);
int GBexisteAresta(Grafo grafo, int vertice1, int vertice2);
int GApegaArestaDir(Grafo grafo, int partida, int chegada);
int GApegaAresta(Grafo grafo, int vertice1, int vertice2);

/* Percurso global de vertices e arestas. */
int GVprimeiroVertice(Grafo grafo);
int GVproximoVertice(Grafo grafo, int vertice);
int GAprimeiraAresta(Grafo grafo);
int GAproximaAresta(Grafo grafo, int aresta);

/* Quantidades, capacidades e arquivos. */
int GInumeroVertices(Grafo grafo);
int GInumeroVerticesMax(Grafo grafo);
int GInumeroArestas(Grafo grafo);
int GInumeroArestasMax(Grafo grafo);
Grafo GGcarregaGrafo(const char *nome_arquivo);
int GBsalvaGrafo(Grafo grafo, const char *nome_arquivo);

/* Grau e estrelas dos vertices. */
int GIpegaGrau(Grafo grafo, int vertice);
int GAprimaAresta(Grafo grafo, int vertice);
int GAproxAresta(Grafo grafo, int vertice, int aresta);
int GAprimaEntrada(Grafo grafo, int vertice);
int GAproxEntrada(Grafo grafo, int vertice, int aresta);
int GAprimaSaida(Grafo grafo, int vertice);
int GAproxSaida(Grafo grafo, int vertice, int aresta);

/* Consultas sobre uma aresta e seus extremos. */
int GBarestaLaco(Grafo grafo, int aresta);
int GValfa(Grafo grafo, int aresta);
int GVomega(Grafo grafo, int aresta);
int GVvizinho(Grafo grafo, int aresta, int vertice);

#endif
