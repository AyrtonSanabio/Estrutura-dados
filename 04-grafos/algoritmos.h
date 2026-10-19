/*
Nome: Ayrton Sanabio
Matricula: 0117925
Data: 19/10/2026
*/

#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "grafo.h"

/* Interfaces originais: pesos unitarios, percursos dirigidos.
 * AGM e componentes sempre consultam o grafo como nao dirigido. */
/* Imprime as arestas da Arvore Geradora Minima (Kruskal). */
void GArvoreGeradoraMinima(Grafo grafo);

/* Imprime o historico de visitas da Busca em Largura. */
void GBuscaLargura(Grafo grafo, int partida);

/* Imprime o historico de visitas da Busca em Profundidade. */
void GBuscaProfundidade(Grafo grafo, int partida);

/* Imprime o caminho mais curto e a distancia percorrida. */
void GCaminhoMaisCurto(Grafo grafo, int partida, int chegada);

/* Imprime os conjuntos de componentes conexos, separados por chaves. */
void GComponentesConexos(Grafo grafo);

/* Variantes para pesos externos e percursos nao dirigidos (dirigido = 0). */
void GArvoreGeradoraMinimaComPesos(Grafo grafo, const float *pesos);
void GBuscaLarguraModo(Grafo grafo, int partida, int dirigido);
void GBuscaProfundidadeModo(Grafo grafo, int partida, int dirigido);
void GCaminhoMaisCurtoComPesos(Grafo grafo, const float *pesos, int partida,
                             int chegada, int dirigido);

#endif
