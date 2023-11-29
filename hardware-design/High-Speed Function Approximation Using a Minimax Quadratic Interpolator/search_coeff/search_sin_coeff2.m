% this script is to compute sin pi / 4 x. without variant multiplication correction.
f = @(x) sin(pi * x/2);
m = 5;
table_size = 2^m;

error_approx = 2^(-22);

t_range = [27, 26, 25, 24, 23];

% p * 2^{-m}, p error is more than 18 bit.
p_range = [19, 18, 17, 16, 15];

% q * 2^{-2m}, q error is less than 12 bit.
q_range = [14, 13, 12, 11];

max_table_bits = 0;

results = struct('t', {}, 'p', {}, 'q', {}, 'coeff_size', {}, 'max_error', {});


for p = p_range
    for t = t_range
        for q = q_range
            [approx, int_approx, normr, max_error, success] = computecoeffs1(f, t, p, q, table_size, error_approx);
            if success
                coeff_size = p + t + q;
                results(end+1) = struct('t', t, 'p', p, 'q', q, 'coeff_size', coeff_size, 'max_error', max_error);
            end
        end
    end
end 

resultsTable = struct2table(results);
resultsTable = sortrows(resultsTable, 'p');
resultsTable = sortrows(resultsTable, 'coeff_size');

% Display or process the sorted results
disp(resultsTable);

    % t     p     q     coeff_size    max_error 
    % __    __    __    __________    __________
    % 
    % 25    17    13        55        1.1564e-07
    % 26    17    13        56        1.0197e-07
    % 25    17    14        56        1.1564e-07
    % 25    18    13        56         1.072e-07
    % 24    19    13        56        1.1348e-07
    % 27    17    13        57        1.0672e-07
    % 26    17    14        57        1.0197e-07
    % 26    18    13        57        1.1044e-07
    % 25    18    14        57        9.9472e-08
    % 25    19    13        57        1.0351e-07
    % 24    19    14        57        1.1348e-07
    % 27    17    14        58        1.0672e-07
    % 27    18    13        58        1.0299e-07
    % 26    18    14        58        9.8575e-08
    % 26    19    13        58        1.1155e-07
    % 25    19    14        58         9.781e-08
    % 27    18    14        59        9.4548e-08
    % 27    19    13        59        1.1155e-07
    % 26    19    14        59        9.8575e-08
    % 27    19    14        60        9.1124e-08

