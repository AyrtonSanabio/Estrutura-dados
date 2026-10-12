(()=>{
  const $=id=>document.getElementById(id);
  const select=$('costExample');
  Object.entries(CostEngine.examples).forEach(([key,example])=>{const option=document.createElement('option');option.value=key;option.textContent=example.title;select.append(option)});
  let result=null,position=0;
  function error(message){$('costError').textContent=message;$('costError').hidden=false;$('costResults').hidden=true}
  function inputs(){return {n:Number($('costN').value),V:Number($('costV').value),E:Number($('costE').value)}}
  function draw(){if(!result)return;const values=inputs();let count;try{count=result.at(values)}catch(ex){error(ex.message);return}$('costError').hidden=true;$('costResults').hidden=false;
    const settings=result.variables.map(name=>`${name}=${values[name]}`).join(', ')||'entrada constante';
    const step=result.steps[position];$('costStepTitle').textContent=step.title;$('costStepText').textContent=step.text;$('costPosition').textContent=`Etapa ${position+1} de ${result.steps.length} · ${settings}: ${count} operações do corpo.`;
    const complete=step.kind==='model'?-1:step.kind==='reduction'?step.opIndex-1:step.kind==='operation'?step.opIndex:result.operations.length-1;
    if(step.kind==='reduction')$('costPartial').textContent='Somatório em andamento; esta linha entra no total após fechar os laços.';
    else if(complete<0)$('costPartial').textContent='Comece pela primeira operação para montar o custo total.';else{const done=result.operations.slice(0,complete+1);const partial=done.reduce((sum,op)=>sum+op.at(values),0);$('costPartial').textContent=`Até aqui: T parcial = ${done.at(-1).cumulative}. Para ${settings}: ${partial} operação(ões).`}
    $('costBars').replaceChildren();const counts=result.operations.map(op=>op.at(values)),max=Math.max(1,...counts);
    result.operations.forEach((op,i)=>{const revealed=i<=complete,column=document.createElement('div');column.className=`cost-bar-column${i===step.opIndex&&step.kind==='operation'?' current':revealed?' completed':''}`;const value=document.createElement('strong');value.textContent=revealed?String(counts[i]):'?';const bar=document.createElement('span');bar.style.height=revealed?`${Math.max(7,Math.round(counts[i]/max*125))}px`:'7px';const label=document.createElement('small');label.textContent=`L${op.line}`;label.title=`Linha ${op.line}: ${op.formula}`;column.append(value,bar,label);$('costBars').append(column)});
    $('costCode').replaceChildren();$('costSource').value.replace(/\r/g,'').split('\n').forEach((line,i)=>{const row=document.createElement('span');row.className='code-line'+(step.line===i+1?' highlighted':'');const number=document.createElement('span');number.className='line-number';number.textContent=String(i+1);row.append(number,document.createTextNode(line||' '));$('costCode').append(row)});
    $('costFirst').disabled=$('costPrev').disabled=position===0;$('costNext').disabled=position===result.steps.length-1;
  }
  function analyze(){try{result=CostEngine.analyze($('costSource').value);result.at(inputs());position=0;for(const [name,id] of [['n','N'],['V','V'],['E','E']])$('cost'+id+'Wrap').hidden=!result.variables.includes(name);const argument=result.variables.join(', ')||'1';$('costFormula').textContent=`T(${argument}) = ${result.formula}`;$('costO').textContent=result.bigO;$('costOmega').textContent=result.omega;$('costTheta').textContent=result.theta;draw()}catch(ex){result=null;error(ex.message)}}
  select.onchange=()=>{$('costSource').value=CostEngine.examples[select.value].source;analyze()};
  $('costAnalyze').onclick=analyze;
  for(const name of ['N','V','E'])$('cost'+name).onchange=()=>{if(result)draw()};
  $('costFirst').onclick=()=>{position=0;draw()};
  $('costPrev').onclick=()=>{position=Math.max(0,position-1);draw()};
  $('costNext').onclick=()=>{position=Math.min(result.steps.length-1,position+1);draw()};
  select.value='triangular';$('costSource').value=CostEngine.examples.triangular.source;analyze();
})();
