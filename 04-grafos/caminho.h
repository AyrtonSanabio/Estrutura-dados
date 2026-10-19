/*
Nome: Ayrton Sanabio
Matricula: 0117925
Data: 19/10/2026
*/

#ifndef CAMINHO_H
#define CAMINHO_H

#include "grafo.h"

/*
Encontra e imprime um caminho de menor custo entre partida e chegada.
O vetor pesos usa o identificador da aresta como indice: pesos[0] nao e usado.
*/
void Gcaminho(Grafo grafo, float *pesos, int partida, int chegada);

/* Auxiliar compartilhado com TP 2. Requer saida nova (sem memoria anterior).
 * Retorna 1: encontrou; 0: inalcançavel; -1: erro. Libere com GliberaCaminho.
 * pesos tem capacidade maxima de arestas + 1 posicoes; NULL indica peso 1. */
typedef struct {
    int *vertices;
    int *arestas;
    int quantidade;
    double distancia;
} Caminho;
int GcalculaCaminho(Grafo grafo, const float *pesos, int partida, int chegada,
                   int dirigido, Caminho *resultado);
void GliberaCaminho(Caminho *caminho);

#endif
