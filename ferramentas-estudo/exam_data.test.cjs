const test=require('node:test');
const assert=require('node:assert/strict');
const fs=require('node:fs');
const vm=require('node:vm');
const data=vm.runInNewContext(fs.readFileSync(require.resolve('./exam_extra.js'),'utf8')+'\n'+fs.readFileSync(require.resolve('./exam_data.js'),'utf8')+'\nExamData');

test('roteiro cobre as oito áreas e cada item possui estudo e questão',()=>{
  assert.equal(data.modules.length,8);
  assert.equal(data.modules.reduce((n,m)=>n+m.questions.length,0),48);
  assert.equal(data.modules.reduce((n,m)=>n+data.allQuestions(m).length,0),144);
  for(const mod of data.modules){
    assert.ok(mod.intro.length>30);
    for(const q of mod.questions){
      assert.ok(q.name&&q.lesson&&q.prompt&&q.why);
      assert.equal(q.choices.length,3);
      assert.ok(q.answer>=0&&q.answer<3);
    }
    const all=data.allQuestions(mod);
    assert.equal(all.length,mod.questions.length*3);
    for(let i=0;i<mod.questions.length;i++)assert.equal(all.filter(q=>q.topicIndex===i).map(q=>q.level).join(','),'Básico,Intermediário,Avançado');
  }
});

test('prontidão exige estudo de todos os itens, respostas completas e 80%',()=>{
  const mod=data.modules[0],correct=data.allQuestions(mod).map(q=>q.answer),studied=mod.questions.map(()=>true);
  assert.equal(data.score(correct,studied,mod).ready,true);
  assert.equal(data.score(correct,[],mod).ready,false);
  assert.equal(data.score(correct.slice(0,-1),studied,mod).ready,false);
  const wrong=correct.map((answer,i)=>i<4?(answer+1)%3:answer);
  assert.equal(data.score(wrong,studied,mod).ready,false);
  const oneError=correct.map((answer,i)=>i===0?(answer+1)%3:answer);
  assert.equal(data.score(oneError,studied,mod).ready,true);
  assert.equal(data.score([null,...correct.slice(1)],studied,mod).done,correct.length-1);
});
