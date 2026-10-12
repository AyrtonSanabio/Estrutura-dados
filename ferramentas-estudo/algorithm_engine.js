/* Núcleo puro dos passos didáticos. O navegador usa o mesmo código testado em Node. */
const AlgorithmEngine=(()=>{
  const graph={A:['B','C'],B:['D','E'],C:['F'],D:[],E:['G'],F:[],G:[]};
  const definitions={
    bubble:{name:'Bubble sort',kind:'sort',help:'Compara vizinhos e troca quando estão fora de ordem. O maior valor vai para o fim a cada rodada. O(n²).',code:['function bubble(a) {','  for (let fim = a.length - 1; fim > 0; fim--) {','    for (let i = 0; i < fim; i++) {','      if (a[i] > a[i + 1]) {','        [a[i], a[i + 1]] = [a[i + 1], a[i]];','      }','    }','  }','  return a;','}']},
    insertion:{name:'Insertion sort',kind:'sort',help:'Insere cada valor na posição certa do prefixo já ordenado. O(n²); eficiente em listas pequenas ou quase ordenadas.',code:['function insertion(a) {','  for (let i = 1; i < a.length; i++) {','    const valor = a[i]; let j = i - 1;','    while (j >= 0 && a[j] > valor) {','      a[j + 1] = a[j]; j--;','    }','    a[j + 1] = valor;','  }','  return a;','}']},
    selection:{name:'Selection sort',kind:'sort',help:'Encontra o menor do trecho restante e coloca na próxima posição. Faz O(n²) comparações.',code:['function selection(a) {','  for (let i = 0; i < a.length; i++) {','    let menor = i;','    for (let j = i + 1; j < a.length; j++) {','      if (a[j] < a[menor]) menor = j;','    }','    [a[i], a[menor]] = [a[menor], a[i]];','  }','  return a;','}']},
    merge:{name:'Merge sort',kind:'sort',help:'Divide a lista, ordena as metades e intercala. O(n log n), usando memória auxiliar.',code:['function mergeSort(a, inicio = 0, fim = a.length) {','  if (fim - inicio <= 1) return a;','  const meio = Math.floor((inicio + fim) / 2);','  mergeSort(a, inicio, meio);','  mergeSort(a, meio, fim);','  const partes = intercalar(a.slice(inicio, meio), a.slice(meio, fim));','  a.splice(inicio, fim - inicio, ...partes);','  return a;','}','function intercalar(esq, dir) {','  const saida = []; let i = 0, j = 0;','  while (i < esq.length || j < dir.length) {','    if (j >= dir.length || i < esq.length && esq[i] <= dir[j])','      saida.push(esq[i++]);','    else saida.push(dir[j++]);','  }','  return saida;','}']},
    quick:{name:'Quick sort',kind:'sort',help:'Escolhe pivô, separa menores e maiores e repete. Média O(n log n); pior caso O(n²).',code:['function quickSort(a, esquerda = 0, direita = a.length - 1) {','  if (esquerda >= direita) return a;','  const pivo = a[direita]; let p = esquerda;','  for (let i = esquerda; i < direita; i++) {','    if (a[i] <= pivo) {','      [a[i], a[p]] = [a[p], a[i]]; p++;','    }','  }','  [a[p], a[direita]] = [a[direita], a[p]];','  quickSort(a, esquerda, p - 1);','  quickSort(a, p + 1, direita);','  return a;','}']},
    linear:{name:'Busca linear',kind:'search',help:'Examina os valores um a um. Funciona em lista desordenada. O(n).',code:['function buscaLinear(a, alvo) {','  for (let i = 0; i < a.length; i++) {','    if (a[i] === alvo) return i;','  }','  return -1;','}']},
    binary:{name:'Busca binária',kind:'search',help:'Exige valores ordenados. Descarta metade a cada comparação. O(log n). A visualização ordena uma cópia antes de buscar.',code:['function buscaBinaria(aOrdenado, alvo) {','  let inicio = 0, fim = aOrdenado.length - 1;','  while (inicio <= fim) {','    const meio = Math.floor((inicio + fim) / 2);','    if (aOrdenado[meio] === alvo) return meio;','    if (aOrdenado[meio] < alvo) inicio = meio + 1;','    else fim = meio - 1;','  }','  return -1;','}']},
    bfs:{name:'Busca em largura (BFS)',kind:'graph',help:'Percorre o grafo por camadas com uma fila. O(V + E). Grafo didático: A→B,C; B→D,E; C→F; E→G.',code:['function bfs(grafo, inicio) {','  const fila = [inicio], vistos = new Set([inicio]);','  while (fila.length) {','    const atual = fila.shift();','    for (const vizinho of grafo[atual]) {','      if (!vistos.has(vizinho)) {','        vistos.add(vizinho); fila.push(vizinho);','      }','    }','  }','  return [...vistos];','}']},
    dfs:{name:'Busca em profundidade (DFS)',kind:'graph',help:'Segue cada ramo até o fim e então volta. Usa pilha; O(V + E). Grafo didático: A→B,C; B→D,E; C→F; E→G.',code:['function dfs(grafo, inicio) {','  const pilha = [inicio], vistos = new Set();','  while (pilha.length) {','    const atual = pilha.pop();','    if (vistos.has(atual)) continue;','    vistos.add(atual);','    for (const vizinho of [...grafo[atual]].reverse()) {','      if (!vistos.has(vizinho)) pilha.push(vizinho);','    }','  }','  return [...vistos];','}']}
  };
  function run(key,input,target){
    if(!definitions[key])throw Error('Algoritmo desconhecido.');
    const def=definitions[key],a=[...input],steps=[];
    const add=(message,line,active=[],sorted=[],extra={})=>steps.push({values:[...a],message,line,active:[...active],sorted:[...sorted],...extra});
    if(def.kind!=='graph'&&(!a.length||a.length>16||a.some(v=>!Number.isFinite(v))))throw Error('Informe de 1 a 16 números válidos, separados por vírgula.');
    if(def.kind==='search'&&!Number.isFinite(target))throw Error('Informe um número a buscar.');
    if(key==='bubble'){
      add('Lista inicial.',1);
      for(let fim=a.length-1;fim>0;fim--){for(let i=0;i<fim;i++){add(`Comparar ${a[i]} e ${a[i+1]}.`,4,[i,i+1]);if(a[i]>a[i+1]){[a[i],a[i+1]]=[a[i+1],a[i]];add('Troca realizada.',5,[i,i+1])}}add(`${a[fim]} fixado na posição ${fim}.`,2,[],Array.from({length:a.length-fim},(_,i)=>fim+i))}
    }else if(key==='insertion'){
      add('O primeiro valor forma um prefixo ordenado.',1,[],[0]);
      for(let i=1;i<a.length;i++){const value=a[i];let j=i-1;add(`Inserir ${value} no prefixo ordenado.`,3,[i],Array.from({length:i},(_,k)=>k));while(j>=0&&a[j]>value){a[j+1]=a[j];add(`Deslocar ${a[j]} para a direita.`,5,[j,j+1]);j--}a[j+1]=value;add(`${value} inserido na posição ${j+1}.`,7,[j+1],Array.from({length:i+1},(_,k)=>k))}
    }else if(key==='selection'){
      add('Lista inicial.',1);
      for(let i=0;i<a.length;i++){let min=i;for(let j=i+1;j<a.length;j++){add(`Comparar ${a[j]} com o menor atual ${a[min]}.`,5,[j,min],Array.from({length:i},(_,k)=>k));if(a[j]<a[min])min=j}if(min!==i)[a[i],a[min]]=[a[min],a[i]];add(`Menor valor colocado na posição ${i}.`,7,[i],Array.from({length:i+1},(_,k)=>k))}
    }else if(key==='merge'){
      add('Dividir e ordenar as metades.',1);
      function merge(lo,hi){if(hi-lo<=1)return;const mid=Math.floor((lo+hi)/2);add(`Dividir [${lo}, ${hi}) no meio ${mid}.`,3,Array.from({length:hi-lo},(_,k)=>lo+k));merge(lo,mid);merge(mid,hi);const left=a.slice(lo,mid),right=a.slice(mid,hi),out=[];let i=0,j=0;while(i<left.length||j<right.length){if(j>=right.length||i<left.length&&left[i]<=right[j])out.push(left[i++]);else out.push(right[j++]);a[lo+out.length-1]=out[out.length-1];add('Intercalar: próximo menor valor entra no trecho.',6,[lo+out.length-1])}a.splice(lo,hi-lo,...out);add(`Trecho [${lo}, ${hi}) ordenado.`,7,Array.from({length:hi-lo},(_,k)=>lo+k))}merge(0,a.length);
    }else if(key==='quick'){
      add('Lista inicial.',1);
      function quick(lo,hi){if(lo>=hi)return;const pivot=a[hi];let p=lo;add(`Pivô ${pivot} na posição ${hi}.`,3,[hi]);for(let i=lo;i<hi;i++){add(`Comparar ${a[i]} ao pivô ${pivot}.`,5,[i,hi]);if(a[i]<=pivot){[a[i],a[p]]=[a[p],a[i]];add('Mover valor para o lado menor ou igual.',6,[i,p]);p++}}[a[p],a[hi]]=[a[hi],a[p]];add(`Pivô ${pivot} fixado na posição ${p}.`,8,[p],[p]);quick(lo,p-1);quick(p+1,hi)}quick(0,a.length-1);
    }else if(key==='linear'){
      add(`Buscar ${target} da esquerda para a direita.`,1);
      let found=-1;for(let i=0;i<a.length;i++){add(`Comparar posição ${i}: ${a[i]} com ${target}.`,3,[i]);if(a[i]===target){found=i;add(`Encontrado na posição ${i}.`,3,[i],[i],{found:i});break}}if(found<0)add(`${target} não encontrado.`,5,[],[],{found:-1});
    }else if(key==='binary'){
      a.sort((x,y)=>x-y);add('Cópia ordenada: condição da busca binária.',1);
      let lo=0,hi=a.length-1,found=-1;while(lo<=hi){const mid=Math.floor((lo+hi)/2);add(`Intervalo [${lo}, ${hi}]. Comparar meio ${a[mid]} com ${target}.`,4,[mid],[],{range:[lo,hi]});if(a[mid]===target){found=mid;add(`Encontrado na posição ${mid} da cópia ordenada.`,5,[mid],[mid],{found:mid,range:[lo,hi]});break}if(a[mid]<target){lo=mid+1;add('Descartar a metade esquerda.',6,[],[],{range:[lo,hi]})}else{hi=mid-1;add('Descartar a metade direita.',7,[],[],{range:[lo,hi]})}}if(found<0)add(`${target} não encontrado.`,9,[],[],{found:-1});
    }else{
      const pending=['A'],seen=new Set(key==='bfs'?['A']:[]),visited=[];
      add(`Começar em A. ${key==='bfs'?'Fila':'Pilha'}: A.`,2,[],[],{pending:[...pending],visited:[...visited],current:null});
      while(pending.length){
        const current=key==='bfs'?pending.shift():pending.pop();
        if(key==='dfs'&&seen.has(current))continue;
        if(key==='dfs')seen.add(current);
        visited.push(current);add(`Visitar ${current}.`,key==='bfs'?4:4,[],[],{pending:[...pending],visited:[...visited],current});
        const neighbors=key==='bfs'?graph[current]:[...graph[current]].reverse();
        for(const neighbor of neighbors){if(!seen.has(neighbor)){if(key==='bfs')seen.add(neighbor);pending.push(neighbor);add(`Adicionar ${neighbor} à ${key==='bfs'?'fila':'pilha'}.`,key==='bfs'?7:7,[],[],{pending:[...pending],visited:[...visited],current})}}
      }
      add(`Ordem final: ${visited.join(' → ')}.`,key==='bfs'?11:11,[],[],{pending:[],visited:[...visited],current:null});
    }
    if(def.kind==='sort')add('Lista ordenada: '+a.join(', ')+'.',def.code.length-2,[],a.map((_,i)=>i));
    return {definition:def,steps,final:[...a]};
  }
  return {definitions,run,graph};
})();
if(typeof module!=='undefined'&&module.exports)module.exports=AlgorithmEngine;
