const test = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const vm = require('node:vm');

const here = __dirname;
const sample = name => JSON.parse(fs.readFileSync(path.join(here, 'exemplos', name), 'utf8'));
const context = vm.createContext({});
vm.runInContext(fs.readFileSync(path.join(here, 'fallback.js'), 'utf8') + '\nthis.study = LocalStudy;', context);
const study = context.study;

test('DOT para Graphviz preserva arestas e separa rótulo visual do peso', () => {
  const graph = study.dotSource(sample('grafo.json'));
  const tree = study.dotSource(sample('arvore.json'));
  assert.match(graph, /digraph G/);
  assert.match(graph, /rankdir="LR"/);
  assert.match(graph, /id="edge-a1", label="a1", tooltip="a1 · peso 4"/);
  assert.match(tree, /digraph T/);
  assert.match(tree, /ordering="out"/);
});

test('verificador local distingue resultados válidos e inválidos', () => {
  const graph = sample('grafo.json');
  for (const [name, expected] of [
    ['resultado-caminho-valido.json', true],
    ['resultado-caminho-invalido.json', false],
    ['resultado-busca-valida.json', true],
    ['resultado-busca-invalida.json', false],
  ]) assert.equal(study.verify({graph,answer:sample(name)}).ok,expected,name);
  const undirected = sample('grafo-nao-dirigido.json');
  assert.equal(study.verify({graph:undirected,answer:sample('resultado-arvore-geradora-valida.json')}).ok,true);
  assert.equal(study.verify({graph:undirected,answer:sample('resultado-arvore-geradora-invalida.json')}).ok,false);
});
