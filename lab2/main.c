//
// Created by sidr on 27.02.23.
//


#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


enum Consts {
    N = 100000,
    COUNT_OF_CYCLES = 10000,
    HM = 500,
    CACHE_LINE_SIZE = 64
};


static void
generate_random_single_cycle_permutation(size_t n, size_t *p) {
    srand(time(NULL));

    for (size_t i = 0; i < n; ++i) {
        p[i] = i;
    }

    for (size_t i = n - 1; i >= 1; --i) {
        size_t j = (size_t) rand() % i;
        size_t k = p[i];
        p[i] = p[j];
        p[j] = k;
    }
}


uint64_t
rdtsc() {
    uint64_t x;
    __asm__ volatile ("lfence\n\t"
                      "rdtsc\n\t"
                      "lfence\n\t"
                      "shlq $32, %%rdx\n\t"
                      "orq %%rdx, %%rax" : "=a" (x) : : "%rdx");
    return x;
}


int summ(int a, int b) {
    int res;
    __asm__ __volatile__("movl %1, %%eax\n\t"
                         "addl %2, %%eax\n\t"
                         "movl %%eax, %0\n\t"
                         "movl $1, %%eax": "=r" (res) : "r" (a), "r" (b) : "%eax");

    return res;
}


#define NOP_X_10() \
do{ \
    __asm__ __volatile__( \
    "nop\n\t" \
    "nop\n\t"\
    "nop\n\t" \
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    :::);  \
}while(0)


static double
measure(const size_t *permutation, size_t *index, int cnt_of_cycles, int cnt_of_nop_x10) {

    size_t curr = *index;
    uint64_t s = rdtsc();
    for (int i = 0; i < cnt_of_cycles; ++i) {
        curr = permutation[curr];
        for (int j = 0; j < cnt_of_nop_x10; ++j) {
            NOP_X_10();
        }
    }
    uint64_t f = rdtsc();

    *index = curr;
    return (double)(f-s) / cnt_of_cycles;
}


static void matrix_multiply(const double *a, const double *b, double *c,
                            size_t n, size_t m, size_t p) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            double sum = 0.0;
            for (int k = 0; k < m; k++) {
                sum += a[i * m + k] * b[k * p + j];
            }
            c[i * p + j] = sum;
        }
    }
}


static void heating(){
    double* a = malloc(sizeof(double) * HM * HM);
    double* b = malloc(sizeof(double) * HM * HM);
    double* c = malloc(sizeof(double) * HM * HM);
    matrix_multiply(a, b, c, HM, HM, HM);
    free(a);
    free(b);
    free(c);
}


int main() {
    //heating();

    size_t* permutation = malloc(sizeof(size_t) * N * CACHE_LINE_SIZE);
    generate_random_single_cycle_permutation(N, permutation);

    size_t start_index = N-1;
    for(int i=0; i<50; ++i) {
        //generate_random_single_cycle_permutation(N, permutation);
        double res = measure(permutation, &start_index, 1, i);
        printf("Cnt of nops: %d, read latency: %f\n", i*10, res);
    }


    return 0;
}
