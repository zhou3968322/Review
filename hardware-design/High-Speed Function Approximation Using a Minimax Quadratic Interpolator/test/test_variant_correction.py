import math
import random
import numpy as np

# n = 3
# y = 1.25x, x in [1,2], y in [1.4, 2.8]

def get_bits(x):
    x_s = format(x, "b")
    return [int(bn) for bn in x_s]

def test_multi(multiplicand, multiplier):
    gt = multiplicand * multiplier
    # if gt >= 2 ** (2 * bit_num - 1):
    #     gt = gt >> bit_num
    # else:
    #     gt = gt >> (bit_num - 1)
    x_list = get_bits(multiplicand)
    y_list = get_bits(multiplier)
    global corrected_constant,max_error, min_error, max_mul, min_mul
    sum_res = 0
    # first compute not extended bits
    for i in range(bit_num):
        pos_sum = 0
        for j in range(i + 1):
            pos_sum += x_list[j] & y_list[i - j]
        sum_res += pos_sum << (2 * bit_num - 2 - i)
        #import pdb;pdb.set_trace()
    # add extra bits
    # import pdb;pdb.set_trace()
    for i in range(1, k + 1):
        # bit_num + k
        pos_sum = 0
        for j in range(i, bit_num):
            pos_sum += x_list[j] & y_list[i + bit_num - 1 - j]
        if(i < k):
            sum_res += (1 + pos_sum) << (bit_num -1 - i)
        else:
            sum_res += pos_sum << (bit_num - i)

    sum_res = sum_res + (corrected_constant << (bit_num - k))
    # sum_res = sum_res + corrected_constant
    # #import pdb;pdb.set_trace()
    # if sum_res >= 2 ** (bit_num + k - 1):
    #     sum_res = sum_res >> k
    # else:
    #     sum_res = sum_res >> (k - 1)

    # error = sum_res - gt
    if gt >= 2 ** (2 * bit_num - 1):
        error = (sum_res - gt) / 2 ** (2 * bit_num - 1)
    else:
        error = (sum_res - gt) / 2 ** (2 * bit_num - 2)
    # if gt >= 2 ** (2 * bit_num - 1):
    #     error = (gt - sum_res) // 2
    # else:
    #     error = gt - sum_res
    if abs(error) > abs(max_error):
        max_error = error
        max_mul = [multiplicand, multiplier, gt, sum_res]
    if abs(error) < abs(min_error):
        min_error = error
        min_mul = [multiplicand, multiplier, gt, sum_res]
    # print("{} * {}, gt:{}, pred:{}".format(format(multiplicand, 'b'), format(multiplier, 'b'), format(gt, 'b'), format(sum_res, 'b')))

bit_num = 8
k = 2 + 1 # for k = 1 we must extend 1+1 bits.

# corrected constant can be combined with 0b111. 
# 0b111 -4 = 0b11  then final corrected constant is 0b11, only one constant.
corrected_constant = -4 

bit_range = range(2 ** (bit_num-1), 2**(bit_num))

# multi res will normalized to [0, 2**(bit_num) - 1]


max_error = 0
min_error = 2 ** (2 * bit_num)
max_mul = []
min_mul = []

for x in bit_range:
# multiplicand = random.randint(2 ** (bit_num-1), 2**(bit_num))
# multiplicand = 0b10011101

    for y in bit_range:
        # multiplier = 0b11111110
        test_multi(x, y)
# test_multi(129, 254)


# max_error > 2^{-bit_num + 1} is ok.
print(max_error, np.log2(abs(max_error)), max_mul)

print(min_error, np.log2(abs(min_error)), min_mul)
# print(max_error, max_mul)
# print(min_error, min_mul)


