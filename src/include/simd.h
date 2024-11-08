#pragma once

#include <immintrin.h>

__always_inline __m256 simd_add(__m256 a, __m256 b) {
  return _mm256_add_ps(a, b);
}