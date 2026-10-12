// Renderização e verificação locais quando a página é aberta por um servidor estático.
// O servidor Python continua usando Graphviz quando está disponível.
const LocalStudy = (() => {
  const quote = value => JSON.stringify(String(value));
  let graphvizPromise;
  function graphviz() {
    graphvizPromise ??= import('./node_modules/@hpcc-js/wasm/dist/graphviz.js')
      .then(module => module.Graphviz.load())
      .catch(() => { graphvizPromise = null; throw Error('Graphviz não está disponível. Execute npm ci nesta pasta ou abra a página pelo servidor Python.'); });
    return graphvizPromise;
  }

  function dotSource(g) {
    const theme=['  graph [bgcolor="transparent", pad="0.4", nodesep="0.7", ranksep="0.9", splines="true", outputorder="edgesfirst"];','  node [shape="circle", style="filled", fillcolor="#e8f3ff", color="#4775a7", fontcolor="#173b61", fontname="Arial", fontsize="15", penwidth="2", margin="0.18"];','  edge [color="#7189a5", fontcolor="#3f5e80", fontname="Arial", fontsize="11", penwidth="2", arrowsize="0.7"];'];
    if (g.kind === 'binary_tree') return ['digraph T {',...theme,'  graph [ordering="out"];', ...g.nodes.map(n => `  ${quote(n.id)} [label=${quote(n.label||n.id)}, fillcolor=${quote(n.id===g.root?'#d9f4e2':'#e8f3ff')}];`), ...g.nodes.flatMap(n => ['left','right'].filter(side=>n[side]).map(side=>`  ${quote(n.id)} -> ${quote(n[side])} [label=${quote(side==='left'?'E':'D')}, color=${quote(side==='left'?'#3273b9':'#9b55a5')}];`)), '}'].join('\n')+'\n';
    const connector = g.directed ? '->' : '--';
    return [g.directed?'digraph G {':'graph G {',...theme,'  graph [rankdir="LR"];', ...g.vertices.map(v=>`  ${quote(v.id)} [label=${quote(v.label||v.id)}];`), ...g.edges.map(e=>`  ${quote(e.from)} ${connector} ${quote(e.to)} [id=${quote('edge-'+e.id)}, label=${quote(e.id)}, tooltip=${quote(e.id+(e.weight==null?'':` · peso ${e.weight}`))}];`), '}'].join('\n')+'\n';
  }

  async function render({graph,vertices=[],edges=[]}) {
    let dot=dotSource(graph);
    for(const id of vertices) dot=dot.replace(`${quote(id)} [label=`,`${quote(id)} [fillcolor="#ffda85", color="#c67815", label=`);
    for(const id of edges) dot=dot.replace(`id=${quote('edge-'+id)}, label=`,`id=${quote('edge-'+id)}, color="#dd711f", fontcolor="#a85012", penwidth="3.5", label=`);
    const engine=await graphviz();
    return {svg:engine.layout(dot,'svg','dot'),dot,local:true};
  }

  function verify({graph,answer}) {
    const vertices = new Set(graph.vertices.map(v=>v.id));
    const edges = new Map(graph.edges.map(e=>[e.id,e]));
    const reject = message => ({ok:false,message});
    const accept = message => ({ok:true,message});
    if (!answer || answer.version !== 1) return reject('O resultado precisa ter "version": 1.');
    const sequence = answer.vertices, chosen = answer.edges;
    if (answer.kind === 'path') {
      if (!Array.isArray(sequence) || !sequence.length) return reject('O caminho precisa listar pelo menos um vértice.');
      for (let i=0;i<sequence.length;i++) {
        if (!vertices.has(sequence[i])) return reject(`Posição ${i}: vértice ${sequence[i]} não existe.`);
        if (sequence.slice(0,i).includes(sequence[i])) return reject(`Posição ${i}: vértice repetido.`);
      }
      if (chosen !== undefined && (!Array.isArray(chosen) || chosen.length !== sequence.length-1)) return reject('Informe uma aresta para cada transição do caminho.');
      for (let i=0;i<sequence.length-1;i++) {
        const a=sequence[i],b=sequence[i+1];
        const candidates=graph.edges.filter(e=>e.from===a&&e.to===b||!graph.directed&&e.from===b&&e.to===a);
        if (!candidates.length) return reject(`Passo ${i+1}: não há aresta de ${a} para ${b}.`);
        if (chosen !== undefined && !candidates.some(e=>e.id===chosen[i])) return reject(`Passo ${i+1}: a aresta ${chosen[i]} não liga ${a} a ${b}.`);
      }
      return accept('Caminho estruturalmente válido. O custo mínimo não foi verificado.');
    }
    if (answer.kind === 'search') {
      if (!Array.isArray(sequence)) return reject('A busca precisa listar os vértices visitados.');
      if (!vertices.has(answer.start)) return reject('O vértice inicial não existe no grafo.');
      if (sequence[0] !== answer.start) return reject('A busca deve começar pelo vértice inicial.');
      if (new Set(sequence).size !== sequence.length) return reject('A busca repete um vértice.');
      if (sequence.some(v=>!vertices.has(v))) return reject('A busca inclui um vértice inexistente.');
      const reachable=new Set([answer.start]);let changed=true;
      while(changed){const before=reachable.size;for(const e of graph.edges){if(reachable.has(e.from))reachable.add(e.to);if(!graph.directed&&reachable.has(e.to))reachable.add(e.from)}changed=reachable.size!==before}
      if(sequence.some(v=>!reachable.has(v)))return reject('A busca inclui um vértice não alcançável.');
      if(answer.complete&&sequence.length!==reachable.size)return reject('Busca incompleta: falta um vértice alcançável.');
      return accept('Visitas distintas e alcançáveis. A ordem de BFS/DFS não foi provada.');
    }
    if (answer.kind === 'spanning_tree') {
      if(graph.directed)return reject('A árvore geradora exige um grafo não dirigido.');
      if(!Array.isArray(chosen))return reject('Informe a lista de IDs das arestas.');
      const parent=new Map([...vertices].map(v=>[v,v]));
      function root(v){while(parent.get(v)!==v)v=parent.get(v);return v}
      for(const id of chosen){const e=edges.get(id);if(!e)return reject(`Aresta ${id} não existe.`);const a=root(e.from),b=root(e.to);if(a===b)return reject(`Aresta ${id} fecha um ciclo.`);parent.set(a,b)}
      if(chosen.length!==Math.max(0,vertices.size-1))return reject(`Esperadas ${Math.max(0,vertices.size-1)} arestas.`);
      if(new Set([...vertices].map(root)).size>1)return reject('As arestas não conectam todos os vértices.');
      return accept('Árvore geradora estruturalmente válida. Peso mínimo não verificado.');
    }
    return reject('Use "kind": "path", "search" ou "spanning_tree".');
  }

  return {render,verify,dotSource};
})();
