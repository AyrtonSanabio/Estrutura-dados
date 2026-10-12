const ConceptVisuals=(()=>{
  const layouts={
    diamond:{A:[80,155],B:[225,70],C:[225,240],D:[390,155],E:[525,155]},
    star:{A:[300,155],B:[130,65],C:[470,65],D:[130,245],E:[470,245]},
    tree:{A:[300,45],B:[175,125],C:[425,125],D:[105,225],E:[245,225],F:[355,225],G:[495,225]},
    split:{A:[80,90],B:[195,65],C:[145,230],D:[400,75],E:[520,100],F:[460,230]}
  };
  const S=(caption,layout,edges,nodes={},notes=[])=>({caption,layout,edges,nodes,notes});
  const scenes=[
    S('Os círculos são entidades; a linha destacada é a relação entre A e B.','diamond',[['A','B','focus']],{A:'focus',B:'focus'},[['vértice',74,205],['aresta',145,90]]),
    S('A seta A → B tem direção; a linha C — D pode ser percorrida nos dois sentidos.','diamond',[['A','B','focus','→'],['C','D','teal']],{A:'focus',B:'focus',C:'teal',D:'teal'},[['dirigido',110,70],['não dirigido',270,265]]),
    S('Os números nas arestas são custos. A rota azul custa 4 + 3 = 7.','diamond',[['A','B','focus','',4],['B','D','focus','',3],['A','C','muted','',9],['C','D','muted','',2]],{A:'focus',B:'focus',D:'focus'},[['custo total = 7',400,70]]),
    S('A toca quatro arestas: grau(A) = 4. Em um grafo dirigido, conte entradas e saídas separadamente.','star',[['A','B','focus'],['A','C','focus'],['A','D','focus'],['A','E','focus']],{A:'focus'},[['grau(A) = 4',260,25]]),
    S('B, C, D e E são vizinhos diretos de A; não é preciso examinar outros pares.','star',[['A','B','focus'],['A','C','focus'],['A','D','focus'],['A','E','focus']],{A:'focus',B:'teal',C:'teal',D:'teal',E:'teal'},[['N(A) = {B, C, D, E}',195,25]]),
    S('O caminho azul A → B → D possui 2 arestas; o custo pode ser diferente do comprimento.','diamond',[['A','B','focus','→',12],['B','D','focus','→',13],['A','C','muted','→'],['C','D','muted','→']],{A:'focus',B:'focus',D:'focus'},[['comprimento = 2',395,75],['custo = 25',395,100]]),
    S('As setas formam uma volta: A → B → D → A.','diamond',[['A','B','focus','→'],['B','D','focus','→'],['D','A','focus','→']],{A:'focus',B:'focus',D:'focus'},[['volta ao início',240,280]]),
    S('Existem dois grupos sem ligação entre eles: {A,B,C} e {D,E,F}.','split',[['A','B','focus'],['B','C','focus'],['A','C','focus'],['D','E','teal'],['E','F','teal'],['D','F','teal']],{A:'focus',B:'focus',C:'focus',D:'teal',E:'teal',F:'teal'},[['componente 1',70,285],['componente 2',395,285]]),
    S('A é a raiz; B e C são filhos; D, E, F e G são folhas.','tree',[['A','B','focus'],['A','C','focus'],['B','D','teal'],['B','E','teal'],['C','F','teal'],['C','G','teal']],{A:'focus',D:'teal',E:'teal',F:'teal',G:'teal'},[['raiz',315,37],['folhas',275,280]]),
    S('Cada nó tem no máximo dois filhos. B tem filho esquerdo D e direito E.','tree',[['A','B','focus'],['A','C','focus'],['B','D','teal'],['B','E','teal']],{B:'focus',D:'teal',E:'teal'},[['esquerda',70,170],['direita',255,170]]),
    S('As folhas estão a duas arestas da raiz: profundidade 2. Os ramos têm alturas semelhantes.','tree',[['A','B','focus'],['A','C','focus'],['B','D','focus'],['B','E','focus'],['C','F','focus'],['C','G','focus']],{A:'focus',B:'focus',C:'focus',D:'teal',E:'teal',F:'teal',G:'teal'},[['profundidade 0',320,30],['profundidade 1',450,115],['profundidade 2',490,270]]),
    S('BFS visita primeiro A, depois B e C, e só então D, E, F e G.','tree',[['A','B','focus'],['A','C','focus'],['B','D','teal'],['B','E','teal'],['C','F','teal'],['C','G','teal']],{A:'focus',B:'focus',C:'focus',D:'teal',E:'teal',F:'teal',G:'teal'},[['1',300,50],['2',175,130],['3',425,130],['4',105,230],['5',245,230],['6',355,230],['7',495,230]]),
    S('DFS desce pelo ramo A → B → D, volta e explora E, depois o ramo de C.','tree',[['A','B','focus','→'],['B','D','focus','→'],['B','E','teal','→'],['A','C','muted','→'],['C','F','muted','→'],['C','G','muted','→']],{A:'focus',B:'focus',D:'focus',E:'teal'},[['1',300,50],['2',175,130],['3',105,230],['4',245,230],['5',425,130],['6',355,230],['7',495,230]]),
    S('O mesmo grafo cabe em uma matriz de pares ou em listas de vizinhos.','diamond',[['A','B','focus'],['A','C','focus'],['B','D','focus']],{A:'focus'},[['A: B, C',405,55],['B: A, D',405,83],['C: A',405,111],['D: B',405,139],['matriz: célula por par',365,250]]),
    S('As setas respeitam a ordem 1 → 2 → 3 → 4; não pode haver ciclo.','diamond',[['A','B','focus','→'],['A','C','focus','→'],['B','D','focus','→'],['C','D','focus','→']],{A:'focus',B:'teal',C:'teal',D:'focus'},[['1',80,155],['2',225,70],['3',225,240],['4',390,155]]),
    S('Dijkstra escolhe a próxima menor distância conhecida. O caminho azul soma 7.','diamond',[['A','B','focus','→',4],['B','D','focus','→',3],['A','C','muted','→',9],['C','D','muted','→',2]],{A:'focus',B:'focus',D:'focus'},[['dist(A)=0',45,285],['dist(B)=4',170,25],['dist(D)=7',390,205]]),
    S('Uma aresta pode reduzir o custo com peso negativo; Bellman-Ford relaxa as ligações repetidamente.','diamond',[['A','B','focus','→',5],['B','D','focus','→',-3],['A','C','muted','→',6],['C','D','muted','→',1]],{A:'focus',B:'focus',D:'focus'},[['5 + (−3) = 2',370,75]]),
    S('As arestas azuis ligam todos os nós sem ciclo e com custo total mínimo.','diamond',[['A','B','focus','',2],['B','D','focus','',3],['A','C','focus','',1],['C','D','muted','',5],['B','C','muted','',4]],{A:'focus',B:'focus',C:'focus',D:'focus'},[['custo = 2 + 3 + 1 = 6',335,270]]),
    S('Dentro de cada grupo, as setas permitem ir e voltar entre quaisquer vértices.','split',[['A','B','focus','→'],['B','C','focus','→'],['C','A','focus','→'],['D','E','teal','→'],['E','F','teal','→'],['F','D','teal','→'],['C','D','muted','→']],{A:'focus',B:'focus',C:'focus',D:'teal',E:'teal',F:'teal'},[['bloco forte',75,285],['bloco forte',405,285]]),
    S('Passam 5 unidades no total. O corte tracejado cruza arestas de capacidade 3 + 2 = 5.','diamond',[['A','B','focus','→','3/3'],['A','C','focus','→','2/2'],['B','D','focus','→','3/3'],['C','D','focus','→','2/2']],{A:'focus',D:'teal'},[['origem',36,215],['destino',380,215],['fluxo 5',420,75]]),
    S('Cada ligação une lados diferentes; as azuis escolhem pares sem repetir vértices.','split',[['A','D','focus'],['B','E','focus'],['C','F','focus'],['A','E','muted'],['C','D','muted']],{A:'focus',B:'focus',C:'focus',D:'teal',E:'teal',F:'teal'},[['grupo 1',90,285],['grupo 2',430,285]]),
    S('A ocupa uma posição de ponte: muitos caminhos entre grupos passam por ela.','star',[['A','B','focus'],['A','C','focus'],['A','D','focus'],['A','E','focus'],['B','D','muted'],['C','E','muted']],{A:'focus',B:'teal',C:'teal',D:'teal',E:'teal'},[['ponte entre regiões',235,25]]),
    S('As ligações dentro de cada comunidade são densas; a ponte entre grupos é rara.','split',[['A','B','focus'],['B','C','focus'],['A','C','focus'],['D','E','teal'],['E','F','teal'],['D','F','teal'],['C','D','muted']],{A:'focus',B:'focus',C:'focus',D:'teal',E:'teal',F:'teal'},[['comunidade azul',60,285],['comunidade verde',380,285]]),
    S('A ligação tracejada surge em t₂. Consultas feitas em t₁ e t₂ podem ter respostas distintas.','diamond',[['A','B','focus'],['B','D','focus'],['A','C','teal'],['C','D','new']],{A:'focus',B:'focus',C:'teal',D:'focus'},[['t₁: A → B → D',330,55],['t₂: C → D adicionada',325,280]]),
    S('Nas camadas altas há poucos atalhos; a camada base refina a busca entre vizinhos próximos.','split',[['A','D','focus','→'],['A','B','teal'],['B','C','teal'],['D','E','teal'],['E','F','teal'],['C','F','muted']],{A:'focus',D:'focus',B:'teal',C:'teal',E:'teal',F:'teal'},[['camada alta: salto longo',100,25],['camada base: vizinhos próximos',155,285]])
  ];
  function render(index){
    const s=scenes[index];if(!s)return null;
    const coords=layouts[s.layout];
    const edges=s.edges.map(([a,b,kind,arrow,label])=>{const [x1,y1]=coords[a],[x2,y2]=coords[b];const mx=(x1+x2)/2,my=(y1+y2)/2;return `<line class="cv-edge cv-${kind}" x1="${x1}" y1="${y1}" x2="${x2}" y2="${y2}" ${arrow==='→'?'marker-end="url(#cv-arrow)"':''}/>${label!==undefined?`<text class="cv-weight" x="${mx}" y="${my-9}">${label}</text>`:''}`}).join('');
    const used=new Set(s.edges.flatMap(edge=>edge.slice(0,2)));
    const nodes=Object.entries(coords).filter(([id])=>used.has(id)).map(([id,[x,y]])=>`<g class="cv-node cv-${s.nodes[id]||'plain'}"><circle cx="${x}" cy="${y}" r="24"/><text x="${x}" y="${y+6}">${id}</text></g>`).join('');
    const notes=s.notes.map(([value,x,y])=>{const order=/^[1-7]$/.test(value);return `<text class="cv-note${order?' cv-order':''}" x="${order?x+30:x}" y="${order?y-20:y}">${value}</text>`}).join('');
    const cut=index===19?'<line x1="315" y1="30" x2="315" y2="280" stroke="#d97b24" stroke-width="3" stroke-dasharray="9 7"/>':'';
    return {caption:s.caption,svg:`<svg viewBox="0 0 600 310" role="img" aria-label="${s.caption}"><defs><marker id="cv-arrow" markerWidth="10" markerHeight="10" refX="31" refY="5" orient="auto" markerUnits="userSpaceOnUse"><path d="M0 0 L10 5 L0 10 Z" fill="#277ab9"/></marker></defs>${edges}${cut}${nodes}${notes}</svg>`};
  }
  return {render,count:scenes.length};
})();
