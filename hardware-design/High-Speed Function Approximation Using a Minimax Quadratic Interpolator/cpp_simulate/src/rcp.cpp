#include "rcp.h"
#include <vector>
#include <cmath>


float eu_rcp(float x) {
    uint32_t s_bit, exp, man;
    get_sign_exp_man(x, s_bit, exp, man);
    uint32_t query_idx = get_query_idx(Opcode::LOG, man);
    uint64_t taylor_items[EXPAND_ITEM_NUM] = { rcp_database[query_idx * EXPAND_ITEM_NUM],
        rcp_database[query_idx * EXPAND_ITEM_NUM + 1], rcp_database[query_idx * EXPAND_ITEM_NUM + 2]
    };



    // 				item_calculate(taylor_items[2], 12, diff_square, 15, item2_carry, item2_sign);
    uint64_t invert_taylor_items[EXPAND_ITEM_NUM] = { ~(rcp_database[query_idx * EXPAND_ITEM_NUM]),
        ~(rcp_database[query_idx * EXPAND_ITEM_NUM + 1]), ~(rcp_database[query_idx * EXPAND_ITEM_NUM + 2])
    };

    // 1.431532865 computed efficient for:
    // minimax(1 / (1 + 1/256 + x), x = -1/256 .. 1/256, [2, 0], 1, 'err')
    // 0.9961089494 + (-0.9922443057 + 0.9883883386*x)*x
    double a2 = 0.9883883386;
    double a1 = -0.9922443057;
    double a0 = 0.9961089494;

    uint32_t o_taylor_item0 = round(a0 * exp2(28));
    uint32_t o_taylor_item1 = round(-a1 * exp2(18));
    uint32_t o_taylor_item2 = round(-a2 * exp2(11));

    // if use following coefficient, no need 
    uint32_t another_taylor_item1 = round(a1 * exp2(28));
    double computed_taylor_items1 = (double)another_taylor_item1 / exp2(28);

    double a2_ = a2 + a1 * exp2(7) - computed_taylor_items1 * exp2(7);
    uint32_t another_taylor_item2 = round(-a2_ * exp2(11));
    double computed_taylor_items2 = -(double)another_taylor_item2 / exp2(11);

    // minimax1 / (1 + 1/256 + x) -C1*x - C2*x^2,  x = -1/256 .. 1/256, [0, 0], 1, 'err')
    // ?
    //double a0_ = 0.01122725920;
    //uint32_t another_taylor_item0 = round(a0_ * exp2(33));
    //double computed_taylor_items0 = (double)another_taylor_item0 / exp2(33);
    return 0;
}