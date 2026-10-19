/* Nome: Ayrton Sanabio | Matricula: 0117925 | Data: 19/10/2026 */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "../05-arvores/arvore.h"

static void testa_remocoes(void) {
    int valores[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    int saida = 123;
    Arvore a = Acria();
    assert(a && Avazia(a) && Atamanho(a) == 0 && Aaltura(a) == -1);
    assert(!Acheia(a) && !Amenor(a, &saida) && saida == 123);
    assert(!Amaior(a, &saida) && !Aremove(a, 8) && !Abusca(a, 8, &saida));
    for (int i = 0; i < 9; ++i) assert(Ainsere(a, valores[i]));
    assert(!Ainsere(a, 6) && Atamanho(a) == 9 && Aaltura(a) == 3);
    assert(Abusca(a, 13, &saida) && saida == 13);
    assert(!Abusca(a, 99, &saida) && saida == 13);
    assert(!Abusca(a, 1, NULL) && !Amenor(a, NULL) && !Amaior(a, NULL));
    assert(Amenor(a, &saida) && saida == 1);
    assert(Amaior(a, &saida) && saida == 14);
    assert(Aremove(a, 8));  /* Raiz: sucessor e o filho direito direto. */
    assert(Aremove(a, 3));  /* Dois filhos: sucessor esta mais abaixo. */
    assert(Aremove(a, 14)); /* Um filho. */
    assert(Aremove(a, 1));  /* Folha. */
    assert(!Aremove(a, 1) && Atamanho(a) == 5);
    for (int i = 0; i < 9; ++i) {
        int v = valores[i];
        if (v != 8 && v != 3 && v != 14 && v != 1) assert(Aremove(a, v));
    }
    assert(Avazia(a) && Atamanho(a) == 0 && Aaltura(a) == -1);
    assert(Ainsere(a, 0) && Aaltura(a) == 0);
    Adestroi(a);

    a = Acria(); assert(a);
    assert(Ainsere(a, 10) && Ainsere(a, 5) && Ainsere(a, 20));
    assert(Ainsere(a, 15) && Ainsere(a, 17));
    assert(Aremove(a, 10)); /* Sucessor possui filho direito. */
    assert(Atamanho(a) == 4 && Aaltura(a) == 2 && Abusca(a, 17, &saida));
    Adestroi(a);
}

static void testa_modelo(void) {
    int presentes[257] = {0}, quantidade = 0;
    Arvore a = Acria(); assert(a);
    srand(117925);
    for (int passo = 0; passo < 1500; ++passo) {
        int indice = rand() % 257, valor = indice - 128;
        if (rand() % 2 == 0) {
            assert(Ainsere(a, valor) == !presentes[indice]);
            if (!presentes[indice]) { presentes[indice] = 1; ++quantidade; }
        } else {
            assert(Aremove(a, valor) == presentes[indice]);
            if (presentes[indice]) { presentes[indice] = 0; --quantidade; }
        }
        assert(Atamanho(a) == quantidade && Avazia(a) == (quantidade == 0));
        for (int i = 0; i < 257; ++i) {
            int encontrado = INT_MAX;
            assert(Abusca(a, i - 128, &encontrado) == presentes[i]);
            if (presentes[i]) assert(encontrado == i - 128);
            else assert(encontrado == INT_MAX);
        }
        if (quantidade > 0) {
            int i = 0, encontrado;
            while (!presentes[i]) ++i;
            assert(Amenor(a, &encontrado) && encontrado == i - 128);
            i = 256;
            while (!presentes[i]) --i;
            assert(Amaior(a, &encontrado) && encontrado == i - 128);
        }
    }
    Adestroi(a);
}

int main(void) {
    Arvore a;
    int saida = 7;
    assert(Avazia(NULL) && Atamanho(NULL) == 0 && Aaltura(NULL) == -1);
    assert(Acheia(NULL) && !Ainsere(NULL, 1) && !Aremove(NULL, 1));
    assert(!Abusca(NULL, 1, &saida) && saida == 7);
    assert(!Amenor(NULL, &saida) && !Amaior(NULL, &saida));
    Adestroi(NULL);
    testa_remocoes();
    testa_modelo();
    a = Acria(); assert(a);
    assert(Ainsere(a, INT_MIN) && Ainsere(a, 0) && Ainsere(a, INT_MAX));
    assert(Amenor(a, &saida) && saida == INT_MIN);
    assert(Amaior(a, &saida) && saida == INT_MAX);
    Adestroi(a);
    a = Acria(); assert(a);
    for (int i = 0; i < 3000; ++i) assert(Ainsere(a, i));
    assert(Atamanho(a) == 3000 && Aaltura(a) == 2999);
    Adestroi(a); /* Destruicao tambem nao depende de recursao. */
    puts("ABB: 14 operacoes OK; 1500 estados aleatorios e cadeia de 3000 nos.");
    return 0;
}
