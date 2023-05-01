//
// Created by sidr on 02.05.23.
//

#include "test.h"


ull test_copy(char* dst, const char* src, size_t cnt){
    return measure_copy_std(dst, src, cnt);
}

ull test_read(const char* src, char** sum, size_t cnt){
    return measure_read_std(src, sum, cnt);
}

ull test_write(char* dst, char val, size_t cnt){
    return measure_write_std(dst, val, cnt);
}