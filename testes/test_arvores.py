"""Saidas e menu da ABB, comparados com exemplos e um conjunto de referencia."""
from pathlib import Path
import random
import re
import subprocess
import unittest

EXE = Path(__file__).resolve().parents[1] / "output" / "arvores.exe"


class TestArvores(unittest.TestCase):
    def executar(self, *valores, entrada=None, codigo=0):
        resultado = subprocess.run([str(EXE), *map(str, valores)], input=entrada,
                                   capture_output=True, text=True, timeout=15)
        self.assertEqual(resultado.returncode, codigo, resultado.stderr)
        if not codigo:
            self.assertEqual(resultado.stderr, "")
        return resultado.stdout

    def test_tres_percursos_e_altura(self):
        esperado = ("Quantidade: 9\nAltura: 3\nMenor: 1\nMaior: 14\n"
                    "Pre-ordem: 8 3 1 6 4 7 10 14 13\n"
                    "Em ordem: 1 3 4 6 7 8 10 13 14\n"
                    "Pos-ordem: 1 4 7 6 3 13 14 10 8\n")
        self.assertEqual(self.executar(8, 3, 10, 1, 6, 14, 4, 7, 13), esperado)

    def test_extremos_inteiros_e_folha(self):
        texto = self.executar(0, -2147483648, 2147483647)
        self.assertIn("Em ordem: -2147483648 0 2147483647\n", texto)
        self.assertIn("Altura: 1\n", texto)
        self.assertIn("Altura: 0\n", self.executar(-7))

    def test_entrada_invalida(self):
        for valores in (("abc",), ("999999999999999999999",), ("2x",), ("",), (1, 1)):
            with self.subTest(valores=valores):
                self.executar(*valores, codigo=2)
        texto = self.executar(entrada="abc\n99\n1\nx\n0\n")
        self.assertEqual(texto.count("Opcao invalida."), 2)
        self.assertIn("Inteiro invalido.", texto)
        self.executar(entrada="")  # EOF encerra sem travar.

    def test_menu_todas_operacoes(self):
        entrada = ("11\n12\n5\n6\n7\n1\n8\n1\n3\n1\n10\n2\n3\n"
                   "8\n9\n10\n4\n5\n6\n7\n3\n8\n2\n8\n4\n0\n")
        texto = self.executar(entrada=entrada)
        for fragmento in ("Vazia: 1", "Cheia: 0", "Altura: -1", "Arvore vazia.",
                          "Encontrado: 3", "Pre-ordem: 8 3 10", "Em ordem: 3 8 10",
                          "Pos-ordem: 3 10 8", "Quantidade: 3", "Altura: 1",
                          "Menor: 3", "Maior: 10", "Removido.",
                          "Valor nao encontrado.", "Quantidade: 2"):
            self.assertIn(fragmento, texto)

    def test_menu_contra_conjunto_independente(self):
        rng = random.Random(117925)
        comandos, esperados, presentes = [], [], set()
        for _ in range(120):
            valor = rng.randint(-30, 30)
            if rng.randrange(2):
                comandos.extend(("1", str(valor)))
                presentes.add(valor)
            else:
                comandos.extend(("3", str(valor)))
                presentes.discard(valor)
            comandos.append("9")
            esperados.append(" ".join(map(str, sorted(presentes))))
        comandos.append("0")
        texto = self.executar(entrada="\n".join(comandos) + "\n")
        encontrados = re.findall(r"Em ordem: ([^\r\n]*)", texto)
        self.assertEqual(encontrados, esperados)

    def test_cadeias_sem_recursao(self):
        n = 1500
        crescente = list(range(n))
        for valores in (crescente, crescente[::-1]):
            texto = self.executar(*valores)
            self.assertIn(f"Altura: {n - 1}\n", texto)
            ordem = re.search(r"Em ordem: ([^\r\n]+)", texto).group(1)
            self.assertEqual(list(map(int, ordem.split())), crescente)


if __name__ == "__main__":
    unittest.main(verbosity=2)
