(()=>{
  const $=id=>document.getElementById(id);
  let run=null,index=0,timer=null;
  Object.entries(AlgorithmEngine.definitions).forEach(([key,definition])=>{
    const option=document.createElement('option');option.value=key;option.textContent=definition.name;
    $('algorithmSelect').append(option);
  });
  const positions={A:[85,170],B:[230,70],C:[230,270],D:[385,25],E:[385,125],F:[385,270],G:[545,125]};
  const links=Object.entries(AlgorithmEngine.graph).flatMap(([from,neighbors])=>neighbors.map(to=>[from,to]));
  function graphSvg(state){
    const visited=new Set(state.visited||[]),pending=new Set(state.pending||[]);
    return '<svg viewBox="0 0 630 335" role="img" aria-label="Grafo didático de busca">'+
      links.map(([from,to])=>{const [x1,y1]=positions[from],[x2,y2]=positions[to];return `<line x1="${x1}" y1="${y1}" x2="${x2}" y2="${y2}" stroke="#98acc5" stroke-width="3"/>`}).join('')+
      Object.entries(positions).map(([id,[x,y]])=>`<g><circle cx="${x}" cy="${y}" r="25" fill="${state.current===id?'#ffc867':visited.has(id)?'#40c995':pending.has(id)?'#92b7f1':'#e5eef9'}" stroke="#33608f" stroke-width="2"/><text x="${x}" y="${y+6}" text-anchor="middle" font-size="19" font-weight="700" fill="#183453">${id}</text></g>`).join('')+'</svg>';
  }
  function stop(){if(timer){clearInterval(timer);timer=null}$('algorithmPlay').textContent='▶ Reproduzir'}
  function show(){
    if(!run)return;
    const step=run.steps[index],definition=run.definition;
    const visual=$('algorithmViz');visual.replaceChildren();
    if(definition.kind==='graph'){
      visual.innerHTML=graphSvg(step);
      const state=document.createElement('p');state.className='queue-state';state.textContent=`Visitados: ${step.visited?.join(' → ')||'nenhum'} · ${$('algorithmSelect').value==='bfs'?'Fila':'Pilha'}: ${step.pending?.join(' → ')||'vazia'}`;visual.append(state);
    }else{
      const chart=document.createElement('div');chart.className='bar-chart';
      const max=Math.max(1,...step.values.map(Math.abs));
      step.values.forEach((value,i)=>{
        const item=document.createElement('div');item.className='bar-item';
        if(step.active.includes(i))item.classList.add('active');
        if(step.sorted.includes(i))item.classList.add('done');
        if(step.range&&!(i>=step.range[0]&&i<=step.range[1]))item.classList.add('discarded');
        const bar=document.createElement('div');bar.className='bar';bar.style.height=`${Math.max(12,Math.abs(value)/max*190)}px`;
        const label=document.createElement('strong');label.textContent=value;
        const number=document.createElement('small');number.textContent=i;
        item.append(bar,label,number);chart.append(item);
      });visual.append(chart);
    }
    $('algorithmStep').textContent=`Passo ${index+1} de ${run.steps.length}: ${step.message}`;
    $('algorithmCode').replaceChildren(...definition.code.map((line,i)=>{
      const row=document.createElement('span');row.className='code-line'+(i+1===step.line?' highlighted':'');
      const number=document.createElement('span');number.className='line-number';number.textContent=String(i+1).padStart(2,'0');
      row.append(number,document.createTextNode(line));return row;
    }));
    $('algorithmFirst').disabled=index===0;$('algorithmPrev').disabled=index===0;$('algorithmNext').disabled=index===run.steps.length-1;
  }
  function make(){
    stop();
    const key=$('algorithmSelect').value,definition=AlgorithmEngine.definitions[key];
    $('algorithmHelp').textContent=definition.help;
    const graph=definition.kind==='graph';
    $('algorithmInput').disabled=graph;
    $('algorithmTarget').disabled=definition.kind!=='search';
    try{
      const raw=$('algorithmInput').value.trim();
      const values=raw?raw.split(',').map(part=>Number(part.trim())):[];
      if(!graph&&(!raw||raw.split(',').some(part=>!part.trim())))throw Error('Digite números separados por vírgula, como 7, 3, 9, 2.');
      const targetRaw=$('algorithmTarget').value.trim();
      if(definition.kind==='search'&&!targetRaw)throw Error('Digite o valor que deseja buscar.');
      run=AlgorithmEngine.run(key,values,targetRaw?Number(targetRaw):0);
      index=0;show();
    }catch(error){run=null;$('algorithmViz').textContent='';$('algorithmCode').textContent='';$('algorithmStep').textContent=error.message}
  }
  $('algorithmRun').onclick=make;
  $('algorithmSelect').onchange=make;
  $('algorithmFirst').onclick=()=>{stop();index=0;show()};
  $('algorithmPrev').onclick=()=>{stop();index=Math.max(0,index-1);show()};
  $('algorithmNext').onclick=()=>{stop();index=Math.min(run.steps.length-1,index+1);show()};
  $('algorithmPlay').onclick=()=>{
    if(!run)return;
    if(timer){stop();return}
    if(index===run.steps.length-1)index=0;
    $('algorithmPlay').textContent='⏸ Pausar';
    timer=setInterval(()=>{if(index>=run.steps.length-1){stop();return}index++;show()},1000/Number($('algorithmSpeed').value));
  };
  $('algorithmSpeed').oninput=()=>{if(timer){stop();$('algorithmPlay').click()}};
  make();
})();
