//
// Created by sidr on 02.05.23.
//

#include "utils.h"

void matrix_multiply(const double * const a, const double * const b, double * const c,
                     const size_t n, const size_t m, const size_t p) {
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

enum Consts {
    N = 500
};

void heating() {
    double *a = malloc(sizeof(double) * N * N);
    double *b = malloc(sizeof(double) * N * N);
    double *c = malloc(sizeof(double) * N * N);
    matrix_multiply(a, b, c, N, N, N);
    free(a);
    free(b);
    free(c);
}