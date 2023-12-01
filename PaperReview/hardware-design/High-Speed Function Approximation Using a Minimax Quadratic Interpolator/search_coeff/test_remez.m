f = @(x)  exp(x);
a = 0;
b = 1;
ref = linspace(a, b, 3);
basis = @(x) [1, x];

[approx, coeff, normr] = RemezExample(f, ref, basis, a, b)

