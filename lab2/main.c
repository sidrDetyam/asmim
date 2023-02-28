//
// Created by sidr on 27.02.23.
//


#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define ASSERT(cond__) \
do{\
    if(!(cond__)){ \
        perror("something goes wrong..."); \
        fprintf(stderr, #cond__);                   \
        fprintf(stderr, "\n");  \
        exit(1); \
    } \
}while(0)


enum Consts {
    CACHE_MEMORY_SIZE = 300000000,
    MAX_CACHE_LINE_SIZE = 64,
    COUNT_OF_PASSES = 3,
    MN_OPS_MULTIPLIER = 1,
    MX_OPS_MULTIPLIER = 30
};


#define SHIFT(i) ((i) * MAX_CACHE_LINE_SIZE / sizeof(size_t))


static void
generate_random_single_cycle_permutation(size_t n, size_t *p) {
    srand(time(NULL));

    for (size_t i = 0; i < n; ++i) {
        p[SHIFT(i)] = i;
    }

    for (size_t i = n - 1; i >= 1; --i) {
        size_t j = (size_t) rand() % i;
        size_t k = p[SHIFT(i)];
        p[SHIFT(i)] = p[SHIFT(j)];
        p[SHIFT(j)] = k;
    }
}


static uint64_t
rdtsc() {
    uint64_t x;
    __asm__ volatile ("lfence\n\t"
                      "rdtsc\n\t"
                      "shlq $32, %%rdx\n\t"
                      "orq %%rdx, %%rax" : "=a" (x) : : "%rdx");
    return x;
}


#define OP_X_10(count_of_ops_x10) \
do{ \
    __asm__ __volatile__(          \
        ".NOP_LOOP_START:\n\t"\
        "nop\n\t"\
        "nop\n\t"\
        "nop\n\t"\
        "nop\n\t"\
        "nop\n\t"\
        "nop\n\t"\
        "nop\n\t"\
        "nop\n\t"\
        "subl $1, %%ecx\n\t"\
        "jnz .NOP_LOOP_START\n\t"\
        ".NOP_LOOP_END:\n\t"\
    :: "c" (cnt_of_ops_x10) :);\
}while(0)


static double
measure(const size_t *permutation, size_t perm_len, int cnt_of_ops_x10) {

    for(size_t i=0, curr=0; i<perm_len; ++i){
        curr = permutation[SHIFT(curr)];
    }

    uint64_t s = rdtsc();
    for (size_t i = 0, curr = rand() % perm_len; i < perm_len * COUNT_OF_PASSES; ++i) {
        curr = permutation[SHIFT(curr)];
        OP_X_10(cnt_of_ops_x10);
    }
    uint64_t f = rdtsc();

    return (double)(f-s) / (double)(perm_len * COUNT_OF_PASSES);
}


int main() {
    const size_t perm_len = CACHE_MEMORY_SIZE / MAX_CACHE_LINE_SIZE;
    size_t* permutation = malloc(perm_len * MAX_CACHE_LINE_SIZE);
    ASSERT(permutation != NULL);
    generate_random_single_cycle_permutation(perm_len, permutation);

    printf("Instructions per cache miss : avg cache miss time(clocks)\n"
           "---------------------------------------------------------\n");
    for(int i=MN_OPS_MULTIPLIER; i<MX_OPS_MULTIPLIER; ++i) {
        double clocks = measure(permutation, perm_len, i);
        printf("%27d : %f\n", i*10, clocks);
    }

    free(permutation);

    return 0;
}
