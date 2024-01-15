syms x;
f_sym = 1/(1+exp(-x));
h_sym = x /2 * (1 + erf(x / sqrt(2)));

f_diff_sym = abs(diff(f_sym, x));
h_diff_sym = abs(diff(h_sym, x));

f_diff_func = matlabFunction(f_diff_sym);
h_diff_func = matlabFunction(h_diff_sym);

disp(f_diff_func);
disp(h_diff_func);

maxsol1 = fminbnd(f_diff_func, -1, 1);
fprintf('The maxsol is %f\n', -maxsol1);

maxsol2 = fminbnd(h_diff_func, -1, 1);
fprintf('The maxsol is %f\n', -maxsol2);
