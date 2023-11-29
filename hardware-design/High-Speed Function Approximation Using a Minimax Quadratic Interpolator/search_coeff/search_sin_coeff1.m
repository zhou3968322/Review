% this script is to compute log2 without variant multiplication correction.
f = @(x) sin(x);
m = 5;
table_size = 2^m;

error_approx = 2^(-22);
% 希望最终的误差 < error_limit = 2^(-25)
% 估计的每一项的误差应该 < 2^(-25)
t_range = [26, 25, 24, 23, 22];

% p * 2^{-m}, p error is more than 18 bit.
p_range = [18, 17, 16, 15, 14, 13];

% q * 2^{-2m}, q error is less than 12 bit.
q_range = [14, 13, 12, 11, 10, 9];

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

