//
// Created by sidr on 02.05.23.
//

#include "test.h"

ull test_copy(char* dst, const char* src, size_t cnt){
    return measure_copy((data_type *) dst, (const data_type *) src, cnt / sizeof(data_type));
}

ull test_read(const char* src, char** sum, size_t cnt){
    data_type sum_;
    ull res = measure_read((const data_type *) src, &sum_, cnt/sizeof(data_type));
    if(sum_ == 0.){
        *sum = NULL;
    }
    return res;
}

ull test_write(char* dst, char val, size_t cnt){
    return measure_write((data_type *) dst, (data_type) val, cnt / sizeof(data_type));
}