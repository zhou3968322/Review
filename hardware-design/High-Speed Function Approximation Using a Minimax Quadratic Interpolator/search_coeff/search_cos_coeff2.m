% this script is to compute sin pi / 4 x. without variant multiplication correction.
f = @(x) cos(x);
m = 5;
table_size = 2^m;

error_approx = 2^(-22);

t_range = [26, 25, 24, 23, 22, 21];

% p * 2^{-m}, p error is more than 18 bit.
p_range = [19, 18, 17, 16, 15, 14];

% q * 2^{-2m}, q error is less than 12 bit.
q_range = [13, 12, 11, 10];

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
