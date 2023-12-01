#include <vector>
#include <cmath>
#include "log.h"
#include "int_op.h"


/*
accuray the same as nvidia's intrinsic
For x in [0.5, 2], the maximum absolute error is 2-22, otherwise, the
maximum ulp error is 2
has tested about 2^22 times.
*/
float log2f_as_nvidia(float x, const int t, const int p, int q) {

    // float gt = log2f(x);
    // uint32_t gt_b = *((uint32_t*)&gt);

    BinaryBase bb;
    bb.value = x;
    int64_t exp_bias = bb.logBinary.exp;
    exp_bias -= 127;

    uint32_t query_idx = bb.logBinary.x1;
    uint64_t taylor_items[EXPAND_ITEM_NUM];

    if (t == 25 && p == 15 && q == 10) {
        taylor_items[0] = enhanced_log_t25_p15_q10_database[query_idx * EXPAND_ITEM_NUM];
        taylor_items[1] = enhanced_log_t25_p15_q10_database[query_idx * EXPAND_ITEM_NUM + 1];
        taylor_items[2] = enhanced_log_t25_p15_q10_database[query_idx * EXPAND_ITEM_NUM + 2];
    }
    else
        assert(0 && "errro coefficients");

    // t = 25;p=15;q=10;
    // 2^{-t} * C0 + 2^{-p}* C1 * x2 + 2^{-q} * C2 * x2^2 = sum
    // t = 25           p + 23 = 38            q + 46 = 56
    // error < 2^{-23}
    int max_bits = std::max(t, p + 23);
    max_bits = std::max(max_bits, q + 46);
    int64_t second_order_item = int64_t(taylor_items[2] * bb.logBinary.x2) * int64_t(bb.logBinary.x2);

    // as table 15 bit * 17 bit, negative
    int64_t first_order_item = taylor_items[1] * int64_t(bb.logBinary.x2);

    // as table only 26 bit
    int64_t zero_order_item = taylor_items[0];

    // 0 <= sum < 1
    // calculate 0 <= sum * 2^max_bits < 2^max_bits
    // only have less than max_bits.
    // sum_item * 2^{-max_bits} is the final result.
    int64_t sum_item = (zero_order_item << (max_bits - t)) + (first_order_item << (max_bits - p - 23)) - (second_order_item << (max_bits - q - 46));

    /*
    for error < 2^{-22}.
    previous approximation error < 2^{-23}
    current round_error < 2^{-23}
    
    |result - sum_item * 2^{-max_bits} | < 2^{-23}
    |round(sum_item) - sum_item | * 2{-max-bits} < 2^{-23}
    |round(sum_item) - sum_item | <= 2^{maxbits-23}
    get 23 bits from max_bits->max_bits 
    */
    int64_t res_int = (exp_bias << max_bits) + sum_item;
    BinaryBase tmp;
    bool is_signed = false;
    if (res_int >> 63) {
        // if signed
        is_signed = true;
        res_int = -res_int;
    }
    else
        is_signed = false;
    // res_int bit num: max_bits + 1 -> exp: 0.
    // res_int bit num: 64 - index -> exp: 63 - index - max_bits.
    // res_man * 2^{63 - index} * 2^{}
    auto res_pair = findFirst24BitsAndIndex(res_int);
    
    // we then compute res's float representation.
    uint32_t res_man = res_pair.first; // 24 bit
    res_man &= 0x7fffffU; //get 23 bit.

    if (is_signed)
        tmp.logBinary.sign = 1;
    else
        tmp.logBinary.sign = 0;
    tmp.logBinary.x1 = res_man >> 17;
    tmp.logBinary.x2 = (res_man << 15) >> 15;
    tmp.logBinary.exp = 127 + 63 - res_pair.second - max_bits;

    float res = tmp.value;
    // uint32_t res_b = *((uint32_t*)&res);
    return res;
}



/*
this is tmp test version for log2.
1 ulp error only for x > 1.
for x < 1's leading zero has not bee eliminated.
*/

float tmp_sfu_log2(float x, const int t, const int p, int q) {
    float gt = log2f(x);
    uint32_t gt_b = *((uint32_t*)&gt);
    
    BinaryBase bb;
    bb.value = x;

    uint32_t query_idx = bb.logBinary.x1;
    uint64_t taylor_items[EXPAND_ITEM_NUM];

    if (t == 25 && p == 15 && q == 11) {
        taylor_items[0] = enhanced_log_t25_p15_q11_database[query_idx * EXPAND_ITEM_NUM];
        taylor_items[1] = enhanced_log_t25_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 1]; 
        taylor_items[2] = enhanced_log_t25_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 2];
    }else if (t == 26 && p == 15 && q == 11) {
        taylor_items[0] = enhanced_log_div_t26_p15_q11_database[query_idx * EXPAND_ITEM_NUM];
        taylor_items[1] = enhanced_log_div_t26_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 1];
        taylor_items[2] = enhanced_log_div_t26_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 2];
    }
    else
        assert(0 && "errro coefficients");

    // t = 25;p=15;q=11;
    // 2^{-t} * C0 + 2^{-p}* C1 * x2 + 2^{-q} * C2 * x2^2 = sum
    // 25            p + 23 = 38            q + 46 = 57
    // res in [1,1.5)
    // res 
    // error < 2^{-24}
    // x2 in [0, 1/table_size]
    // we only need trunc 24 bit.
    // first we use integer mul
    // 
    // as table 11 bit * 17 bit * 17 bit, 45 bit
    int max_bits = std::max(t, p + 23);
    max_bits = std::max(max_bits, q + 46);
    int64_t second_order_item = int64_t(taylor_items[2] * bb.logBinary.x2) * int64_t(bb.logBinary.x2);

    // as table 15 bit * 17 bit, negative
    int64_t first_order_item = taylor_items[1] * int64_t(bb.logBinary.x2);

    // as table only 26 bit
    int64_t zero_order_item = taylor_items[0];

    // 1 <= sum < 1.5
    // calculate 2^57 <= sum * 2^57 < 2^58
    // must and only has 58 bit
    // sum_item * 2^{-max_bits} is the final result.
    int64_t sum_item = (second_order_item << (max_bits - q - 46)) + (zero_order_item << (max_bits - t)) - (first_order_item << (max_bits - p - 23));

    // all integer mul better use variant 
    // sum_item will mul dx. 23 bit:
    // sum_item 58 bit, dx 23 bit. 81 bit res.
    //  
    // 2^{-max_bits - 23} * low_mul + 2^{-max_bits - 23 + 32} * high_mul
    // this is to compute log2(x), 1 < x <=2 , 0 < log2(1 + x) <= 1
    // we need add exponent.
    // for low 64 is zero ,any need to add exp >> 16 for high64.
    uint64_t u_sum_item = *((uint64_t*)&sum_item);
    uint64_t low = (u_sum_item << 32) >> 32;
    uint64_t high = u_sum_item >> 32;
    uint64_t dx = (bb.logBinary.x1 << 17) +  bb.logBinary.x2;
    uint64_t low_mul = low * dx;
    uint64_t high_mul = high * dx;
    uint64_t high_mul_low32 = high_mul << 32; // add with low_mul must shirt right 32 bits.
    uint64_t low64;
    uint64_t high64;
    uint64_t carry;
    addWithCarry(low_mul, high_mul_low32, low64, carry);
    // sum_item* x * 2^{-max_bits - 23} is the final calucate integer res. 
    // for high64 and low 64.
    // final we want to calculate sum_item* x * 2^{-57 - 23} - exponent. 
    // exponent is integer.
    high64 = (high_mul >> 32) + carry;
    // we just get 24 bit of high64 and low64.
    // then calculate sum_item * dx + org_exp.
 
    // prvious calculate sum_item* dx * 2^{-max_bits-23}, so to compute (org_exp << (max_bits + 23)) + sum_item* x

    int64_t exp_bias = bb.logBinary.exp;
    exp_bias -= 127;
    
    int64_t exp_high = exp_bias << (max_bits + 23 - 64);
    uint64_t high64_res = high64 + exp_high;
    uint64_t low64_res = low64;
    BinaryBase tmp;
    bool is_signed = false;
    if (high64_res >> 63) {
        // if signed
        is_signed = true;
        if (low64 == 0)
            high64_res = (~high64_res) + 1;
        else
            high64_res = ~high64_res;
        low64_res = (~low64_res) + 1;
    }
    else
        is_signed = false;
    // m * 2^{23} * 2^{128 - a - 24} 约等于 high + low.
    // m * 2^{127 - a} 约等于 high + low = sum_item *x
    // sum_item *x *2^{-maxbits - 23} 约等于 m * 2^{127 - a - maxbits - 23}
    auto res_pair = findFirst32BitsAndIndex(high64_res, low64_res);

    // we then compute res's float representation.
    uint32_t res_man = res_pair.first; // 32 bit
    // user round mode rne.
    uint32_t res_man_26 = (res_man << 25) >> 31;
    // use 25 bit res_man for res_man may add 1 and overflow.
    // if overflow then we has to add bias.
    res_man = res_man >> 7; // 25 bit

    if (res_man_26)
        res_man++;
    //if (is_signed && !res_man_26)
    //    res_man++;
    uint32_t ov_exp_bias = 0;
    uint32_t res_man_ov = res_man >> 25;
    if (res_man_ov) {
        ov_exp_bias = 1;
        res_man = res_man >> 2; // res_man is 26 bit, get 24 bit.
    }
    else
        res_man = res_man >> 1; // get 24 bit
    res_man &= 0x7fffffU; //get 23 bit.

    // then we just get only first 24 bit or (high64 << 64) + low64..
    if (is_signed)
        tmp.logBinary.sign = 1;
    else
        tmp.logBinary.sign = 0;
    tmp.logBinary.x1 = res_man >> 17;
    tmp.logBinary.x2 = (res_man << 15) >> 15;
    tmp.logBinary.exp = 127 + 104 - max_bits - res_pair.second + ov_exp_bias;
    
    float res = tmp.value;


    uint32_t res_b = *((uint32_t*)&res);

    return res;
}


/*
1 ulp error version for log2f.
with table_size = 64 * (t + p +q)
               = 64 * (25 + 15 + 11) byte.
    has tested for number (0.5, 2) for about 2^20 times.
    has tested for normalized number for about 2^22 times.
*/

float log2_ulp1(float x, const int t, const int p, int q) {
    BinaryBase bb;
    bb.value = x;
    int64_t exp_bias = bb.logBinary.exp;
    exp_bias -= 127;
    if (bb.logBinary.x1 == 0 && bb.logBinary.x2 == 0)
        return float(exp_bias);

    uint32_t query_idx = bb.logBinary.x1;
    /*
     for 1.5 <= x < 2, 0.5 <= dx < 1
     dx = x1 * 2^{17} + x2.
     log2(1+x1+x2) = 
     for 0 <= x1 < 32/64, compute.
     [log2(1+x1+x2)/(x1+x2)]*(x1+x2), 
     = [log2(1+x1+x2)/(x1+x2)]*(dx * 2^{-23}), 


     for 32/64 <= x1 < 1,
     compute log2(1/2 + x1/2 + x2/2) + 1.
     then compute log2(1 - (1/2 - x1/2 - x2/2))
     [log2(1 - (1/2 - x1/2 - x2/2))]  / (1/2 - x1/2 - x2/2)
     = {log2[1/2 + (x1/2+1/128) - (1/128 - x2/2)]  / [1/2 - (x1 /2 + 1/128) + (1/128- x2/2)]
     = {log2[1/2 + (x1/2+1/128) - (1/128 - x2/2)]  / [1/2 - (x1 /2 + 1/128) + (1/128- x2/2)]
     1/64 - x2 is 2^{-23} * (~x2+1).
     (1/128- x2/2) is 2^{-24} * (~x2+1).
     1 - x1 - 1/64 is 2^{-6} * ~x1
    .1/2-x1/2 - 1/128  is 2^{-7} * ~x1
     log2(x) / [2^{-24} * (~x2+1) + 2^{-7} ~x1] = f(x2)

     log2(x) / [2^{-24} * (~dx + 1)] = f(x2'/2), x2' = ~x2+ 1

     
    
     so if query_idx >= 32, c_x2 is ~x2
     following to use 
    */
    uint32_t c_x2 = query_idx < 32 ? bb.logBinary.x2 : (((~bb.logBinary.x2) & 0x1ffffU) + 1);


    uint64_t taylor_items[EXPAND_ITEM_NUM];

    if (t == 25 && p == 15 && q == 11) {
        taylor_items[0] = enhanced_log_div_split_t25_p15_q11_database[query_idx * EXPAND_ITEM_NUM];
        taylor_items[1] = enhanced_log_div_split_t25_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 1];
        taylor_items[2] = enhanced_log_div_split_t25_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 2];
    }
    else
        assert(0 && "errro coefficients");

    /*
     t = 25;p=15;q=11;
     for query_idx <= 32.
     2^{-t} * C0 + 2^{-p}* C1 * x2 + 2^{-q} * C2 * x2^2 = sum
     t = 25           p + 23 = 38            q + 46 = 57
     for query_idx > 32.
     2^{-t} * C0 + 2^{-p}* C1 * x2'/2 + 2^{-q} * C2 * x2'^2/4 = sum
     2^{-t} * C0 + 2^{-p-1}* C1 * x2' + 2^{-q-2} * C2 * x2'^2 = sum
     t = 25           p + 23 + 1 = 39            q + 46 + 2 = 59

          if use variant multiplication,less op needed.
    */

    int64_t second_order_item = int64_t(taylor_items[2] * c_x2) * int64_t(c_x2);

    // as table p bit * 17 bit, negative
    int64_t first_order_item = int64_t(taylor_items[1]) * int64_t(c_x2);

    // as table only t bit
    int64_t zero_order_item = taylor_items[0];

    int max_bits;
    if (query_idx < 32) {
        max_bits = std::max(t, p + 23);
        max_bits = std::max(max_bits, q + 46);
    }
    else {
        max_bits = std::max(t, p + 23 + 1);
        max_bits = std::max(max_bits, q + 46 + 2);
    }

    /*
     1 <= sum < 1.5
     calculate 2^{max_bits}} <= sum * 2^{max_bits} < 2^{max_bits+1}
     must and only has max_bits bit
     sum_item = sum * 2^{max_bits}
     sum_item * 2^{-max_bits} is the final result.
     
     for query_idx < 32, C0, C2 's signed is positive, C1 is negative.
     for query_idx >= 32, C0, C1, C2 's signed is negative

    */
    int64_t sum_item;
    if (query_idx < 32)
        sum_item = (second_order_item << (max_bits - q - 46)) + (zero_order_item << (max_bits - t)) - (first_order_item << (max_bits - p - 23));
    else
        sum_item = (second_order_item << (max_bits - q - 48)) + (zero_order_item << (max_bits - t)) + (first_order_item << (max_bits - p - 24));

    uint64_t dx;
    /*
    for query_idx < 32
        dx = mantissa = (x1 << 17 + x2) / 2^23
    for query_idx >= 32
        dx = 1/2(1 - mantissa) = 1/2 (1 - (x1 << 17 + x2)/ 2^23)
                           = (2^23 - (x1 << 17 + x2))/2^24
                           = (~tmp_dx + 1) / 2^24

    that is ~dx.
    */
    uint64_t tmp_dx = (bb.logBinary.x1 << 17) + bb.logBinary.x2;
    if (query_idx >= 32)
        dx = ((~tmp_dx) & 0x7fffff) +1;
    else
        dx = tmp_dx;
    // dx = tmp_dx;

    // compute sum_mul_dx_item = int_dx * sum_item
    uint64_t u_sum_item = *((uint64_t*)&sum_item);
    uint64_t low = (u_sum_item << 32) >> 32;
    uint64_t high = u_sum_item >> 32;
    uint64_t low_mul = int64_t(low) * int64_t(dx);
    uint64_t high_mul = int64_t(high) * int64_t(dx);
    uint64_t high_mul_low32 = high_mul << 32; // add with low_mul must shirt right 32 bits.
    uint64_t low64;
    uint64_t high64;
    uint64_t carry;
    addWithCarry(low_mul, high_mul_low32, low64, carry);
    high64 = (high_mul >> 32) + carry;

    /*
    for query_idx < 32.
        final_result / dx = sum = sum_item * 2^{-max_bits}
        final_result = int_dx * 2^{-scale_exponet} * sum_item * 2^{-max_bits}
                     = sum_mul_dx_item * 2^{-scale_exponet - max_bits}
    for query_idx >= 32.
        (final_result - 1) / dx = sum = -sum_item * 2^{-max_bits}
        final_result = int_dx * 2^{-scale_exponet} * sum_item * 2^{-max_bits} + 1
                     = 1 - sum_mul_dx_item * 2^{-scale_exponet - max_bits}
        sum_item has a bit num: max_bits.


     scale_exponent = 23, 24; 24 for query_idx >= 32.
     bias_exp = 0,1; 1 for query_idx >= 32.
     
     prvious calculate sum_item* dx * 2^{-max_bits - scale_exponent}, 24 is for query_idx >= 32.

     dx <= 2^{24}, sum_item* dx 's bit num <= max_bits + 23 
     for query_idx >= 32, compute:
            1 - sum_item* dx * 2^{-max_bits - 24} = (2^{max_bits + 24} -  sum_item* dx ) * 2^{-max_bits - 24}
            = (~(sum_item* dx) + 1) * 2^{-max_bits - 24}.
         for query_idx >= 1, bias_exp is 1.

         just to compute (~(sum_item* dx) + 1) * 2^{-max_bits - scale_exponent}  + org_exp.
    for query_idx < 32, compute:
         just to compute sum_item* dx * 2^{-max_bits - 23} + org_exp
   
    final high low res is  2^{-scale_exponet - max_bits} * (positive_high + positive_low)
    */
    int64_t high64_res;
    uint64_t low64_res; // low64 need 64 bits.
    carry = 0;
    if (query_idx < 32) {
        low64_res = low64;
    } else {
        low64_res = (~low64) + 1;
        if (low64_res == 0)
            carry = 1;
    }
    if (query_idx < 32) {
        high64_res = high64;
    }else {
        high64_res = (~high64) + 1 + carry;;
    }

    int64_t exp_high;
    if (query_idx < 32)
        exp_high = exp_bias << (max_bits + 23  - 64);
    else
        exp_high = (exp_bias + 1) << (max_bits + 24 -  64);
    high64_res += exp_high;

    //high64_res = high64 + exp_high;
    //low64_res = low64;
    BinaryBase tmp;
    if (high64_res < 0) {
        // if signed
        tmp.logBinary.sign = 1;
        if (low64 == 0)
            high64_res = (~high64_res) + 1;
        else
            high64_res = ~high64_res;
        low64_res = (~low64_res) + 1;
    }
    else
        tmp.logBinary.sign = 0;
    /*
     (1 + mantissa) * 2^{23} = res_man *  2^{23}  是最终的结果.
     a 为 high64_res的leading zero count.
     那么 res_man * 2^{128 - 1 - a}  约等于 high64_res + low64_res = sum_item *x
     
     那么 sum_item *x *2^{-maxbits - scale_exponet} 约等于 m * 2^{127 - a - maxbits - scale_exponet}
     so the final exponet, final_exp - 127 = 127 - a - maxbits - scale_exponet
     final_exp = 127 + 128 -a - maxbits - a - scale_exponent
    
    */
    auto res_pair = findFirst32BitsAndIndex(high64_res, low64_res);

    // we then compute res's float representation.
    uint32_t res_man = res_pair.first; // 32 bit
    /*
       use correct rounding mode.  
    */
    uint32_t res_man_25 = (res_man << 24) >> 31;
    res_man = res_man >> 8; // 24 bit


    if (res_man_25)
        res_man += 1;

    uint32_t ov_exp_bias = 0;
    uint32_t res_man_ov = res_man >> 24;
    if (res_man_ov) {
        ov_exp_bias = 1;
        res_man = res_man >> 1;
    }
    res_man &= 0x7fffffU;

    // then we just get only first 24 bit or (high64 << 64) + low64..

    tmp.logBinary.x1 = res_man >> 17;
    tmp.logBinary.x2 = (res_man << 15) >> 15;
    if(query_idx < 32)
        tmp.logBinary.exp = 127 + 127 - 23 - max_bits - res_pair.second + ov_exp_bias;
    else
        tmp.logBinary.exp = 127 + 127 - 24 - max_bits - res_pair.second + ov_exp_bias;
    float res = tmp.value;

    float gt = log2f(x);
    BinaryBase gt_bb;
    gt_bb.value = gt;

    return res;
}

