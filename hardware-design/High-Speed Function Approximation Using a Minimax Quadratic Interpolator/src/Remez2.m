% Execute one iteration of the Remez algorithm
% Input:    approximant f
%           initial reference ref
%           basis for Haar system basis
% Output:   location of max of |r| ymaxr
%           residue function r
%           reference for nextiteration refnew
%           delta for stopping criterion

function [ymaxr,r,newref,approx, coeff,delta] = Remez2(f,ref,basis,a,b)

% compute the best minimax approx on reference
[lambda] = minmaxsol2(f, ref, basis);

%define approximation and residue functions

approx = @(x) dot(basis(x), lambda);
r = @(x) f(x) - dot(basis(x), lambda);
coeff = lambda;
h = r(ref(1));

[ymaxr] = findmaxr2(r, a, b); % find location of max of  r

normr = abs(r(ymaxr));  % compute ||r||
delta = abs(normr - abs(h)); % compute delta for stopcrit

s = size(ref);
n = s(2) -1;

% Create vector z iwth roots

z(1) = a;
for i = 2:n+1
    z(i) = fzero(r, [ref(i-1)  ref(i)]);
end

z(n+2) = b;

for i = 1: n+1 % create trial set {y0, y1, ... yn}
    sigma = sign(r(ref(i)));
    newref(i) = fmaxsigmar(sigma, r, z(i), z(i+1), ref(i), a, b);
end

if a <=ymaxr && ymaxr < newref(1) % Inserting ymaxr into the reference
    if sign(r(ymaxr)) == sign(r(newref(1)))
        newref(1) = ymaxr;
    else
        for i =n+1:-1:2
            newref(i) = newref(i-1);
        end
        newref(1) = ymaxr;
    end
elseif newref(n+1) <= ymaxr && ymaxr <=b
    if sign(r(ymaxr)) == sign(r(newref(n+1)))
       newref(n+1) = ymaxr;
    else
        for i = 1:n
            newref(i) = newref(i+1);
        end
        newref(n+1) = ymaxr;
    end
else
    for i=2:n+1
    if newref(i-1) <= ymaxr && ymaxr < newref(i)
        if sign(r(ymaxr)) == sign(r(newref(i - 1)))
            newref(i-1) = ymaxr;
        elseif sign(r(ymaxr)) == sign(r(newref(i)))
            newref(i) = ymaxr;
        end
    end
    end
end

end