const test=require('node:test');
const assert=require('node:assert/strict');
const fs=require('node:fs');
const vm=require('node:vm');
const engine=vm.runInNewContext(fs.readFileSync(require.resolve('./cost_engine.js'),'utf8')+'\nCostEngine');

test('exemplos mostram contagem e classe esperadas',()=>{
  const linear=engine.analyze(engine.examples.linear.source);
  const square=engine.analyze(engine.examples.quadratic.source);
  const triangle=engine.analyze(engine.examples.triangular.source);
  const logarithm=engine.analyze(engine.examples.logarithmic.source);
  assert.equal(linear.theta,'Θ(n)');
  assert.equal(square.theta,'Θ(n^2)');
  assert.equal(triangle.theta,'Θ(n^2)');
  assert.equal(logarithm.theta,'Θ(log n)');
  const graph=engine.analyze(engine.examples.graph.source);
  assert.equal(graph.theta,'Θ(V + E)');
  assert.equal(graph.at({V:5,E:8}),13);
  for(let n=1;n<=12;n++){
    assert.equal(linear.at(n),1+2*n);
    assert.equal(square.at(n),n*n);
    assert.equal(triangle.at(n),n*(n+1)/2);
    assert.equal(logarithm.at(n),Math.ceil(Math.log2(n)));
  }
});

test('somatórios aninhados, sequenciais e constantes mantêm custo exato',()=>{
  const cubic=engine.analyze('for i = 1..n\n for j = 1..i\n  for k = 1..j\n   op\n  end\n end\nend');
  const sequential=engine.analyze('op 3\nfor i = 1..n\n op 2\nend');
  assert.equal(cubic.theta,'Θ(n^3)');
  for(let n=1;n<=8;n++)assert.equal(cubic.at(n),n*(n+1)*(n+2)/6);
  assert.equal(sequential.at(7),17);
  assert.equal(engine.analyze('op 2').theta,'Θ(1)');
  const nestedGraph=engine.analyze('for i = 1..V\n for j = 1..E\n  op\n end\nend');
  assert.equal(nestedGraph.at({V:5,E:8}),40);
  assert.match(nestedGraph.theta,/V/);
  assert.match(nestedGraph.theta,/E/);
  assert.equal(sequential.operations[1].cumulative,sequential.formula);
  const combined=engine.analyze('for i = 1..n\n op 2\n for j = 1..i\n  op 3\n end\nend\nwhile x < n doubling\n op\nend');
  assert.equal(combined.theta,'Θ(n^2)');
  for(let n=1;n<=20;n++)assert.equal(combined.at(n),2*n+3*n*(n+1)/2+Math.ceil(Math.log2(n)));
});

test('rejeita sintaxe e hipóteses fora do modelo em vez de adivinhar',()=>{
  for(const source of ['for i = 1..n\n op','end','for j = 1..i\n op\nend','for i = 1..n\n end','int x = 0;','while x < n doubling\n for i = 1..n\n op\n end\nend']){
    assert.throws(()=>engine.analyze(source));
  }
  assert.throws(()=>engine.analyze(engine.examples.graph.source).at({V:0,E:8}));
});
