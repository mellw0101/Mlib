#pragma once
// clang-format off

#include <immintrin.h>
#include "def.h"
#include "Attributes.h"

namespace /* Defines. */ {
  #define __copy __inline__ __avx __attribute__((__const__, __always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __ref __inline__ __avx & __attribute__((__always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __avx_T_ref __inline__ T & __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __avx_T_ptr __inline__ T * __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __avx_v(type) __attribute__((__vector_size__(((256 / 8) * sizeof(type)) / sizeof(type)))) type
}

template<typename T>
static __inline__ constexpr __avx_v(T) __avx_set1(T __b) {
  if constexpr (std::is_same<T, char>::value) {
    return _mm256_set1_epi8(__b);
  }
  else if constexpr (std::is_same<T, short>::value) {
    return _mm256_set1_epi16(__b);
  }
  else if constexpr (std::is_same<T, int>::value) {
    return _mm256_set1_epi32(__b);
  }
  else if constexpr (std::is_same<T, long long>::value) {
    return _mm256_set1_epi64x(__b);
  }
  else if constexpr (std::is_same<T, float>::value) {
    return _mm256_set1_ps(__b);
  }
  else if constexpr (std::is_same<T, double>::value) {
    return _mm256_set1_pd(__b);
  }
}

template<typename T, typename __ret = __avx_v(T)>
static __inline__ constexpr __ret __avx_setzero(void) {
  if constexpr (std::is_same<T, char>::value) {
    return __extension__ (__ret){
      '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
      '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
      '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
      '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
    };
  }
  else if constexpr (std::is_same<T, short>::value) {
    return __extension__ (__ret){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  }
  else if constexpr (std::is_same<T, int>::value) {
    return __extension__ (__ret){ 0, 0, 0, 0, 0, 0, 0, 0 };
  }
  else if constexpr (std::is_same<T, long long>::value) {
    return __extension__ (__ret){ 0, 0, 0, 0 };
  }
  else if constexpr (std::is_same<T, float>::value) {
    return __extension__ (__ret){ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
  }
  else if constexpr (std::is_same<T, double>::value) {
    return __extension__ (__ret){ 0.0, 0.0, 0.0, 0.0 };
  }
}

template<typename T>
union __avx {
  // Data.
  __avx_v(T) __a;
  T arr[8];
  struct { T a, b, c, d, e, f, g, h; };

  // Constructors.
  __avx(T __a, T __b, T __c, T __d, T __e, T __f, T __g, T __h)
  : arr{__a, __b, __c, __d, __e, __f, __g, __h} {}

  __avx(void) : __a(__avx_setzero<T>()) {}
  __avx(T val) : __a(__avx_set1<T>(val)) {}
  __avx(__avx_v(T) __b) : __a(__b) {}

  operator __avx_v(T)(void) const {
    return __a;
  }

  __copy operator+(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a+(__avx_v(T))__b);
  }

  __copy operator-(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a-(__avx_v(T))__b);
  }

  __copy operator/(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a/(__avx_v(T))__b);
  }

  __copy operator*(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a*(__avx_v(T))__b);
  }

  __ref operator+=(__avx __b) {
    __a = (__a + __b);
    return *this;
  }
  
  __ref operator-=(__avx __b) {
    __a = (__a - __b);
    return *this;
  }

  __ref operator/=(__avx __b) {
    __a = (__a / __b);
    return *this;
  }

  __ref operator*=(__avx __b) {
    __a = (__a * __b);
    return *this;
  }

  __copy max(__avx __b) const {
    static_assert(std::is_floating_point<T>::value, "max can only be used on float or double vector.");
    if constexpr (std::is_same<T, float>::value) {
      return (__avx_v(T))__builtin_ia32_maxps256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
    else if constexpr (std::is_same<T, double>::value) {
      return (__avx_v(T))__builtin_ia32_maxpd256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
  }

  __avx_T_ref operator[](Uint index) {
    return arr[index];
  }

  __avx_T_ptr data(void) {
    return &arr[0];
  }
} __align_size(32);

namespace /* Undef. */ {
  #undef __copy
  #undef __ref
  #undef __float_ref
  #undef __float_ptr
  #undef __avx_v
}
