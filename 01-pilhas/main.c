/*
Nome:
Matricula:
Data:
*/

#include <stdio.h>
#include "pilha.h"

#define COR_RESET    "\033[0m"
#define COR_VERMELHA "\033[31m"
#define COR_VERDE    "\033[32m"
#define COR_AMARELA  "\033[33m"
#define COR_AZUL     "\033[34m"
#define COR_CIANO    "\033[36m"

int main(void){
    Pilha *pilha = Pcria();
    int opcao = -1;
    int valor;

    if (pilha == NULL) {
        printf(COR_VERMELHA "Nao foi possivel criar a pilha.\n" COR_RESET);
        return 1;
    }

    do {
        imprimir_pilha(pilha);
        printf(COR_AZUL "\n1" COR_RESET " - Inserir elemento\n");
        printf(COR_AZUL "2" COR_RESET " - Remover elemento\n");
        printf(COR_AZUL "3" COR_RESET " - Examinar o topo\n");
        printf(COR_AZUL "4" COR_RESET " - Verificar se esta vazia\n");
        printf(COR_AZUL "5" COR_RESET " - Verificar se esta cheia\n");
        printf(COR_AZUL "6" COR_RESET " - Consultar o tamanho\n");
        printf(COR_VERMELHA "0" COR_RESET " - Sair\n");
        printf(COR_AMARELA "Escolha uma opcao: " COR_RESET);

        if (scanf("%d", &opcao) != 1) {
            printf(COR_VERMELHA "\nEntrada invalida. Encerrando o programa.\n" COR_RESET);
            opcao = 0;
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Digite o valor: ");
                if (scanf("%d", &valor) == 1) {
                    Pinsere(pilha, valor);
                    printf(COR_VERDE "Elemento inserido.\n" COR_RESET);
                } else {
                    printf(COR_VERMELHA "Valor invalido. Encerrando o programa.\n" COR_RESET);
                    opcao = 0;
                }
                break;
            case 2:
                if (Pvazia(pilha)) {
                    printf(COR_AMARELA "A pilha esta vazia.\n" COR_RESET);
                } else {
                    printf(COR_VERDE "Elemento removido: %d\n" COR_RESET, Premove(pilha));
                }
                break;
            case 3:
                if (Pvazia(pilha)) {
                    printf(COR_AMARELA "A pilha esta vazia.\n" COR_RESET);
                } else {
                    printf(COR_VERDE "Elemento no topo: %d\n" COR_RESET, Pexamina(pilha));
                }
                break;
            case 4:
                printf(Pvazia(pilha) ? "A pilha esta vazia.\n" : "A pilha nao esta vazia.\n");
                break;
            case 5:
                printf(Pcheia(pilha) ? "A pilha esta cheia.\n" : "A pilha nao esta cheia.\n");
                break;
            case 6:
                printf("Tamanho da pilha: %d\n", Ptamanho(pilha));
                break;
            case 0:
                printf(COR_CIANO "Encerrando o programa.\n" COR_RESET);
                break;
            default:
                printf(COR_VERMELHA "Opcao invalida.\n" COR_RESET);
        }
    } while (opcao != 0);

    Pdestroi(pilha);
    return 0;
}
