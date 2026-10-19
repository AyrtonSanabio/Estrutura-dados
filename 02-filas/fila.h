/*
Nome:
Matricula:
Data:
*/

#ifndef FILA_H
#define FILA_H

typedef struct Fila Fila;

Fila *Fcria(void);
void Fdestroi(Fila *fila);
int Fvazia(Fila *fila);
int Fcheia(Fila *fila);
int Ftamanho(Fila *fila);
int Fexamina(Fila *fila);
void Finsere(Fila *fila, int elemento);
int Fremove(Fila *fila);
void imprimir_fila(Fila *fila);

#endif
