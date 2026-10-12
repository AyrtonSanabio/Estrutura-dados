const StudyPages=(()=>{
  const topics=[
    ['Vértice e aresta','Básico','Vértices representam entidades; arestas representam relações entre duas entidades.','Em uma rede social, pessoas são vértices e amizades são arestas.','Base de qualquer modelo de rede.'],
    ['Grafo dirigido e não dirigido','Básico','No dirigido, a aresta A→B tem sentido. No não dirigido, A—B vale nos dois sentidos.','Seguir alguém é dirigido; amizade recíproca pode ser não dirigida.','Evita interpretar uma ligação no sentido errado.'],
    ['Peso e custo','Básico','Um peso é um valor associado à aresta: distância, tempo, preço ou risco.','A estrada A—B leva 12 minutos; B—C leva 7.','Caminhos mínimos e planejamento de rotas.'],
    ['Grau de um vértice','Básico','Grau é o número de arestas incidentes. Em grafos dirigidos, distinguimos entrada e saída.','Uma conta seguida por 40 pessoas tem grau de entrada 40.','Detectar hubs e medir conectividade.'],
    ['Vizinhança e adjacência','Básico','Dois vértices são adjacentes quando uma aresta os liga. A lista de adjacência guarda os vizinhos de cada um.','A lista de A pode ser [B, C, D].','Percorrer grafos sem examinar todos os pares.'],
    ['Caminho e comprimento','Básico','Caminho é uma sequência de vértices ligados por arestas. Comprimento conta arestas; custo soma pesos.','A→B→D usa duas arestas, mas pode custar 25 minutos.','Rotas, navegação e cadeias de dependências.'],
    ['Ciclo','Básico','Um ciclo sai de um vértice e volta a ele seguindo arestas.','A depende de B, B de C e C de A: há dependência circular.','Diagnosticar loops e pré-requisitos impossíveis.'],
    ['Conectividade e componentes','Intermediário','Um componente é um conjunto de vértices ligados entre si. Um grafo pode ter vários componentes isolados.','Duas comunidades sem contatos entre elas formam componentes separados.','Detectar regiões desconectadas de uma rede.'],
    ['Árvore, raiz e folhas','Básico','Árvore conectada com n vértices tem n−1 arestas e nenhum ciclo. Na árvore enraizada há raiz, pais, filhos e folhas.','Pastas de um projeto formam uma hierarquia de árvore.','Hierarquias, menus e índices.'],
    ['Árvore binária','Básico','Cada nó tem no máximo dois filhos identificados como esquerdo e direito.','Em uma árvore de decisão: sim à esquerda, não à direita.','Busca, expressões e decisões.'],
    ['Altura, profundidade e balanceamento','Intermediário','Profundidade é a distância até a raiz; altura mede o maior caminho até uma folha. Balancear limita caminhos longos.','Uma árvore de busca muito inclinada pode virar quase uma lista.','Manter consultas eficientes.'],
    ['Busca em largura (BFS)','Intermediário','BFS explora por camadas usando uma fila. Em grafo sem pesos, encontra o caminho com menos arestas.','Primeiro amigos diretos, depois amigos dos amigos.','Alcançabilidade e menor número de saltos.'],
    ['Busca em profundidade (DFS)','Intermediário','DFS segue um ramo até não poder continuar e então volta. Usa pilha ou recursão.','Explorar uma sequência de pré-requisitos antes de outra.','Ciclos, componentes e ordenação topológica.'],
    ['Matriz versus lista de adjacência','Intermediário','Matriz usa uma célula por par de vértices; lista guarda apenas ligações existentes.','Rede esparsa com milhares de pessoas e poucas amizades por pessoa favorece lista.','Escolher espaço e velocidade de consulta.'],
    ['DAG e ordenação topológica','Intermediário','DAG é um grafo dirigido sem ciclos. Uma ordenação topológica respeita todas as dependências.','Compilar módulo B só depois de A.','Pipelines, currículo e agendamento.'],
    ['Caminho mínimo e Dijkstra','Intermediário','Dijkstra expande o menor custo conhecido quando todos os pesos são não negativos.','Escolher ruas com menor tempo estimado.','Roteamento e logística.'],
    ['Bellman-Ford e pesos negativos','Avançado','Bellman-Ford relaxa todas as arestas repetidamente e detecta ciclos de custo negativo alcançáveis.','Créditos ou descontos modelados como pesos negativos.','Quando Dijkstra não se aplica.'],
    ['Árvore geradora mínima','Intermediário','Liga todos os vértices de um grafo não dirigido com custo total mínimo, sem ciclos.','Conectar centros de dados com menor comprimento de cabos.','Projeto de redes físicas.'],
    ['Componentes fortemente conexos','Avançado','Em um grafo dirigido, um componente forte permite ir de qualquer vértice a qualquer outro do mesmo grupo.','Serviços que se chamam mutuamente formam um bloco fortemente conectado.','Encontrar ciclos estruturais e modularizar.'],
    ['Fluxo máximo e corte mínimo','Avançado','Fluxo máximo calcula quanto pode atravessar uma rede com capacidades; corte mínimo revela gargalos.','Tubulações com capacidades por trecho.','Transporte, alocação e redes.'],
    ['Grafo bipartido e emparelhamento','Avançado','Vértices se dividem em dois grupos; arestas ligam apenas grupos diferentes. Emparelhamento escolhe pares sem conflito.','Motoristas de um lado, viagens do outro.','Atribuição de tarefas e recursos.'],
    ['Centralidade','Avançado','Medidas como grau, proximidade e intermediação tentam identificar vértices influentes por critérios diferentes.','Uma estação pode não ter mais conexões, mas servir de ponte entre regiões.','Análise social e pontos críticos.'],
    ['Comunidades','Avançado','Comunidades são grupos com mais conexões internas do que externas, conforme um critério de agrupamento.','Pessoas com interesses próximos podem formar um grupo denso.','Segmentação e descoberta de grupos.'],
    ['Grafos dinâmicos','Avançado','Vértices e arestas mudam com o tempo; respostas precisam considerar o instante ou atualizar índices.','Ruas fechadas e abertas ao longo do dia.','Sistemas em tempo real.'],
    ['HNSW e busca aproximada','Avançado','Índices como HNSW ligam vetores semelhantes em camadas de um grafo para buscar vizinhos aproximados.','Encontrar imagens parecidas em milhões de vetores.','Busca semântica e recomendação em escala.']
  ];
  const cards=[
    {title:'Uber · H3',tag:'Índice espacial em C',description:'O H3 divide a Terra em células hexagonais. A API pública percorre células vizinhas e caminhos na grade. É uma estrutura geoespacial com relações de vizinhança; não é o código privado de despacho de viagens.',repo:'https://github.com/uber/h3',source:'https://github.com/uber/h3/blob/master/src/h3lib/lib/algos.c',docs:'https://h3geo.org/docs/api/traversal/',code:'// Exemplo didático de chamada à API pública H3\nH3Index origem = /* célula obtida de uma coordenada */;\nint passos = 2;\n// gridDisk devolve células até 2 passos da origem.\nH3Error erro = gridDisk(origem, passos, vizinhos);'},
    {title:'Google · OR-Tools',tag:'Roteamento de veículos',description:'O repositório público contém um exemplo executável de roteamento. Um modelo liga locais por custos e procura rotas para veículos. É código aberto da biblioteca, sem afirmar que seja o sistema interno do Google.',repo:'https://github.com/google/or-tools',source:'https://github.com/google/or-tools/blob/stable/ortools/constraint_solver/samples/simple_routing_program.py',docs:'https://developers.google.com/optimization/routing/vrp',code:'# Exemplo didático com a API pública OR-Tools\nmanager = pywrapcp.RoutingIndexManager(n, 1, deposito)\nrouting = pywrapcp.RoutingModel(manager)\n# Um callback fornece o custo de cada deslocamento.\nrouting.SetArcCostEvaluatorOfAllVehicles(custo_index)'},
    {title:'Meta · Faiss HNSW',tag:'Índice em grafo para vetores',description:'O Faiss oferece HNSW, índice baseado em ligações entre vetores próximos. O código público mostra a estrutura de vizinhos e a busca aproximada. Não implica que todo produto da Meta use esta configuração.',repo:'https://github.com/facebookresearch/faiss',source:'https://github.com/facebookresearch/faiss/blob/main/faiss/impl/HNSW.h',docs:'https://github.com/facebookresearch/faiss/wiki/The-index-factory',code:'# Exemplo didático com a API pública Faiss\nindex = faiss.IndexHNSWFlat(dimensoes, 32)\nindex.add(vetores)\ndistancias, ids = index.search(consulta, 5)'}
  ];
  const applications=[
    {title:'Rede social',nodes:'Pessoas ou contas',edges:'Amizade, seguir ou interação',question:'Quem está a dois contatos de Ana? Quem conecta duas comunidades?',algorithm:'BFS, centralidade, comunidades',example:'rede-social.json'},
    {title:'Logística urbana',nodes:'Depósitos, entregas e cruzamentos',edges:'Ruas com distância ou tempo',question:'Qual rota tem menor custo? Onde há gargalos?',algorithm:'Dijkstra, fluxo máximo, roteamento',example:'logistica.json'},
    {title:'Dependências de software',nodes:'Pacotes e módulos',edges:'A depende de B',question:'Qual ordem de construção respeita os pré-requisitos?',algorithm:'DFS, detecção de ciclos, ordem topológica',example:'dependencias.json'},
    {title:'Detecção de fraude',nodes:'Contas, cartões e dispositivos',edges:'Uso compartilhado ou transferência',question:'Há um grupo de contas fortemente ligado por um dispositivo?',algorithm:'Componentes, comunidades, caminhos',example:null},
    {title:'Conhecimento e recomendações',nodes:'Pessoas, produtos, filmes ou conceitos',edges:'Comprou, assistiu, pertence a',question:'Que itens se ligam aos interesses de alguém?',algorithm:'Vizinhança, caminhos e ranqueamento',example:null},
    {title:'Biologia e saúde',nodes:'Proteínas, genes ou substâncias',edges:'Interação ou regulação',question:'Quais entidades podem influenciar um alvo?',algorithm:'Caminhos, centralidade, comunidades',example:null}
  ];
  const $=id=>document.getElementById(id);
  const tabs=['laboratorio','conceitos','galeria','codigo-publico','aplicacoes','algoritmos'];
  function showTab(name,scroll=true){
    if(!tabs.includes(name))name='laboratorio';
    tabs.forEach(id=>$(id).hidden=id!==name);
    document.querySelectorAll('[data-tab]').forEach(link=>link.setAttribute('aria-current',link.dataset.tab===name?'page':'false'));
    if(scroll)window.scrollTo({top:0,behavior:'smooth'});
    if(name==='galeria')requestAnimationFrame(drawGallery);
  }
  function route(){const hash=location.hash.slice(1);showTab(tabs.includes(hash)?hash:'laboratorio',false)}
  document.querySelectorAll('[data-tab]').forEach(link=>link.addEventListener('click',()=>showTab(link.dataset.tab)));
  window.addEventListener('hashchange',route);
  route();
  const conceptList=$('conceptList'),detail=$('conceptDetail');
  function chooseConcept(index){
    const [title,level,definition,example,application]=topics[index];
    detail.replaceChildren();
    const badge=document.createElement('span');badge.className='concept-level';badge.textContent=level;
    const h=document.createElement('h3');h.textContent=title;
    const intro=document.createElement('p');intro.textContent=definition;
    const section=document.createElement('div');section.className='concept-example';
    const label=document.createElement('strong');label.textContent='Exemplo concreto';
    const ex=document.createElement('p');ex.textContent=example;
    const use=document.createElement('p');use.innerHTML='<strong>Para que serve:</strong> ';use.append(document.createTextNode(application));
    const visual=ConceptVisuals.render(index);
    const figure=document.createElement('figure');figure.className='concept-visual';
    figure.innerHTML=visual.svg;
    const caption=document.createElement('figcaption');caption.textContent=visual.caption;figure.append(caption);
    section.append(label,ex,use);detail.append(badge,h,figure,intro,section);
    conceptList.querySelectorAll('button').forEach(button=>button.setAttribute('aria-pressed',String(Number(button.dataset.index)===index)));
  }
  function renderConcepts(){
    const query=$('conceptFilter').value.toLocaleLowerCase('pt-BR');
    conceptList.replaceChildren();
    topics.forEach((topic,index)=>{
      if(!topic.join(' ').toLocaleLowerCase('pt-BR').includes(query))return;
      const button=document.createElement('button');button.type='button';button.dataset.index=index;button.textContent=topic[0];
      const small=document.createElement('small');small.textContent=topic[1];button.append(small);
      button.onclick=()=>chooseConcept(index);conceptList.append(button);
    });
    if(!conceptList.children.length)conceptList.textContent='Nenhum conceito encontrado.';
    else chooseConcept(Number(conceptList.querySelector('button').dataset.index));
  }
  $('conceptFilter').oninput=renderConcepts;renderConcepts();
  const companyCards=$('companyCards');
  cards.forEach(card=>{
    const article=document.createElement('article');article.className='case-card';
    const tag=document.createElement('span');tag.className='kind';tag.textContent=card.tag;
    const title=document.createElement('h3');title.textContent=card.title;
    const p=document.createElement('p');p.textContent=card.description;
    const pre=document.createElement('pre');pre.textContent=card.code;
    const links=document.createElement('div');links.className='source-links';
    [['Ver código real',card.source],['Repositório',card.repo],['Documentação',card.docs]].forEach(([label,url])=>{const a=document.createElement('a');a.href=url;a.target='_blank';a.rel='noopener noreferrer';a.textContent=label;links.append(a)});
    article.append(tag,title,p,pre,links);companyCards.append(article);
  });
  applications.forEach(item=>{
    const article=document.createElement('article');article.className='case-card';
    const h=document.createElement('h3');h.textContent=item.title;
    const dl=document.createElement('dl');
    [['Nós',item.nodes],['Arestas',item.edges],['Pergunta',item.question],['Algoritmos',item.algorithm]].forEach(([label,value])=>{const dt=document.createElement('dt');dt.textContent=label;const dd=document.createElement('dd');dd.textContent=value;dl.append(dt,dd)});
    article.append(h,dl);
    if(item.example){const button=document.createElement('button');button.type='button';button.textContent='Abrir grafo ilustrativo';button.onclick=async()=>{try{const response=await fetch('./exemplos/'+item.example);if(!response.ok)throw Error('Exemplo indisponível.');await window.studyLoadGraph(await response.json(),item.title);location.hash='laboratorio';setTimeout(()=>$('visualizador').scrollIntoView({behavior:'smooth'}),30)}catch(error){button.textContent=error.message}};article.append(button)}
    $('applicationCards').append(article);
  });
  const gallery=[
    {title:'Rede social com comunidades',type:'social',nodes:270,description:'Três grupos densos e poucas ligações entre eles. Pontos centrais representam contas com muitas conexões.'},
    {title:'Malha logística',type:'logistics',nodes:180,description:'Centros, cruzamentos e rotas regionais. A espessura destaca corredores de transporte.'},
    {title:'Dependências de software',type:'dependencies',nodes:220,description:'Camadas de pacotes e módulos. As setas indicam dependências entre níveis.'}
  ];
  gallery.forEach((item,index)=>{
    const article=document.createElement('article');article.className='gallery-card';
    const canvas=document.createElement('canvas');canvas.width=1000;canvas.height=620;canvas.id='gallery'+index;canvas.setAttribute('role','img');canvas.setAttribute('aria-label',item.title+'; visualização ilustrativa');
    const h=document.createElement('h3');h.textContent=item.title;
    const p=document.createElement('p');p.textContent=item.description;
    const badge=document.createElement('small');badge.textContent=item.nodes+' nós · ilustração gerada localmente';
    const save=document.createElement('button');save.className='secondary';save.textContent='Baixar imagem PNG';save.onclick=()=>{const a=document.createElement('a');a.href=canvas.toDataURL('image/png');a.download='grafo-'+item.type+'.png';a.click()};
    article.append(canvas,h,badge,p,save);$('galleryCards').append(article);
  });
  function drawGallery(){
    let seed=42;const rand=()=>((seed=(seed*1664525+1013904223)>>>0)/4294967296);
    gallery.forEach((item,index)=>{
      const canvas=$('gallery'+index),c=canvas.getContext('2d'),points=[];
      c.fillStyle='#0e2137';c.fillRect(0,0,1000,620);
      if(item.type==='social'){
        const centers=[[245,315],[510,215],[745,360]];
        for(let i=0;i<item.nodes;i++){const k=i%3,angle=rand()*Math.PI*2,r=Math.sqrt(rand())*175;points.push({x:centers[k][0]+Math.cos(angle)*r,y:centers[k][1]+Math.sin(angle)*r,k})}
        for(let i=0;i<850;i++){const a=Math.floor(rand()*points.length),b=Math.floor(rand()*points.length);if(points[a].k!==points[b].k&&rand()>.08)continue;line(c,points[a],points[b],points[a].k===points[b].k?'#59bdad25':'#f4ae5a55',1)}
      }else if(item.type==='logistics'){
        for(let i=0;i<item.nodes;i++){const col=i%18,row=Math.floor(i/18);points.push({x:55+col*52+(rand()-.5)*18,y:60+row*54+(rand()-.5)*18,k:row})}
        for(let i=0;i<points.length;i++){if(i%18<17)line(c,points[i],points[i+1],'#45b5d349',1.4);if(i+18<points.length)line(c,points[i],points[i+18],'#45b5d330',1);if(i%43===0&&i+36<points.length)line(c,points[i],points[i+36],'#ffca6377',3)}
      }else{
        for(let i=0;i<item.nodes;i++){const layer=Math.floor(i/44);points.push({x:100+layer*200+(rand()-.5)*80,y:35+(i%44)*13+(rand()-.5)*10,k:layer})}
        for(let i=0;i<points.length;i++)for(let n=0;n<2;n++){const next=i+44+Math.floor(rand()*44);if(next<points.length)line(c,points[i],points[next],'#8bb6e12c',1)}
      }
      points.forEach((p,i)=>{c.beginPath();c.arc(p.x,p.y,item.type==='logistics'&&i%43===0?6:2.8,0,Math.PI*2);c.fillStyle=item.type==='social'?['#64d4c0','#89b4f8','#f5bc73'][p.k]:item.type==='logistics'?'#75d3e5':'#a7c5f6';c.fill()});
    });
  }
  function line(c,a,b,color,width){c.strokeStyle=color;c.lineWidth=width;c.beginPath();c.moveTo(a.x,a.y);c.lineTo(b.x,b.y);c.stroke()}
  return {showTab};
})();
