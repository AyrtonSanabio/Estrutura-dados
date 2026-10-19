/* Nome: Ayrton Sanabio | Matricula: 0117925 | Data: 19/10/2026 */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "../01-pilhas/pilha.h"
#include "../02-filas/fila.h"
#include "../03-listas/lista.h"
#include "../04-grafos/grafo.h"

int main(void) {
    Pilha *p = Pcria();
    Fila *f = Fcria();
    Lista *l = Lcria();
    Grafo g, copia;
    FILE *arquivo;
    assert(p && f && l);
    assert(Pvazia(p) && Ptamanho(p) == 0 && Premove(p) == 0 && !Pcheia(p));
    Pinsere(p, 0); Pinsere(p, -3); Pinsere(p, 9);
    assert(Pexamina(p) == 9 && Ptamanho(p) == 3);
    assert(Premove(p) == 9 && Premove(p) == -3 && Premove(p) == 0 && Pvazia(p));
    Pdestroi(p); Pdestroi(NULL);
    assert(Fvazia(f) && Ftamanho(f) == 0 && Fremove(f) == 0 && !Fcheia(f));
    Finsere(f, 0); Finsere(f, -3); Finsere(f, 9);
    assert(Fexamina(f) == 0 && Ftamanho(f) == 3);
    assert(Fremove(f) == 0 && Fremove(f) == -3 && Fremove(f) == 9 && Fvazia(f));
    Finsere(f, 4); assert(Fremove(f) == 4 && Fvazia(f));
    Fdestroi(f); Fdestroi(NULL);
    assert(Lvazia(l) && Ltamanho(l) == 0 && !Lcheia(l));
    Linsere(l, 10, 0); Linsere(l, 30, 1); Linsere(l, 20, 1); Linsere(l, 5, 0);
    assert(Ltamanho(l) == 4 && Lexamina(l, 2) == 20 && Llocaliza(l, 30) == 3);
    Linsere(l, 99, -1); Linsere(l, 99, 5);
    assert(Ltamanho(l) == 4 && Llocaliza(l, 99) == -1);
    assert(Lremove(l, 1) == 10 && Lremove(l, 2) == 30);
    assert(Lremove(l, 0) == 5 && Lremove(l, 0) == 20 && Lvazia(l));
    Linsere(l, 7, 0); assert(Lremove(l, 0) == 7);
    Ldestroi(l); Ldestroi(NULL);
    assert(!GGcriaGrafo(0, 1) && !GGcriaGrafo(1, 0) && !GGcriaGrafo(INT_MAX, 1));
    g = GGcriaGrafo(5, 8); assert(g);
    assert(GInumeroVerticesMax(g) == 5 && GInumeroArestasMax(g) == 8);
    assert(!GVprimeiroVertice(g) && !GAprimeiraAresta(g));
    for (int i = 1; i <= 5; ++i) assert(GVcriaVertice(g) == i);
    assert(!GVcriaVertice(g) && !GAcriaAresta(g, 0, 1) && !GAcriaAresta(g, 1, 6));
    assert(GAcriaAresta(g, 1, 2) == 1);
    assert(GAcriaAresta(g, 2, 1) == 2);
    assert(GAcriaAresta(g, 1, 2) == 3);
    assert(GAcriaAresta(g, 2, 2) == 4);
    assert(GAcriaAresta(g, 3, 4) == 5);
    assert(GInumeroVertices(g) == 5 && GInumeroArestas(g) == 5);
    assert(GBexisteIdVertice(g, 5) && !GBexisteIdVertice(g, -1));
    assert(GBexisteIdAresta(g, 5) && !GBexisteIdAresta(g, 6));
    assert(GBexisteArestaDir(g, 1, 2) && !GBexisteArestaDir(g, 4, 3));
    assert(GBexisteAresta(g, 4, 3) && GApegaAresta(g, 2, 1) == 1);
    assert(GApegaArestaDir(g, 2, 1) == 2 && GApegaArestaDir(g, 1, 2) == 1);
    assert(GVprimeiroVertice(g) == 1 && GVproximoVertice(g, 1) == 2);
    assert(!GVproximoVertice(g, INT_MAX) && !GAproximaAresta(g, INT_MAX));
    assert(GAprimeiraAresta(g) == 1 && GAproximaAresta(g, 3) == 4);
    assert(GIpegaGrau(g, 2) == 5 && GIpegaGrau(g, 5) == 0);
    assert(GAprimaEntrada(g, 2) == 1 && GAproxEntrada(g, 2, 1) == 3);
    assert(GAprimaSaida(g, 2) == 2 && GAproxSaida(g, 2, 2) == 4);
    assert(GAprimaAresta(g, 2) == 1 && GAproxAresta(g, 2, 3) == 4);
    assert(!GAproxAresta(g, 2, 4) && GBarestaLaco(g, 4) && !GBarestaLaco(g, 0));
    assert(GValfa(g, 1) == 1 && GVomega(g, 1) == 2);
    assert(GVvizinho(g, 1, 1) == 2 && !GVvizinho(g, 1, 3));
    assert(removeAresta(g, 1) && !removeAresta(g, 1));
    assert(GAprimaEntrada(g, 2) == 3 && GApegaAresta(g, 1, 2) == 2);
    assert(removeVertice(g, 2) && !removeVertice(g, 2));
    assert(GInumeroVertices(g) == 4 && GInumeroArestas(g) == 1);
    assert(GAprimeiraAresta(g) == 5 && !GIpegaGrau(g, 1));
    assert(GBsalvaGrafo(g, "output/testes/grafo-salvo.txt"));
    copia = GGcarregaGrafo("output/testes/grafo-salvo.txt"); assert(copia);
    assert(GInumeroVertices(copia) == 4 && GInumeroArestas(copia) == 1);
    assert(GValfa(copia, 1) == 2 && GVomega(copia, 1) == 3);
    copia = GGdestroiGrafo(copia); assert(!copia);
    assert(GAcriaAresta(g, 1, 3) == 6 && GAcriaAresta(g, 1, 1) == 7);
    assert(GAcriaAresta(g, 1, 4) == 8 && !GAcriaAresta(g, 1, 5));
    assert(removeVertice(g, 1) && GInumeroArestas(g) == 1);
    g = GGdestroiGrafo(g); assert(!g && !GGdestroiGrafo(NULL));
    arquivo = fopen("output/testes/invalido.txt", "w"); assert(arquivo);
    fputs("2 1\n1 3\n", arquivo); fclose(arquivo);
    assert(!GGcarregaGrafo("output/testes/invalido.txt"));
    assert(!GGcarregaGrafo("output/testes/ausente.txt"));
    puts("TADs basicas e 31 operacoes do grafo: OK");
    return 0;
}
