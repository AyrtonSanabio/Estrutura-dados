(()=>{
  const $=id=>document.getElementById(id);
  const select=$('costExample');
  Object.entries(CostEngine.examples).forEach(([key,example])=>{const option=document.createElement('option');option.value=key;option.textContent=example.title;select.append(option)});
  const cExamples={
    linear:'int contar(int n, int *a) {\n  int soma = 0;\n  for (int i = 0; i < n; i++) {\n    soma += a[i];\n  }\n  return soma;\n}',
    quadratic:'int comparar(int n) {\n  int total = 0;\n  for (int i = 0; i < n; i++) {\n    for (int j = 0; j < n; j++) {\n      total++;\n    }\n  }\n  return total;\n}',
    triangular:'int triangular(int n) {\n  int total = 0;\n  for (int i = 1; i <= n; i++) {\n    for (int j = 1; j <= i; j++) {\n      total++;\n    }\n  }\n  return total;\n}',
    logarithmic:'int duplicar(int n) {\n  int total = 0;\n  for (int i = 1; i < n; i *= 2) {\n    total++;\n  }\n  return total;\n}',
    graph:'int percorrer(int V, int E) {\n  int visitas = 0;\n  for (int i = 0; i < V; i++) {\n    visitas++;\n  }\n  for (int j = 0; j < E; j++) {\n    visitas++;\n  }\n  return visitas;\n}'
  };
  let result=null,position=0,cMap=null;
  function error(message){$('costError').textContent=message;$('costError').hidden=false;$('costResults').hidden=true}
  function inputs(){return {n:Number($('costN').value),V:Number($('costV').value),E:Number($('costE').value)}}
  function draw(){if(!result)return;const values=inputs();let count;try{count=result.at(values)}catch(ex){error(ex.message);return}$('costError').hidden=true;$('costResults').hidden=false;
    const settings=result.variables.map(name=>`${name}=${values[name]}`).join(', ')||'entrada constante';
    const step=result.steps[position];$('costStepTitle').textContent=cMap&&step.line?`Linha C ${cMap[step.line-1]} · ${step.title}`:step.title;$('costStepText').textContent=step.text;$('costPosition').textContent=`Etapa ${position+1} de ${result.steps.length} · ${settings}: ${count} operações do corpo.`;
    const complete=step.kind==='model'?-1:step.kind==='reduction'?step.opIndex-1:step.kind==='operation'?step.opIndex:result.operations.length-1;
    if(step.kind==='reduction')$('costPartial').textContent='Somatório em andamento; esta linha entra no total após fechar os laços.';
    else if(complete<0)$('costPartial').textContent='Comece pela primeira operação para montar o custo total.';else{const done=result.operations.slice(0,complete+1);const partial=done.reduce((sum,op)=>sum+op.at(values),0);$('costPartial').textContent=`Até aqui: T parcial = ${done.at(-1).cumulative}. Para ${settings}: ${partial} operação(ões).`}
    $('costBars').replaceChildren();const counts=result.operations.map(op=>op.at(values)),max=Math.max(1,...counts);
    result.operations.forEach((op,i)=>{const revealed=i<=complete,column=document.createElement('div');column.className=`cost-bar-column${i===step.opIndex&&step.kind==='operation'?' current':revealed?' completed':''}`;const value=document.createElement('strong');value.textContent=revealed?String(counts[i]):'?';const bar=document.createElement('span');bar.style.height=revealed?`${Math.max(7,Math.round(counts[i]/max*125))}px`:'7px';const label=document.createElement('small');const line=cMap?cMap[op.line-1]:op.line;label.textContent=`L${line}`;label.title=`Linha ${line}: ${op.formula}`;column.append(value,bar,label);$('costBars').append(column)});
    $('costCode').replaceChildren();const original=cMap?$('cCode').value:$('costSource').value;const activeLine=cMap&&step.line?cMap[step.line-1]:step.line;original.replace(/\r/g,'').split('\n').forEach((line,i)=>{const row=document.createElement('span');row.className='code-line'+(activeLine===i+1?' highlighted':'');const number=document.createElement('span');number.className='line-number';number.textContent=String(i+1);row.append(number,document.createTextNode(line||' '));$('costCode').append(row)});
    $('costFirst').disabled=$('costPrev').disabled=position===0;$('costNext').disabled=position===result.steps.length-1;
  }
  function analyze(){try{result=CostEngine.analyze($('costSource').value);result.at(inputs());position=0;for(const [name,id] of [['n','N'],['V','V'],['E','E']])$('cost'+id+'Wrap').hidden=!result.variables.includes(name);const argument=result.variables.join(', ')||'1';$('costFormula').textContent=`T(${argument}) = ${result.formula}`;$('costO').textContent=result.bigO;$('costOmega').textContent=result.omega;$('costTheta').textContent=result.theta;draw()}catch(ex){result=null;error(ex.message)}}
  select.onchange=()=>{cMap=null;$('costSource').value=CostEngine.examples[select.value].source;analyze()};
  $('costAnalyze').onclick=()=>{cMap=null;analyze()};
  $('cExample').onchange=()=>{$('cCode').value=cExamples[$('cExample').value]};
  $('cAnalyze').onclick=async()=>{const button=$('cAnalyze');button.disabled=true;$('cStatus').hidden=false;$('cStatus').textContent='Lendo a árvore sintática de C...';try{const response=await fetch('/api/c-cost',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({source:$('cCode').value,function:$('cFunction').value.trim()||undefined})});const data=await response.json();if(!response.ok)throw Error(data.error||'Não foi possível analisar o código C.');cMap=data.lineMap;$('costSource').value=data.pseudocode;analyze();if(!result)throw Error($('costError').textContent);$('cStatus').className='c-status';$('cStatus').textContent=`Função ${data.function}: ${data.assumption} ${data.notes.join(' ')}`;$('costResults').scrollIntoView({behavior:'smooth',block:'start'})}catch(ex){cMap=null;error(ex.message);$('cStatus').className='cost-error';$('cStatus').textContent=ex instanceof SyntaxError?'Abra pelo servidor Python: python study.py serve --port 8765.':ex.message}finally{button.disabled=false}};
  for(const name of ['N','V','E'])$('cost'+name).onchange=()=>{if(result)draw()};
  $('costFirst').onclick=()=>{position=0;draw()};
  $('costPrev').onclick=()=>{position=Math.max(0,position-1);draw()};
  $('costNext').onclick=()=>{position=Math.min(result.steps.length-1,position+1);draw()};
  $('cCode').value=cExamples.linear;select.value='triangular';$('costSource').value=CostEngine.examples.triangular.source;analyze();$('costResults').hidden=true;
})();
