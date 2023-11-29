% this script is to compute sin pi / 4 x. without variant multiplication correction.
f = @(x) cos(pi * x/4);
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

    % t     p     q     coeff_size    max_error 
    % __    __    __    __________    __________
    % 
    % 22    15    12        49        2.0766e-07
    % 22    16    11        49         2.269e-07
    % 23    15    12        50           1.5e-07
    % 22    15    13        50        2.0766e-07
    % 23    16    11        50        1.9553e-07
    % 22    16    12        50        2.0018e-07
    % 22    17    11        50         2.269e-07
    % 25    15    11        51        2.1055e-07
    % 24    15    12        51           1.5e-07
    % 23    15    13        51        1.8769e-07
    % 24    16    11        51        1.5299e-07
    % 23    16    12        51        1.2616e-07
    % 22    16    13        51        1.8537e-07
    % 23    17    11        51        1.6778e-07
    % 22    17    12        51        1.7753e-07
    % 22    18    11        51         2.269e-07
    % 26    15    11        52        2.1055e-07
    % 25    15    12        52        1.4806e-07
    % 24    15    13        52           1.5e-07
    % 25    16    11        52        1.6177e-07
    % 24    16    12        52        1.0927e-07
    % 23    16    13        52        1.3817e-07
    % 24    17    11        52        1.6778e-07
    % 23    17    12        52        1.2616e-07
    % 22    17    13        52        1.7753e-07
    % 23    18    11        52        1.7528e-07
    % 22    18    12        52        1.7202e-07
    % 22    19    11        52        2.3421e-07
    % 26    15    12        53        1.4254e-07
    % 25    15    13        53        1.4806e-07
    % 26    16    11        53        1.3809e-07
    % 25    16    12        53        1.1078e-07
    % 24    16    13        53        1.0177e-07
    % 25    17    11        53        1.3798e-07
    % 24    17    12        53        1.1792e-07
    % 23    17    13        53        1.2616e-07
    % 24    18    11        53        1.5509e-07
    % 23    18    12        53        1.2842e-07
    % 22    18    13        53        1.6438e-07
    % 23    19    11        53        1.6939e-07
    % 22    19    12        53        2.0637e-07
    % 26    15    13        54        1.4254e-07
    % 26    16    12        54        9.6014e-08
    % 25    16    13        54        1.0836e-07
    % 26    17    11        54        1.5288e-07
    % 25    17    12        54        1.0927e-07
    % 24    17    13        54        1.1792e-07
    % 25    18    11        54        1.4547e-07
    % 24    18    12        54        1.0256e-07
    % 23    18    13        54        1.2616e-07
    % 24    19    11        54        1.5509e-07
    % 23    19    12        54         1.632e-07
    % 22    19    13        54        1.7008e-07
    % 26    16    13        55        9.6014e-08
    % 26    17    12        55        9.4373e-08
    % 25    17    13        55        9.6354e-08
    % 26    18    11        55         1.354e-07
    % 25    18    12        55        9.6354e-08
    % 24    18    13        55        9.6198e-08
    % 26    19    10        55        2.3388e-07
    % 25    19    11        55        1.4481e-07
    % 24    19    12        55        1.1222e-07
    % 23    19    13        55        1.0292e-07
    % 26    17    13        56        8.8123e-08
    % 26    18    12        56        9.1782e-08
    % 25    18    13        56        9.6354e-08
    % 26    19    11        56        1.4481e-07
    % 25    19    12        56        1.1222e-07
    % 24    19    13        56        7.2537e-08
    % 26    18    13        57        8.1453e-08
    % 26    19    12        57         1.036e-07
    % 25    19    13        57         7.203e-08
    % 26    19    13        58        7.1441e-08
