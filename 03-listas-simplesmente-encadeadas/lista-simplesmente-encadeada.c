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

int inserir_posicao(Lista  *lista, int posicao, int valor){

    // Não aceita posição negativa ou depois do fim
    if(posicao > lista->quantidade || posicao < 0){
        return 0;
    }

    // Posição 0 é um caso especial: não existe nó anterior.
    if (posicao == 0) {
        return inserir_inicio(lista, valor);
    }


    No *novo_no = malloc(sizeof(No));

    if(novo_no == NULL){
        return 0;
    }

    novo_no->valor = valor;

    // Encontra o nó imediatamente anterior à posição desejada.
    No *anterior = lista->inicio;

    for(int i=0; i<posicao; i++){
        anterior = anterior->proximo;
    }

    // Primeiro o novo nó aponta para o nó que vinha depois
    novo_no->proximo = anterior->proximo;

    // Depois, o nó anterior passa a apontar para o novo nó
    anterior->proximo = novo_no;

    lista->quantidade++;

    return 1;
}

void imprimir_lista(Lista *lista){
    No *no_aux = malloc(sizeof(No));

    while(no_aux->proximo != NULL){
        printf("\n%d", no_aux->valor);
    }
}

void limpar_lista(Lista *lista){

    No *no_removido = lista->inicio;

    while(lista->inicio != NULL){

        no_removido->proximo = lista->inicio;
        lista->inicio = no_removido->proximo;

        free(no_removido);

    }
    
}

int contem_valor(Lista *lista, int valor){
    No *no_aux = malloc(sizeof(No));

    while(no_aux != NULL){
        if(no_aux->valor == valor){
            return 1;
        }
    }

    return 0;
}

int contar_ocorrencias(Lista *lista, int valor){
    No *no_aux = malloc(sizeof(No));
    int cont = 0;
    while(no_aux->proximo != NULL){
        if(no_aux->valor == valor){
            cont ++;
        }
    }
    return cont;
}

int remover_inicio(Lista *lista){

    // Não há nó para remover
    if (lista->quantidade == 0){
        return 0;
    }

    // Guarda o endereço do primeiro nó.
    No *no_removido = lista->inicio;

    // O segundo nó passa a ser o primeiro
    lista->inicio = no_removido->proximo;

    // Libera o nó que deixou a lista
    free(no_removido);

    lista->quantidade --;

    return 1;
}

int buscar_posicao(Lista *lista, int posicao, int *resultado){
    
    // A posição precisa existir na lista.
    if (posicao < 0 || posicao >= lista->quantidade) {
        return 0;
    }

    // Começa no primeiro índice
    No *no_atual = lista->inicio;   

    int contador = 0;

    // Avança até chegar à posição pedida.
    for (int i = 0; i < posicao; i++) {
        no_atual = no_atual->proximo;
    }

    // Guarda o valor encontrado na variável enviada pela pessoa.
    *resultado = no_atual->valor;

    return 1;
}



/*



int remover_inicio(Lista *lista);
int remover_fim(Lista *lista);
int remover_valor(Lista *lista, int valor);
int remover_posicao(Lista *lista, int posicao);

int consultar_primeiro(Lista *lista, int *resultado);
int consultar_ultimo(Lista *lista, int *resultado);

int maior_valor(Lista *lista, int *resultado);
int menor_valor(Lista *lista, int *resultado);


void inverter_lista(Lista *lista);
int inserir_ordenado(Lista *lista, int valor);
int lista_esta_ordenada(Lista *lista);
int sao_iguais(Lista *lista1, Lista *lista2);


*/




int main(){
    return 0;
}