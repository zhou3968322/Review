% Computes approximation approx for functionf on interval [a, b] and gives
% the infinity norm of the residu function, normr using Remez
% algorithm. Stops when delta <= epsilon
% Input: approximimant f
%       initial reference ref
%        basis for Harrar system basis
%       endpoints a,b of the interval [a, b]
% Output: approximation for f approx
%       infinity norm of residue function normr

function [approx, coeff,normr] = RemezExample(f,ref, basis,a,b,epsilon, maxIteration)
if nargin < 7
    maxIteration = 1;
end
if nargin < 6
    epsilon = 2^(-53);
end
delta = epsilon + 1;
iterCount = 0;
while delta > epsilon && iterCount < maxIteration
    [ymaxr, r, newref, approx, coeff, delta] = Remez2(f, ref, basis, a, b);
    ref = newref;
    iterCount = iterCount + 1;
end
normr = abs(r(ymaxr));
end