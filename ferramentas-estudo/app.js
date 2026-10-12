let graph=null, trace=null, step=0, timer=null, dot='', svg='', renderMode='local';
let selectedV=new Set(), selectedE=new Set();
const $=id=>document.getElementById(id);
const esc=value=>String(value).replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const examples={graph:'grafo.json',tree:'arvore.json',trace:'rastro-bfs.json',result:'resultado-caminho-valido.json'};
const descriptions={graph:'Grafo: "directed" define o sentido; cada aresta usa IDs de vértices declarados em "vertices". "weight" é opcional.',tree:'Árvore binária: "root" aponta para a raiz; "left" e "right" são IDs de nós da lista "nodes".',trace:'Rastro: importe primeiro o grafo correspondente. Cada evento guarda o estado do passo; os IDs precisam existir no grafo.',result:'Resultado: use este modelo na caixa do verificador. Os vértices e as arestas precisam existir no grafo carregado.'};
let formatName='graph',formatText='';

function status(message,error=false){$('graphStatus').textContent=message;$('graphStatus').className='status '+(error?'error':'good')}
function traceStatus(message,error=false){$('traceState').textContent=message;$('traceState').className='trace-state '+(error?'bad':'muted')}
function download(name,content,type='application/json'){const a=document.createElement('a');a.href=URL.createObjectURL(new Blob([content],{type}));a.download=name;a.click();setTimeout(()=>URL.revokeObjectURL(a.href),1000)}
async function example(name){const response=await fetch('./exemplos/'+name);if(!response.ok)throw Error(`Não consegui abrir o exemplo ${name}. Abra esta pasta por um servidor local.`);const text=await response.text();try{return {text,data:JSON.parse(text)}}catch{throw Error(`O exemplo ${name} não contém JSON válido.`)}}
async function parseFile(input){const file=input.files?.[0];if(!file)throw Error('Escolha um arquivo .json.');if(!file.name.toLowerCase().endsWith('.json'))throw Error('Este importador aceita arquivos com extensão .json.');if(file.size===0)throw Error(`O arquivo ${file.name} está vazio. Baixe um modelo acima para começar.`);if(file.size>2_000_000)throw Error('O arquivo é muito grande. Limite: 2 MB.');const text=await file.text();try{return JSON.parse(text)}catch{throw Error(`O arquivo ${file.name} não contém JSON válido. Confira vírgulas, aspas e chaves; compare com o modelo acima.`)}}

async function api(path,body){
  try{
    const response=await fetch(path,{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(body)});
    if(response.headers.get('content-type')?.includes('application/json')){
      const data=await response.json();
      if(response.ok)return data;
      // A renderização local permite estudar mesmo sem Graphviz no servidor.
      if(path==='/api/render')return LocalStudy.render(body);
      if(path==='/api/verify')return LocalStudy.verify(body);
      throw Error(data.error||'Não foi possível concluir a operação.');
    }
  }catch(error){if(error instanceof TypeError){}else if(!String(error).includes('JSON'))throw error}
  return path==='/api/render'?LocalStudy.render(body):LocalStudy.verify(body);
}

function checkId(value,label){if(typeof value!=='string'||!value||value.length>100)throw Error(`${label}: use um ID de texto não vazio, como "1".`)}
function validateGraph(g){
  if(!g||g.version!==1||!['graph','binary_tree'].includes(g.kind))throw Error('Formato inválido: use "version": 1 e "kind": "graph" ou "binary_tree". Veja os modelos acima.');
  if(g.kind==='graph'){
    if(typeof g.directed!=='boolean'||!Array.isArray(g.vertices)||!Array.isArray(g.edges))throw Error('Grafo: informe "directed" (true/false), "vertices" e "edges" como listas.');
    if(g.vertices.length>500||g.edges.length>1000)throw Error('Exemplo muito grande para esta visualização: limite de 500 vértices e 1000 arestas.');
    const vs=new Set(),es=new Set();
    for(const v of g.vertices){checkId(v?.id,'Vértice');if(vs.has(v.id))throw Error(`Vértice duplicado: ${v.id}.`);vs.add(v.id)}
    for(const e of g.edges){checkId(e?.id,'Aresta');if(es.has(e.id))throw Error(`Aresta duplicada: ${e.id}.`);es.add(e.id);if(!vs.has(e.from)||!vs.has(e.to))throw Error(`Aresta ${e.id}: "from" e "to" devem apontar para vértices existentes.`);if(e.weight!=null&&(typeof e.weight!=='number'||!Number.isFinite(e.weight)))throw Error(`Aresta ${e.id}: "weight" precisa ser um número.`)}
    return;
  }
  if(!Array.isArray(g.nodes))throw Error('Árvore: informe "nodes" como uma lista.');
  if(g.nodes.length>500)throw Error('Árvore muito grande para esta visualização: limite de 500 nós.');
  const ids=new Set(),parents=new Set(),lookup=new Map();
  for(const n of g.nodes){checkId(n?.id,'Nó');if(ids.has(n.id))throw Error(`Nó duplicado: ${n.id}.`);ids.add(n.id);lookup.set(n.id,n)}
  if(Boolean(ids.size)!==Boolean(g.root)||g.root&&!ids.has(g.root))throw Error('Árvore: "root" deve apontar para um nó existente; para árvore vazia, use null.');
  for(const n of g.nodes)for(const side of ['left','right'])if(n[side]!=null){if(!ids.has(n[side]))throw Error(`Nó ${n.id}: filho ${n[side]} não existe.`);if(parents.has(n[side]))throw Error(`Nó ${n[side]} tem mais de um pai.`);parents.add(n[side])}
  if(parents.has(g.root))throw Error('A raiz não pode ter pai.');
  const visited=new Set(),pending=g.root?[g.root]:[];
  while(pending.length){const id=pending.pop();if(visited.has(id))throw Error('A árvore contém um ciclo.');visited.add(id);const n=lookup.get(id);for(const side of ['left','right'])if(n[side])pending.push(n[side])}
  if(visited.size!==ids.size)throw Error('A árvore tem nós desconectados da raiz.');
}

async function loadGraph(g,label='Exemplo'){
  validateGraph(g);stop();graph=g;trace=null;step=0;selectedV.clear();selectedE.clear();
  await update();
  const type=g.kind==='binary_tree'?`Árvore binária · ${g.nodes.length} nós`:`Grafo ${g.directed?'dirigido':'não dirigido'} · ${g.vertices.length} vértices · ${g.edges.length} arestas`;
  status(`${label} carregado. ${type}. Clique nos elementos para explorar.${renderMode==='local'?' Desenho gerado com Graphviz no navegador.':''}`);
  showTrace();
  if(g.kind==='graph'&&!$('answer').value.trim())setAnswer({version:1,kind:'path',vertices:['1','2','3'],edges:['a1','a3']});
}
async function update(){
  if(!graph)return;
  try{
    const result=await api('/api/render',{graph,vertices:[...selectedV],edges:[...selectedE]});
    dot=result.dot;svg=result.svg;renderMode=result.local?'local':'graphviz';$('drawing').innerHTML=svg;
    attachSvgClicks();drawRepresentations();
    $('selection').textContent=[...selectedV].map(v=>'Vértice '+v).concat([...selectedE].map(e=>'Aresta '+e)).join(', ')||'Nenhuma seleção.';
  }catch(error){$('drawing').textContent='Não foi possível desenhar: '+error.message;status(error.message,true)}
}
function selectVertex(id){selectedV=new Set([id]);selectedE.clear();update()}
function selectEdge(id){selectedE=new Set([id]);selectedV.clear();update()}
function attachSvgClicks(){
  if(graph.kind!=='graph')return;
  const vertexIds=new Set(graph.vertices.map(v=>v.id));
  for(const node of $('drawing').querySelectorAll('g.node')){const id=node.querySelector('title')?.textContent;if(vertexIds.has(id)){node.style.cursor='pointer';node.onclick=()=>selectVertex(id)}}
  for(const edge of $('drawing').querySelectorAll('g.edge')){const id=edge.getAttribute('id')?.replace(/^edge-/,'')||edge.querySelector('title')?.textContent;if(graph.edges.some(e=>e.id===id)){edge.style.cursor='pointer';edge.onclick=()=>selectEdge(id)}}
}
function drawRepresentations(){
  if(graph?.kind!=='graph'){$('matrix').textContent='Disponível para grafos. Abra o exemplo de grafo acima.';$('adjacency').textContent='Disponível para grafos. Abra o exemplo de grafo acima.';return}
  const vs=graph.vertices.map(v=>v.id),cell=(a,b)=>graph.edges.filter(e=>e.from===a&&e.to===b||!graph.directed&&e.from===b&&e.to===a);
  let html='<table><tr><th></th>'+vs.map(v=>`<th data-v="${esc(v)}">${esc(v)}</th>`).join('')+'</tr>';
  for(const a of vs){html+=`<tr><th data-v="${esc(a)}">${esc(a)}</th>`;for(const b of vs){const es=cell(a,b);html+=`<td data-a="${esc(a)}" class="${es.some(e=>selectedE.has(e.id))||selectedV.has(a)||selectedV.has(b)?'active':''}">${es.length?es.map(e=>`<span data-e="${esc(e.id)}">${esc(e.id)}${e.weight==null?'':` (${esc(e.weight)})`}</span>`).join('<br>'):'·'}</td>`}html+='</tr>'}html+='</table>';
  $('matrix').innerHTML=html;
  $('matrix').querySelectorAll('[data-v]').forEach(el=>el.onclick=()=>selectVertex(el.dataset.v));
  $('matrix').querySelectorAll('td').forEach(el=>el.onclick=event=>{const edge=event.target.closest('[data-e]');edge?selectEdge(edge.dataset.e):selectVertex(el.dataset.a)});
  let list='';
  for(const v of vs){list+=`<div class="item ${selectedV.has(v)?'active':''}"><strong data-v="${esc(v)}">${esc(v)}</strong> → `;const es=graph.edges.filter(e=>e.from===v||!graph.directed&&e.to===v);list+=es.length?es.map(e=>{const other=e.from===v?e.to:e.from;return `<span class="chip ${selectedE.has(e.id)?'active':''}" data-e="${esc(e.id)}">${esc(other)} via ${esc(e.id)}${e.weight==null?'':` (peso ${esc(e.weight)})`}</span>`}).join(''):'(isolado)';list+='</div>'}
  $('adjacency').innerHTML=list;
  $('adjacency').querySelectorAll('[data-v]').forEach(el=>el.onclick=()=>selectVertex(el.dataset.v));
  $('adjacency').querySelectorAll('[data-e]').forEach(el=>el.onclick=()=>selectEdge(el.dataset.e));
}

function stop(){if(timer){clearInterval(timer);timer=null}$('play').textContent='▶ Iniciar'}
function showTrace(){
  if(!trace){traceStatus('Abra o exemplo de rastro ou importe seu arquivo.');return}
  const event=trace.events[step];
  $('traceState').className='trace-state';
  $('traceState').innerHTML=`<p><strong>Passo ${step+1}/${trace.events.length}: ${esc(event.label||'Evento')}</strong> · atual: ${esc(event.current??'—')}</p><p>Fila: ${esc((event.queue||[]).join(' → ')||'vazia')} · Pilha: ${esc((event.stack||[]).join(' → ')||'vazia')} · Visitados: ${esc((event.visited||[]).join(', ')||'nenhum')}</p>`;
  selectedV=new Set(event.highlight_vertices||[]);selectedE=new Set(event.highlight_edges||[]);update();
}
function move(delta){if(!trace)return;step=Math.max(0,Math.min(trace.events.length-1,step+delta));showTrace();if(step===trace.events.length-1)stop()}
function start(){if(!trace)return;if(timer){stop();return}if(step===trace.events.length-1)step=0;showTrace();timer=setInterval(()=>move(1),1000/Number($('speed').value));$('play').textContent='⏸ Pausar'}
function checkTrace(t){
  if(t?.version!==1||t.kind!=='trace'||!Array.isArray(t.events)||!t.events.length)throw Error('Rastro: use "version": 1, "kind": "trace" e uma lista "events" não vazia.');
  const vs=new Set(graph.vertices.map(v=>v.id)),es=new Set(graph.edges.map(e=>e.id));
  for(const [i,event] of t.events.entries()){
    if(!event||typeof event!=='object')throw Error(`Evento ${i+1} inválido.`);
    if(event.current!=null&&!vs.has(event.current))throw Error(`Evento ${i+1}: vértice atual não existe no grafo.`);
    for(const key of ['queue','stack','visited','highlight_vertices','highlight_edges']){
      const values=event[key]||[],allowed=key==='highlight_edges'?es:vs;
      if(!Array.isArray(values)||values.some(v=>!allowed.has(v)))throw Error(`Evento ${i+1}: "${key}" contém IDs inexistentes.`);
    }
  }
}
function setAnswer(data){$('answer').value=JSON.stringify(data,null,2);$('verdict').textContent='';$('verdict').className=''}
function parseAnswer(){const raw=$('answer').value.trim();if(!raw)throw Error('Cole um resultado JSON ou escolha um exemplo acima.');try{return JSON.parse(raw)}catch{throw Error('O resultado não é um JSON válido. Confira aspas, vírgulas e chaves.')}}
async function showFormat(name){
  formatName=name;
  document.querySelectorAll('[data-format]').forEach(button=>button.setAttribute('aria-pressed',String(button.dataset.format===name)));
  $('formatHelp').textContent=descriptions[name];
  try{const sample=await example(examples[name]);if(formatName===name){formatText=JSON.stringify(sample.data,null,2);$('formatCode').textContent=formatText}}
  catch(error){$('formatCode').textContent=error.message;formatText=''}
}
async function runDemo(name){
  try{
    if(name==='trace'){
      const base=await example(examples.graph);await loadGraph(base.data,'Grafo do rastro');
      const sample=await example(examples.trace);checkTrace(sample.data);trace=sample.data;step=0;stop();showTrace();
      traceStatus('');showTrace();$('traceFileName').textContent='Exemplo: rastro-bfs.json';
    }else{const sample=await example(examples[name]);await loadGraph(sample.data,`Exemplo ${name==='graph'?'grafo.json':'arvore.json'}`);$('graphFileName').textContent=`Exemplo: ${examples[name]}`}
  }catch(error){name==='trace'?traceStatus(error.message,true):status(error.message,true)}
}

$('graphFile').addEventListener('change',async event=>{
  try{const file=event.target.files?.[0];const data=await parseFile(event.target);await loadGraph(data,file.name);$('graphFileName').textContent=file.name}
  catch(error){status(error.message,true)}finally{event.target.value=''}
});
$('quickFile').addEventListener('change',async event=>{
  try{
    const file=event.target.files?.[0];if(!file)return;
    const data=await parseFile(event.target);await loadGraph(data,file.name);
    $('graphFileName').textContent=file.name;
    $('quickStatus').textContent=`${file.name} importado. Veja o desenho no Visualizador abaixo.`;
    $('quickStatus').className='status good';
    document.getElementById('visualizador').scrollIntoView({behavior:'smooth'});
  }catch(error){$('quickStatus').textContent=error.message;$('quickStatus').className='status error'}
  finally{event.target.value=''}
});
$('traceFile').addEventListener('change',async event=>{
  try{const file=event.target.files?.[0];const data=await parseFile(event.target);if(!graph||graph.kind!=='graph')throw Error('Abra primeiro o grafo correspondente ao rastro.');checkTrace(data);trace=data;step=0;stop();showTrace();$('traceFileName').textContent=file.name}
  catch(error){traceStatus(error.message,true)}finally{event.target.value=''}
});
$('demoGraph').onclick=()=>runDemo('graph');$('demoTree').onclick=()=>runDemo('tree');$('demoTrace').onclick=()=>runDemo('trace');
document.querySelectorAll('[data-open-example]').forEach(button=>button.onclick=async()=>{
  const name=button.dataset.openExample;
  try{
    if(name==='rastro-bfs.json'){await runDemo('trace');return}
    const sample=await example(name);await loadGraph(sample.data,`Exemplo ${name}`);$('graphFileName').textContent=`Pasta exemplos: ${name}`;
  }catch(error){status(error.message,true)}
});
document.querySelectorAll('[data-format]').forEach(button=>button.onclick=()=>showFormat(button.dataset.format));
document.querySelectorAll('[data-download]').forEach(button=>button.onclick=async()=>{try{const name=button.dataset.download;const sample=await example(name);download(name,sample.text)}catch(error){status(error.message,true)}});
$('copyFormat').onclick=async()=>{if(!formatText)return;try{await navigator.clipboard.writeText(formatText);$('copyFormat').textContent='Copiado!';setTimeout(()=>$('copyFormat').textContent='Copiar modelo',1500)}catch{$('formatHelp').textContent='Não foi possível copiar automaticamente. Selecione o texto do modelo acima ou use Baixar modelo .json.'}};
$('downloadFormat').onclick=()=>formatText&&download(examples[formatName],formatText+'\n');
$('first').onclick=()=>{step=0;showTrace()};$('prev').onclick=()=>move(-1);$('next').onclick=()=>move(1);$('play').onclick=start;
$('speed').oninput=()=>{$('speedText').textContent=$('speed').value;if(timer){stop();start()}};
$('downloadDot').onclick=()=>dot&&download(graph.kind==='graph'?'grafo.dot':'arvore.dot',dot,'text/vnd.graphviz');
$('downloadSvg').onclick=()=>svg&&download(graph.kind==='graph'?'grafo.svg':'arvore.svg',svg,'image/svg+xml');
$('validPath').onclick=()=>setAnswer({version:1,kind:'path',vertices:['1','2','3'],edges:['a1','a3']});
$('invalidPath').onclick=()=>setAnswer({version:1,kind:'path',vertices:['2','1']});
$('validSearch').onclick=()=>setAnswer({version:1,kind:'search',start:'1',vertices:['1','2','3'],complete:true});
$('invalidSearch').onclick=()=>setAnswer({version:1,kind:'search',start:'1',vertices:['1','5'],complete:true});
$('verify').onclick=async()=>{
  try{if(!graph||graph.kind!=='graph')throw Error('Abra um grafo antes de verificar o resultado.');const result=await api('/api/verify',{graph,answer:parseAnswer()});$('verdict').className=result.ok?'good':'bad';$('verdict').textContent=(result.ok?'ACEITO: ':'REJEITADO: ')+result.message}
  catch(error){$('verdict').className='bad';$('verdict').textContent=error.message}
};
showFormat('graph');runDemo('graph');
window.studyLoadGraph=loadGraph;
