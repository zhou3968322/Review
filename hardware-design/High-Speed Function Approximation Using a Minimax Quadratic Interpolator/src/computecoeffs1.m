% Computes minimax quadratic apprximation f \approx C0 + C1x2 +C2 x2^2
% x = 1.x1x1x1...x1 x2...x2
% for coefficients on interval [0, 1/table_size]
% Input: approximimant f
%        t C0 bit nums
%        p C1 bit nums
%        q C2 bit nums
%        table_size   splited table size for x1.
%        errmax allowed max error for this coeffs
% Output: approximation for f approx, table_size * 3
%         normr infinity norm error, max error, table_size
%         success  flag current t,p,q is valid in limited err.

function [approx, normr, max_err, success] = computecoeffs1(f, t, p, q, table_size, errlimit)
normr = zeros(table_size);
max_err = 0;
a = 0;
b = 1/table_size;
table_bit = log2(table_size);

basis1 = @(x) [1 x x^2];
basis2 = @(x) [1];

ref1 = linspace(a, b, 4);
ref2 = linspace(a, b, 2);
approx = zeros(table_size, 3);
for i = 1:table_size
    % current function for interval [i/table_size, (i + 1)/table_size]
    % fist pass minimax approximation
    cf = @(x) f(x + (i-1)/table_size);
    [~, pol1, ~] = RemezExample(cf, ref1, basis1,a,b,errlimit);
    approx(i, 2) = 2^(-p) * round(pol1(2) * 2^p);

    % second pass minimax
    approx(i, 3) = 2^(-q) * round((pol1(3) + (pol1(2) - approx(i, 2)) * 2^table_bit) * 2^q);

    % third pass minimax approximation
    cf = @(x) f(x + (i-1)/table_size) - approx(i, 2) * x - approx(i, 3) * x^2;
    [~, pol3, err] = RemezExample(cf, ref2, basis2,a,b,errlimit);

    approx(i, 1) = 2^(-t) * round(pol3(1) * 2^t);

    if err > max_err
        max_err = err;
    end
    if err > errlimit
       success = false;
       return;
    end
    normr(i) = max_err;
end
success = true;
end