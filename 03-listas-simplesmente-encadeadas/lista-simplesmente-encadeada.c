#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int valor;
    struct No *proximo;

}No;

typedef struct{
    No *inicio;
    int quantidade;
}Lista;

void inicializar_lista(Lista *lista){
    lista->inicio = NULL;
    lista->quantidade = 0;
}

int esta_vazia(Lista *lista) {
    return lista->inicio == NULL;
}

int inserir_inicio(Lista *lista, int valor){
    // Cria espaço para um novo Nó
    No *novo_no = malloc(sizeof(No));

    // Sem memória: a lista não é alterada
    if(novo_no == NULL){
        return 0;
    }
    
    // Guarda o valor no Nó criado
    novo_no->valor = valor;

    //O ponteiro de novo nó guarda o endereço do primeiro nó da lista
    novo_no->proximo = lista->inicio;

    // Agora o novo nó se torna o primeiro da lista.
    lista->inicio = novo_no;

    // A lista ganhou um elemento.
    lista->quantidade++;

    return 1;
}

int inserir_fim(Lista *lista, int valor){
    // Cria espaço para um novo nó
    No *novo_no = malloc(sizeof(No));
    

    // Sem memória a lista não é alterada
    if (novo_no = NULL)
    {
        return 0;
    }

    novo_no->valor = valor;
    novo_no->proximo = NULL;

    if (lista->inicio == NULL) {

        // Sem nós: o novo nó é também o primeiro.
        lista->inicio = novo_no;
    }else{
        No *atual = lista->inicio;

        // Encontra o último nó
        // Enquanto o nó atual tiver outro nó depois dele, avance.
        while(atual->proximo != NULL){
            atual = atual->proximo;
        }

        atual->proximo = novo_no;
    }

    lista->quantidade ++;

    return 1;
   
}

/*

int inserir_inicio(Lista *lista, int valor);
int inserir_fim(Lista *lista, int valor);
void imprimir_lista(Lista *lista);
void limpar_lista(Lista *lista);
int contem_valor(Lista *lista, int valor);
int buscar_posicao(Lista *lista, int valor);
int contar_ocorrencias(Lista *lista, int valor);
int inserir_posicao(Lista *lista, int posicao, int valor);
int remover_inicio(Lista *lista);
int remover_fim(Lista *lista);
int remover_valor(Lista *lista, int valor);
int remover_posicao(Lista *lista, int posicao);
int consultar_primeiro(Lista *lista, int *resultado);
int consultar_ultimo(Lista *lista, int *resultado);
int maior_valor(Lista *lista, int *resultado);
int menor_valor(Lista *lista, int *resultado);
int somar_elementos(Lista *lista);
void inverter_lista(Lista *lista);
int inserir_ordenado(Lista *lista, int valor);
int lista_esta_ordenada(Lista *lista);
int sao_iguais(Lista *lista1, Lista *lista2);


*/




int main(){
    return 0;
}