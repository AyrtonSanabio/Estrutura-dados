import { Graphviz } from '@hpcc-js/wasm';
let dot = '';
for await (const chunk of process.stdin) dot += chunk;
const graphviz = await Graphviz.load();
process.stdout.write(graphviz.layout(dot, 'svg', 'dot'));
