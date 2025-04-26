#pragma once

#include "../Mint.h"
#include "../simd.h"

namespace /* Defines */ {
  #define __constructor(...) __inline__ constexpr __attribute__((__always_inline__, __nodebug__, __nothrow__)) vec(__VA_ARGS__)
  #define __ref              __inline__ constexpr vec & __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __copy             __inline__ constexpr vec __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __bool             __inline__ constexpr bool __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  #define __copy_(s, t)   __inline__ constexpr vec<s, t> __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  #define __type_ref         __inline__ constexpr T &__warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __type_T           __inline__ constexpr T __warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
  
  namespace /* Internal data */ {
    #define VEC_STRUCT_2  \
      struct { T x, y; }; \
      struct { T w, h; }; \
      T data[2];

    #define VEC_STRUCT_3     \
      struct { T x, y, z; }; \
      T data[3];

    #define VEC_STRUCT_4                                \
      struct { T x, y, z, w; };                         \
      struct { T left, right, top, bottom; };           \
      struct { T s, t, p, q; }; /* Texture cordinats */ \
      struct { T x_, y_, width, height; };              \
      struct { T r, g, b, a; };                         \
      struct { T red, green, blue, alpha; };            \
      struct { T vstart, vcount, istart, icount; };     \
      T data[4];

    #define VEC_DATA(size) \
      union {              \
        VEC_STRUCT_##size  \
      }
  }
  namespace /* Operator */ {
    #define VEC_IDX_OPERATOR \
      __type_ref       operator[](Uint idx)       { return data[idx]; } \
      const __type_ref operator[](Uint idx) const { return data[idx]; }

    #define VEC_OPERATOR(size, op)             \
      __copy operator op(const vec &v) const { \
        vec ret;                               \
        for (Uint i = 0; i < size; ++i) {      \
          ret[i] = (data[i] op v[i]);          \
        }                                      \
        return ret;                            \
      }                                        \
      __copy operator op(T value) const {      \
        vec ret;                               \
        for (Uint i = 0; i < size; ++i) {      \
          ret[i] = (data[i] op value);         \
        }                                      \
        return ret;                            \
      }                                        \
      __ref operator op##=(const vec &v) {     \
        for (Uint i = 0; i < size; ++i) {      \
          data[i] op##= v[i];                  \
        }                                      \
        return *this;                          \
      }                                        \
      __ref operator op##=(T value) {          \
        for (Uint i = 0; i < size; ++i) {      \
          data[i] op##= value;                 \
        }                                      \
        return *this;                          \
      }

      #define OP__add +
      #define OP__sub -
      #define OP__mul *
      #define OP__div /

      #define OP_add operator +
      #define OP_sub operator -
      #define OP_mul operator *
      #define OP_div operator /
      #define OP_eq_add operator +=
      #define OP_eq_sub operator -=
      #define OP_eq_mul operator *=
      #define OP_eq_div operator /=

      #define OP(op) OP_##op
      #define OP_(op) OP__##op
      #define OP_eq(op) OP_eq_##op

      #define VEC4_H_OPERATOR(op)                                                                                         \
        __copy OP(op)(const vec &other) const {                                                                           \
          __avx combined(data[0], other.data[0], data[1], other.data[1], data[2], other.data[2], data[3], other.data[3]); \
          __avx result = combined.h##op(combined);                                                                        \
          vec ret(result[0], result[1], result[4], result[5]);                                                            \
          return ret;                                                                                                     \
        }                                                                                                                 \
        __copy OP(op)(T value) const {                                                                                    \
          __avx combined(data[0], value, data[1], value, data[2], value, data[3], value);                                 \
          __avx result = combined.h##op(combined);                                                                        \
          vec ret(result[0], result[1], result[4], result[5]);                                                            \
          return ret;                                                                                                     \
        }                                                                                                                 \
        __ref OP_eq(op)(const vec &other) {                                                                               \
          *this = (*this OP_(op) other);                                                                                  \
          return *this;                                                                                                   \
        }                                                                                                                 \
        __ref OP_eq(op)(T value) {                                                                                        \
          *this = (*this OP_(op) vec(value));                                                                             \
          return *this;                                                                                                   \
        }

      #define VEC4_OPERATOR(op)             \
        __copy OP(op)(const vec &v) const { \
          vec ret;                          \
          __m128 a = _mm_load_ps(data);     \
          __m128 b = _mm_load_ps(v.data);   \
          __m128 res = _mm_##op##_ps(a, b); \
          _mm_store_ps(ret.data, res);      \
          return ret;                       \
        }                                   \
        __copy OP(op)(T value) const {      \
          vec ret;                          \
          __m128 a = _mm_load_ps(data);     \
          __m128 b = _mm_set1_ps(value);    \
          __m128 res = _mm_##op##_ps(a, b); \
          _mm_store_ps(ret.data, res);      \
          return ret;                       \
        }                                   \
        __ref OP_eq(op)(const vec &v) {     \
          __m128 a = _mm_load_ps(data);     \
          __m128 b = _mm_load_ps(v.data);   \
          __m128 res = _mm_##op##_ps(a, b); \
          _mm_store_ps(data, res);          \
          return *this;                     \
        }                                   \
        __ref OP_eq(op)(T value) {          \
          __m128 a = _mm_load_ps(data);     \
          __m128 b = _mm_set1_ps(value);    \
          __m128 res = _mm_##op##_ps(a, b); \
          _mm_store_ps(data, res);          \
          return *this;                     \
        }
  }
  namespace /* Swizzling */ {
    #define VEC_SWIZZLING_2                       \
      __copy xx(void) const { return vec(x, x); } \
      __copy yy(void) const { return vec(y, y); } \
      __copy xy(void) const { return vec(x, y); } \
      __copy yx(void) const { return vec(y, x); }
    
    #define VEC_SWIZZLING_3                                    \
      __copy_(2, T) xx(void)  const { return vec<2, T>(x, x); } \
      __copy_(2, T) yy(void)  const { return vec<2, T>(y, y); } \
      __copy_(2, T) xy(void)  const { return vec<2, T>(x, y); } \
      __copy_(2, T) yx(void)  const { return vec<2, T>(y, x); } \
      __copy_(3, T) xxx(void) const { return vec<3, T>(x, x, x); } \
      __copy_(3, T) yyy(void) const { return vec<3, T>(y, y, y); } \
      __copy_(3, T) zzz(void) const { return vec<3, T>(z, z, z); } \
      __copy_(3, T) yxx(void) const { return vec<3, T>(y, x, x); } \
      __copy_(3, T) xyx(void) const { return vec<3, T>(x, y, x); } \
      __copy_(3, T) xxy(void) const { return vec<3, T>(x, x, y); } \
      __copy_(3, T) xyy(void) const { return vec<3, T>(x, y, y); } \
      __copy_(3, T) yxy(void) const { return vec<3, T>(y, x, y); } \
      __copy_(3, T) yyx(void) const { return vec<3, T>(y, y, x); }

    #define VEC_SWIZZLING_4 \
      VEC_SWIZZLING_3 \
      __copy_(2, T) zw(void)  const { return vec<2, T>(z, w); }

    #define VEC_SWIZZLING(size) VEC_SWIZZLING_##size
  }
  namespace /* Types */ {
    #define bvec2  vec<2, bool, 2>
    #define bvec3  vec<3, bool, 4>
    #define bvec4  vec<4, bool, 4>
    #define ivec2  vec<2, int, 8>
    #define ivec3  vec<3, int, 16>
    #define ivec4  vec<4, int, 16>
    #define uvec2  vec<2, Uint, 8>
    #define uvec3  vec<3, Uint, 16>
    #define uvec4  vec<4, Uint, 16>
    #define vec2   vec<2, float, 8>
    #define vec3   vec<3, float, 16>
    #define vec4   vec<4, float, 16>
    #define dvec2  vec<2, double, 16>
    #define dvec3  vec<3, double, 32>
    #define dvec4  vec<4, double, 32>
  }
}

template <Uint Size, typename T, Uint Alignment = alignof(T)> struct vec;

template <typename T, Uint Alignment>
struct vec<2, T, Alignment> {
  VEC_DATA(2);
  __constructor(void) : data{ T{}, T{} } {}
  __constructor(T v) : data{ v, v } {}
  __constructor(T x, T y) : data{ x, y } {}
  __constructor(const vec &other) : data{ other.x, other.y } {}
  template <Uint Size, typename U, Uint A>
  __constructor(const vec<Size, U, A> &other) {
    for (Uint i = 0; i < 2; ++i) {
      data[i] = (T)other[i];
    }
  }
  __bool operator==(const vec &other) const {
    return ((x == other.x) && (y == other.y));
  }
  __bool operator!=(const vec &other) const {
    return !(this == other);
  }
  __bool operator<(const vec &other) const {
    return ((x < other.x) && (y < other.y));
  }
  __bool operator>(const vec &other) const {
    return ((x > other.x) && (y > other.y));
  }
  __bool operator<=(const vec &other) const {
    return ((x <= other.x) && (y <= other.y));
  }
  __bool operator>=(const vec &other) const {
    return ((x >= other.x) && (y >= other.y));
  }
  VEC_SWIZZLING(2)
  VEC_IDX_OPERATOR
  VEC_OPERATOR(2, *);
  VEC_OPERATOR(2, +);
  VEC_OPERATOR(2, -);
  VEC_OPERATOR(2, /);
} __align_size(Alignment);

template <typename T, Uint Alignment>
struct vec<3, T, Alignment> {
  VEC_DATA(3);
  __constructor(void) : data{ T{}, T{}, T{} } {}
  __constructor(T v) : data{ v, v, v } {}
  __constructor(T x, T y, T z) : data{ x, y, z } {}
  __constructor(const vec &other) : data{ other.x, other.y, other.z } {}
  template <Uint Size, typename U, Uint A, typename... Args>
  __constructor(const vec<Size, U, A> &other, Args &&...args) {
    static_assert(sizeof...(Args) + Size <= 3, "To meny elements.");
    for (Uint i = 0; i < Size && i < 3; ++i) {
      data[i] = (T)other[i];
    }
    if constexpr (Size < 3) {
      if constexpr (sizeof...(Args) > 0) {
        float tmp[sizeof...(Args)] = {(T)args...};
        for (Uint i = Size; i < 3; ++i) {
          data = tmp[i - Size];
        }
      }
      else {
        for (Uint i = Size; i < 3; ++i) {
          data[i] = (T)(0);
        }
      }
    }
  }
  __bool operator==(const vec &other) const {
    return ((x == other.x) && (y == other.y) && (z == other.z));
  }
  __bool operator!=(const vec &other) const {
    return !(this == other);
  }
  VEC_SWIZZLING(3)
  VEC_IDX_OPERATOR
  VEC_OPERATOR(3, +)
  VEC_OPERATOR(3, -)
  VEC_OPERATOR(3, *)
  VEC_OPERATOR(3, /)
} __align_size(Alignment);

template <typename T, Uint Alignment>
struct vec<4, T, Alignment> {
  VEC_DATA(4);
  __constructor(void) : data{ T{}, T{}, T{}, T{} } {}
  __constructor(T v) : data{ v, v, v, v } {}
  __constructor(T x, T y, T z, T w) : data{ x, y, z, w } {}
  __constructor(const vec &other) : data{ other.x, other.y, other.z, other.w } {}
  template <Uint Size, typename U, Uint A, typename... Args>
  __constructor(const vec<Size, U, A> &other, Args &&...args) {
    static_assert(sizeof...(Args) + Size <= 4, "To meny elements.");
    for (Uint i = 0; i < Size; ++i) {
      data[i] = (T)other[i];
    }
    if constexpr (Size < 4) {
      if constexpr (sizeof...(Args) > 0) {
        T tmp[sizeof...(Args)] = {(T)(args)...};
        for (Uint i = Size; i < 4; ++i) {
          data[i] = tmp[i - Size];
        }
      }
      else {
        for (Uint i = Size; i < 4; ++i) {
          data[i] = (T)0;
        }
      }
    }
  }
  __bool operator==(const vec &other) const {
    return ((x == other.x) && (y == other.y) && (z == other.z) && (w == other.w));
  }
  __bool operator!=(const vec &other) const {
    return !(this == other);
  }
  VEC_SWIZZLING(4)
  VEC_IDX_OPERATOR
  VEC4_H_OPERATOR(add);
  VEC4_H_OPERATOR(sub);
  VEC4_H_OPERATOR(mul);
  VEC4_H_OPERATOR(div);
} __align_size(Alignment);

namespace /* Undef */ {
  #undef __constructor
  #undef __ref
  #undef __copy
  #undef __bool
  #undef __copy_
  #undef __type_ref
  #undef __type_T

  #undef VEC_STRUCT_2
  #undef VEC_STRUCT_3
  #undef VEC_STRUCT_4
  #undef VEC_DATA
  #undef VEC_IDX_OPERATOR
  #undef VEC_OPERATOR

  #undef OP__add
  #undef OP__sub
  #undef OP__mul
  #undef OP__div

  #undef OP_add
  #undef OP_sub
  #undef OP_mul
  #undef OP_div
  #undef OP_eq_add
  #undef OP_eq_sub
  #undef OP_eq_mul
  #undef OP_eq_div

  #undef OP
  #undef OP_
  #undef OP_eq

  #undef VEC4_H_OPERATOR
  #undef VEC4_OPERATOR

  #undef VEC_SWIZZLING_2
  #undef VEC_SWIZZLING_3
  #undef VEC_SWIZZLING_4
  #undef VEC_SWIZZLING
}
