"""Traduz um subconjunto verificavel de C para o modelo didatico de custos."""
import re

try:
    from pycparser import c_ast, c_parser, plyparser
except ImportError:
    c_ast = c_parser = plyparser = None


class UnsupportedC(ValueError):
    pass


def reject(node, reason):
    line = getattr(getattr(node, 'coord', None), 'line', '?')
    raise UnsupportedC(f'Linha {line}: {reason}')


def remove_comments(source):
    def blank(match):
        return re.sub(r'[^\n]', ' ', match.group())
    return re.sub(r'/\*.*?\*/|//[^\n]*', blank, source, flags=re.S)


def constant(node, value):
    return isinstance(node, c_ast.Constant) and node.type == 'int' and node.value == str(value)


def identifier(node):
    return node.name if isinstance(node, c_ast.ID) else None


def has_call(node):
    if isinstance(node, c_ast.FuncCall):
        return True
    return any(has_call(child) for _, child in node.children())


def translate(source, function_name=None):
    if c_parser is None:
        raise UnsupportedC('Instale pycparser: python -m pip install -r requirements.txt')
    if not isinstance(source, str) or len(source) > 12000:
        raise UnsupportedC('Cole até 12.000 caracteres de código C.')
    cleaned = remove_comments(source)
    includes = re.findall(r'^[ \t]*#[ \t]*include\b[^\n]*', cleaned, re.M)
    cleaned = re.sub(r'^[ \t]*#[ \t]*include\b[^\n]*', lambda match: ' ' * len(match.group()), cleaned, flags=re.M)
    if re.search(r'^[ \t]*#', cleaned, re.M):
        raise UnsupportedC('Diretivas diferentes de #include precisam ser expandidas antes da análise.')
    try:
        ast = c_parser.CParser().parse(cleaned, filename='entrada.c')
    except plyparser.ParseError as exc:
        raise UnsupportedC(f'C inválido ou fora do C99 reconhecido: {exc}') from exc
    functions = [item for item in ast.ext if isinstance(item, c_ast.FuncDef)]
    if not functions or any(not isinstance(item, (c_ast.FuncDef, c_ast.Decl, c_ast.Typedef)) for item in ast.ext):
        raise UnsupportedC('Cole pelo menos uma definição de função C válida.')
    if function_name:
        matches = [item for item in functions if item.decl.name == function_name]
        if len(matches) != 1:
            raise UnsupportedC(f'Função {function_name} não encontrada. Disponíveis: {", ".join(item.decl.name for item in functions)}.')
        function = matches[0]
    elif len(functions) == 1:
        function = functions[0]
    else:
        raise UnsupportedC(f'Escolha a função a analisar: {", ".join(item.decl.name for item in functions)}.')
    params = function.decl.type.args.params if function.decl.type.args else []
    allowed = {item.name for item in params if isinstance(item, c_ast.Decl)} & {'n', 'V', 'E'}
    output, mapping, notes = [], [], []
    if includes:
        notes.append(f'{len(includes)} diretiva(s) #include ignorada(s); chamadas das bibliotecas não são estimadas.')
    active = []

    def emit(text, node):
        output.append('  ' * len(active) + text)
        mapping.append(node.coord.line if node.coord else None)

    def simple(node):
        if isinstance(node, c_ast.If):
            reject(node, 'if cria caminhos diferentes; analise melhor e pior caso separadamente.')
        if isinstance(node, (c_ast.While, c_ast.DoWhile)):
            reject(node, 'while exige uma prova do número de iterações; este formato ainda não é reconhecido.')
        if has_call(node):
            reject(node, 'a chamada de função pode ter custo próprio; analise-a separadamente.')
        protected = allowed | {scope[0] for scope in active}
        if isinstance(node, c_ast.Return) and active:
            reject(node, 'return dentro do laço pode encerrá-lo antes do limite.')
        if isinstance(node, c_ast.Assignment) and identifier(node.lvalue) in protected:
            reject(node, 'alterar um contador ou limite dentro do corpo muda a quantidade de iterações.')
        if isinstance(node, c_ast.UnaryOp) and identifier(node.expr) in protected:
            reject(node, 'alterar um contador ou limite dentro do corpo muda a quantidade de iterações.')
        if isinstance(node, c_ast.Decl):
            if node.name in protected:
                reject(node, 'redefinição de contador ou parâmetro de tamanho.')
            if node.init is None:
                return
            if not isinstance(node.type, c_ast.TypeDecl):
                reject(node, 'declaração composta ou alocação não tem custo modelado.')
        elif not isinstance(node, (c_ast.Assignment, c_ast.UnaryOp, c_ast.Return)):
            reject(node, 'instrução ainda não suportada pelo contador.')
        emit('op', node)

    def loop(node):
        init = node.init
        if isinstance(init, c_ast.DeclList) and len(init.decls) == 1:
            decl = init.decls[0]
            var, start = decl.name, decl.init
        elif isinstance(init, c_ast.Assignment) and init.op == '=':
            var, start = identifier(init.lvalue), init.rvalue
        else:
            reject(node, 'use for (int i=0; i<n; i++) ou for (int i=1; i<=n; i++).')
        if not re.fullmatch(r'[a-z]', var or '') or var in allowed or not (constant(start, 0) or constant(start, 1)):
            reject(node, 'o contador deve iniciar em 0 ou 1 e ter nome de uma letra minúscula.')
        start_value = 0 if constant(start, 0) else 1
        cond = node.cond
        if not isinstance(cond, c_ast.BinaryOp) or identifier(cond.left) != var:
            reject(node, 'a condição deve comparar diretamente o contador com seu limite.')
        upper = identifier(cond.right)
        step = node.next
        doubling = isinstance(step, c_ast.Assignment) and step.op == '*=' and identifier(step.lvalue) == var and constant(step.rvalue, 2)
        doubling |= isinstance(step, c_ast.Assignment) and step.op == '=' and identifier(step.lvalue) == var and isinstance(step.rvalue, c_ast.BinaryOp) and step.rvalue.op == '*' and identifier(step.rvalue.left) == var and constant(step.rvalue.right, 2)
        if doubling:
            if active or start_value != 1 or cond.op != '<' or upper != 'n' or 'n' not in allowed:
                reject(node, 'duplicação exige for externo com contador inicial 1, condição i<n e passo i*=2.')
            emit('while x < n doubling', node)
            active.append((var, start_value))
            body(node.stmt)
            active.pop()
            emit('end', node)
            notes.append(f'Linha {node.coord.line}: {var} dobra a cada passo; o corpo executa ⌈log₂ n⌉ vezes.')
            return
        if upper not in allowed and upper not in [scope[0] for scope in active]:
            reject(node, 'o limite deve ser n, V, E ou um contador externo.')
        if not ((start_value == 0 and cond.op == '<') or (start_value == 1 and cond.op == '<=')):
            reject(node, 'use 0 até limite exclusivo ou 1 até limite inclusivo.')
        if upper in [scope[0] for scope in active] and any(scope[0] == upper and scope[1] == 0 for scope in active):
            reject(node, 'limite dependente de contador que começa em 0 exige deslocamento; este caso ainda não é calculado.')
        increment = isinstance(step, c_ast.UnaryOp) and step.op in ('p++', '++') and identifier(step.expr) == var
        increment |= isinstance(step, c_ast.Assignment) and step.op == '+=' and identifier(step.lvalue) == var and constant(step.rvalue, 1)
        if not increment:
            reject(node, 'o contador deve aumentar exatamente 1 em cada iteração.')
        if var in [scope[0] for scope in active] or len(active) >= 3:
            reject(node, 'use até três contadores distintos aninhados.')
        emit(f'for {var} = 1..{upper}', node)
        active.append((var, start_value))
        body(node.stmt)
        active.pop()
        emit('end', node)
        notes.append(f'Linha {node.coord.line}: o controle do for é ignorado; o corpo é contado por iteração.')

    def body(node):
        items = node.block_items or [] if isinstance(node, c_ast.Compound) else [node]
        for item in items:
            if isinstance(item, c_ast.For):
                loop(item)
            elif isinstance(item, c_ast.Compound):
                body(item)
            else:
                simple(item)

    body(function.body)
    if not any(line.strip().startswith('op') for line in output):
        raise UnsupportedC('A função não contém operações de corpo reconhecidas.')
    return {'pseudocode': '\n'.join(output), 'lineMap': mapping, 'notes': notes,
            'function': function.decl.name,
            'assumption': 'Uma atribuição, declaração inicializada, incremento ou retorno vale 1 operação. Acesso a vetor é constante. Controle de laços é ignorado.'}
