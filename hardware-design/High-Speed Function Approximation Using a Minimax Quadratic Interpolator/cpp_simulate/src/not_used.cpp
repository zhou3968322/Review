#include <vector>
#include <cmath>
#include "log.h"
#include "int_op.h"



// float not_used_sfu_log2(float x) {
//     uint32_t s_bit, exp, man;
//     get_sign_exp_man(x, s_bit, exp, man);
//     uint32_t s_in_man = 0;
//     uint32_t diff, diff_square;
//     get_s_in_man(Opcode::LOG, man, s_in_man, diff, diff_square);
//     uint32_t query_idx = get_query_idx(Opcode::LOG, man);
//     uint64_t taylor_items[EXPAND_ITEM_NUM] = { enhanced_log_t25_p15_q11_database[query_idx * EXPAND_ITEM_NUM],
//         log_database[query_idx * EXPAND_ITEM_NUM + 1], log_database[query_idx * EXPAND_ITEM_NUM + 2]
//     };
//     // log2(1 + x + dx) - log2(1 + x)
//     // = dx / ((1 +x�� *ln(2)) - dx^2 / 2((1 + x)^2 * ln(2)) +  1/(3 * (1+x + dx')^3 *ln(2)) dx'^3.
//     // dx is 2^-6

//     uint32_t item1_carry, item1_sign;
//     // item_calculate(taylor_items[1], 19, diff, 19, item1_carry, item1_sign);

//     // for first item, diff is 2^-6, so to ensure
//     // taylor items 1 only has max 18 bit
//     // so is sign, then be complement code of  taylor_items[1]
//     // sign_bit is bit 19. always sign
//     uint32_t t1_sign = (taylor_items[1] >> 18) & 0x1;
//     // (0x18000U | taylor_items[1]): bit 31-18:1, bit 17-0:taylor_items[1]
//     // << 1:  bit 31-19:1, bit 18-1:taylor_items[1], bit[0]:0 
//     //uint32_t t1_pad = t1_sign ? ((0x18000U | taylor_items[1]) << 1): (taylor_items[1] << 1);
//     //uint64_t combine_width = 19 + 19 + 1;
//     //uint64_t mag_num1 = 19 / 2 + 2;
//     //uint64_t tmp_res1[64] = { 0ull };
//     //for (uint32_t idx = 0; idx < mag_num1 - 1; ++idx)
//     //{
//     //    uint64_t a_pad_shift = t1_pad >> (2 * idx); 
//     //    uint64_t tmp = tmp_compute(a_width, b, b_width, a_pad_shift & 0b111);
//     //    tmp = (tmp << (2 * idx)) & ((1ull << (combine_width + 4)) - 1ull);
//     //    tmp_res1[idx + 1] = (tmp >> 1) & ((1ull << combine_width) - 1ull);

//     //    uint64_t tmp2 = (a_pad >> (2 * idx)) & 0x7;
//     //    if ((tmp2 >= 0x4) && (tmp2 <= 0x6))
//     //    {
//     //        tmp_res1[0] = tmp_res1[0] | (1ull << (2 * idx));
//     //    }
//     //}



//     // 				item_calculate(taylor_items[2], 12, diff_square, 15, item2_carry, item2_sign);
//     uint64_t debug_taylor_items[EXPAND_ITEM_NUM] = { ~(log_database[query_idx * EXPAND_ITEM_NUM]),
//         ~(log_database[query_idx * EXPAND_ITEM_NUM + 1]), ~(log_database[query_idx * EXPAND_ITEM_NUM + 2])
//     };
//     uint32_t taylor_items0 = debug_taylor_items[0];
//     uint32_t taylor_items1 = debug_taylor_items[1];
//     uint32_t taylor_items2 = debug_taylor_items[2];

//     // 1.431532865 computed efficient for:
//     // minimax(log2(1 + 1/128 + x), x = -1/128 .. 1/128, [2, 0], 1, 'err')
//     // 0.01122725575 + (1.431532865 - 0.7102338184*x)*x
//     // p = 17, q = 11, t =33.
//     // m = 6, C0:, C1:18, C2:
//     double a2 = -0.7102338184;
//     double a1 = 1.431532865;
//     double a0 = 0.01122725575;

//     uint32_t o_taylor_item0 = round(a0 * exp2(33));
//     uint32_t o_taylor_item1 = round(a1 * exp2(17));
//     uint32_t o_taylor_item2 = round(-a2 * exp2(11));

//     // if use following coefficient, no need 
//     uint32_t another_taylor_item1 = round(a1 * exp2(17));
//     double computed_taylor_items1 = (double)another_taylor_item1 / exp2(17);

//     double a2_ = a2 + a1 * exp2(6) - computed_taylor_items1 * exp2(6);
//     uint32_t another_taylor_item2 = round(-a2_ * exp2(11));
//     double computed_taylor_items2 = -(double)another_taylor_item2 / exp2(11);

//     // minimax(log2(1 + 1/128 + x) - 1.4315338134765625*x + 0.71044921875000000*x^2, x = -1/128 .. 1/128, [0, 0], 1, 'err')
//     // 0.01122725920
//     double a0_ = 0.01122725920;
//     uint32_t another_taylor_item0 = round(a0_ * exp2(33));
//     double computed_taylor_items0 = (double)another_taylor_item0 / exp2(33);

//     /*
//   minimax(log2(1 + 127/128 + x), x = -1/128 .. 1/128, [2, 0], 1, 'err');
//         0.9943534368 + (0.7241791167 - 0.1817548201 x) x

//     */
//     a2 = -0.1817548201;
//     a1 = 0.7241791167;
//     a0 = 0.9943534368;

//     o_taylor_item0 = round(-a0 * exp2(33));
//     o_taylor_item1 = round(a1 * exp2(17));
//     o_taylor_item2 = round(-a2 * exp2(11));


//     /*
//     minimax(log2(1 + 127/128.0 + x)/(127/128.0 + x), x = -1/128 .. 1/128, [2, 0], 1, 'err');
//        1.002182991 + (-0.2801974400 + 0.09921484367 x) x
//     */
//     float gt = log2f(x);
//     float tmp_gt = log2f(2 * x) / (2 * x - 1);
//     float tmp_gt1 = log2f(2 * x);
//     double x_d = double(x);
//     double tmp_gtd = log2(x_d * 2);
//     double delta = x * 2 - 1.0 - 127 / 128.0;

//     double ta2 = 0.09921484367;
//     double ta1 = -0.2801974400;
//     double ta0 = 1.002182991;
//     double res_d = fma(fma(ta2, delta, ta1), delta, ta0);
//     res_d = res_d * (2 * x - 1);
//     float res_gt1 = (float)res_d;

//     double final_res = fma(fma(double(x), 2, -1), res_d, -1);
//     float res = (float)final_res;
//     uint32_t res_i = *((uint32_t*)&res);
//     uint32_t gt_i = *((uint32_t*)&gt);
//     uint32_t res1_i = *((uint32_t*)&res_gt1);
//     uint32_t gt1_i = *((uint32_t*)&tmp_gt1);

//     /*
// minimax(log2(1 - x)/x, x = 0 .. 1/128, [2, 0], 1, 'err');
//        -1.442695047 + (-0.7213350922 - 0.4851478406 x) x

//     */
//     delta = x_d - 1;
//     ta2 = 0.472573353;
//     ta1 = -0.7212990073;
//     ta0 = 1.442695047;
//     res_d = fma(fma(ta2, delta, ta1), delta, ta0);

//     final_res = res_d * delta;
//     res = (float)final_res;
//     res_i = *((uint32_t*)&res);
//     gt_i = *((uint32_t*)&gt);
//     return 0;
// }
