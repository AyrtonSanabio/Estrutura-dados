const test=require('node:test');
const assert=require('node:assert/strict');
const fs=require('node:fs');
const vm=require('node:vm');
const path=require('node:path');
const context=vm.createContext({});
vm.runInContext(fs.readFileSync(path.join(__dirname,'algorithm_engine.js'),'utf8')+'\nthis.engine=AlgorithmEngine;',context);
const engine=context.engine;

test('cinco algoritmos de ordenação produzem a mesma lista ordenada',()=>{
  for(const key of ['bubble','insertion','selection','merge','quick']){
    const result=engine.run(key,[7,3,9,2,6,1],6);
    assert.equal(JSON.stringify(result.final),'[1,2,3,6,7,9]',key);
    assert.ok(result.steps.length>3,key);
    assert.match(result.steps.at(-1).message,/Lista ordenada/,key);
  }
});

test('ordenações lidam com repetidos, negativos e listas curtas',()=>{
  const cases=[[2],[3,3,1,0,-2],[5,4,3,2,1],[-1,8,-1,8,0]];
  for(const values of cases)for(const key of ['bubble','insertion','selection','merge','quick']){
    const expected=[...values].sort((a,b)=>a-b);
    assert.equal(JSON.stringify(engine.run(key,values,0).final),JSON.stringify(expected),key+' '+values);
  }
});

test('buscas encontram e rejeitam alvos',()=>{
  const linear=engine.run('linear',[7,3,9,2,6,1],6);
  const binary=engine.run('binary',[7,3,9,2,6,1],6);
  assert.equal(linear.steps.at(-1).found,4);
  assert.equal(binary.steps.at(-1).found,3);
  assert.equal(engine.run('binary',[7,3,9,2,6,1],100).steps.at(-1).found,-1);
});

test('BFS e DFS visitam todos os vértices e mostram estados distintos',()=>{
  const bfs=engine.run('bfs',[],0),dfs=engine.run('dfs',[],0);
  assert.equal(bfs.steps.at(-1).visited.join(''),'ABCDEFG');
  assert.equal(dfs.steps.at(-1).visited.join(''),'ABDEGCF');
  assert.notEqual(bfs.steps.at(-1).visited.join(''),dfs.steps.at(-1).visited.join(''));
});
