/*
Nome:
Matricula:
Data:
*/

#include <stdio.h>
#include "fila.h"

#define COR_RESET "\033[0m"
#define COR_VERMELHA "\033[31m"
#define COR_VERDE "\033[32m"
#define COR_AMARELA "\033[33m"
#define COR_AZUL "\033[34m"
#define COR_CIANO "\033[36m"

int main(void) {
    // Cria a fila que sera utilizada durante todo o menu.
    Fila *fila = Fcria();
    int opcao = -1;
    int valor;

    if (fila == NULL) {
        printf(COR_VERMELHA "Nao foi possivel criar a fila.\n" COR_RESET);
        return 1;
    }

    do {
        // Mostra o estado da fila antes de cada nova escolha.
        imprimir_fila(fila);
        printf(COR_AZUL "\n1" COR_RESET " - Inserir elemento\n");
        printf(COR_AZUL "2" COR_RESET " - Remover elemento\n");
        printf(COR_AZUL "3" COR_RESET " - Examinar o inicio\n");
        printf(COR_AZUL "4" COR_RESET " - Verificar se esta vazia\n");
        printf(COR_AZUL "5" COR_RESET " - Verificar se esta cheia\n");
        printf(COR_AZUL "6" COR_RESET " - Consultar o tamanho\n");
        printf(COR_VERMELHA "0" COR_RESET " - Sair\n");
        printf(COR_AMARELA "Escolha uma opcao: " COR_RESET);

        // scanf retorna 1 quando consegue ler um numero inteiro.
        if (scanf("%d", &opcao) != 1) {
            printf(COR_VERMELHA "\nEntrada invalida. Encerrando o programa.\n" COR_RESET);
            opcao = 0;
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Digite o valor: ");
                if (scanf("%d", &valor) == 1) {
                    Finsere(fila, valor);
                    printf(COR_VERDE "Elemento inserido no fim da fila.\n" COR_RESET);
                } else {
                    printf(COR_VERMELHA "Valor invalido. Encerrando.\n" COR_RESET);
                    opcao = 0;
                }
                break;

            case 2:
                if (Fvazia(fila)) {
                    printf(COR_AMARELA "A fila esta vazia.\n" COR_RESET);
                } else {
                    printf(COR_VERDE "Elemento removido do inicio: %d\n" COR_RESET,
                           Fremove(fila));
                }
                break;

            case 3:
                if (Fvazia(fila)) {
                    printf(COR_AMARELA "A fila esta vazia.\n" COR_RESET);
                } else {
                    printf(COR_VERDE "Proximo elemento a ser removido: %d\n" COR_RESET,
                           Fexamina(fila));
                }
                break;

            case 4:
                printf(Fvazia(fila) ? "A fila esta vazia.\n" : "A fila nao esta vazia.\n");
                break;

            case 5:
                printf(Fcheia(fila) ? "A fila esta cheia.\n" : "A fila nao esta cheia.\n");
                break;

            case 6:
                printf("Tamanho da fila: %d\n", Ftamanho(fila));
                break;

            case 0:
                printf(COR_CIANO "Encerrando o programa.\n" COR_RESET);
                break;

            default:
                printf(COR_VERMELHA "Opcao invalida.\n" COR_RESET);
        }
    } while (opcao != 0);

    // Libera os nos restantes e a estrutura da fila.
    Fdestroi(fila);
    return 0;
}
