#pragma once
#include "query.h"
#include "fp.h"

float log2f_as_nvidia(float x, const int t = 25, const int p = 15, const int q = 10);

float tmp_sfu_log2(float x, const int t = 25, const int p = 15, const int q = 11);

float log2_ulp1(float x, const int t = 25, const int p = 15, const int q = 11);
