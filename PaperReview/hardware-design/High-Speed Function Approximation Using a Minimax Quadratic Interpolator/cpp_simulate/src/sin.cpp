#define _USE_MATH_DEFINES
#include <vector>
#include "sin.h"
#include "int_op.h"


/*

 we compute sin(pi /4 *  x)
abs error < 2^{-22}
for sin x = sin (pi /4 * 4/pi * x)
the maximum absolute error maybe 2^{-21.41}.
has tested about 2^22 times.

*/
float sinpidiv4_abs2_neg22(float x, const int m, const int t, const int p, const int q) {


    //float gt = sin(M_PI / 4 * x);
    //uint32_t gt_b = *((uint32_t*)&gt);
    // 
    /*
    first normalize x/ 2pi
    */
    BinaryBase bb;
    bb.value = x;
    uint32_t dx = (bb.logBinary.x1 << 17) + bb.logBinary.x2;
    dx |= 0x800000U;
    if (bb.logBinary.exp >= 127)
        dx = dx << (bb.logBinary.exp - 127);
    else if (bb.logBinary.exp >= 103)
        // dx >> 32 is dx in msvc.
        dx = dx >> (127 - bb.logBinary.exp);
    else
        dx = 0;
    //if (dx == 0)
    //    return 0.0f;
    bool bit_24 = (dx >> 23) & 0x1; // pi /4  has or not.
    bool bit_25 = (dx >> 24) & 0x1; // pi /2  has or not.
    bool bit_26 = (dx >> 25) & 0x1; // pi has or not.
    /*
         if has pi / 4,
         sin(pi /4 +  pi / 4 * dx)= cos(pi /4 * (1 - dx))
                                  = cos(pi /4 * ~dx)
    */

    bool is_singend = bb.logBinary.sign ^ bit_26;
    bool use_sin = (bit_24 ^ bit_25) == 0;
    bool use_complement = bit_24;
    if (use_complement)
        dx = (~dx) + 1;
    // get dx bit 23-19, 5 bit.
    BinaryBase tmp;
    tmp.logBinary.sign = is_singend;

    uint32_t x1 = (dx << 9) >> 27;
    uint32_t query_idx = x1;
    uint32_t x2 = (dx << 14) >> 14;


    /*
    * scale_x = x / 2pi
    * compute sin(x) \simeq sin(2pi * scale_x)
    * scale_x's integer
    */


    uint64_t taylor_items[EXPAND_ITEM_NUM];

    if (m == 5 && t == 23 && p == 15 && q == 11) {
        if (use_sin) {
            taylor_items[0] = enhanced_sin_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM];
            taylor_items[1] = enhanced_sin_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 1];
            taylor_items[2] = enhanced_sin_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 2];
        }
        else {
            taylor_items[0] = enhanced_cos_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM];
            taylor_items[1] = enhanced_cos_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 1];
            taylor_items[2] = enhanced_cos_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 2];
        }
    }
    else
        assert(0 && "errro coefficients");

    // t = 23;p=15;q=11;
    // 2^{-t} * C0 + 2^{-p}* C1 * x2 + 2^{-q} * C2 * x2^2 = sum
    // t = 23           p + 23 = 38            q + 46 = 57
    // error < 2^{-23}
    int max_bits = std::max(t, p + 23);
    max_bits = std::max(max_bits, q + 46);
    int64_t second_order_item = int64_t(taylor_items[2] * x2) * int64_t(x2);

    // as table 15 bit * 17 bit, negative
    int64_t first_order_item = taylor_items[1] * int64_t(x2);

    // as table only 26 bit
    int64_t zero_order_item = taylor_items[0];

    // 0 <= sum < 1
    // calculate 0 <= sum * 2^max_bits < 2^max_bits
    // only have less than max_bits.
    // sum_item * 2^{-max_bits} is the final result.
    int64_t sum_item;
    if (use_sin)
        sum_item = (zero_order_item << (max_bits - t)) + (first_order_item << (max_bits - p - 23)) - (second_order_item << (max_bits - q - 46));
    else
        sum_item = (zero_order_item << (max_bits - t)) - (first_order_item << (max_bits - p - 23)) - (second_order_item << (max_bits - q - 46));

    /*
    for error < 2^{-22}.
    previous approximation error < 2^{-23}
    current round_error < 2^{-23}

    |result - sum_item * 2^{-max_bits} | < 2^{-23}
    |round(sum_item) - sum_item | * 2{-max-bits} < 2^{-23}
    |round(sum_item) - sum_item | <= 2^{maxbits-23}
    get 23 bits from max_bits->max_bits
    */


    int64_t res_int = sum_item;
    // res_int bit num: max_bits + 1 -> exp: 0.
    // res_int bit num: 64 - index -> exp: 63 - index - max_bits.
    // res_man * 2^{63 - index} * 2^{}
    uint32_t res_man = 0;
    uint32_t res_exp;
    if (sum_item == 0){
        res_exp = 1;
    }else {
        auto res_pair = findFirst24BitsAndIndex(res_int);
        // we then compute res's float representation.
        res_man = res_pair.first; // 24 bit
        res_exp = 127 + 63 - res_pair.second - max_bits;
    }
    res_man &= 0x7fffffU; //get 23 bit.
    tmp.logBinary.x1 = res_man >> 17;
    tmp.logBinary.x2 = (res_man << 15) >> 15;
    tmp.logBinary.exp = res_exp;
    float res = tmp.value;
    //uint32_t res_b = *((uint32_t*)&res);
    //float abs_error_log2 = log2f(fabsf(res - gt));
    return res;
}


/*

 we compute cos(pi /4 *  x)

 abs error < 2^{-22}

for cos x = cos (pi /4 * 4/pi * x)
maximum absolute is maybe error is maybe 2^{-21.19}.
has tested about 2^22 times.

*/
float cospidiv4_abs2_neg22(float x, const int m, const int t, const int p, const int q) {


    //float gt = sin(M_PI / 4 * x);
    //uint32_t gt_b = *((uint32_t*)&gt);
    // 
    /*
    first normalize x/ 2pi
    */
    BinaryBase bb;
    bb.value = x;
    uint32_t dx = (bb.logBinary.x1 << 17) + bb.logBinary.x2;
    dx |= 0x800000U;
    if (bb.logBinary.exp >= 127)
        dx = dx << (bb.logBinary.exp - 127);
    else if (bb.logBinary.exp >= 103)
        // dx >> 32 is dx in msvc.
        dx = dx >> (127 - bb.logBinary.exp);
    else
        dx = 0;
    //if (dx == 0)
    //    return 0.0f;
    bool bit_24 = (dx >> 23) & 0x1; // pi /4  has or not.
    bool bit_25 = (dx >> 24) & 0x1; // pi /2  has or not.
    bool bit_26 = (dx >> 25) & 0x1; // pi has or not.
    /*
         if has pi / 4,
         sin(pi /4 +  pi / 4 * dx)= cos(pi /4 * (1 - dx))
                                  = cos(pi /4 * ~dx)
    */

    bool is_singend = bit_26 ^ bit_25;
    bool use_sin = bit_24 ^ bit_25;
    bool use_complement = bit_24;
    if (use_complement)
        dx = (~dx) + 1;
    // get dx bit 23-19, 5 bit.
    BinaryBase tmp;
    tmp.logBinary.sign = is_singend;

    uint32_t x1 = (dx << 9) >> 27;
    uint32_t query_idx = x1;
    uint32_t x2 = (dx << 14) >> 14;


    /*
    * scale_x = x / 2pi
    * compute sin(x) \simeq sin(2pi * scale_x)
    * scale_x's integer
    */


    uint64_t taylor_items[EXPAND_ITEM_NUM];

    if (m == 5 && t == 23 && p == 15 && q == 11) {
        if (use_sin) {
            taylor_items[0] = enhanced_sin_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM];
            taylor_items[1] = enhanced_sin_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 1];
            taylor_items[2] = enhanced_sin_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 2];
        }
        else {
            taylor_items[0] = enhanced_cos_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM];
            taylor_items[1] = enhanced_cos_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 1];
            taylor_items[2] = enhanced_cos_t23_p15_q11_database[query_idx * EXPAND_ITEM_NUM + 2];
        }
    }
    else
        assert(0 && "errro coefficients");

    // t = 23;p=15;q=11;
    // 2^{-t} * C0 + 2^{-p}* C1 * x2 + 2^{-q} * C2 * x2^2 = sum
    // t = 23           p + 23 = 38            q + 46 = 57
    // error < 2^{-23}
    int max_bits = std::max(t, p + 23);
    max_bits = std::max(max_bits, q + 46);
    int64_t second_order_item = int64_t(taylor_items[2] * x2) * int64_t(x2);

    // as table 15 bit * 17 bit, negative
    int64_t first_order_item = taylor_items[1] * int64_t(x2);

    // as table only 26 bit
    int64_t zero_order_item = taylor_items[0];

    // 0 <= sum < 1
    // calculate 0 <= sum * 2^max_bits < 2^max_bits
    // only have less than max_bits.
    // sum_item * 2^{-max_bits} is the final result.
    int64_t sum_item;
    if (use_sin)
        sum_item = (zero_order_item << (max_bits - t)) + (first_order_item << (max_bits - p - 23)) - (second_order_item << (max_bits - q - 46));
    else
        sum_item = (zero_order_item << (max_bits - t)) - (first_order_item << (max_bits - p - 23)) - (second_order_item << (max_bits - q - 46));

    /*
    for error < 2^{-22}.
    previous approximation error < 2^{-23}
    current round_error < 2^{-23}

    |result - sum_item * 2^{-max_bits} | < 2^{-23}
    |round(sum_item) - sum_item | * 2{-max-bits} < 2^{-23}
    |round(sum_item) - sum_item | <= 2^{maxbits-23}
    get 23 bits from max_bits->max_bits
    */


    int64_t res_int = sum_item;
    // res_int bit num: max_bits + 1 -> exp: 0.
    // res_int bit num: 64 - index -> exp: 63 - index - max_bits.
    // res_man * 2^{63 - index} * 2^{}
    uint32_t res_man = 0;
    uint32_t res_exp;
    if (sum_item == 0) {
        res_exp = 1;
    }
    else {
        auto res_pair = findFirst24BitsAndIndex(res_int);
        // we then compute res's float representation.
        res_man = res_pair.first; // 24 bit
        res_exp = 127 + 63 - res_pair.second - max_bits;
    }
    res_man &= 0x7fffffU; //get 23 bit.
    tmp.logBinary.x1 = res_man >> 17;
    tmp.logBinary.x2 = (res_man << 15) >> 15;
    tmp.logBinary.exp = res_exp;
    float res = tmp.value;
    //uint32_t res_b = *((uint32_t*)&res);
    //float abs_error_log2 = log2f(fabsf(res - gt));
    return res;
}
