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
    COUNT_OF_PASSES = 2,
    TESTS = 5
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


extern size_t foo(const size_t*, size_t, size_t);


static double
measure(const size_t *permutation, size_t perm_len) {

    uint64_t s = rdtsc();
    foo(permutation, rand() % perm_len, perm_len * COUNT_OF_PASSES);
    uint64_t f = rdtsc();

    return (double) (f - s) / (double) (perm_len * COUNT_OF_PASSES);
}


#define MIN(a, b) ((a)<(b)? (a) : (b))


int main(int argc, char** argv) {
    const size_t perm_len = CACHE_MEMORY_SIZE / MAX_CACHE_LINE_SIZE;
    size_t *permutation = malloc(perm_len * MAX_CACHE_LINE_SIZE);
    ASSERT(permutation != NULL);
    generate_random_single_cycle_permutation(perm_len, permutation);

    size_t curr = 0;
    for (size_t i = 0; i < perm_len * 2; ++i) {
        curr = permutation[SHIFT(curr)];
    }

    double clocks = 1e10;
    for (int i = 0; i < TESTS; ++i) {
        clocks = MIN(measure(permutation, perm_len), clocks);
    }
    printf("%27d : %f", atoi(argv[1]), clocks);

    free(permutation);

    return 0;
}
