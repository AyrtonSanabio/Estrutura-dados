"""Regenera RELATORIO.pdf. Requer ReportLab apenas para esta etapa documental."""
from pathlib import Path
from xml.sax.saxutils import escape

from reportlab.lib import colors
from reportlab.lib.enums import TA_LEFT
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import mm
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, PageBreak, Table, TableStyle, Preformatted,
)

PASTA = Path(__file__).resolve().parent
DESTINO = PASTA / "RELATORIO.pdf"
AZUL = colors.HexColor("#17334c")
VERDE = colors.HexColor("#157b79")
styles = getSampleStyleSheet()
styles.add(ParagraphStyle("TituloTP", fontName="Helvetica-Bold", fontSize=21,
                          leading=25, textColor=AZUL, spaceAfter=12))
styles.add(ParagraphStyle("SecaoTP", fontName="Helvetica-Bold", fontSize=12,
                          leading=16, textColor=VERDE, spaceBefore=10, spaceAfter=7))
styles.add(ParagraphStyle("TextoTP", fontName="Helvetica", fontSize=10.2,
                          leading=14.7, spaceAfter=8, alignment=TA_LEFT))
styles.add(ParagraphStyle("PequenoTP", fontName="Helvetica", fontSize=8.5,
                          leading=12, spaceAfter=5))
styles.add(ParagraphStyle("CodigoTP", fontName="Courier", fontSize=8.3,
                          leading=12, spaceBefore=5, spaceAfter=10,
                          backColor=colors.HexColor("#eff4f6"), borderPadding=8))
story = []


def p(text):
    story.append(Paragraph(text, styles["TextoTP"]))


def h(text):
    story.append(Paragraph(text, styles["SecaoTP"]))


def tabela(linhas, larguras):
    dados = [[Paragraph(escape(str(c)), styles["PequenoTP"]) for c in row] for row in linhas]
    tab = Table(dados, colWidths=larguras, repeatRows=1, hAlign="LEFT")
    tab.setStyle(TableStyle([
        ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#e1eef0")),
        ("ROWBACKGROUNDS", (0, 1), (-1, -1), [colors.white, colors.HexColor("#f5f7f9")]),
        ("VALIGN", (0, 0), (-1, -1), "TOP"),
        ("LEFTPADDING", (0, 0), (-1, -1), 8),
        ("RIGHTPADDING", (0, 0), (-1, -1), 8),
        ("TOPPADDING", (0, 0), (-1, -1), 6),
        ("BOTTOMPADDING", (0, 0), (-1, -1), 5),
        ("LINEBELOW", (0, 0), (-1, 0), 0.7, VERDE),
    ]))
    story.append(tab)
    story.append(Spacer(1, 6))


def codigo(text):
    story.append(Preformatted(text, styles["CodigoTP"]))


def rodape(canvas, doc):
    canvas.saveState()
    canvas.setStrokeColor(VERDE)
    canvas.line(20 * mm, 18 * mm, 190 * mm, 18 * mm)
    canvas.setFont("Helvetica", 8)
    canvas.setFillColor(AZUL)
    canvas.drawString(20 * mm, 13 * mm, "Estruturas de Dados 2 | Ayrton Sanabio | 0117925")
    canvas.drawRightString(190 * mm, 13 * mm, f"{doc.page}")
    canvas.restoreState()


story.append(Paragraph("TADs e algoritmos<br/>em grafos", styles["TituloTP"]))
p("<b>Relatório técnico dos trabalhos práticos</b><br/>"
  "Ayrton Sanabio - Matrícula 0117925<br/>"
  "Estruturas de Dados 2 - Professor Walace de Almeida Rodrigues<br/>"
  "Versão de 19 de outubro de 2026")
h("1. Introdução")
p("O trabalho implementa bibliotecas em C para pilha, fila, lista duplamente "
  "encadeada e grafo estático. Sobre a TAD de grafo são executadas busca em largura, "
  "busca em profundidade, caminho mínimo, componentes conexos e árvore geradora mínima. "
  "A entrada e a saída são textuais, com carregamento de grafos por arquivo.")
p("A implementação é dividida em interfaces e módulos independentes. O programa "
  "principal valida as entradas e seleciona a operação; os algoritmos utilizam apenas "
  "as funções públicas da TAD. Não acessam diretamente seus campos internos.")
h("2. Estruturas e módulos")
tabela([
    ("Módulo", "Responsabilidade"),
    ("01-pilhas / 02-filas", "Nós encadeados, políticas LIFO/FIFO, contagem e demonstração."),
    ("03-listas", "Nós com anterior e próximo; inserção, consulta e remoção por posição."),
    ("04-grafos/grafo", "Vetores fixos de vértices e arestas, estrelas e persistência."),
    ("04-grafos/caminho", "Dijkstra compartilhado e formato de saída do TP 1."),
    ("04-grafos/algoritmos", "Percursos, componentes, Kruskal e saídas do TP 2."),
    ("05-arvores (adicional)", "ABB completa, remoções, altura e três percursos."),
], [52 * mm, 118 * mm])
p("O grafo reserva vetores de vértices e arestas na criação. Cada elemento tem ID "
  "e marca de atividade. Uma aresta armazena alfa, ômega e elos inteiros para as "
  "estrelas de entrada, saída e incidência. As listas de IDs de cada estrela são "
  "mantidas em ordem crescente.")
codigo("Grafo\n  +-- vertices[id]: ativo, inicio/fim das estrelas\n  +-- arestas[id]: ativa, alfa, omega, elos das estrelas\n\nAlgoritmos -> interface grafo.h -> armazenamento interno")
p("O índice zero significa ausência. Laços e paralelas são preservados com IDs "
  "próprios. Um laço aparece uma vez na estrela de incidência e contribui duas "
  "unidades ao grau total. A orientação depende das consultas realizadas.")

story.append(PageBreak())
h("3. Decisões de implementação")
p("<b>Percursos.</b> BFS utiliza uma fila em vetor e marca o vértice na descoberta. "
  "DFS usa uma pilha explícita de quadros com o vértice atual e a próxima aresta. "
  "Isso reproduz a DFS recursiva sem depender da profundidade da pilha do processo. "
  "Vizinhos são explorados por ID de aresta crescente.")
p("<b>Caminho mínimo.</b> Dijkstra mantém distância, predecessor e aresta "
  "predecessora. Seleciona linearmente o vértice aberto de menor distância. Pesos "
  "são externos à TAD, em float; a soma das distâncias usa double. Pesos negativos "
  "ou não finitos são rejeitados. No TP 1, a ausência de caminho imprime a partida; "
  "no TP 2, a mensagem é Sem caminho.")
p("<b>AGM.</b> Kruskal ordena por peso, desempata pelo ID e usa Union-Find com "
  "compressão de caminhos e união por tamanho. Arestas que uniriam vértices do mesmo "
  "componente são descartadas. Pesos negativos finitos são permitidos. Em grafo "
  "desconexo, é informada a ausência de AGM para o conjunto completo.")
p("<b>Componentes.</b> A busca é reiniciada em cada vértice não marcado. A função "
  "consulta incidências sem considerar a orientação; portanto, em dígrafos, obtém "
  "componentes fracamente conexos. Vértices isolados formam componentes próprios.")
p("<b>Remoções.</b> Remover uma aresta atualiza suas listas de incidência. Remover "
  "um vértice remove primeiro suas arestas. Os IDs restantes ficam estáveis e os "
  "removidos não são reciclados. Ao salvar, o arquivo original exige compactação "
  "dos IDs; essa renumeração não modifica o grafo em memória.")
h("4. Custos")
p("V e E são quantidades ativas; Cv e Ce são capacidades reservadas. Com lacunas "
  "de remoção, uma varredura global de IDs pode percorrer a capacidade. Os custos "
  "abaixo correspondem ao código entregue.")
tabela([
    ("Operação", "Tempo"),
    ("Consulta por ID / extremos / criar elemento", "O(1)"),
    ("Remover aresta / remover vértice", "O(1) / O(grau do vértice)"),
    ("Próxima aresta de uma estrela, partindo de membro", "O(1)"),
    ("BFS/DFS completa / componentes", "O(Cv + E)"),
    ("Dijkstra com seleção linear", "O(V * Cv + E + Ce)"),
    ("Kruskal, com ordenação usual", "O(Cv + Ce + E log E)"),
], [103 * mm, 67 * mm])
p("A TAD ocupa O(Cv + Ce). Percursos e Dijkstra usam O(Cv) auxiliar; Kruskal usa "
  "O(Cv + E). Com capacidades proporcionais ao tamanho e sem lacunas, os custos "
  "usuais são O(V + E), O(V² + E) e O(V + E log E), respectivamente.")

story.append(PageBreak())
h("5. Validação")
p("Os cinco programas foram compilados com GCC 6.3.0, C11 e as opções -Wall, "
  "-Wextra, -Wpedantic e -Werror. A execução em 19/10/2026 terminou sem avisos "
  "de compilação e com os testes aprovados.")
tabela([
    ("Verificação", "Critério utilizado"),
    ("TADs básicas", "Ordem LIFO/FIFO, consulta, tamanho, extremos, remoção até vazio e reinserção."),
    ("TAD de grafo", "31 operações, capacidades, IDs, estrelas, laços, paralelas, arquivos e remoções."),
    ("540 estados de remoção", "Graus e estrelas comparados com um modelo independente em C."),
    ("16 grupos de regressão", "Saídas, formatos, menu, direção, desconexão, percursos e erros."),
    ("25 grafos aleatórios", "BFS/DFS de referência; Bellman-Ford para distâncias; enumeração para AGM."),
    ("DFS com 4.000 vértices", "Cadeia completa sem recursão na implementação."),
    ("ABB adicional", "1.500 estados aleatórios, 120 estados de menu e cadeia de 3.000 nós."),
], [58 * mm, 112 * mm])
p("Foram incluídos grafo vazio, vértice isolado, origem igual ao destino, laços, "
  "arestas paralelas, pesos zero, pesos negativos, arquivo truncado, conteúdo extra, "
  "extremos inexistentes e argumentos inválidos. No caminho mínimo, cada aresta "
  "impressa também é confrontada com os vértices e com a soma dos pesos.")
h("Exemplo reproduzível")
p("O arquivo exemplos/grafo.txt contém seis vértices e oito arestas. Os pesos "
  "correspondentes estão em exemplos/pesos.txt. O menor caminho de 1 a 6 custa 8; "
  "a AGM possui cinco arestas e custo total 9.")
codigo("BFS: v1,a1,v2,a2,v3,a3,v4,a6,v5,a8,v6\nDFS: v1,a1,v2,a3,v4,a5,v5,a8,v6,a2,v3\n\nCaminho: v1,a2,v3,a4,v4,a5,v5,a8,v6\nDistancia: 8\n\nAGM: a2,a3,a4,a8,a5")
p("No exemplo do TP 1, a partida 3 e a chegada 2 produzem 3 -> 6 -> 1 -> 2. "
  "Quando a partida é 1 e a chegada é 4, a saída contém somente 1, como determinado "
  "pelo enunciado.")
p("Os testes são determinísticos e podem ser repetidos com testar.ps1. Não "
  "simulam falta de memória e não demonstram correção para todos os grafos "
  "possíveis; fornecem evidências de funcionamento e regressão dos contratos.")

story.append(PageBreak())
h("6. Uso, convenções e limites")
p("O grafo é lido em texto: primeira linha com o número de vértices e arestas, "
  "seguida por uma linha origem destino para cada aresta. Cada linha contém "
  "exatamente dois inteiros. O arquivo opcional de pesos contém um número finito "
  "por aresta, na mesma ordem. Sem esse arquivo, cada peso vale 1.")
codigo(".\\compilar.ps1\n.\\testar.ps1\n\n$g = 'exemplos/grafo.txt'\n$p = 'exemplos/pesos.txt'\n.\\output\\grafos.exe $g bfs 1\n.\\output\\grafos.exe $g componentes\n.\\output\\grafos.exe $g caminho 1 6 --pesos $p\n.\\output\\grafos.exe $g agm --pesos $p")
p("Nas buscas e no caminho do TP 2, --nao-dirigido permite considerar os dois "
  "sentidos. AGM e componentes sempre usam essa interpretação. A saída dos "
  "percursos registra a descoberta de cada vértice e a aresta que o descobriu; "
  "não registra retornos nem todas as arestas examinadas.")
p("As assinaturas originais do TP 2 não recebem pesos. Foram preservadas como "
  "operações de peso unitário e complementadas por variantes ComPesos. As rotinas "
  "da TAD retornam zero ou NULL em erro. O terminal usa código 2 para entrada "
  "inválida; ausência de caminho ou de AGM é tratada como resultado normal.")
p("IDs não reciclados podem esgotar a capacidade histórica mesmo depois de "
  "remoções. Salvar compacta IDs e persiste somente topologia: pesos externos "
  "precisam acompanhar a nova ordem. Essas escolhas cobrem pontos não especificados "
  "nas remoções adicionais e estão detalhadas no manual técnico.")
h("7. Conclusão")
p("A separação entre TAD, algoritmos e entrada/saída permite testar contratos "
  "isoladamente e reutilizar o armazenamento em tarefas distintas. As verificações "
  "mostraram a importância de tratar IDs, laços, paralelas, direção e estados vazios "
  "antes de analisar o custo dos algoritmos. A implementação oferece uma base "
  "concreta para explicar percursos, conjuntos disjuntos e caminhos mínimos.")
p("Código e documentação foram concluídos com assistência de IA. Este texto "
  "relata decisões e validações técnicas; a avaliação pessoal da experiência e "
  "a preparação para a apresentação oral devem ser feitas pelo aluno. A pasta "
  "adicional 05-arvores foi concluída a pedido, com as 14 operações da ABB. "
  "A altura mede arestas: vazia = -1 e folha = 0. Busca, inserção e remoção custam "
  "O(h); altura e percursos iterativos custam O(n) em tempo e espaço auxiliar. "
  "Não há balanceamento automático. Os módulos originais de pilha, fila e lista "
  "permanecem preservados.")
h("8. Referências")
p("RODRIGUES, Walace de Almeida. <i>TP - TADs basicas.pdf</i>; "
  "<i>TP - TAD Grafo estatico.pdf</i>, especialmente pp. 3-10; "
  "<i>TP - algoritmos.pdf</i>. Enunciados de Estruturas de Dados 2. "
  "Publicação complementar no Classroom: remoção de vértices e arestas.")

if __name__ == "__main__":
    doc = SimpleDocTemplate(str(DESTINO), pagesize=A4,
                            leftMargin=20 * mm, rightMargin=20 * mm,
                            topMargin=18 * mm, bottomMargin=25 * mm,
                            title="TADs e algoritmos em grafos",
                            author="Ayrton Sanabio")
    doc.build(story, onFirstPage=rodape, onLaterPages=rodape)
    print(DESTINO)
