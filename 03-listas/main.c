/*
Nome:
Matricula:
Data:
*/

#include <stdio.h>
#include "lista.h"

#define COR_RESET "\033[0m"
#define COR_VERMELHA "\033[31m"
#define COR_VERDE "\033[32m"
#define COR_AMARELA "\033[33m"
#define COR_AZUL "\033[34m"
#define COR_CIANO "\033[36m"

int main(void) {
    Lista *lista = Lcria();
    int opcao = -1;
    int elemento;
    int posicao;
    int resultado;

    if (lista == NULL) {
        printf(COR_VERMELHA "Nao foi possivel criar a lista.\n" COR_RESET);
        return 1;
    }

    do {
        imprimir_lista(lista);
        printf(COR_AZUL "\n1" COR_RESET " - Inserir elemento\n");
        printf(COR_AZUL "2" COR_RESET " - Remover elemento\n");
        printf(COR_AZUL "3" COR_RESET " - Examinar uma posicao\n");
        printf(COR_AZUL "4" COR_RESET " - Localizar um elemento\n");
        printf(COR_AZUL "5" COR_RESET " - Verificar se esta vazia\n");
        printf(COR_AZUL "6" COR_RESET " - Verificar se esta cheia\n");
        printf(COR_AZUL "7" COR_RESET " - Consultar o tamanho\n");
        printf(COR_VERMELHA "0" COR_RESET " - Sair\n");
        printf(COR_AMARELA "Escolha uma opcao: " COR_RESET);

        if (scanf("%d", &opcao) != 1) {
            printf(COR_VERMELHA "\nEntrada invalida. Encerrando.\n" COR_RESET);
            opcao = 0;
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Digite o elemento: ");
                if (scanf("%d", &elemento) != 1) {
                    printf(COR_VERMELHA "Elemento invalido. Encerrando.\n" COR_RESET);
                    opcao = 0;
                    break;
                }

                printf("Digite a posicao de 0 ate %d: ", Ltamanho(lista));
                if (scanf("%d", &posicao) != 1 || posicao < 0 || posicao > Ltamanho(lista)) {
                    printf(COR_VERMELHA "Posicao invalida.\n" COR_RESET);
                    break;
                }

                Linsere(lista, elemento, posicao);
                printf(COR_VERDE "Elemento inserido.\n" COR_RESET);
                break;

            case 2:
                if (Lvazia(lista)) {
                    printf(COR_AMARELA "A lista esta vazia.\n" COR_RESET);
                    break;
                }

                printf("Digite a posicao de 0 ate %d: ", Ltamanho(lista) - 1);
                if (scanf("%d", &posicao) != 1 || posicao < 0 || posicao >= Ltamanho(lista)) {
                    printf(COR_VERMELHA "Posicao invalida.\n" COR_RESET);
                    break;
                }

                printf(COR_VERDE "Elemento removido: %d\n" COR_RESET,
                       Lremove(lista, posicao));
                break;

            case 3:
                if (Lvazia(lista)) {
                    printf(COR_AMARELA "A lista esta vazia.\n" COR_RESET);
                    break;
                }

                printf("Digite a posicao de 0 ate %d: ", Ltamanho(lista) - 1);
                if (scanf("%d", &posicao) != 1 || posicao < 0 || posicao >= Ltamanho(lista)) {
                    printf(COR_VERMELHA "Posicao invalida.\n" COR_RESET);
                    break;
                }

                printf(COR_VERDE "Elemento na posicao %d: %d\n" COR_RESET,
                       posicao, Lexamina(lista, posicao));
                break;

            case 4:
                printf("Digite o elemento procurado: ");
                if (scanf("%d", &elemento) != 1) {
                    printf(COR_VERMELHA "Elemento invalido. Encerrando.\n" COR_RESET);
                    opcao = 0;
                    break;
                }

                resultado = Llocaliza(lista, elemento);
                if (resultado == -1) {
                    printf(COR_AMARELA "Elemento nao encontrado.\n" COR_RESET);
                } else {
                    printf(COR_VERDE "Elemento encontrado na posicao %d.\n" COR_RESET,
                           resultado);
                }
                break;

            case 5:
                printf(Lvazia(lista) ? "A lista esta vazia.\n" : "A lista nao esta vazia.\n");
                break;

            case 6:
                printf(Lcheia(lista) ? "A lista esta cheia.\n" : "A lista nao esta cheia.\n");
                break;

            case 7:
                printf("Tamanho da lista: %d\n", Ltamanho(lista));
                break;

            case 0:
                printf(COR_CIANO "Encerrando o programa.\n" COR_RESET);
                break;

            default:
                printf(COR_VERMELHA "Opcao invalida.\n" COR_RESET);
        }
    } while (opcao != 0);

    Ldestroi(lista);
    return 0;
}
