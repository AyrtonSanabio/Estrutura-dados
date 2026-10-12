const test=require('node:test');
const assert=require('node:assert/strict');
const fs=require('node:fs');
const vm=require('node:vm');
const visuals=vm.runInNewContext(fs.readFileSync(require.resolve('./concept_visuals.js'),'utf8')+'\nConceptVisuals');

test('os 25 conceitos têm um diagrama legível e uma explicação visual',()=>{
  assert.equal(visuals.count,25);
  for(let index=0;index<visuals.count;index++){
    const scene=visuals.render(index);
    assert.ok(scene.caption.length>35,`legenda ${index}`);
    assert.match(scene.svg,/^<svg /);
    assert.match(scene.svg,/<line /);
    assert.match(scene.svg,/<circle /);
    assert.match(scene.svg,/<\/svg>$/);
  }
});

test('conceitos diferentes mostram ligações e destaques diferentes',()=>{
  assert.notEqual(visuals.render(0).svg,visuals.render(11).svg);
  assert.notEqual(visuals.render(11).svg,visuals.render(24).svg);
  assert.equal(visuals.render(-1),null);
});
