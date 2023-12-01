#define _USE_MATH_DEFINES

#include <iostream>
#include <random>
#include <iomanip>
#include <vector>
#include "log.h"
#include "sin.h"


void test_log2_1ulp() {
    const int N = 0x4000000U;  // Number of random tests, 2^24.
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> distribution(0, UINT_MAX);  // Range for random floats

    int failedCount = 0;
    int testCount = 0;
    int max_error = 0;
    int max_error_count = 0;
    uint32_t maxBRandomValue = 0;
    while(testCount < N) {
        // uint32_t brandomValue = distribution(generator);
        uint32_t brandomValue = 0x22ae9ef6;
        float randomValue = *((float*)&brandomValue);
        // float randomValue = 0x1.1p+1;
        //float randomValue = 1.91290304e+09;
        float resultStandard = log2f(randomValue);
        if (!isNotDenormal(randomValue) || !isNotDenormal(resultStandard) || isnan(randomValue) || isnan(resultStandard)
            || isinf(randomValue) || isinf(resultStandard))
            continue;
        testCount++;
        float resultCustom;
        resultCustom = log2_ulp1(randomValue, 25);

        int bresultStandard = *((int*)&resultStandard);
        int bresultCustom = *((int*)&resultCustom);
        int error = abs(bresultStandard - bresultCustom);
        float relative_error = fabsf(resultCustom - resultStandard)/ fabsf(resultStandard);
        if (error > max_error) {
            max_error = error;
            maxBRandomValue = brandomValue;
        }
        if (error == 3)
            max_error_count++;
        if (error >= 3) {  // You can adjust this threshold based on acceptable error
            float resultCustom = log2_ulp1(randomValue, 25, 15, 11);
            std::cout << "Value: " << randomValue
                << " | expf: " << resultStandard
                << " | my_exp: " << resultCustom
                << " | Error: " << error << std::endl;
            failedCount++;
        }
        if (testCount % 0x10000 == 0) {
            printf("testing group:%d\n", testCount / 0x10000);
        }
    }

    std::cout << "Total tests: " << N << std::endl;
    std::cout << "Failed tests: " << failedCount << std::endl;
    std::cout << "max_error: " << max_error << std::endl;
    std::cout << "max_error_count: " << max_error_count << std::endl;
    std::cout << "maxBRandomValue: " << std::hex << maxBRandomValue << std::endl;
}


void test_log2_nvidia() {
    const int N = 0x4000000U;  // Number of random tests, 2^24.
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> distribution(0, UINT_MAX);  // Range for random floats

    int failedCount = 0;
    int testCount = 0;
    int max_error = 0;
    int max_error_count = 0;
    uint32_t maxBRandomValue = 0;
    while (testCount < N) {
        uint32_t brandomValue = distribution(generator);
        // uint32_t brandomValue = 0x22ae9ef6;
        float randomValue = *((float*)&brandomValue);
        // float randomValue = 0x1.1p+1;
        //float randomValue = 1.91290304e+09;
        float resultStandard = log2f(randomValue);
        if (!isNotDenormal(randomValue) || !isNotDenormal(resultStandard) || isnan(randomValue) || isnan(resultStandard)
            || isinf(randomValue) || isinf(resultStandard))
            continue;
        //if (randomValue >= 2 || randomValue < 1)
        //    continue;
        testCount++;
        float resultCustom;
        resultCustom = log2f_as_nvidia(randomValue);

        float abs_error = fabsf(resultCustom - resultStandard);
        bool condition1 = randomValue >= 0.5 && randomValue <= 2 && abs_error <= exp2f(-22);
        int bresultStandard = *((int*)&resultStandard);
        int bresultCustom = *((int*)&resultCustom);
        int error = abs(bresultStandard - bresultCustom);
        bool condition2 = randomValue < 0.5 || randomValue > 2 && error <= 2;
        if (! condition1 && !condition2) {  // You can adjust this threshold based on acceptable error
            float resultCustom = log2f_as_nvidia(randomValue);
            std::cout << "Value: " << randomValue
                << " | expf: " << resultStandard
                << " | my_exp: " << resultCustom
                << " | Error: " << abs_error << std::endl;
            failedCount++;
        }
        if (testCount % 0x10000 == 0) {
            printf("testing group:%d\n", testCount / 0x10000);
        }
    }

    std::cout << "Total tests: " << N << std::endl;
    std::cout << "Failed tests: " << failedCount << std::endl;
    std::cout << "max_error: " << max_error << std::endl;
    std::cout << "max_error_count: " << max_error_count << std::endl;
    std::cout << "maxBRandomValue: " << std::hex << maxBRandomValue << std::endl;
}


void test_sinf() {
    const int N = 0x8000000U;  // Number of random tests, 2^24.
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> distribution(0, UINT_MAX);  // Range for random floats

    int failedCount = 0;
    int testCount = 0;
    int max_error = 0;
    int max_error_count = 0;
    uint32_t maxBRandomValue = 0;
    while (testCount < N) {
        uint32_t brandomValue = distribution(generator);
        // uint32_t brandomValue = 0x22ae9ef6;
        float randomValue = *((float*)&brandomValue);
        // float randomValue = 0x1.1p+1;
        //float randomValue = 1.91290304e+09;
        float resultStandard = sinf(randomValue);
        // float resultStandard = sinf(randomValue);
        if (!isNotDenormal(randomValue) || !isNotDenormal(resultStandard) || isnan(randomValue) || isnan(resultStandard)
            || isinf(randomValue) || isinf(resultStandard))
            continue;
        if (abs(randomValue) > M_PI)
            continue;
        testCount++;
        float resultCustom;
        resultCustom = sinpidiv4_abs2_neg22(4 / M_PI * randomValue);

        float abs_error = fabsf(resultCustom - resultStandard);
        float abs_error_log2 = log2f(abs_error);
        bool condition1 = abs_error <= exp2f(-21.19);
        if (!condition1) {  // You can adjust this threshold based on acceptable error
            float resultCustom = sinpidiv4_abs2_neg22(4 / M_PI * randomValue);
            std::cout << "Value: " << randomValue
                << " | sin: " << resultStandard
                << " | my_sin: " << resultCustom
                << " | Error: " << abs_error << std::endl;
            failedCount++;
        }
        if (testCount % 0x10000 == 0) {
            printf("testing group:%d\n", testCount / 0x10000);
        }
    }

    std::cout << "Total tests: " << N << std::endl;
    std::cout << "Failed tests: " << failedCount << std::endl;
    std::cout << "max_error: " << max_error << std::endl;
    std::cout << "max_error_count: " << max_error_count << std::endl;
    std::cout << "maxBRandomValue: " << std::hex << maxBRandomValue << std::endl;
}

void test_cosf() {
    const int N = 0x8000000U;  // Number of random tests, 2^24.
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> distribution(0, UINT_MAX);  // Range for random floats

    int failedCount = 0;
    int testCount = 0;
    int max_error = 0;
    int max_error_count = 0;
    uint32_t maxBRandomValue = 0;
    while (testCount < N) {
        uint32_t brandomValue = distribution(generator);
        // uint32_t brandomValue = 0x22ae9ef6;
        float randomValue = *((float*)&brandomValue);
        // float randomValue = 0x1.1p+1;
        //float randomValue = 1.91290304e+09;
        float resultStandard = cosf(randomValue);
        // float resultStandard = sinf(randomValue);
        if (!isNotDenormal(randomValue) || !isNotDenormal(resultStandard) || isnan(randomValue) || isnan(resultStandard)
            || isinf(randomValue) || isinf(resultStandard))
            continue;
        if (abs(randomValue) > M_PI )
            continue;
        testCount++;
        float resultCustom;
        resultCustom = cospidiv4_abs2_neg22(4 / M_PI * randomValue);

        float abs_error = fabsf(resultCustom - resultStandard);
        float abs_error_log2 = log2f(abs_error);
        bool condition1 = abs_error <= exp2f(-21.19);
        if (!condition1) {  // You can adjust this threshold based on acceptable error
            float resultCustom = cospidiv4_abs2_neg22(4 / M_PI * randomValue);
            std::cout << "Value: " << randomValue
                << " | sin: " << resultStandard
                << " | my_sin: " << resultCustom
                << " | Error: " << abs_error << std::endl;
            failedCount++;
        }
        if (testCount % 0x10000 == 0) {
            printf("testing group:%d\n", testCount / 0x10000);
        }
    }

    std::cout << "Total tests: " << N << std::endl;
    std::cout << "Failed tests: " << failedCount << std::endl;
    std::cout << "max_error: " << max_error << std::endl;
    std::cout << "max_error_count: " << max_error_count << std::endl;
    std::cout << "maxBRandomValue: " << std::hex << maxBRandomValue << std::endl;
}

int main() {
    test_cosf();
    return 0;
}