#include <stdio.h>
#include <stdlib.h>


typedef struct No {
    int valor;
    struct No *abaixo;
} No;

typedef struct {
    No *topo;
    int quantidade;
} Pilha;


void inicializar_pilha(Pilha *pilha){
    pilha->topo = NULL;
    pilha->quantidade = 0;

}

int esta_vazia(Pilha *pilha) {
    return pilha->topo == NULL;
}

int empilhar(Pilha *pilha, int valor){
    No *novo_no = malloc(sizeof(No)); // Aloca memória para criar um novo nó

    if(novo_no == NULL){
        return 0; // Não foi possível alocar memória
    }

    novo_no->valor = valor; // Guarda o valor recebido dentro do novo_no

    //O novo_no guarda o endereco do topo antigo no campo "abaixo"
    //SE a pilha estava vazia, pilha->topo é NULL
    novo_no->abaixo = pilha->topo;

    // Agora o topo da pilha passa a ser o novo nó.
    pilha->topo = novo_no;
    pilha->quantidade++;

    return 1;

}

int desempilhar(Pilha *pilha){

    if (esta_vazia(pilha)) {
        return 0;
    }

    // Guarda o endereco do nó removido
    // Esse será o nó removido da pilha.
    No *no_removido = pilha->topo;

    // O campo "abaixo" do nó removido contém o endereço
    // do nó que estava logo abaixo dele.
    // Esse nó passa a ser o novo topo da pilha.
    pilha->topo = no_removido->abaixo;

    free(no_removido);
    pilha->quantidade--;

    return  1;
}

int consultar_topo(Pilha *pilha, int *valor_topo){

    if (esta_vazia(pilha)) {
        return 0;
    }

    *valor_topo = pilha->topo->valor;

    return 1;
}

int quantidade_elementos(Pilha *pilha){
    return pilha->quantidade;
    
}

void limpar_pilha(Pilha *pilha){
    while (pilha != esta_vazia(pilha))
    {
        desempilhar(pilha);
    }

}

void imprimir_pilha(Pilha *pilha){
    No *atual = pilha->topo;

    while(atual != NULL){
        printf("\n%d", atual->valor);
        atual = atual->abaixo;
    }

    printf("\n");
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


    if (esta_vazia(pilha)) {
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

    if (esta_vazia(pilha)) {
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

/*
int somar_elementos(Pilha *pilha);
float calcular_media(Pilha *pilha);
int sao_iguais(Pilha *pilha1, Pilha *pilha2);
*/


int main(){
    return 0;
}