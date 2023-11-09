% log2(1 +x) / x, x in [0,1].
function [y] = log2_div_approx(x)
    if x == 0
        y = 1 / log(2);
    else
        y = log2(1 + x)/x;
    end
end