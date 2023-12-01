
% this script is to compute log2 with variant multiplication correction.
f = @(x) log2_div_approx(x);
m = 6;
table_size = m^2;

r = 23; 
delta = 2^(-r-2);
error_approx = 2^(-r-1) + delta;

% trunc的时候希望保证error < 2^{-r-2} = 2^{-25}, so need no more than 28 bits.

% t error < 2^{-r-1}, p error is less than 2^{-24} eror
% t <= 28
t_range = [28, 27, 26, 25, 24];

% p * 2^{-m}, p error is less than 22 bit.
p_range = [22, 21, 20, 19, 18, 17, 16, 15];

% q * 2^{-2m}, q error is less than 16 bit.
q_range = [22, 21, 20, 19, 18, 17, 16, 15];

[approx, normr, max_error, success] = computecoeffs1(f, t, p, q, table_size, error_approx);

