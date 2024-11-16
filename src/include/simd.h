#pragma once
// clang-format off

#include "def.h"
#include "Attributes.h"
#include "Init_list.h"

#include <immintrin.h>

namespace /* Defines. */ {
  #define __copy __inline__ __avx __attribute__((__const__, __always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __ref __inline__ __avx & __attribute__((__always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __avx_T_ref __inline__ T & __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __avx_T_ptr __inline__ T * __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __avx_v(type) __attribute__((__vector_size__((256 / 8)))) type
  #define __avx_v_size(type) ((256 / 8) / sizeof(type))
  #define __avx_func static __inline__ constexpr __avx_v(T) __attribute__((__const__, __always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __assert_avx_T \
    static_assert( \
      std::is_same<T, char>  ::value || std::is_same<T, short>    ::value || \
      std::is_same<T, int>   ::value || std::is_same<T, long long>::value || \
      std::is_same<T, long>  ::value || std::is_same<T, Uchar>    ::value || \
      std::is_same<T, Ushort>::value || std::is_same<T, Uint>     ::value || \
      std::is_same<T, Ulong> ::value || std::is_same<T, float>    ::value || \
      std::is_same<T, double>::value,   \
      "Unsupported type for __avx. Use char, short, int, long long, float, or double." \
    )

  #define __assert_floating_point static_assert(std::is_floating_point<T>::value, "Can only be used on float or double vector.")
}

template<typename T>
__avx_func __avx_set1(T __b) {
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
__avx_func __avx_setzero(void) {
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
struct __avx {
  __assert_avx_T;

  union /* Data */ {
    __avx_v(T) __a;
    T __arr[__avx_v_size(T)];
  };

  /* Constructors. */
  __avx<float>(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
  : __a{v0, v1, v2, v3, v4, v5, v6, v7} {}

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
  
  __copy operator>=(__avx __b) const {
    static_assert(std::is_floating_point<T>::value, "Must be floating point vec.");
    if constexpr (std::is_same<T, float>::value) {
      return _mm256_cmp_ps(__a, __b, _CMP_GE_OQ);
    }
    else if constexpr (std::is_same<T, double>::value) {
      return _mm256_cmp_pd(__a, __b, _CMP_GE_OQ);
    }
  }

  __copy operator<=(__avx __b) const {
    static_assert(std::is_floating_point<T>::value, "Must be floating point vec.");
    if constexpr (std::is_same<T, float>::value) {
      return _mm256_cmp_ps(__a, __b, _CMP_LE_OQ);
    }
    else if constexpr (std::is_same<T, double>::value) {
      return _mm256_cmp_pd(__a, __b, _CMP_LE_OQ);
    }
  }

  __copy operator&(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a & (__avx_v(T))__b);
  }

  __copy operator|(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a | (__avx_v(T))__b);
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

  __copy min(__avx __b) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_minps256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_minpd256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
  }

  __copy max(__avx __b) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_maxps256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_maxpd256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
  }

  __copy sqrt(void) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_sqrtps256((__avx_v(T))__a);
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_sqrtpd256((__avx_v(T))__a);
    }
  }

  /* Add the even-indexed values and subtracts the odd-indexed values. */
  __copy add_sub(__avx __b) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_addsubps256((__avx_v(T))__a, (__avx_v(T))__b);
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_addsubpd256((__avx_v(T))__a, (__avx_v(T))__b);
    }
  }

  __avx_T_ref operator[](Uint index) {
    return __arr[index];
  }

  const __avx_T_ref operator[](Uint index) const {
    return __arr[index];
  }

  __avx_T_ptr data(void) {
    return &__arr[0];
  }

  const __avx_T_ptr data(void) const {
    return &__arr[0];
  }
} __align_size(32);

namespace /* Undef. */ {
  #undef __copy
  #undef __ref
  #undef __avx_T_ref
  #undef __avx_T_ptr
  #undef __avx_v
  #undef __avx_func
  #undef __assert_avx_T
  #undef __assert_floating_point
}

inline Ulong SSE2_strlen(const char *str) {
  const __m128i zero = _mm_setzero_si128();
  const char *ptr = str;
  while (true) {
    __m128i chunk = _mm_loadu_si128((const __m128i *)ptr);
    __m128i result = _mm_cmpeq_epi8(chunk, zero);
    int mask = _mm_movemask_epi8(result);
    if (mask) {
      return ((ptr - str) + __builtin_ctz(mask));
    }
    ptr += 16;
  }
}