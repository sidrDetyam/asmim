//
// Created by sidr on 27.02.23.
//


#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


enum Consts {
    N = 1000000,
    COUNT_OF_CYCLES = 100000,
    HM = 500,
    CACHE_LINE_SIZE = 64
};

const int shift = CACHE_LINE_SIZE / sizeof(size_t);

static void
generate_random_single_cycle_permutation(size_t n, size_t *p) {
    srand(time(NULL));

    for (size_t i = 0; i < n; ++i) {
        p[i*shift] = i;
    }

    for (size_t i = n - 1; i >= 1; --i) {
        size_t j = (size_t) rand() % i;
        size_t k = p[i*shift];
        p[i*shift] = p[j*shift];
        p[j*shift] = k;
    }
}


uint64_t
rdtsc() {
    uint64_t x;
    __asm__ volatile ("lfence\n\t"
                      "rdtsc\n\t"
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


#define NOP_X_10(count_of_ops_x10) \
do{ \
    __asm__ __volatile__( \
    "LOOP:"               \
    "nop\n\t" \
    "nop\n\t"\
    "nop\n\t" \
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t"\
    "nop\n\t" \
    "jmp LLL\n\t"\
    :::);  \
}while(0)


static double
measure(const size_t *permutation, size_t *index, int cnt_of_cycles, int cnt_of_nop_x10) {

    size_t k = 0;
    for(int i=0; i<cnt_of_cycles; ++i){
        k = permutation[k * shift];
        //printf("%zu\n", k);
    }

    size_t curr = *index;
    uint64_t s = rdtsc();
    for (int i = 0; i < cnt_of_cycles; ++i) {
        curr = permutation[curr * shift];
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

    size_t* permutation = malloc(N * CACHE_LINE_SIZE);
    generate_random_single_cycle_permutation(N, permutation);


    //sleep(1);

    for(int i=0; i<500; ++i) {
        size_t start_index = rand() % N;
        //generate_random_single_cycle_permutation(N, permutation);
        double res = measure(permutation, &start_index, N*5, i);
        printf("Cnt of nops: %d, read latency: %f     %zu\n", i*10, res, start_index);
    }


    return 0;
}
