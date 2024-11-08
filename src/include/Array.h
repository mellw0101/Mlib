#pragma once

#include "def.h"

/* clang-format off */

namespace /* Define decl. */ {
  #define __ret \
    __inline__ constexpr

  #define __MArray_attr \
    __attribute((__always_inline__, __nodebug__, __nothrow__))

  #define __const_MArray_attr \
    __attribute((__always_inline__, __nodebug__, __nothrow__, __const__))

  #define __assert_index \
    static_assert(((index >= 0)) && (index < Size), "Index must be within Size")
}

template <typename T, Ulong Size>
class MArray {
 private:
  T data[Size];

 public:
  __ret MArray(void)
      : data {} {};

  __ret T & __MArray_attr operator[](Ulong index) {
    return data[index];
  }

  __ret const T & __const_MArray_attr operator[](Ulong index) const {
    return data[index];
  }

  template <Ulong index>
  __ret T & __MArray_attr operator[](void) {
    __assert_index;
    return data[index];
  }
  
  template <Ulong index>
  __ret const T & __const_MArray_attr operator[](void) const {
    __assert_index;
    return data[index];
  }

  __ret Ulong __MArray_attr size(void) {
    return Size;
  }

  __ret const Ulong __const_MArray_attr size(void) const {
    return Size;
  }

  __ret T * __MArray_attr begin(void) {
    return data;
  }

  __ret const T * __const_MArray_attr begin(void) const {
    return data;
  }

  __ret T * __MArray_attr end(void) {
    return data + Size;
  }

  __ret const T * __const_MArray_attr end(void) const {
    return data + Size;
  }
};

#define ARRIDX(num) operator[]<num>()

namespace /* Define cleanup. */ {
  #undef __ret
  #undef __MArray_attr
  #undef __const_MArray_attr
  #undef __assert_index
}
