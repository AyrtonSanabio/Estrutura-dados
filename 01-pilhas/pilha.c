/*
Nome:
Matricula:
Data:
*/

#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

#define COR_RESET    "\033[0m"
#define COR_VERMELHA "\033[31m"
#define COR_VERDE    "\033[32m"
#define COR_AMARELA  "\033[33m"
#define COR_AZUL     "\033[34m"
#define COR_CIANO    "\033[36m"
#define COR_BRANCA   "\033[97m"

typedef struct No {
    int valor;
    struct No *abaixo;
} No;

struct Pilha {
    No *topo;
    int quantidade;
};


Pilha *Pcria(void){
    Pilha *pilha = malloc(sizeof(Pilha));

    if(pilha == NULL){
        return NULL;
    }

    pilha->topo = NULL;
    pilha->quantidade = 0;

    return pilha;

}

int Pvazia(Pilha *pilha) {
    return pilha->topo == NULL;
}

int Pcheia(Pilha *pilha){
    (void) pilha;
    No *novo_no = malloc(sizeof(No));
    if(novo_no == NULL){
        return 1;
    }

    free(novo_no);
    return 0;
}

void Pinsere(Pilha *pilha, int valor){
    No *novo_no = malloc(sizeof(No)); // Aloca memória para criar um novo nó

    if(novo_no == NULL){
        return; // Não foi possível alocar memória
    }

    novo_no->valor = valor; // Guarda o valor recebido dentro do novo_no

    //O novo_no guarda o endereco do topo antigo no campo "abaixo"
    //SE a pilha estava vazia, pilha->topo é NULL
    novo_no->abaixo = pilha->topo;

    // Agora o topo da pilha passa a ser o novo nó.
    pilha->topo = novo_no;
    pilha->quantidade++;

}

int Premove(Pilha *pilha){

    if (Pvazia(pilha)) {
        return 0;
    }

    // Guarda o endereco do nó removido
    // Esse será o nó removido da pilha.
    No *no_removido = pilha->topo;


    int valor_removido = no_removido->valor;

    // O campo "abaixo" do nó removido contém o endereço
    // do nó que estava logo abaixo dele.
    // Esse nó passa a ser o novo topo da pilha.

    pilha->topo = no_removido->abaixo;
    pilha->quantidade--;

    free(no_removido);

    return  valor_removido;
}

int Pexamina(Pilha *pilha){

    if (Pvazia(pilha)) {
        return 0;
    }

    return pilha->topo->valor;
}

int Ptamanho(Pilha *pilha){
    return pilha->quantidade;
    
}

void Pdestroi(Pilha *pilha){

    if (pilha == NULL) {
        return;
    }

    while (!Pvazia(pilha))
    {
        Premove(pilha);
    }

    free(pilha);

}

void imprimir_pilha(Pilha *pilha){
    No *atual = pilha->topo;

    printf(COR_CIANO "\n================================\n");
    printf("           PILHA ATUAL          \n");
    printf("================================\n" COR_RESET);

    if (Pvazia(pilha)) {
        printf(COR_AMARELA "          [ pilha vazia ]\n" COR_RESET);
    }

    while(atual != NULL){
        if (atual == pilha->topo) {
            printf(COR_VERDE " topo -> " COR_BRANCA "| %10d |\n" COR_RESET,
                   atual->valor);
        } else {
            printf(COR_BRANCA "         | %10d |\n" COR_RESET, atual->valor);
        }
        printf(COR_AZUL "         +------------+\n" COR_RESET);
        atual = atual->abaixo;
    }

    printf(COR_CIANO "Quantidade de elementos: " COR_BRANCA "%d\n", Ptamanho(pilha));
    printf(COR_CIANO "================================\n" COR_RESET);
}

int contar_ocorrencias(Pilha *pilha, int valor){

    No *no_atual = pilha->topo;
    int qtd = 0;

    while(no_atual != NULL){
        if(no_atual->valor == valor){
            qtd++;
        }
        no_atual = no_atual->abaixo;
    }
    return qtd;
}

int contem_valor(Pilha *pilha, int valor){
    No *no_atual = pilha->topo;
    while(no_atual != NULL){
        if(no_atual->valor == valor){
            return 1;
        }
        no_atual = no_atual->abaixo;
    }
    return 0;
}

int maior_valor(Pilha *pilha, int *resultado){


    if (Pvazia(pilha)) {
        return 0;
    }

    int maior = pilha->topo->valor;
    No *no_atual = pilha->topo;
    while(no_atual != NULL){
        if(no_atual->valor > maior){
            maior = no_atual->valor;
        }
        no_atual = no_atual->abaixo;
    }

    *resultado = maior;

    return 1;
}

int menor_valor(Pilha *pilha, int *resultado){

    if (Pvazia(pilha)) {
        return 0;
    }

    int menor = pilha->topo->valor;
    No *no_atual = pilha->topo;
    while(no_atual != NULL){
        if(no_atual->valor < menor){
            menor = no_atual->valor;
        }
        no_atual = no_atual->abaixo;
    }

    *resultado = menor;

    return 1;
}
