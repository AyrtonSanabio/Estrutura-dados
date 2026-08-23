#include <stdio.h>
#include <stdlib.h>


#define tamanho 10

typedef struct{
    int elementos[tamanho];
    int quantidade;
}Lista;

void inicializar(Lista *lista){
    lista->quantidade = 0; //Inicialmente não existe nenhum elemento no Lista
}

int adicionar_elemento(Lista *lista, int valor){
    if(lista->quantidade >= tamanho){
        return 0; // Lista está cheia
    }

    lista->elementos[lista->quantidade] = valor;
    lista->quantidade++;

    return 1;
}


int remover_elemento(Lista *lista, int valor){

    int posicao = -1;

    // Procura o valor na lista
    for (int i = 0; i < lista->quantidade; i++)
    {
        if(lista->elementos[i] == valor){
            posicao = i;
        }
    }

    // O valor não foi encontrado 
    if (posicao == -1){
        return 0;
    }

    // Desloca os elementos para a esquerda
    for (int i = posicao; i < lista->quantidade -1; i++){
        lista->elementos[i] = lista->elementos[i+1];
    }
    
    lista->quantidade --;

    return 1;
}

int estaVazia(Lista *lista){
    if (lista->quantidade == 0)
    {
        return 1;
    }
    return 0;
}

int estaCheia(Lista *lista){
    if (lista->quantidade == tamanho)
    {
        return 1;
    }
    return 0;
}

int limparLista(Lista *lista){
    lista->quantidade = 0;
}

void imprime_lista(Lista *lista){
    for (int i = 0; i < lista->quantidade; i++)
    {
        printf("\n%d", lista->elementos[i]);
    }
}

int total_elementos(Lista *lista){
    return lista->quantidade;
}

int posicao_elemento(Lista *lista, int elemento_procurado){
    for (int i = 0; i < lista->quantidade; i++)
    {
        if(lista->elementos[i] == elemento_procurado){
            return i;
        }
    }

    return -1;
}

int contar_ocorrencias(Lista *lista, int valor){
    int total = 0;
    for (int i = 0; i < lista->quantidade; i++){
        if(lista->elementos[i] == valor){
            total ++;
        }
    }
    return total; 
}

int inserir_posicao(Lista *lista, int posicao, int valor) {
    if (lista->quantidade >= tamanho) {
        return -1; // Lista cheia
    }

    if (posicao < 0 || posicao > lista->quantidade) {
        return -2; // Posição inválida
    }

    for (int i = lista->quantidade; i > posicao; i--) {
        lista->elementos[i] = lista->elementos[i - 1];
    }

    lista->elementos[posicao] = valor;
    lista->quantidade++;

    return 1;
}

int main(){
    return 0;
}