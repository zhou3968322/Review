%Solve linear system to obtain best approximation on a reference
function [lambda] = minmaxsol2(f, ref, basis)

s = size(ref);
n = s(2) - 1;

%Construct right hand side vector
for i = 2:(n+1)
   b(i-1) = f(ref(i)) - power(-1, i-1) * f(ref(1));
end

%Construct matrix A
vector2 = basis(ref(1));

for i=1:n
    vector1 = basis(ref(i+1));
    for j =1:n
        A(i,j) = vector1(j) - power(-1, i) * vector2(j);
    end
end

b = transpose(b);
lambda = A \ b;

     
