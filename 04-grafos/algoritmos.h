/*
Nome: Ayrton Sanabio
Matricula: 0117925
*/

#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "grafo.h"

/* Imprime as arestas da Arvore Geradora Minima. */
void GArvoreGeradoraMinima(Grafo grafo);

/* Imprime o historico de visitas da Busca em Largura. */
void GBuscaLargura(Grafo grafo, int partida);

/* Imprime o historico de visitas da Busca em Profundidade. */
void GBuscaProfundidade(Grafo grafo, int partida);

/* Imprime o caminho mais curto e a distancia percorrida. */
void GCaminhoMaisCurto(Grafo grafo, int partida, int chegada);

/* Imprime os conjuntos de componentes conexos, separados por chaves. */
void GComponentesConexos(Grafo grafo);

#endif
