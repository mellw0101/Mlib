#pragma once
/* clang-format off */

#include "def.h"
#include "float_calc.h"

#include <glm/vec3.hpp>

namespace /* Defines */ {
  #define __INLINE(type) __inline__ constexpr type __attribute__((__const__, __always_inline__, __nothrow__, __nodebug__))
  
  #define __operator_double(_op) operator _op(const MVec3 &vec1, const MVec3 &vec2)
  #define __operator_single(_op) operator _op(const MVec3 &vec, float value)
  
  #define __operator_decl(_op)  \
    __INLINE(MVec3) __operator_double(_op); \
    __INLINE(MVec3) __operator_single(_op)

  #define __operator_def(_op) \
    __INLINE(MVec3) __operator_double(_op) { return {vec1.x _op vec2.x, vec1.y _op vec2.y, vec1.z _op vec2.z}; } \
    __INLINE(MVec3) __operator_single(_op) { return {vec.x  _op value,  vec.y  _op value,  vec.z  _op value};  }

  #define __OPERATOR(type) __inline__ constexpr operator type __attribute__((__const__, __always_inline__, __nothrow__, __nodebug__))

  #define __OPERATOR_RETURN(ret, type) __inline__ constexpr ret operator type __attribute__((__const__, __always_inline__, __nothrow__, __nodebug__))

  #define __ALIGNED_TYPEDEF(struct_name, alignment, alias) \
    typedef struct alignas(alignment) alias : struct_name {} alias;
}

typedef struct MVec3 MVec3;

__operator_decl(/);
__operator_decl(*);
__operator_decl(+);
__operator_decl(-);

typedef struct MVec3 {
  union /* Data */ {
    float arr[3];
    struct { float x, y, z; };
  };

  __OPERATOR(glm::vec3(void) const) {
    return {x, y, z};
  }

  __OPERATOR_RETURN(MVec3 &, =(const MVec3 &other)) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
  }

  __OPERATOR_RETURN(MVec3 &, =(const glm::vec3 &other)) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
  }

  __INLINE(float) magnitude(void) const {
    return sqrt((x * x) + (y * y) + (z * z));
  }

  __INLINE(MVec3) normalize(void) const {
    float mag = magnitude();
    return (mag) ? (*this / mag) : MVec3{0.0f, 0.0f, 0.0f};
  }

  __INLINE(MVec3) direction_to(const MVec3 &other) const {
    return (other - *this).normalize();
  }

  static __INLINE(MVec3) from(const glm::vec3 &v) {
    return {v.x, v.y, v.z};
  }
} MVec3;

__ALIGNED_TYPEDEF(MVec3, 16, MVec3_16);

__operator_def(/);
__operator_def(*);
__operator_def(+);
__operator_def(-);

namespace /* Undef */ {
  #undef __INLINE
  #undef __operator_double
  #undef __operator_single
  #undef __operator_decl
  #undef __operator_def
  #undef __OPERATOR
  #undef __OPERATOR_RETURN
  #undef __ALIGNED_TYPEDEF
}