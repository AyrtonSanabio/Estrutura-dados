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

// Cria uma lista vazia e retorna o endereco da estrutura alocada.
Lista *Lcria(void){
    Lista *lista = malloc(sizeof(Lista));

    // Sem memoria, a lista nao pode ser criada.
    if (lista == NULL) {
        return NULL;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->quantidade = 0;

    return lista;
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

int Lcheia(Lista *lista){
    (void) lista;

    No *novo_no = malloc(sizeof(No));

    if(novo_no == NULL){
        return 1;
    }

    free(novo_no);
    return 0;
}


int Lvazia(Lista *lista){

    return lista->quantidade == 0;
}

int Ltamanho(Lista *lista){

    if(lista ==  NULL){
        return 0;
    }

    return lista->quantidade;
}

void Ldestroi(Lista *lista){
    if(lista == NULL){
        return;
    }

    No *no_auxiliar;
    while(lista->inicio != NULL){

        // Guarda o nó que será liberado.
        no_auxiliar = lista->inicio;

        // O próximo nó passa a ser o início.
        lista->inicio = no_auxiliar->proximo;

        // Libera o antigo início.
        free(no_auxiliar);
    }

    // A lista não possui mais nenhum nó.
    lista->fim = NULL;
    lista->quantidade = 0;

    free(lista);
}


int Lexamina(Lista *lista, int posicao){


    if (lista == NULL || posicao < 0 || posicao >= lista->quantidade) {
        return 0;
    }

    No *no_auxiliar = lista->inicio;
    int contador = 0;

    while(contador != posicao){
        no_auxiliar = no_auxiliar->proximo;
        contador ++;

    }

    return no_auxiliar->valor;
}

int Llocaliza(Lista *lista, int elemento){


    if (lista == NULL) {
        return -1;
    }

    No *no_auxiliar = lista->inicio;
    int contador = 0;

    while (no_auxiliar != NULL) {
        if (no_auxiliar->valor == elemento) {
            return contador;
        }

        no_auxiliar = no_auxiliar->proximo;
        contador++;
    }

    // Percorreu a lista inteira e não encontrou.
    return -1;
}

void Linsere(Lista *lista, int elemento, int posicao){

    if (lista == NULL || posicao < 0 || posicao > lista->quantidade) {
        return;
    }


    // Inserção no começo.
    if (posicao == 0) {
        inserir_inicio(lista, elemento);
        return;
    }

    // Inserção depois do último elemento.
    if (posicao == lista->quantidade) {
        inserir_fim(lista, elemento);
        return;
    }

    No *novo_no = malloc(sizeof(No));

    if(novo_no == NULL){
        return;
    }

    novo_no->valor = elemento;

    No *no_auxiliar = lista->inicio;

    for (int contador = 0; contador < posicao; contador++) {
        no_auxiliar = no_auxiliar->proximo;
    }

    // Guarda o nó que está imediatamente antes da posição.
    No *no_anterior = no_auxiliar->anterior;

    // O novo nó aponta para os dois vizinhos.
    novo_no->anterior = no_anterior;
    novo_no->proximo = no_auxiliar;

    // Os dois vizinhos passam a apontar para o novo nó.
    no_anterior->proximo = novo_no;
    no_auxiliar->anterior = novo_no;

    lista->quantidade++;
}

int Lremove(Lista *lista, int posicao) {
    if (lista == NULL || posicao < 0 || posicao >= lista->quantidade) {
        return 0;
    }

    // Encontra o nó que será removido.
    No *no_removido = lista->inicio;

    for (int contador = 0; contador < posicao; contador++) {
        no_removido = no_removido->proximo;
    }

    // Guarda o valor antes de liberar o nó.
    int elemento = no_removido->valor;

    if (no_removido->anterior != NULL) {
        // O nó anterior passa a apontar para o próximo.
        no_removido->anterior->proximo = no_removido->proximo;
    } else {
        // Não existe anterior: estamos removendo o início.
        lista->inicio = no_removido->proximo;
    }

    if (no_removido->proximo != NULL) {
        // O próximo nó passa a apontar para o anterior.
        no_removido->proximo->anterior = no_removido->anterior;
    } else {
        // Não existe próximo: estamos removendo o fim.
        lista->fim = no_removido->anterior;
    }

    lista->quantidade--;

    free(no_removido);

    return elemento;
}

void imprimir_lista(Lista *lista) {
    No *atual = lista->inicio;
    int posicao = 0;

    printf("\033[36m\n============================================================\n");
    printf("                        LISTA ATUAL                         \n");
    printf("============================================================\n\033[0m");

    if (Lvazia(lista)) {
        printf("\033[33m                     [ lista vazia ]\n\033[0m");
    } else {
        printf("\033[32m inicio -> \033[0m");

        while (atual != NULL) {
            printf("\033[97m[%d: %d]\033[0m", posicao, atual->valor);
            if (atual->proximo != NULL) {
                printf("\033[34m <-> \033[0m");
            }
            atual = atual->proximo;
            posicao++;
        }

        printf("\033[32m <- fim\n\033[0m");
    }

    printf("\033[36mQuantidade de elementos: \033[97m%d\n", Ltamanho(lista));
    printf("\033[36m============================================================\n\033[0m");
}
