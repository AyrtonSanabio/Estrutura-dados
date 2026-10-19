/*
Nome: Ayrton Sanabio
Matricula: 0117925
Data: 19/10/2026
*/
#ifndef ARVORE_H
#define ARVORE_H

/* ABB de inteiros distintos, sem balanceamento automatico. */
typedef struct arvore *Arvore;

Arvore Acria(void);
void Adestroi(Arvore arvore);
/* NULL e tratado como vazio nas consultas de tamanho/vazio. */
int Avazia(Arvore arvore);
/* Sonda a alocacao de um no; nao garante alocacoes futuras. */
int Acheia(Arvore arvore);
/* Retorna 1 se inseriu; 0 para repetido, arvore nula ou falta de memoria. */
int Ainsere(Arvore arvore, int elemento);
/* Consultas retornam 1 em sucesso e preenchem valor; 0 preserva a saida. */
int Abusca(Arvore arvore, int elemento, int *valor);
int Aremove(Arvore arvore, int elemento);
int Atamanho(Arvore arvore);
/* Altura em arestas: vazia = -1, folha = 0; -2 indica falta de memoria. */
int Aaltura(Arvore arvore);
int Amenor(Arvore arvore, int *valor);
int Amaior(Arvore arvore, int *valor);
/* Percursos imprimem inteiros separados por espaco e uma quebra de linha. */
void ApreOrdem(Arvore arvore);
void AemOrdem(Arvore arvore);
void AposOrdem(Arvore arvore);
#endif
