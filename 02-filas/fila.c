/*
Nome:
Matricula:
Data:
*/

#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

#define COR_RESET "\033[0m"
#define COR_VERDE "\033[32m"
#define COR_AMARELA "\033[33m"
#define COR_AZUL "\033[34m"
#define COR_CIANO "\033[36m"
#define COR_BRANCA "\033[97m"

// Cada no armazena um elemento e o endereco do proximo no.
typedef struct No {
    int valor;
    struct No *proximo;
} No;

// A fila guarda suas duas extremidades e a quantidade de elementos.
struct Fila {
    No *inicio; // Primeiro no: sera consultado e removido primeiro.
    No *fim;    // Ultimo no: local onde um elemento sera inserido.
    int quantidade;
};

Fila *Fcria(void) {
    // Reserva memoria para a estrutura que controla a fila.
    Fila *fila = malloc(sizeof(Fila));

    if (fila == NULL) {
        return NULL; // Nao foi possivel criar a fila.
    }

    // Uma fila nova nao possui nenhum no.
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->quantidade = 0;
    return fila;
}

int Fvazia(Fila *fila) {
    // Sem primeiro no, nao existe elemento armazenado.
    return fila->inicio == NULL;
}

int Fcheia(Fila *fila) {
    // A fila dinamica so fica cheia quando nao ha memoria para outro no.
    (void) fila;
    No *novo_no = malloc(sizeof(No));

    if (novo_no == NULL) {
        return 1;
    }

    // O no foi criado apenas para o teste e deve ser liberado.
    free(novo_no);
    return 0;
}

void Fdestroi(Fila *fila) {
    // Nao existe estrutura para destruir.
    if (fila == NULL) {
        return;
    }

    // Remove e libera todos os nos, do inicio ate o fim.
    while (!Fvazia(fila)) {
        Fremove(fila);
    }

    // Libera a estrutura que guardava inicio, fim e quantidade.
    free(fila);
}

int Ftamanho(Fila *fila) {
    return fila->quantidade;
}

int Fexamina(Fila *fila) {
    // Nao e possivel consultar uma fila inexistente ou vazia.
    if (fila == NULL || fila->inicio == NULL) {
        return 0;
    }

    // Consulta o primeiro elemento sem remove-lo.
    return fila->inicio->valor;
}

void Finsere(Fila *fila, int elemento) {
    if (fila == NULL) {
        return; // Nao existe estrutura onde inserir.
    }

    // Cria o no que sera colocado no final da fila.
    No *novo_no = malloc(sizeof(No));

    if (novo_no == NULL) {
        return;
    }

    novo_no->valor = elemento;
    novo_no->proximo = NULL;

    if (Fvazia(fila)) {
        // Primeiro elemento: o mesmo no e o inicio e o fim.
        fila->inicio = novo_no;
        fila->fim = novo_no;
    } else {
        // Liga o antigo ultimo no ao novo no.
        fila->fim->proximo = novo_no;

        // Agora o novo no e o ultimo da fila.
        fila->fim = novo_no;
    }

    fila->quantidade++;
}

int Fremove(Fila *fila) {
    // Nao existe elemento para remover.
    if (fila == NULL || Fvazia(fila)) {
        return 0;
    }

    // Guarda o primeiro no e seu valor antes de libera-lo.
    No *no_removido = fila->inicio;
    int valor_removido = no_removido->valor;

    // O segundo no, se existir, passa a ser o primeiro.
    fila->inicio = no_removido->proximo;
    fila->quantidade--;

    // Se o ultimo elemento saiu, o ponteiro fim tambem fica nulo.
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(no_removido);
    return valor_removido;
}

void imprimir_fila(Fila *fila) {
    No *atual = fila->inicio;

    printf(COR_CIANO "\n============================================================\n");
    printf("                        FILA ATUAL                          \n");
    printf("============================================================\n" COR_RESET);

    if (Fvazia(fila)) {
        printf(COR_AMARELA "                      [ fila vazia ]\n" COR_RESET);
    } else {
        printf(COR_VERDE " inicio -> " COR_RESET);

        // Percorre e imprime os elementos sem modificar a fila.
        while (atual != NULL) {
            printf(COR_BRANCA "[ %d ]" COR_RESET, atual->valor);
            if (atual->proximo != NULL) {
                printf(COR_AZUL " -> " COR_RESET);
            }
            atual = atual->proximo;
        }

        printf(COR_VERDE " <- fim\n" COR_RESET);
    }

    printf(COR_CIANO "Quantidade de elementos: " COR_BRANCA "%d\n", Ftamanho(fila));
    printf(COR_CIANO "============================================================\n" COR_RESET);
}
