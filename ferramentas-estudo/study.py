"""Ferramentas externas ao trabalho academico. Python 3.10+, biblioteca padrao."""
import argparse
import json
import math
import shutil
import subprocess
from pathlib import Path
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HERE = Path(__file__).resolve().parent


def fail(message):
    raise ValueError(message)


def ident(value, label):
    if not isinstance(value, str) or not value or len(value) > 100:
        fail(f"{label}: identificador deve ser texto nao vazio de ate 100 caracteres")
    return value


def validate(data):
    if not isinstance(data, dict) or type(data.get('version')) is not int or data.get('version') != 1:
        fail('version deve ser 1')
    kind = data.get('kind')
    if kind == 'graph':
        if type(data.get('directed')) is not bool:
            fail('directed deve ser true ou false')
        vertices, edges = data.get('vertices'), data.get('edges')
        if not isinstance(vertices, list) or not isinstance(edges, list):
            fail('vertices e edges devem ser listas')
        ids, edgeids = set(), set()
        for vertex in vertices:
            if not isinstance(vertex, dict): fail('vertice invalido')
            vid = ident(vertex.get('id'), 'vertice')
            if vid in ids: fail(f'vertice duplicado: {vid}')
            ids.add(vid)
        for edge in edges:
            if not isinstance(edge, dict): fail('aresta invalida')
            eid = ident(edge.get('id'), 'aresta')
            if eid in edgeids: fail(f'aresta duplicada: {eid}')
            edgeids.add(eid)
            for key in ('from', 'to'):
                if ident(edge.get(key), key) not in ids: fail(f'aresta {eid}: extremo {key} inexistente')
            weight = edge.get('weight')
            if weight is not None and (type(weight) not in (int, float) or not math.isfinite(weight)):
                fail(f'aresta {eid}: peso deve ser numero finito')
        return data
    if kind == 'binary_tree':
        nodes = data.get('nodes')
        if not isinstance(nodes, list): fail('nodes deve ser lista')
        ids, parent = set(), {}
        for node in nodes:
            if not isinstance(node, dict): fail('no invalido')
            nid = ident(node.get('id'), 'no')
            if nid in ids: fail(f'no duplicado: {nid}')
            ids.add(nid)
        root = data.get('root')
        if root is not None and root not in ids: fail('raiz inexistente')
        if bool(ids) != bool(root): fail('arvore vazia requer root null; nao vazia requer raiz')
        for node in nodes:
            for side in ('left', 'right'):
                child = node.get(side)
                if child is None: continue
                if child not in ids: fail(f'filho {child} inexistente')
                if child in parent: fail(f'no {child} tem mais de um pai')
                parent[child] = node['id']
        if root in parent: fail('raiz possui pai')
        if len(parent) != max(0, len(ids)-1): fail('arvore desconexa ou ciclica')
        seen, stack = set(), [root] if root else []
        lookup = {n['id']: n for n in nodes}
        while stack:
            nid = stack.pop()
            if nid in seen: fail('ciclo na arvore')
            seen.add(nid)
            stack += [lookup[nid][side] for side in ('left','right') if lookup[nid].get(side)]
        if seen != ids: fail('arvore desconexa')
        return data
    fail('kind deve ser graph ou binary_tree')


def q(value):
    return json.dumps(str(value), ensure_ascii=False)


def dot_source(data, selected_vertices=(), selected_edges=()):
    validate(data)
    sv, se = set(selected_vertices), set(selected_edges)
    if data['kind'] == 'graph':
        directed = data['directed']
        lines = ['digraph G {' if directed else 'graph G {', 'graph [bgcolor="transparent", overlap=false];', 'node [shape=circle, style=filled, fillcolor="#e9f2ff", color="#4875a9", fontname="Arial"];', 'edge [color="#75869a", fontname="Arial"];']
        for vertex in data['vertices']:
            vid = vertex['id']; attrs = [f'label={q(vertex.get("label",vid))}']
            if vid in sv: attrs += ['fillcolor="#ffdc7a"', 'color="#b36b00"', 'penwidth=2.5']
            lines.append(f'{q(vid)} [{", ".join(attrs)}];')
        connector = '->' if directed else '--'
        for edge in data['edges']:
            attrs = [f'id={q("edge-"+edge["id"])}']
            labels = [edge['id']]
            if edge.get('weight') is not None: labels.append(f'peso {edge["weight"]}')
            if edge.get('label'): labels.append(str(edge['label']))
            attrs.append(f'label={q(" · ".join(labels))}')
            if edge['id'] in se: attrs += ['color="#d65a00"', 'fontcolor="#d65a00"', 'penwidth=3']
            lines.append(f'{q(edge["from"])} {connector} {q(edge["to"])} [{", ".join(attrs)}];')
        return '\n'.join(lines + ['}']) + '\n'
    lines = ['digraph T {', 'graph [bgcolor="transparent", ordering=out];', 'node [shape=circle, style=filled, fillcolor="#e9f2ff", fontname="Arial"];', 'edge [fontname="Arial"];']
    for node in data['nodes']:
        nid = node['id']
        attrs = [f'label={q(node.get("label",nid))}']
        if nid == data['root']: attrs += ['fillcolor="#a9e9c7"', 'xlabel="raiz"']
        if nid in sv: attrs += ['color="#d65a00"', 'penwidth=3']
        lines.append(f'{q(nid)} [{", ".join(attrs)}];')
        for side, label, color in (('left','E','#3273b9'),('right','D','#9b55a5')):
            if node.get(side): lines.append(f'{q(nid)} -> {q(node[side])} [label={q(label)}, color={q(color)}];')
    return '\n'.join(lines + ['}']) + '\n'


def render(data, engine=None, selected_vertices=(), selected_edges=()):
    dot = dot_source(data, selected_vertices, selected_edges)
    engine = engine or shutil.which('dot')
    if engine:
        command = [engine, '-Tsvg']
    elif (HERE/'node_modules'/'@hpcc-js'/'wasm').exists() and shutil.which('node'):
        command = ['node',str(HERE/'render_wasm.mjs')]
    else:
        fail('Graphviz nao encontrado. Instale dot.exe no PATH ou execute npm install nesta pasta.')
    run = subprocess.run(command, input=dot, text=True, capture_output=True, encoding='utf-8', timeout=15)
    if run.returncode: fail('Graphviz: ' + run.stderr.strip())
    return dot, run.stdout


def check_trace(trace, graph):
    validate(graph)
    if graph['kind'] != 'graph' or not isinstance(trace, dict) or type(trace.get('version')) is not int or trace.get('version') != 1 or trace.get('kind') != 'trace': fail('rastro v1 invalido')
    events = trace.get('events')
    if not isinstance(events, list) or not events: fail('rastro sem eventos')
    vertices = {v['id'] for v in graph['vertices']}; edges = {e['id'] for e in graph['edges']}
    for i,event in enumerate(events):
        if not isinstance(event,dict): fail(f'evento {i} invalido')
        for key in ('queue','stack','visited','highlight_vertices','highlight_edges'):
            values = event.get(key,[])
            if not isinstance(values,list): fail(f'evento {i}: {key} deve ser lista')
            allowed = edges if key == 'highlight_edges' else vertices
            if any(v not in allowed for v in values): fail(f'evento {i}: {key} contem id inexistente')
        if event.get('current') is not None and event['current'] not in vertices: fail(f'evento {i}: vertice atual inexistente')
    return trace


def verify(graph, answer):
    validate(graph)
    if graph['kind'] != 'graph': fail('verificador requer grafo')
    if not isinstance(answer,dict) or type(answer.get('version')) is not int or answer.get('version') != 1: fail('resultado versionado invalido')
    vertices = {v['id'] for v in graph['vertices']}; edges = {e['id']:e for e in graph['edges']}
    kind = answer.get('kind'); sequence = answer.get('vertices'); chosen = answer.get('edges')
    if kind == 'path':
        if not isinstance(sequence,list) or not sequence: return False,'Caminho precisa de ao menos um vertice.'
        for i,v in enumerate(sequence):
            if v not in vertices: return False,f'Posicao {i}: vertice {v} nao existe.'
            if v in sequence[:i]: return False,f'Posicao {i}: vertice {v} repetido; caminho simples nao repete vertices.'
        if chosen is not None and (not isinstance(chosen,list) or len(chosen) != len(sequence)-1): return False,'Lista de arestas deve ter uma entrada por transicao.'
        for i,(a,b) in enumerate(zip(sequence,sequence[1:])):
            candidates = [e for e in edges.values() if e['from']==a and e['to']==b or not graph['directed'] and e['from']==b and e['to']==a]
            if not candidates: return False,f'Passo {i+1}: falta aresta de {a} para {b}.'
            if chosen is not None and chosen[i] not in {e['id'] for e in candidates}: return False,f'Passo {i+1}: aresta {chosen[i]} nao liga {a} a {b} na direcao exigida.'
        return True,'Caminho estruturalmente valido; custo minimo nao foi verificado.'
    if kind == 'search':
        if not isinstance(sequence,list): return False,'Busca requer lista vertices.'
        seen=set()
        for i,v in enumerate(sequence):
            if v not in vertices: return False,f'Posicao {i}: vertice {v} nao existe.'
            if v in seen: return False,f'Posicao {i}: vertice {v} repetido.'
            seen.add(v)
        start=answer.get('start')
        if start not in vertices: return False,'Vertice inicial inexistente.'
        if not sequence or sequence[0] != start: return False,'A busca deve comecar pelo vertice inicial.'
        reachable={start}; changed=True
        while changed:
            old=len(reachable)
            for e in edges.values():
                if e['from'] in reachable: reachable.add(e['to'])
                if not graph['directed'] and e['to'] in reachable: reachable.add(e['from'])
            changed=len(reachable)!=old
        for v in sequence:
            if v not in reachable: return False,f'Vertice {v} nao e alcancavel a partir de {start}.'
        if answer.get('complete') and seen != reachable: return False,f'Busca incompleta: falta vertice alcancavel {sorted(reachable-seen)[0]}.'
        return True,'Visitas distintas e alcancaveis; ordem especifica de BFS/DFS nao foi provada.'
    if kind == 'spanning_tree':
        if graph['directed']: return False,'Arvore geradora aqui requer grafo nao dirigido.'
        if not isinstance(chosen,list): return False,'Informe ids das arestas da arvore.'
        parent={v:v for v in vertices}
        def root(v):
            while parent[v]!=v: v=parent[v]
            return v
        for eid in chosen:
            if eid not in edges: return False,f'Aresta {eid} nao existe no grafo.'
            e=edges[eid]; a,b=root(e['from']),root(e['to'])
            if a==b: return False,f'Aresta {eid} fecha um ciclo (ou e laco).'
            parent[a]=b
        if len(chosen)!=max(0,len(vertices)-1): return False,f'Esperadas {max(0,len(vertices)-1)} arestas; recebidas {len(chosen)}.'
        if len({root(v) for v in vertices})>1: return False,'Arestas escolhidas nao conectam todos os vertices.'
        return True,'Arvore geradora estruturalmente valida; peso minimo nao foi verificado.'
    return False,'kind deve ser path, search ou spanning_tree.'


class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        files = {'/': HERE/'index.html', '/index.html': HERE/'index.html',
                 '/style.css': HERE/'style.css', '/app.js': HERE/'app.js',
                 '/fallback.js': HERE/'fallback.js',
                 '/learning.js': HERE/'learning.js',
                 '/concept_visuals.js': HERE/'concept_visuals.js',
                 '/algorithm_engine.js': HERE/'algorithm_engine.js',
                 '/algorithms_ui.js': HERE/'algorithms_ui.js',
                 '/exam_data.js': HERE/'exam_data.js',
                 '/exam_extra.js': HERE/'exam_extra.js',
                 '/exam_prep.js': HERE/'exam_prep.js',
                 '/cost_engine.js': HERE/'cost_engine.js',
                 '/cost_ui.js': HERE/'cost_ui.js',
                 '/node_modules/@hpcc-js/wasm/dist/graphviz.js': HERE/'node_modules'/'@hpcc-js'/'wasm'/'dist'/'graphviz.js',
                 '/examples/grafo.json': HERE/'exemplos'/'grafo.json',
                 '/examples/arvore.json': HERE/'exemplos'/'arvore.json',
                 '/examples/rastro-bfs.json': HERE/'exemplos'/'rastro-bfs.json',
                 **{f'/exemplos/{path.name}': path for path in (HERE/'exemplos').glob('*.json')}}
        if self.path not in files:
            self.send_error(404); return
        body=files[self.path].read_bytes()
        mime=('application/json' if self.path.endswith('.json') else
              'text/css' if self.path.endswith('.css') else
              'text/javascript' if self.path.endswith('.js') else 'text/html')
        self.send_response(200); self.send_header('Content-Type',mime+'; charset=utf-8'); self.send_header('Content-Length',str(len(body))); self.end_headers(); self.wfile.write(body)
    def do_POST(self):
        try:
            size=int(self.headers.get('Content-Length','0'))
            if size>2_000_000: fail('arquivo muito grande')
            data=json.loads(self.rfile.read(size))
            if self.path=='/api/render':
                dot,svg=render(data['graph'],selected_vertices=data.get('vertices',[]),selected_edges=data.get('edges',[])); result={'dot':dot,'svg':svg}
            elif self.path=='/api/verify':
                ok,message=verify(data['graph'],data['answer']); result={'ok':ok,'message':message}
            elif self.path=='/api/c-cost':
                from c_cost import translate
                result=translate(data['source'], data.get('function'))
            else: self.send_error(404); return
            status=200
        except (ValueError,KeyError,TypeError,subprocess.TimeoutExpired,OSError) as exc:
            status=400; result={'error':str(exc)}
        body=json.dumps(result,ensure_ascii=False).encode()
        self.send_response(status); self.send_header('Content-Type','application/json; charset=utf-8'); self.send_header('Content-Length',str(len(body))); self.end_headers(); self.wfile.write(body)


def main():
    parser=argparse.ArgumentParser(description='Ferramentas externas de grafos e arvores')
    sub=parser.add_subparsers(dest='command',required=True)
    p=sub.add_parser('render'); p.add_argument('input'); p.add_argument('--output'); p.add_argument('--dot-exe')
    p=sub.add_parser('verify'); p.add_argument('graph'); p.add_argument('answer')
    p=sub.add_parser('serve'); p.add_argument('--port',type=int,default=8765)
    args=parser.parse_args()
    if args.command=='serve':
        server=ThreadingHTTPServer(('127.0.0.1',args.port),Handler)
        print(f'Abra http://127.0.0.1:{args.port}',flush=True)
        server.serve_forever(); return
    if args.command=='render':
        path=Path(args.input); data=json.loads(path.read_text(encoding='utf-8'))
        dot,svg=render(data,args.dot_exe)
        output=Path(args.output) if args.output else HERE.parent/'output'/'ferramentas-estudo'/(path.stem+'.svg')
        output.parent.mkdir(parents=True,exist_ok=True)
        output.write_text(svg,encoding='utf-8'); output.with_suffix('.dot').write_text(dot,encoding='utf-8')
        print(output)
    else:
        graph=json.loads(Path(args.graph).read_text(encoding='utf-8'))
        answer=json.loads(Path(args.answer).read_text(encoding='utf-8'))
        ok,message=verify(graph,answer); print(('ACEITO: ' if ok else 'REJEITADO: ')+message)
        raise SystemExit(0 if ok else 1)

if __name__=='__main__': main()
