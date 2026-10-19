/*
Nome: Ayrton Sanabio
Matricula: 0117925
Data: 19/10/2026
*/
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

static int inteiro(const char *texto, int *valor) {
    char *fim;
    long numero;
    errno = 0;
    numero = strtol(texto, &fim, 10);
    if (fim == texto || errno || numero < INT_MIN || numero > INT_MAX) return 0;
    while (*fim == ' ' || *fim == '\t' || *fim == '\r' || *fim == '\n') ++fim;
    if (*fim != '\0') return 0;
    *valor = (int)numero;
    return 1;
}

/* 1: inteiro; 0: fim da entrada; -1: entrada invalida. */
static int le_inteiro(int *valor) {
    char linha[128];
    if (fgets(linha, sizeof(linha), stdin) == NULL) return 0;
    if (strchr(linha, '\n') == NULL && !feof(stdin)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        return -1;
    }
    if (!inteiro(linha, valor)) return -1;
    return 1;
}

static void resumo(Arvore arvore) {
    int valor;
    printf("Quantidade: %d\n", Atamanho(arvore));
    printf("Altura: %d\n", Aaltura(arvore));
    if (Amenor(arvore, &valor)) printf("Menor: %d\n", valor);
    if (Amaior(arvore, &valor)) printf("Maior: %d\n", valor);
    fputs("Pre-ordem: ", stdout); ApreOrdem(arvore);
    fputs("Em ordem: ", stdout); AemOrdem(arvore);
    fputs("Pos-ordem: ", stdout); AposOrdem(arvore);
}

int main(int argc, char **argv) {
    Arvore arvore = Acria();
    int opcao, elemento, valor, leitura;
    if (arvore == NULL) { fputs("Nao foi possivel criar a arvore.\n", stderr); return 2; }
    /* Com argumentos, executa uma demonstracao reproduzivel e encerra. */
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (!inteiro(argv[i], &elemento) || !Ainsere(arvore, elemento)) {
                fputs("Inteiro invalido, repetido ou memoria insuficiente.\n", stderr);
                Adestroi(arvore);
                return 2;
            }
        }
        resumo(arvore);
        Adestroi(arvore);
        return 0;
    }
    for (;;) {
        puts("\nARVORE BINARIA DE BUSCA");
        puts("1 - Inserir | 2 - Buscar | 3 - Remover");
        puts("4 - Tamanho | 5 - Altura | 6 - Menor | 7 - Maior");
        puts("8 - Pre-ordem | 9 - Em ordem | 10 - Pos-ordem");
        puts("11 - Vazia | 12 - Cheia | 0 - Sair");
        fputs("Opcao: ", stdout);
        leitura = le_inteiro(&opcao);
        if (leitura == 0) break;
        if (leitura < 0 || opcao < 0 || opcao > 12) { puts("Opcao invalida."); continue; }
        if (opcao == 0) break;
        if (opcao >= 1 && opcao <= 3) {
            fputs("Elemento: ", stdout);
            leitura = le_inteiro(&elemento);
            if (leitura == 0) break;
            if (leitura < 0) { puts("Inteiro invalido."); continue; }
        }
        switch (opcao) {
            case 1:
                if (Ainsere(arvore, elemento)) puts("Inserido.");
                else puts("Valor repetido ou memoria insuficiente.");
                break;
            case 2:
                if (Abusca(arvore, elemento, &valor)) printf("Encontrado: %d\n", valor);
                else puts("Valor nao encontrado.");
                break;
            case 3:
                if (Aremove(arvore, elemento)) puts("Removido.");
                else puts("Valor nao encontrado.");
                break;
            case 4: printf("Quantidade: %d\n", Atamanho(arvore)); break;
            case 5: printf("Altura: %d\n", Aaltura(arvore)); break;
            case 6:
                if (Amenor(arvore, &valor)) printf("Menor: %d\n", valor);
                else puts("Arvore vazia.");
                break;
            case 7:
                if (Amaior(arvore, &valor)) printf("Maior: %d\n", valor);
                else puts("Arvore vazia.");
                break;
            case 8: fputs("Pre-ordem: ", stdout); ApreOrdem(arvore); break;
            case 9: fputs("Em ordem: ", stdout); AemOrdem(arvore); break;
            case 10: fputs("Pos-ordem: ", stdout); AposOrdem(arvore); break;
            case 11: printf("Vazia: %d\n", Avazia(arvore)); break;
            case 12: printf("Cheia: %d\n", Acheia(arvore)); break;
        }
    }
    Adestroi(arvore);
    return 0;
}
