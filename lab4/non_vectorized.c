//
// Created by sidr on 02.05.23.
//

#include <stdlib.h>
#include <x86gprintrin.h>
#include <fcntl.h>
#include <unistd.h>
#include <immintrin.h>
#include <string.h>
#include "stdio.h"
#include "utils.h"
#include "plain.h"
#include "pthread.h"


struct Context{

};
typedef struct Context context_t;


int main(const int argc, const char * const * argv){

    heating();
//    system("cat /proc/cpuinfo | grep Hz");

//    FILE* devNull = fopen("/dev/null", "w+");

    int devnull = open("/dev/null", O_WRONLY);

    size_t cnt = 100000000;
    data_type * a = aligned_alloc(32, sizeof(data_type) * cnt);
    data_type * b = aligned_alloc(32, sizeof(data_type) * cnt);
//    for(int i=0; i<cnt; ++i){
//        b[i] = (data_type) (rand() % 30);
//    }

    ull res = measure_copy_v(a, b, cnt);
    printf("%f\n", (double) sizeof(data_type) * (double) cnt / ((double) res / 1600000000) / 1000000000.);
    ASSERT(write(devnull, a+cnt/2, 1)==1);
}
