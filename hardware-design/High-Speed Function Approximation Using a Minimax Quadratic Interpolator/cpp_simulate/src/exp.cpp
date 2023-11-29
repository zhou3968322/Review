#pragma once
#include "exp.h"
float test_exp(float x) {
    const float f1 = 0.5f;
    const float f2 = 0x1.77313ap-8;
    //float f3 = fma(x, f2, f1);
    float f3 = fma_sat(x, f2, f1);
    const float f4 = 0x1.f8p+7;
    const float f5 = 0x1.800002p+23;
    float f6 = fma_round_neg_inf(f3, f4, f5);
    uint r6 = *((uint*)&f6);
    r6 <<= 23;
    float coeff = *((float*)&r6);
    const float f7 = -0x1.8000fep+23;
    float f8 = f6 + f7;
    float f9 = -f8;
    const float f10 = 0x1.715476p+0;
    float f11 = fma(x, f10, f9);
    const float f12 = 0x1.4ae0cp-26;
    float f13 = fma(x, f12, f11);
    float f14 = exp2f(f13);
    float f15 = f14 * coeff;
    return f15;
    // Reduce x

}
