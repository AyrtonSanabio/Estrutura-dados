/*
Nome: Ayrton Sanabio
Matricula: 0117925
Data: 19/10/2026
*/
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

typedef struct no {
    int valor;
    struct no *esquerda;
    struct no *direita;
} No;

struct arvore {
    No *raiz;
    int quantidade;
};

/* Quadros substituem chamadas recursivas, inclusive em arvores degeneradas. */
typedef struct {
    No *no;
    int profundidade;
    int etapa;
} Quadro;

static Quadro *cria_pilha(Arvore arvore) {
    size_t tamanho = (size_t)arvore->quantidade;
    if (tamanho > SIZE_MAX / sizeof(Quadro)) return NULL;
    return malloc(tamanho * sizeof(Quadro));
}

Arvore Acria(void) {
    Arvore arvore = malloc(sizeof(*arvore));
    if (arvore == NULL) return NULL;
    arvore->raiz = NULL;
    arvore->quantidade = 0;
    return arvore;
}

int Avazia(Arvore arvore) {
    return arvore == NULL || arvore->raiz == NULL;
}

int Acheia(Arvore arvore) {
    No *teste;
    if (arvore == NULL) return 1;
    teste = malloc(sizeof(*teste));
    if (teste == NULL) return 1;
    free(teste);
    return 0;
}

int Atamanho(Arvore arvore) {
    if (arvore == NULL) return 0;
    return arvore->quantidade;
}

int Ainsere(Arvore arvore, int elemento) {
    No **ligacao, *novo;
    if (arvore == NULL || arvore->quantidade == INT_MAX) return 0;
    /* ligacao aponta para o campo que deve receber o novo no:
     * a raiz da arvore ou um dos ponteiros de filho de um no existente. */
    ligacao = &arvore->raiz;
    while (*ligacao != NULL) {
        if (elemento < (*ligacao)->valor) {
            ligacao = &(*ligacao)->esquerda;
        } else if (elemento > (*ligacao)->valor) {
            ligacao = &(*ligacao)->direita;
        } else {
            return 0; /* Nao armazena repetidos. */
        }
    }
    novo = malloc(sizeof(*novo));
    if (novo == NULL) return 0;
    novo->valor = elemento;
    novo->esquerda = NULL;
    novo->direita = NULL;
    *ligacao = novo;
    ++arvore->quantidade;
    return 1;
}

int Abusca(Arvore arvore, int elemento, int *valor) {
    No *atual;
    if (arvore == NULL || valor == NULL) return 0;
    atual = arvore->raiz;
    while (atual != NULL) {
        if (elemento < atual->valor) {
            atual = atual->esquerda;
        } else if (elemento > atual->valor) {
            atual = atual->direita;
        } else {
            *valor = atual->valor;
            return 1;
        }
    }
    return 0;
}

int Aremove(Arvore arvore, int elemento) {
    No **ligacao, *removido;
    if (arvore == NULL) return 0;
    ligacao = &arvore->raiz;
    while (*ligacao != NULL && (*ligacao)->valor != elemento) {
        if (elemento < (*ligacao)->valor) {
            ligacao = &(*ligacao)->esquerda;
        } else {
            ligacao = &(*ligacao)->direita;
        }
    }
    if (*ligacao == NULL) return 0;
    removido = *ligacao;
    if (removido->esquerda != NULL && removido->direita != NULL) {
        /* Dois filhos: copia o sucessor (menor da subarvore direita).
         * O sucessor nao tem filho esquerdo, entao sua retirada e simples. */
        No **sucessor = &removido->direita;
        while ((*sucessor)->esquerda != NULL) sucessor = &(*sucessor)->esquerda;
        removido->valor = (*sucessor)->valor;
        ligacao = sucessor;
        removido = *sucessor;
    }
    /* Serve para folha, um filho e sucessor; tambem atualiza a raiz. */
    if (removido->esquerda != NULL) {
        *ligacao = removido->esquerda;
    } else {
        *ligacao = removido->direita;
    }
    free(removido);
    --arvore->quantidade;
    return 1;
}

int Amenor(Arvore arvore, int *valor) {
    No *atual;
    if (Avazia(arvore) || valor == NULL) return 0;
    atual = arvore->raiz;
    while (atual->esquerda != NULL) atual = atual->esquerda;
    *valor = atual->valor;
    return 1;
}

int Amaior(Arvore arvore, int *valor) {
    No *atual;
    if (Avazia(arvore) || valor == NULL) return 0;
    atual = arvore->raiz;
    while (atual->direita != NULL) atual = atual->direita;
    *valor = atual->valor;
    return 1;
}

int Aaltura(Arvore arvore) {
    Quadro *pilha;
    int topo = 0, altura = 0;
    if (Avazia(arvore)) return -1;
    pilha = cria_pilha(arvore);
    if (pilha == NULL) return -2;
    pilha[0] = (Quadro){arvore->raiz, 0, 0};
    while (topo >= 0) {
        Quadro atual = pilha[topo--];
        if (atual.profundidade > altura) altura = atual.profundidade;
        if (atual.no->direita != NULL)
            pilha[++topo] = (Quadro){atual.no->direita, atual.profundidade + 1, 0};
        if (atual.no->esquerda != NULL)
            pilha[++topo] = (Quadro){atual.no->esquerda, atual.profundidade + 1, 0};
    }
    free(pilha);
    return altura;
}

static void imprime_valor(int valor, int *primeiro) {
    if (!*primeiro) putchar(' ');
    printf("%d", valor);
    *primeiro = 0;
}

/* etapa 0: entrada; etapa 1: apos esquerda; etapa 2: apos direita. */
static void percorre(Arvore arvore, int ordem) {
    Quadro *pilha;
    int topo = 0, primeiro = 1;
    if (Avazia(arvore)) { putchar('\n'); return; }
    pilha = cria_pilha(arvore);
    if (pilha == NULL) { fputs("Memoria insuficiente para o percurso.\n", stderr); return; }
    pilha[0] = (Quadro){arvore->raiz, 0, 0};
    while (topo >= 0) {
        Quadro *atual = &pilha[topo];
        No *no = atual->no;
        if (atual->etapa == 0) {
            if (ordem == 0) imprime_valor(no->valor, &primeiro);
            atual->etapa = 1;
            if (no->esquerda != NULL) pilha[++topo] = (Quadro){no->esquerda, 0, 0};
        } else if (atual->etapa == 1) {
            if (ordem == 1) imprime_valor(no->valor, &primeiro);
            atual->etapa = 2;
            if (no->direita != NULL) pilha[++topo] = (Quadro){no->direita, 0, 0};
        } else {
            if (ordem == 2) imprime_valor(no->valor, &primeiro);
            --topo;
        }
    }
    free(pilha);
    putchar('\n');
}

void ApreOrdem(Arvore arvore) { percorre(arvore, 0); }
void AemOrdem(Arvore arvore) { percorre(arvore, 1); }
void AposOrdem(Arvore arvore) { percorre(arvore, 2); }

void Adestroi(Arvore arvore) {
    No *atual;
    if (arvore == NULL) return;
    atual = arvore->raiz;
    while (atual != NULL) {
        if (atual->esquerda != NULL) {
            /* Eleva o filho esquerdo e conserva sua subarvore direita.
             * Assim e possivel liberar tudo sem recursao nem nova alocacao. */
            No *esquerda = atual->esquerda;
            atual->esquerda = esquerda->direita;
            esquerda->direita = atual;
            atual = esquerda;
        } else {
            No *proximo = atual->direita;
            free(atual);
            atual = proximo;
        }
    }
    free(arvore);
}
