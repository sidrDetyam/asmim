
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

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

enum Consts{
    N = 1000
};

static void heating(){
    double* a = malloc(sizeof(double) * N * N);
    double* b = malloc(sizeof(double) * N * N);
    double* c = malloc(sizeof(double) * N * N);
    matrix_multiply(a, b, c, N, N, N);
    free(a);
    free(b);
    free(c);
}

extern double measure_latency(int32_t);

int main() {
    heating();
    printf("%f\n", measure_latency(1000000));
    return 0;
}
