//
// Created by sidr on 02.05.23.
//

#include <x86gprintrin.h>
#include <emmintrin.h>
#include "plain.h"


ull measure_copy(data_type* dst, const data_type* const src, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/2; ++i){
        const size_t ind = i*2;

        dst[ind] = src[ind];
        dst[ind+1] = src[ind+1];
    }

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_read(const data_type* const src, data_type* const sum, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    data_type sum_;
    for(size_t i=0; i<cnt/2; ++i){
        const size_t ind = i*2;

        sum_ += src[ind] + src[ind+1];
    }
    *sum = sum_;

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_write(data_type* const dst, const data_type val, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/2; ++i){
        const size_t ind = i*2;

        dst[ind] = val;
        dst[ind+1] = val;
    }

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_copy_v(float* const dst, const float* const src, size_t const cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/8; ++i){
        const size_t ind = i*8;

        __m128 tmp = _mm_load_ps(src + ind);
        _mm_store_ps(dst + ind, tmp);

        tmp = _mm_load_ps(src + ind+4);
        _mm_store_ps(dst + ind+4, tmp);
    }

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_read_v(const float* const src, float* const sum, const size_t cnt){
    unsigned int _;
    __m128 sum_vec = _mm_setzero_ps();

    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/8; ++i){
        const size_t ind = i*8;

        __m128 tmp = _mm_load_ps(src + ind);
        sum_vec = _mm_add_ps(sum_vec, tmp);

        tmp = _mm_load_ps(src + ind+4);
        sum_vec = _mm_add_ps(sum_vec, tmp);
    }

    const ull finish = __rdtscp(&_);
    float* sum_ = (float*)&sum_vec;
    *sum = sum_[0]+sum_[1]+sum_[2]+sum_[3];

    return finish - start;
}

ull measure_write_v(float* const dst, const float val, const size_t cnt){
    unsigned int _;
    __m128 val4 = _mm_set1_ps(val);

    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/8; ++i){
        const size_t ind = i*8;

        _mm_store_ps(dst + ind, val4);
        _mm_store_ps(dst + ind+4, val4);
    }

    const ull finish = __rdtscp(&_);

    return finish - start;
}
