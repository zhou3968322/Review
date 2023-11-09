function [maxsol] = fmaxsigmar(sigma,r,lb,ub,xi,a, b)
%Find desired local maxima of sigma * r

f = @(x) - 1 * sigma * r(x);
maxsol = fminbnd(f, lb, ub);

%In case a local maximum was found with sigma * r(x_i) > sigma * r(y_i)
%we start a brute force search for a desired maximum

if sigma * r(xi) > sigma * r(maxsol)
    stepsize = (ub - lb) / 10000;
    for i = 1:10001
        valvector(i) = sigma * r(lb + stepsize * (i - 1));
    end
    maxval = max(valvector);
    for i = 1:10001
        if valvector(i) == maxval
            maxsol = lb + stepsize*(i-1);
        end
    end
end

% In case sigma * r(x_i) was already maximal
if sigma * r(xi) > sigma * r(maxsol)
    maxsol = xi;
end
if lb == a && sigma & r(a) > sigma * r(maxsol)
    maxsol = a;
end
if ub == b && sigma * r(b) > sigma * r(maxsol)
    maxsol = b
end


end
