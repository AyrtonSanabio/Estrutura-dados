"""Regressoes do TP: saidas exatas e comparacao com referencias independentes."""
from itertools import combinations
from pathlib import Path
import math
import random
import subprocess
import tempfile
import unittest

RAIZ = Path(__file__).resolve().parents[1]
EXE = RAIZ / "output" / "grafos.exe"


class TestGrafos(unittest.TestCase):
    def setUp(self):
        self.pasta = tempfile.TemporaryDirectory(dir=RAIZ / "output" / "testes")
        self.addCleanup(self.pasta.cleanup)
        self.grafo = Path(self.pasta.name) / "grafo.txt"
        self.pesos = Path(self.pasta.name) / "pesos.txt"

    def entrada(self, n, arestas):
        self.grafo.write_text(
            f"{n} {len(arestas)}\n" + "".join(f"{u} {v}\n" for u, v, _ in arestas),
            encoding="utf-8",
        )
        self.pesos.write_text("".join(f"{w}\n" for _, _, w in arestas), encoding="utf-8")

    def executar(self, *args, codigo=0, ponderado=True):
        comando = [str(EXE), str(self.grafo), *map(str, args)]
        if ponderado:
            comando += ["--pesos", str(self.pesos)]
        resultado = subprocess.run(comando, capture_output=True, text=True, timeout=10)
        self.assertEqual(resultado.returncode, codigo, resultado.stderr)
        if not codigo:
            self.assertEqual(resultado.stderr, "")
        return resultado.stdout.strip()

    def test_ordens_e_caminho_ponderado(self):
        self.entrada(6, [(1, 2, 4), (1, 3, 1), (2, 4, 1), (3, 4, 2),
                         (4, 5, 3), (2, 5, 10), (3, 5, 9), (5, 6, 2)])
        self.assertEqual(self.executar("bfs", 1), "v1,a1,v2,a2,v3,a3,v4,a6,v5,a8,v6")
        self.assertEqual(self.executar("dfs", 1), "v1,a1,v2,a3,v4,a5,v5,a8,v6,a2,v3")
        self.assertEqual(self.executar("caminho", 1, 6),
                         "v1,a2,v3,a4,v4,a5,v5,a8,v6\nDistancia: 8")
        self.assertEqual(self.executar("agm"), "a2,a3,a4,a8,a5")
        self.assertEqual(self.executar("componentes"), "{1,2,3,4,5,6}")
        self.assertEqual(self.executar("caminho", 1, 6, ponderado=False).splitlines()[-1],
                         "Distancia: 3")

    def test_contrato_tp1(self):
        self.entrada(6, [(6, 1, 1), (6, 3, 1), (3, 6, 1), (1, 3, 1),
                         (1, 2, 1), (2, 3, 1), (2, 2, 1), (5, 4, 1)])
        self.assertEqual(self.executar("tp1", 3, 2), "3 -> 6 -> 1 -> 2")
        self.assertEqual(self.executar("tp1", 1, 4), "1")
        self.assertEqual(self.executar("tp1", 2, 2), "2")
        self.assertEqual(self.executar("caminho", 1, 4), "Sem caminho.")

    def test_sentido_e_componentes(self):
        self.entrada(5, [(2, 1, 2), (2, 3, 3), (4, 4, 0)])
        self.assertEqual(self.executar("bfs", 1), "v1")
        self.assertEqual(self.executar("dfs", 1, "--nao-dirigido"), "v1,a1,v2,a2,v3")
        self.assertEqual(self.executar("componentes"), "{1,2,3} {4} {5}")
        self.assertEqual(self.executar("caminho", 1, 3, "--nao-dirigido"),
                         "v1,a1,v2,a2,v3\nDistancia: 5")
        self.assertEqual(self.executar("agm"), "Sem AGM: grafo desconexo.")

    def test_lacos_paralelas_pesos_zero_e_negativos(self):
        self.entrada(3, [(1, 1, -10), (1, 2, 8), (1, 2, 0), (2, 3, 2), (1, 3, 7)])
        self.assertEqual(self.executar("agm"), "a3,a4")
        self.executar("caminho", 1, 3, codigo=2)
        self.entrada(3, [(1, 1, 0), (1, 2, 8), (1, 2, 0), (2, 3, 2), (1, 3, 7)])
        self.assertEqual(self.executar("caminho", 1, 3), "v1,a3,v2,a4,v3\nDistancia: 2")

    def test_grafo_vazio_e_unitario(self):
        self.entrada(0, [])
        self.assertEqual(self.executar("componentes"), "{}")
        self.assertEqual(self.executar("agm"), "")
        self.executar("bfs", 1, codigo=2)
        self.entrada(1, [])
        self.assertEqual(self.executar("componentes"), "{1}")
        self.assertEqual(self.executar("dfs", 1), "v1")
        self.assertEqual(self.executar("agm"), "")
        self.assertEqual(self.executar("caminho", 1, 1), "v1\nDistancia: 0")

    def test_arquivos_invalidos(self):
        for texto in ("", "2 1\n1 3\n", "2 2\n1 2\n", "2 0\nlixo",
                      "-1 0\n", "99999999999999999999999 0\n",
                      "2 1\n1 2 9\n", "0 1\n1 1\n", "2+1\n1 2\n"):
            with self.subTest(texto=texto):
                self.grafo.write_text(texto, encoding="utf-8")
                self.executar("arestas", codigo=2, ponderado=False)

    def test_pesos_e_argumentos_invalidos(self):
        self.entrada(2, [(1, 2, 1)])
        for texto in ("", "1 2", "nan", "inf", "abc", "1e999"):
            self.pesos.write_text(texto, encoding="utf-8")
            self.executar("agm", codigo=2)
        self.entrada(2, [(1, 2, 1)])
        for args in (("bfs", 0), ("dfs", 3), ("bfs", "1x"), ("bfs", "9999999999999"),
                     ("caminho", 1), ("caminho", 1, 9), ("desconhecido",),
                     ("agm", "extra"), ("bfs", 1, "--xyz"), ("tp1", 1, 2, "--nao-dirigido")):
            self.executar(*args, codigo=2)

    def test_salvar_e_remover(self):
        self.entrada(4, [(1, 2, 1), (2, 2, 1), (2, 3, 1), (3, 4, 1)])
        saida = Path(self.pasta.name) / "salvo.txt"
        self.executar("remover-vertice", 2, saida)
        self.assertEqual(saida.read_text(), "3 1\n2 3\n")
        self.executar("remover-aresta", 2, saida)
        self.assertEqual(saida.read_text(), "4 3\n1 2\n2 3\n3 4\n")
        self.executar("remover-aresta", 99, saida, codigo=2)
        self.executar("salvar", saida)
        self.assertEqual(saida.read_text(), self.grafo.read_text())

    def test_dfs_sem_estourar_pilha_recursiva(self):
        n = 4000
        self.entrada(n, [(i, i + 1, 1) for i in range(1, n)])
        historico = self.executar("dfs", 1).split(",")
        self.assertEqual(len(historico), 2 * n - 1)
        self.assertEqual(historico[-1], f"v{n}")

    def test_referencias_independentes_aleatorias(self):
        rng = random.Random(17925)
        for caso in range(25):
            n = rng.randint(2, 6)
            arestas = [(rng.randint(1, n), rng.randint(1, n), rng.randint(0, 8))
                       for _ in range(rng.randint(0, 9))]
            self.entrada(n, arestas)
            adj = {v: [] for v in range(1, n + 1)}
            for a, (u, v, _) in enumerate(arestas, 1):
                adj[u].append((a, v))
            for inicio in (1, n):
                for tipo in ("bfs", "dfs"):
                    esperado, vistos = [f"v{inicio}"], {inicio}
                    if tipo == "bfs":
                        fila = [inicio]
                        for u in fila:
                            for a, v in adj[u]:
                                if v not in vistos:
                                    vistos.add(v); fila.append(v)
                                    esperado.extend((f"a{a}", f"v{v}"))
                    else:
                        def visita(u):
                            for a, v in adj[u]:
                                if v not in vistos:
                                    vistos.add(v); esperado.extend((f"a{a}", f"v{v}")); visita(v)
                        visita(inicio)
                    with self.subTest(caso=caso, tipo=tipo, inicio=inicio):
                        self.assertEqual(self.executar(tipo, inicio), ",".join(esperado))
                # Bellman-Ford independente do Dijkstra do programa.
                dist = [math.inf] * (n + 1); dist[inicio] = 0
                for _ in range(n - 1):
                    for u, v, w in arestas:
                        dist[v] = min(dist[v], dist[u] + w)
                for fim in (1, n):
                    texto = self.executar("caminho", inicio, fim)
                    if math.isinf(dist[fim]):
                        self.assertEqual(texto, "Sem caminho.")
                    else:
                        self.assertEqual(float(texto.split("Distancia: ")[1]), dist[fim])
                        tokens = texto.splitlines()[0].split(",")
                        self.assertEqual(tokens[0], f"v{inicio}")
                        self.assertEqual(tokens[-1], f"v{fim}")
                        custo = 0
                        for i in range(1, len(tokens), 2):
                            u, v, w = arestas[int(tokens[i][1:]) - 1]
                            self.assertEqual((f"v{u}", f"v{v}"), (tokens[i - 1], tokens[i + 1]))
                            custo += w
                        self.assertEqual(custo, dist[fim])
            # AGM por enumeracao exaustiva, sem usar o Kruskal implementado.
            melhor = math.inf
            for conjunto in combinations(range(len(arestas)), n - 1):
                alcancados = {1}
                for _ in range(n):
                    for i in conjunto:
                        u, v, _ = arestas[i]
                        if u in alcancados or v in alcancados:
                            alcancados.update((u, v))
                if len(alcancados) == n:
                    melhor = min(melhor, sum(arestas[i][2] for i in conjunto))
            saida = self.executar("agm")
            if math.isinf(melhor):
                self.assertEqual(saida, "Sem AGM: grafo desconexo.")
            else:
                ids = [int(a[1:]) - 1 for a in saida.split(",")]
                self.assertEqual(len(ids), n - 1)
                self.assertEqual(sum(arestas[i][2] for i in ids), melhor)


if __name__ == "__main__":
    unittest.main(verbosity=2)
