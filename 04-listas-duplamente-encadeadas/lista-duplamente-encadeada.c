#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor; 
    struct No *anterior;
    struct No *proximo;
}No;

typedef struct{
    No *inicio;
    No *fim;
    int quantidade;
}Lista;

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

int esta_vazia(Lista *lista);

int inserir_posicao(Lista *lista, int posicao, int valor);

int remover_inicio(Lista *lista);
int remover_fim(Lista *lista);
int remover_posicao(Lista *lista, int posicao);

int obter_valor_posicao(Lista *lista, int posicao, int *resultado);
int buscar_posicao(Lista *lista, int valor);

void imprimir_inicio_fim(Lista *lista);
void imprimir_fim_inicio(Lista *lista);
void limpar_lista(Lista *lista);

*/