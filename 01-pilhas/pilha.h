/*
Nome:
Matricula:
Data:
*/

#ifndef PILHA_H
#define PILHA_H

typedef struct Pilha Pilha;

Pilha *Pcria(void);
void Pdestroi(Pilha *pilha);
int Pvazia(Pilha *pilha);
int Pcheia(Pilha *pilha);
int Ptamanho(Pilha *pilha);
int Pexamina(Pilha *pilha);
void Pinsere(Pilha *pilha, int valor);
int Premove(Pilha *pilha);
void imprimir_pilha(Pilha *pilha);

#endif
