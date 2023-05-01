//
// Created by sidr on 02.05.23.
//

#ifndef ASMIM_TEST_H
#define ASMIM_TEST_H

#include "plain.h"

ull test_copy(char* dst, const char* src, size_t cnt);

ull test_read(const char* src, char** sum, size_t cnt);

ull test_write(char* dst, char val, size_t cnt);


#endif //ASMIM_TEST_H
