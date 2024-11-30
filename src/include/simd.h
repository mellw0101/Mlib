#pragma once
/* clang-format off */

#include "def.h"
#include "Attributes.h"
#include "Init_list.h"

#include <immintrin.h>

template <typename T>
using __sse_v = __attribute__((__vector_size__(16), __aligned__((16)))) T;

namespace /* __avx defines. */ {
  #define __constructor(type, ...) __inline__ __attribute__((__always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512"))) __avx<type>(__VA_ARGS__)
  #define __copy(type) __inline__ __avx<type> __attribute__((__const__, __always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __ref __inline__ __avx & __attribute__((__always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __ref_type(type) __inline__ __avx<type> & __attribute__((__always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __avx_T_ref __inline__ T & __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __avx_T_copy __inline__ T __attribute__((__always_inline__, __nodebug__, __nothrow__))
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
  #define __asm_avx(ret) extern "C" ret __attribute__((__nodebug__, __nothrow__))
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

inline void print_m256(__m256 vec) {
  alignas(32) float data[8];
  /* Store the __m256 to memory for printing. */
  _mm256_store_ps(data, vec);
  printf("[ ");
  for (int i = 0; i < 8; ++i) {
    if (i != 7) {
      printf("%.2f, ", data[i]);
    }
    else {
      printf("%.2f", data[i]);
    }
  }
  printf(" ]\n");
}

__asm_avx(void) __asm_avx_addf(const float *__a, const float *__b, float *__r);
__asm_avx(void) __asm_avx_subf(const float *__a, const float *__b, float *__r);

template<typename T>
struct __avx {
  __assert_avx_T;

  union /* Data */ {
    __avx_v(T) __a;
    T __arr[__avx_v_size(T)];
    __sse_v<T> __s[2];
  };

  /* Constructors. */
  __constructor(float, T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
  : __a(__extension__(__avx_v(T)){v0, v1, v2, v3, v4, v5, v6, v7}) {}

  __constructor(T, void) : __a(__avx_setzero<T>()) {}
  __constructor(T, T val) : __a(__avx_set1<T>(val)) {}
  __constructor(T, __avx_v(T) __b) : __a(__b) {}
  __constructor(T, __sse_v<T> __b, __sse_v<T> __c) : __s{__b, __c} {}

  operator __avx_v(T)(void) const {
    return __a;
  }

  __copy(T) operator+(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a+(__avx_v(T))__b);
  }

  __copy(T) operator-(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a-(__avx_v(T))__b);
  }

  __copy(T) operator/(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a/(__avx_v(T))__b);
  }

  __copy(T) operator*(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a*(__avx_v(T))__b);
  }
  
  __copy(T) operator>=(__avx __b) const {
    static_assert(std::is_floating_point<T>::value, "Must be floating point vec.");
    if constexpr (std::is_same<T, float>::value) {
      return _mm256_cmp_ps(__a, __b, _CMP_GE_OQ);
    }
    else if constexpr (std::is_same<T, double>::value) {
      return _mm256_cmp_pd(__a, __b, _CMP_GE_OQ);
    }
  }

  __copy(T) operator<=(__avx __b) const {
    static_assert(std::is_floating_point<T>::value, "Must be floating point vec.");
    if constexpr (std::is_same<T, float>::value) {
      return _mm256_cmp_ps(__a, __b, _CMP_LE_OQ);
    }
    else if constexpr (std::is_same<T, double>::value) {
      return _mm256_cmp_pd(__a, __b, _CMP_LE_OQ);
    }
  }

  __copy(T) operator&(__avx __b) const {
    return (__avx_v(T))((__avx_v(T))__a & (__avx_v(T))__b);
  }

  __copy(T) operator|(__avx __b) const {
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

  __copy(T) min(__avx __b) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_minps256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_minpd256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
  }

  __copy(T) max(__avx __b) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_maxps256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_maxpd256((__avx_v(T))__a, (__avx_v(T))__b); 
    }
  }

  __copy(T) sqrt(void) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_sqrtps256((__avx_v(T))__a);
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_sqrtpd256((__avx_v(T))__a);
    }
  }

  __ref_type(float) reorder(Uchar v0, Uchar v1, Uchar v2, Uchar v3, Uchar v4, Uchar v5, Uchar v6, Uchar v7) {
    *this = {__a[v0], __a[v1], __a[v2], __a[v3], __a[v4], __a[v5], __a[v6], __a[v7]};
    return *this;
  }

  __copy(float) hadd(const __avx<float> &__b) const {
    return((__avx_v(float))__builtin_ia32_haddps256((__avx_v(float))__a, (__avx_v(float))__b.__a));
  }

  __copy(float) hsub(const __avx<float> &__b) const {
    return((__avx_v(float))__builtin_ia32_hsubps256((__avx_v(float))__a, (__avx_v(float))__b.__a));
  }

  __copy(float) hmul(const __avx<float> &__b) const {
    __avx<float> vec(__a);
    __avx<float> other(__b);
    vec.reorder(0, 2, 4, 6, 1, 3, 5, 7);
    other.reorder(0, 2, 4, 6, 1, 3, 5, 7);
    vec.__s[0]   = _mm_mul_ps(vec.__s[0], vec.__s[1]);
    other.__s[0] = _mm_mul_ps(other.__s[0], other.__s[1]);
    __avx<float> ret(
      vec.__s[0][0], vec.__s[0][1], other.__s[0][0], other.__s[0][1],
      vec.__s[0][2], vec.__s[0][3], other.__s[0][2], other.__s[0][3]
    );
    return ret;
  }

  __copy(float) hdiv(const __avx<float> &__b) const {
    __avx<float> vec(__a);
    __avx<float> other(__b);
    vec.reorder(0, 2, 4, 6, 1, 3, 5, 7);
    other.reorder(0, 2, 4, 6, 1, 3, 5, 7);
    vec.__s[0]   = _mm_div_ps(vec.__s[0], vec.__s[1]);
    other.__s[0] = _mm_div_ps(other.__s[0], other.__s[1]);
    __avx<float> ret(
      vec.__s[0][0], vec.__s[0][1], other.__s[0][0], other.__s[0][1],
      vec.__s[0][2], vec.__s[0][3], other.__s[0][2], other.__s[0][3]
    );
    return ret;
  }

  /* Add the even-indexed values and subtracts the odd-indexed values. */
  __copy(T) add_sub(__avx __b) const {
    __assert_floating_point;
    if constexpr (std::is_same_v<T, float>) {
      return (__avx_v(T))__builtin_ia32_addsubps256((__avx_v(T))__a, (__avx_v(T))__b);
    }
    else if constexpr (std::is_same_v<T, double>) {
      return (__avx_v(T))__builtin_ia32_addsubpd256((__avx_v(T))__a, (__avx_v(T))__b);
    }
  }

  __avx_T_copy reduce(void) const {
    if constexpr (std::is_same_v<T, float>) {
      __m256 vec = __a;
      __m128 low = _mm256_castps256_ps128(vec);
      __m128 high = _mm256_extractf128_ps(vec, 1);
      __m128 sum = _mm_add_ps(low, high);
      sum = _mm_hadd_ps(sum, sum);
      sum = _mm_hadd_ps(sum, sum);
      return _mm_cvtss_f32(sum);
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

namespace /* __avx undef. */ {
  #undef __constructor
  #undef __copy
  #undef __ref
  #undef __ref_type
  #undef __avx_T_ref
  #undef __avx_T_copy
  #undef __avx_T_ptr
  #undef __avx_v
  #undef __avx_func
  #undef __assert_avx_T
  #undef __assert_floating_point
  #undef __asm_avx
}

typedef __attribute__((__vector_size__(16), __aligned__((16)))) float __sse;
typedef __attribute__((__vector_size__(16))) float __v4ps;

namespace /* __sse defines. */ {
  #define __copy __inline__ __ssef __attribute__((__const__, __always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))
  #define __ref __inline__ __ssef & __attribute__((__always_inline__, __nodebug__, __nothrow__, __min_vector_width__(256), __target__("avx,no-evex512")))

  #define __sse_call              static __inline__ constexpr __sse __attribute__((__always_inline__, __nodebug__, __nothrow__, __target__("sse3,no-evex512"), __min_vector_width__(128)))
  #define __sse_call_no_constexpr static __inline__ __sse __attribute__((__always_inline__, __nodebug__, __nothrow__, __target__("sse3,no-evex512"), __min_vector_width__(128)))
  #define __sse_float             static __inline__ constexpr float __attribute__((__always_inline__, __nodebug__, __nothrow__, __target__("sse3,no-evex512"), __min_vector_width__(128)))

  #define __float_ref   __inline__ float & __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __float_copy  __inline__ float   __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __float_ptr   __inline__ float * __attribute__((__always_inline__, __nodebug__, __nothrow__))

  #define __operator(type) __inline__ operator type __attribute__((__always_inline__, __nodebug__, __nothrow__)) 
  #define __operator_ret(ret, type) __inline__ ret operator type __attribute__((__always_inline__, __nodebug__, __nothrow__)) 
}

__sse_call __ssef_set_zero(void) {
  return __extension__(__sse){ 0.0f, 0.0f, 0.0f, 0.0f };
}

__sse_call __ssef_set1(float value) {
  return __extension__(__sse){ value, value, value, value };
}

__sse_call __ssef_set(float __x, float __y, float __z, float __w) {
  return __extension__(__sse){ __x, __y, __z, __w };
}

__sse_call_no_constexpr sse_hadd(__sse __a, __sse __b) {
  return (__sse)__builtin_ia32_haddps((__v4ps)__a, (__v4ps)__b);
}

class __ssef {
 private:
  union /* Data */ {
    float data[4];
    __sse __a;
  };

 public:
  __ssef(void) : __a(__ssef_set_zero()) {}
  __ssef(__sse __b) : __a(__ssef_set(__b[0], __b[1], __b[2], __b[3])) {}
  __ssef(float value) : __a(__ssef_set1(value)) {}
  __ssef(float x, float y, float z, float w) : __a(__ssef_set(x, y, z, w)) {}

  __float_ref       operator[](Uint idx)       { return data[idx]; }
  const __float_ref operator[](Uint idx) const { return data[idx]; }
  
  __copy operator +(__ssef __b) const {
    return ((__sse)((__v4ps)__a + (__v4ps)__b));
  }

  __copy operator -(__ssef __b) const {
    return ((__sse)((__v4ps)__a - (__v4ps)__b));
  }

  __copy operator*(__ssef __b) const {
    return ((__sse)((__v4ps)__a * (__v4ps)__b));
  }

  __copy operator/(__ssef __b) const {
    return ((__sse)((__v4ps)__a / (__v4ps)__b));
  }

  __ref operator+=(__ssef __b) {
    *this = (*this + __b);
    return *this;
  }

  __ref operator-=(__ssef __b) {
    *this = (*this - __b);
    return *this;
  }
  
  __ref operator*=(__ssef __b) {
    *this = (*this * __b);
    return *this;
  }
  
  __ref operator/=(__ssef __b) {
    *this = (*this / __b);
    return *this;
  }

  __operator(__sse(void) const) {
    return __a;
  }
};

namespace /* __sse undef. */ {
  #undef __ref
  #undef __copy

  #undef __sse_call
  #undef __sse_call_no_constexpr
  #undef __sse_float

  #undef __float_ref
  #undef __float_copy
  #undef __float_ptr

  #undef __operator
  #undef __operator_ret
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