function [ymaxr] = findmaxr2(r,a,b)
%Brute force search for x-coordinate of maximum of r

stepsize = (b-a)/10000;
for i = 1:10001
    valvector(i) = abs(r(a + stepsize * (i - 1)));
end

%Find location of largest value

maxval = max(valvector);
for i = 1:10001
    if valvector(i) == maxval
        ymaxr = a + stepsize * (i - 1);
    end
end

end