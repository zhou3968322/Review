#pragma once
#include <stdint.h>
#include <limits.h>
#include <cmath>


static uint32_t get_exp(float x) {
    uint32_t bits = *((uint32_t*)&x);
    uint32_t exp = (bits >> 23) & 0xFF;       // Extract the 8 exponent bits
    return exp;
}


static uint32_t get_man(float x) {
    uint32_t bits = *((uint32_t*)&x);
    uint32_t man = bits & 0x007FFFFFU;           // Extract the 23 mantissa bits
    return man;
}

static void get_sign_exp_man(float x, uint32_t &sign, uint32_t &exp, uint32_t &man) {
    uint32_t bits = *((uint32_t*)&x);
    sign = bits >> 31;       // Extract the 8 exponent bits
    exp = (bits >> 23) & 0xFF;       // Extract the 8 exponent bits
    man = bits & 0x7FFFFF;           // Extract the 23 mantissa bits
}


static bool isNotDenormal(float value) {
    return std::fpclassify(value) != FP_SUBNORMAL;
}