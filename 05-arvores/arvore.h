/*
Nome:
Matricula:
Data:
*/

#ifndef ARVORE_H
#define ARVORE_H

/*
 * TAD de arvore binaria de busca (ABB).
 * A estrutura interna e os nos devem ser definidos em arvore.c.
 */
typedef struct arvore *Arvore;

/* TODO: alocar e retornar uma arvore inicialmente vazia. */
Arvore Acria(void);

/* TODO: liberar todos os nos e, ao final, a estrutura da arvore. */
void Adestroi(Arvore arvore).;

/* TODO: retornar 1 se a arvore nao possuir nos; 0 caso contrario. */
int Avazia(Arvore arvore);

/* TODO: retornar 1 se nao for possivel alocar um novo no; 0 caso contrario. */
int Acheia(Arvore arvore);

/* TODO: inserir elemento respeitando a propriedade de arvore binaria de busca. */
int Ainsere(Arvore arvore, int elemento);

/* TODO: localizar elemento e retorna-lo por meio de valor. Retorna 1 se encontrou. */
int Abusca(Arvore arvore, int elemento, int *valor);

/* TODO: remover elemento da arvore. Retorna 1 se removeu; 0 se nao encontrou. */
int Aremove(Arvore arvore, int elemento);

/* TODO: retornar a quantidade de nos da arvore. */
int Atamanho(Arvore arvore);

/* TODO: retornar a altura da arvore vazia ou nao vazia conforme convencao escolhida. */
int Aaltura(Arvore arvore);

/* TODO: obter o menor elemento; retorna 1 em caso de sucesso. */
int Amenor(Arvore arvore, int *valor);

/* TODO: obter o maior elemento; retorna 1 em caso de sucesso. */
int Amaior(Arvore arvore, int *valor);

/* TODO: visitar/imprimir os nos na ordem raiz, esquerda, direita. */
void ApreOrdem(Arvore arvore);

/* TODO: visitar/imprimir os nos na ordem esquerda, raiz, direita. */
void AemOrdem(Arvore arvore);

/* TODO: visitar/imprimir os nos na ordem esquerda, direita, raiz. */
void AposOrdem(Arvore arvore);

#endif
