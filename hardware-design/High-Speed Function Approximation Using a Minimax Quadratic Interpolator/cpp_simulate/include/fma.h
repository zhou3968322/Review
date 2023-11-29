#pragma once
#include <cmath>
#include <cfenv>

static float fma_sat(float a, float b, float c) {
    float result = std::fma(a, b, c);
    // Assuming IEEE 754 floating point representation
    const float max_float = 0x1.fffffep127f;
    const float min_float = -max_float;
    if (result > max_float) return max_float;
    if (result < min_float) return min_float;
    return result;
}

static float fma_round_neg_inf(float a, float b, float c) {
    int originalRoundingMode = fegetround();
    fesetround(FE_DOWNWARD);
    float result = std::fma(a, b, c);
    fesetround(originalRoundingMode);  // Restore original rounding mode
    return result;
}