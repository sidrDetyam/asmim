//
// Created by sidr on 02.05.23.
//

#ifndef ASMIM_PLAIN_H
#define ASMIM_PLAIN_H

#include "utils.h"

typedef int data_type;


//naive

ull measure_copy(data_type* dst, const data_type* src, size_t cnt);

ull measure_read(const data_type* src, data_type* sum, size_t cnt);

ull measure_write(data_type* dst, data_type val, size_t cnt);


//SIMD

ull measure_copy_v(float* dst, const float* src, size_t cnt);

ull measure_read_v(const float* src, float* sum, size_t cnt);

ull measure_write_v(float* dst, float val, size_t cnt);


//memcpy memchr memset

ull measure_copy_std(char* dst, const char* src, size_t cnt);

ull measure_read_std(const char* src, char** sum, size_t cnt);

ull measure_write_std(char* dst, char val, size_t cnt);


#endif //ASMIM_PLAIN_H
