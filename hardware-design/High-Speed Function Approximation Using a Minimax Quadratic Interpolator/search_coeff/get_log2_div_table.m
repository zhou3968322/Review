% this script is to compute log2 table.
f = @(x) log2_div_approx(x);
m = 6;
table_size = 2^m;

t = 25;p=15;q=11;
error_limit = 2^(-24) + 2^(-25);
[approx, int_approx, normr, max_error, success] = computecoeffs1(f, t, p, q, table_size, error_limit);
[rows, cols] = size(int_approx);

for j = 1:cols
    ensure_signed = true;
    if int_approx(1,j) >= 0
        positive = true;
    else
        positive = false;
    end
    for i = 2:rows
        if positive && int_approx(i, j) < 0
            ensure_signed = false;
            break;
        elseif ~positive && int_approx(i, j) > 0
            ensure_signed = false;
            break;
        end
    end
    if ensure_signed && ~positive
        for i = 1:rows
            int_approx(i, j) = -int_approx(i, j);
        end
    end
    fprintf("j:%d,ensure_signed:%d,positive:%d\n", j, ensure_signed, positive);
end


saveToCppArray(int_approx, "log2_no_variant_talbe_t25_p15_q11.txt");