//
// Created by sidr on 02.05.23.
//

#include <x86gprintrin.h>
#include <emmintrin.h>
//#include <xmmintrin.h>
#include "plain.h"


ull measure_copy(data_type* dst, const data_type* const src, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt; ++i){
        dst[i] = src[i];
    }

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_read(const data_type* const src, data_type* const sum, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    data_type sum_;
    for(size_t i=0; i<cnt; ++i){
        sum_ += src[i];
    }
    *sum = sum_;

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_write(data_type* const dst, const data_type val, const size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    data_type sum_;
    for(size_t i=0; i<cnt; ++i){
        dst[i] = val;
    }

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_copy_v(float* dst, const float* src, size_t cnt){
    unsigned int _;
    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/4; ++i){
        __m128 tmp = _mm_load_ps(src + i*4);
        _mm_store_ps(dst + i*4, tmp);
    }

    const ull finish = __rdtscp(&_);
    return finish - start;
}

ull measure_read_v(const float* src, float* sum, size_t cnt){
    unsigned int _;
    __m128 sum_vec = _mm_setzero_ps();

    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/4; ++i){
        __m128 tmp = _mm_load_ps(src + i*4);
        sum_vec = _mm_add_ps(sum_vec, tmp);
    }

    const ull finish = __rdtscp(&_);
    float* sum_ = (float*)&sum_vec;
    *sum_ = sum_[0]+sum_[1]+sum_[2]+sum_[3];

    return finish - start;
}

ull measure_write_v(float* dst, float val, size_t cnt){
    unsigned int _;
    __m128 val4 = _mm_set1_ps(val);

    const ull start = __rdtscp(&_);

    for(size_t i=0; i<cnt/4; ++i){
        _mm_store_ps(dst + i*4, val4);
    }

    const ull finish = __rdtscp(&_);

    return finish - start;
}
