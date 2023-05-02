//
// Created by sidr on 02.05.23.
//

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <spawn.h>
#include "stdio.h"
#include "utils.h"
#include "pthread.h"
#include "test/test.h"


enum Consts{
    ALIGNMENT = 1024
};


typedef ull(*operation_t)(char*, char*, size_t);


struct Context{
    pthread_barrier_t *barrier;
    ull res;
    int cpu_id;
    size_t data_size;
    operation_t operation;
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

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(context->cpu_id, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

    char * a = aligned_alloc(ALIGNMENT, context->data_size);
    char * b = aligned_alloc(ALIGNMENT, context->data_size);
    memset(a, 't', context->data_size);
    memset(b, 't', context->data_size);

    pthread_barrier_wait(context->barrier);
    context->operation(a, b, context->data_size);
    context->res = context->operation(a, b, context->data_size);
    context->operation(a, b, context->data_size);

    free(a);
    free(b);
    return NULL;
}


const static size_t data_size = 1<<27;

static void
test_operation(const char* const description,
               const operation_t operation,
               const size_t t_count,
               const double freq){

    printf("  %s ", description);

    pthread_t* const threads = malloc(sizeof(pthread_t) * t_count);
    context_t* const contexts = malloc(sizeof(context_t) * t_count);
    ASSERT(threads && contexts);
    pthread_barrier_t barrier;

    ASSERT(pthread_barrier_init(&barrier, NULL, t_count) == 0);
    for(size_t i=0; i<t_count; ++i){
        contexts[i].data_size = data_size;
        contexts[i].operation = operation;
        contexts[i].barrier = &barrier;
        contexts[i].cpu_id = (int)i;

        ASSERT(pthread_create(threads+i, NULL, subroutine, contexts+i)==0);
    }

    ull total_clocks = 0;
    for(size_t i=0; i<t_count; ++i){
        ASSERT(pthread_join(threads[i], NULL) == 0);
        total_clocks += contexts[i].res;
    }

    free(threads);
    free(contexts);

    printf("%f;", (double)(t_count * data_size) / ((double)total_clocks / freq));
}


int main(const int argc, const char * const * argv){
    ASSERT(argc == 3);

    char* endptr;
    const long t_cnt = strtol(argv[1], &endptr, 10);
    const double freq = strtod(argv[2], &endptr);

    heating();
//    system("cat /proc/cpuinfo | grep Hz");

    test_operation("copy", copy_operation, t_cnt, freq);
    test_operation("read", read_operation, t_cnt, freq);
    test_operation("write", write_operation, t_cnt, freq);
//    printf("\n");

    return 0;
}
