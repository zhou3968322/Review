% Computes approximation approx for functionf on interval [a, b] and gives
% the infinity norm of the residu function, normr using Remez
% algorithm. Stops when delta <= epsilon
% Input: approximimant f
%       initial reference ref
%        basis for Harar system basis
%       endpoints a,b of the interval [a, b]
% Output: approximation for f approx
%       infinity norm of residue function normr

function [approx, coeff,normr] = RemezExample(f,ref, basis,a,b,epsilon)
delta = epsilon + 1;
while delta > epsilon
    [ymaxr, r, newref, approx, coeff, delta] = Remez2(f, ref, basis, a, b);
    ref = newref;
end
normr = abs(r(ymaxr));
end