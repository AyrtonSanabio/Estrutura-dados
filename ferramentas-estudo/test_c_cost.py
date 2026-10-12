import unittest

from c_cost import UnsupportedC, translate


class CCostTests(unittest.TestCase):
    def test_linear_and_mapping(self):
        result = translate('int sum(int n, int *a) {\n int s=0;\n for (int i=0;i<n;i++) {\n  s+=a[i];\n }\n return s;\n}')
        self.assertEqual(result['pseudocode'], 'op\nfor i = 1..n\n  op\nend\nop')
        self.assertEqual(result['lineMap'], [2, 3, 4, 3, 6])

    def test_nested_and_graph(self):
        triangle = translate('int f(int n){int x=0; for(int i=1;i<=n;i++){for(int j=1;j<=i;j++){x++;}}return x;}')
        self.assertIn('for j = 1..i', triangle['pseudocode'])
        graph = translate('int f(int V,int E){int x=0;for(int i=0;i<V;i++){x++;}for(int j=0;j<E;j++){x++;}return x;}')
        self.assertIn('for i = 1..V', graph['pseudocode'])
        self.assertIn('for j = 1..E', graph['pseudocode'])

    def test_includes_and_function_choice(self):
        source = '#include <stdio.h>\nint helper(int n){return n;}\nint count(int n){int x=0;for(int i=0;i<n;i++){x++;}return x;}'
        with self.assertRaisesRegex(UnsupportedC, 'Escolha a função'):
            translate(source)
        result = translate(source, 'count')
        self.assertEqual(result['function'], 'count')
        self.assertIn('#include ignorada', ' '.join(result['notes']))

    def test_logarithmic_for(self):
        result = translate('int count(int n){int x=0;for(int i=1;i<n;i*=2){x++;}return x;}')
        self.assertIn('while x < n doubling', result['pseudocode'])
        self.assertIn('log₂ n', ' '.join(result['notes']))

    def test_unsupported_is_rejected(self):
        cases = [
            '#define MAX 100\nint f(int n){return n;}',
            'int f(int n){for(int i=0;i<n;i++){if(i) return i;}return 0;}',
            'int f(int n){for(int i=0;i<n;i++){n--; }return n;}',
            'int f(int n){for(int i=0;i<n;i++){for(int j=0;j<i;j++){n++;}}return n;}',
            'int f(int n){return g(n);}',
            'int f(int n){while(n>0){n--;}return n;}',
        ]
        for source in cases:
            with self.subTest(source=source), self.assertRaises(UnsupportedC):
                translate(source)


if __name__ == '__main__':
    unittest.main()
