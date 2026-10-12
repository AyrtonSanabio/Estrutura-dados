const CostEngine=(()=>{
  const examples={
    linear:{title:'Varredura linear',source:'op\nfor i = 1..n\n  op 2\nend'},
    quadratic:{title:'Dois laços completos',source:'for i = 1..n\n  for j = 1..n\n    op\n  end\nend'},
    triangular:{title:'Laço triangular',source:'for i = 1..n\n  for j = 1..i\n    op\n  end\nend'},
    mixed:{title:'Laços em sequência',source:'for i = 1..n\n  op\nend\nfor j = 1..n\n  for k = 1..n\n    op\n  end\nend'},
    logarithmic:{title:'Duplicação logarítmica',source:'while x < n doubling\n  op\nend'},
    graph:{title:'Percurso de grafo: V + E',source:'for i = 1..V\n  op\nend\nfor j = 1..E\n  op\nend'}
  };
  const key=exp=>Object.entries(exp).filter(([,power])=>power).sort(([a],[b])=>a.localeCompare(b)).map(([v,p])=>`${v}:${p}`).join('|')||'1';
  const decode=value=>value==='1'?{}:Object.fromEntries(value.split('|').map(part=>{const [v,p]=part.split(':');return [v,Number(p)]}));
  const constant=value=>new Map([['1',value]]);
  function addTerm(poly,coef,exp){const k=key(exp),next=(poly.get(k)||0)+coef;if(Math.abs(next)<1e-10)poly.delete(k);else poly.set(k,next)}
  function add(a,b){const out=new Map(a);for(const [k,c] of b)addTerm(out,c,decode(k));return out}
  function multiply(a,b){const out=new Map;for(const [ka,ca] of a)for(const [kb,cb] of b){const exp=decode(ka);for(const [v,p] of Object.entries(decode(kb)))exp[v]=(exp[v]||0)+p;addTerm(out,ca*cb,exp)}return out}
  function powerSum(power,upper){const term=(coef,p)=>[key({[upper]:p}),coef];switch(power){
    case 0:return new Map([term(1,1)]);
    case 1:return new Map([term(.5,2),term(.5,1)]);
    case 2:return new Map([term(1/3,3),term(.5,2),term(1/6,1)]);
    case 3:return new Map([term(.25,4),term(.5,3),term(.25,2)]);
    default:throw Error('Profundidade de laços excede o limite didático (até 3).');
  }}
  function sumOver(poly,variable,upper){const out=new Map;for(const [k,coef] of poly){const exp=decode(k),power=exp[variable]||0;delete exp[variable];const product=multiply(new Map([[key(exp),coef]]),powerSum(power,upper));for(const [pk,pc] of product)addTerm(out,pc,decode(pk))}return out}
  function formatNumber(value){for(let denominator=1;denominator<=120;denominator++){const numerator=Math.round(value*denominator);if(Math.abs(value-numerator/denominator)<1e-9)return denominator===1?String(numerator):`${numerator}/${denominator}`}return value.toFixed(4)}
  function formatPoly(poly){const terms=[...poly].map(([k,coef])=>({exp:decode(k),coef})).sort((a,b)=>Object.values(b.exp).reduce((n,p)=>n+p,0)-Object.values(a.exp).reduce((n,p)=>n+p,0));if(!terms.length)return '0';return terms.map(({exp,coef},i)=>{const sign=coef<0?(i?' − ':'−'):(i?' + ':'');const abs=Math.abs(coef),variable=Object.entries(exp).filter(([,p])=>p).sort(([a],[b])=>a.localeCompare(b)).map(([v,p])=>p===1?v:`${v}^${p}`).join('·');const number=variable&&abs===1?'':formatNumber(abs);return sign+number+(number&&variable?'·':'')+variable}).join('')}
  function formatTotal(poly,logCoeff){const polynomial=formatPoly(poly);if(!logCoeff)return polynomial;const logarithm=`${logCoeff===1?'':formatNumber(logCoeff)+'·'}⌈log₂ n⌉`;return polynomial==='0'?logarithm:`${polynomial} + ${logarithm}`}
  function evaluate(poly,logCoeff,input){const values=typeof input==='number'?{n:input,V:input,E:input}:input;let value=0;for(const [k,coef] of poly){const exp=decode(k);let term=coef;for(const [name,power] of Object.entries(exp))term*=values[name]**power;value+=term}return Math.round(value+(logCoeff?logCoeff*Math.ceil(Math.log2(values.n)):0))}
  function asymptoticClass(poly,logCoeff){const monomials=[...poly].filter(([,coef])=>Math.abs(coef)>1e-10).map(([k])=>decode(k));const maximal=monomials.filter((candidate,i)=>!monomials.some((other,j)=>i!==j&&Object.keys({...candidate,...other}).every(name=>(other[name]||0)>=(candidate[name]||0))&&Object.keys({...candidate,...other}).some(name=>(other[name]||0)>(candidate[name]||0))));const parts=maximal.map(exp=>Object.entries(exp).filter(([,power])=>power).sort(([a],[b])=>a.localeCompare(b)).map(([name,power])=>power===1?name:`${name}^${power}`).join('·')||'1');if(logCoeff>0&&!maximal.some(exp=>(exp.n||0)>0))parts.push('log n');return parts.filter(part=>part!=='1'||parts.length===1).join(' + ')||'1'}
  function analyze(source){if(typeof source!=='string'||source.length>6000)throw Error('Digite até 6.000 caracteres no pseudocódigo.');const lines=source.replace(/\r/g,'').split('\n');if(lines.length>120)throw Error('Use no máximo 120 linhas.');const stack=[],operations=[];let polynomial=new Map,logCoeff=0;
    lines.forEach((raw,index)=>{const line=raw.split('#',1)[0].trim();if(!line)return;const number=index+1;
      const forMatch=/^for\s+([a-z])\s*=\s*1\s*\.\.\s*(n|V|E|[a-z])$/.exec(line);
      if(forMatch){const variable=forMatch[1],upper=forMatch[2];if(variable==='n')throw Error(`Linha ${number}: n é o tamanho da entrada, não um contador.`);if(stack.some(scope=>scope.type==='log'))throw Error(`Linha ${number}: não aninhe for dentro do laço logarítmico.`);if(stack.length>=3)throw Error(`Linha ${number}: use no máximo três laços for aninhados.`);if(stack.some(scope=>scope.variable===variable))throw Error(`Linha ${number}: o contador ${variable} já está ativo.`);if(!['n','V','E'].includes(upper)&&!stack.some(scope=>scope.variable===upper))throw Error(`Linha ${number}: o limite ${upper} deve ser n, V, E ou um contador externo.`);stack.push({type:'for',variable,upper,line:number,hasOp:false});return}
      if(/^while\s+x\s*<\s*n\s+doubling$/i.test(line)){if(stack.length)throw Error(`Linha ${number}: o laço logarítmico deve estar no nível externo.`);stack.push({type:'log',line:number,hasOp:false});return}
      if(/^end$/i.test(line)){if(!stack.length)throw Error(`Linha ${number}: end sem laço aberto.`);const closed=stack.pop();if(!closed.hasOp)throw Error(`Linha ${number}: laço da linha ${closed.line} não contém op.`);return}
      const opMatch=/^op(?:\s+([1-9]\d{0,2}))?$/i.exec(line);
      if(opMatch){const cost=opMatch[1]?Number(opMatch[1]):1;for(const scope of stack)scope.hasOp=true;let contribution=constant(cost),log=0,notation='';const reductions=[];
        if(stack.some(scope=>scope.type==='log')){log=cost;contribution=new Map;notation=`${cost===1?'':cost+'·'}⌈log₂ n⌉`;reductions.push({title:'Contar as duplicações',text:`x começa em 1 e dobra até alcançar n. O corpo executa ⌈log₂ n⌉ vezes; esta linha contribui ${notation}.`})}
        else{for(const scope of [...stack].reverse()){const before=formatPoly(contribution),after=sumOver(contribution,scope.variable,scope.upper);reductions.push({title:`Somar o laço ${scope.variable}`,text:`Σ(${scope.variable}=1…${scope.upper}) ${before} = ${formatPoly(after)}.${before==='1'?` O corpo executa ${scope.upper} vez(es) nesse laço.`:''}`});contribution=after}notation=stack.length?stack.map(scope=>`Σ(${scope.variable}=1…${scope.upper})`).join(' ')+` ${cost}`:String(cost)}
        polynomial=add(polynomial,contribution);logCoeff+=log;operations.push({line:number,source:line,scopes:stack.map(scope=>({...scope})),reductions,notation,formula:formatTotal(contribution,log),cumulative:formatTotal(polynomial,logCoeff),at:n=>evaluate(contribution,log,n)});return}
      throw Error(`Linha ${number}: comando não reconhecido. Use op, op 2, for i = 1..n, for i = 1..V, for j = 1..E, for j = 1..i, while x < n doubling ou end.`)
    });
    if(stack.length)throw Error(`Falta end para o laço iniciado na linha ${stack.at(-1).line}.`);if(!operations.length)throw Error('Inclua pelo menos uma linha op.');
    const asymptotic=asymptoticClass(polynomial,logCoeff);
    const steps=[{kind:'model',title:'Modelo RAM simplificado',text:'Cada op conta uma operação elementar; op 2 conta duas. Contamos execuções do corpo e ignoramos o controle dos laços. n, V e E representam tamanhos de entrada positivos.'}];
    operations.forEach((op,i)=>{op.reductions.forEach(reduction=>steps.push({kind:'reduction',line:op.line,opIndex:i,title:reduction.title,text:reduction.text}));steps.push({kind:'operation',line:op.line,opIndex:i,title:`Contribuição da linha ${op.line}: ${op.source}`,text:`${op.notation} = ${op.formula}`})});
    steps.push({kind:'total',title:'Soma das contribuições',text:`T(n) = ${formatTotal(polynomial,logCoeff)}`},{kind:'complexity',title:'Classe assintótica',text:`O(${asymptotic}), Ω(${asymptotic}) e Θ(${asymptotic}). O termo dominante determina a classe.`});
    const variables=[...new Set([...polynomial.keys()].flatMap(k=>Object.keys(decode(k))).concat(logCoeff?['n']:[]))].sort((a,b)=>['n','V','E'].indexOf(a)-['n','V','E'].indexOf(b));
    return {operations,steps,variables,formula:formatTotal(polynomial,logCoeff),theta:`Θ(${asymptotic})`,bigO:`O(${asymptotic})`,omega:`Ω(${asymptotic})`,at:input=>{const values=typeof input==='number'?{n:input,V:input,E:input}:input;for(const name of variables)if(!Number.isInteger(values?.[name])||values[name]<1||values[name]>1000)throw Error(`Escolha ${name} inteiro entre 1 e 1.000.`);return evaluate(polynomial,logCoeff,values)}};
  }
  return {examples,analyze};
})();
