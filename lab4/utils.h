//
// Created by sidr on 02.05.23.
//

#ifndef ASMIM_UTILS_H
#define ASMIM_UTILS_H

#include "stdlib.h"
#include "stdio.h"

typedef unsigned long long ull;

#define ASSERT(cond__) \
do{\
    if(!(cond__)){ \
        perror("something goes wrong..."); \
        fprintf(stderr, #cond__);                   \
        fprintf(stderr, "\n");  \
        exit(1); \
    } \
}while(0)

void matrix_multiply(const double *a, const double *b, double *c,
                     size_t n, size_t m, size_t p);

void heating();

#endif //ASMIM_UTILS_H
