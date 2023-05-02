//
// Created by sidr on 02.05.23.
//

#include <x86gprintrin.h>
#include <string.h>
#include "plain.h"

ull
measure_copy_std(char* const dst, const char * const src, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    memcpy(dst, src, cnt);

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull
measure_read_std(const char* const src, char** const sum, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    void* res = memchr(src, 'a', cnt);

    const ull finish = __rdtscp(&_);
    if(res != NULL){
        *sum = res;
    }

    return finish - start;
}

ull
measure_write_std(char* const dst, const char val, const size_t cnt){
    unsigned int _;

    const ull start = __rdtscp(&_);

    memset(dst, val, cnt);

    const ull finish = __rdtscp(&_);

    return finish - start;
}