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
#include "pthread.h"
#include "test/test.h"


enum Consts{
    ALIGNMENT = 64
};


struct Context{
    pthread_barrier_t *barrier;
    ull res;
    size_t data_size;
    ull(*operation)(char*, char*, size_t);
};
typedef struct Context context_t;


static ull
read_operation(char* a, char* b, size_t cnt){
    return test_read(a, &b, cnt);
}

static ull
write_operation(char* a, char* b, size_t cnt){
    return test_write(a, b[0], cnt);
}

static ull
copy_operation(char* a, char* b, size_t cnt){
    return test_copy(a, b, cnt);
}

static void *
subroutine(void * const arg) {
    context_t* const context = (context_t*) arg;

    char * a = aligned_alloc(ALIGNMENT, context->data_size);
    char * b = aligned_alloc(ALIGNMENT, context->data_size);
    memset(a, 't', context->data_size);
    memset(b, 't', context->data_size);

    //pthread_barrier_wait(context->barrier);
    context->operation(a, b, context->data_size);
    context->res = context->operation(a, b, context->data_size);
    context->operation(a, b, context->data_size);

    free(a);
    free(b);
    return NULL;
}


const static size_t data_size = 1<<27;


int main(const int argc, const char * const * argv){

    heating();
////    system("cat /proc/cpuinfo | grep Hz");
//
////    FILE* devNull = fopen("/dev/null", "w+");
//
//    int devnull = open("/dev/null", O_WRONLY);
//
//    size_t cnt = 1<<27;
//    char * a = aligned_alloc(32, cnt);
//    char * b = aligned_alloc(32, cnt);
////    for(int i=0; i<cnt; ++i){
////        b[i] = (data_type) (rand() % 30);
////    }

    context_t context;
    //pthread_barrier_init(context.barrier, NULL, 1);
    context.operation = copy_operation;
    context.data_size = data_size;

    subroutine(&context);


    ull res = context.res;
    printf("%f\n", (double) data_size / ((double) res / 1600000000) / 1000000000.);
//    ASSERT(write(devnull, a+cnt/2, 1)==1);

    return 0;
}
