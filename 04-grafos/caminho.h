/*
Nome:
Matricula:
Data:
*/

#ifndef CAMINHO_H
#define CAMINHO_H

#include "grafo.h"

/*
Encontra e imprime um caminho de menor custo entre partida e chegada.
O vetor pesos usa o identificador da aresta como indice: pesos[0] nao e usado.
*/
void Gcaminho(Grafo grafo, float *pesos, int partida, int chegada);

#endif
