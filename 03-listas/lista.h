/*
Nome:
Matricula:
Data:
*/

#ifndef LISTA_H
#define LISTA_H

typedef struct Lista Lista;

Lista *Lcria(void);
void Ldestroi(Lista *lista);
int Lvazia(Lista *lista);
int Lcheia(Lista *lista);
int Ltamanho(Lista *lista);
int Lexamina(Lista *lista, int posicao);
int Llocaliza(Lista *lista, int elemento);
void Linsere(Lista *lista, int elemento, int posicao);
int Lremove(Lista *lista, int posicao);
void imprimir_lista(Lista *lista);

#endif
