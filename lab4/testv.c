//
// Created by sidr on 02.05.23.
//

#include "test.h"


ull test_copy(char* dst, const char* src, size_t cnt){
    return measure_copy_v((float*) dst, (const float *) src, cnt / sizeof(float));
}

ull test_read(const char* src, char** sum, size_t cnt){
    float sum_;
    ull res = measure_read_v((const float*) src, &sum_, cnt/sizeof(float));
    if(sum_ == 0.){
        *sum = NULL;
    }
    return res;
}

ull test_write(char* dst, char val, size_t cnt){
    return measure_write_v((float*) dst, val, cnt / sizeof(float));
}