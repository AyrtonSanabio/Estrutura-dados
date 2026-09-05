/*
Nome:
Matricula:
Data:
*/

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

typedef struct No {
    int valor; 
    struct No *anterior;
    struct No *proximo;
}No;

struct Lista {
    No *inicio;
    No *fim;
    int quantidade;
};

void inicializar(Lista *lista){
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;
}

int inserir_inicio(Lista *lista, int valor){
    
    No *novo_no = malloc(sizeof(No));

    if (novo_no == NULL){
        return 0;
    }


    novo_no->valor = valor;
    novo_no->anterior = NULL;

    No *antigo_inicio = lista->inicio;

    novo_no->proximo = antigo_inicio;

    if(antigo_inicio == NULL){
        // O novo nó é o único nó: início e fim.
        lista->fim = novo_no;
    }else{
        antigo_inicio->anterior = novo_no;
    }

    lista->inicio = novo_no;
    lista->quantidade ++;

    return 1;
}


int inserir_fim(Lista *lista, int valor){
    No *novo_no = malloc(sizeof(No));

    if(novo_no == NULL){
        return 0;
    }


    novo_no->valor = valor;
    novo_no->proximo = NULL;


    if(lista->inicio == NULL){
        // Esse nó será o início e o fim
        lista->inicio = novo_no;
        lista->fim = novo_no;
        novo_no->anterior = NULL;
    }else{
        // O novo nó aponta para o antigo último.
        novo_no->anterior = lista->fim;

        // O antigo último aponta para o novo.
        lista->fim->proximo = novo_no;

        // Agora o novo nó é o fim.
        lista->fim = novo_no;
    }
    
    lista->quantidade ++;

    return 1;
    
}
/*
Operacoes solicitadas pelo trabalho que ainda precisam ser implementadas:

Lista *Lcria(void);
void Ldestroi(Lista *lista);
int Lvazia(Lista *lista);
int Lcheia(Lista *lista);
int Ltamanho(Lista *lista);
int Lexamina(Lista *lista, int posicao);
int Llocaliza(Lista *lista, int elemento);
void Linsere(Lista *lista, int elemento, int posicao);
int Lremove(Lista *lista, int posicao);

As funcoes inserir_inicio e inserir_fim acima ja podem ser reaproveitadas
durante a implementacao de Linsere.
*/
