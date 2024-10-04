#pragma once

#include "Attributes.h"
#include "def.h"

#include <cstdlib>
#include <type_traits>

#define __MVector_attr       __attribute__((__always_inline__, __nodebug__))
#define __MVector_const_attr __attribute__((__always_inline__, __nodebug__, __const__))

template <class T>
class MVector {
  T   *data;
  Uint len;
  Uint cap;

 public:
  __inline__ void __MVector_attr push_back(const T &element) noexcept(std::is_nothrow_default_constructible<T>::value) {
    if (len == cap) {
      cap *= 2;
      data = AREALLOC_ARRAY(data, cap);
    }
    new (data + len) T(element);
    len++;
  }

  __inline__ void __MVector_attr push_back(T &&element) noexcept(std::is_nothrow_default_constructible<T>::value) {
    if (len == cap) {
      cap *= 2;
      data = AREALLOC_ARRAY(data, cap);
    }
    *(data + len) = (T &&)element;
    ++len;
  }

  __inline__ void __MVector_attr reorder_from(Uint newstart) noexcept {
    if (newstart < len) {
      for (Uint i = newstart; i < len; ++i) {
        data[i - newstart] = std::move(data[i]);
      }
      len -= newstart;
    }
  }

  __inline__ void __MVector_attr erase_at(Uint index) noexcept(std::is_nothrow_destructible<T>::value) {
    if (index < len) {
      data[index].~T();
      for (Uint i = index; i < (len - 1); ++i) {
        data[i] = std::move(data[i + 1]);
      }
      len -= 1;
    }
  }

  /* Note that this does not free any elements in the array
   * it only free`s the current array and set`s 'len' to 0
   * then mallocs the 'data' ptr again. */
  __inline__ void __MVector_attr clear(void) noexcept(std::is_nothrow_destructible<T>::value) {
    for (Uint i = 0; i < len; i++) {
      data[i].~T();
    }
    free(data);
    len  = 0;
    cap  = 10;
    data = AMALLOC_ARRAY(data, cap);
  }

  __inline__ Uint __warn_unused __MVector_attr size(void) noexcept {
    return len;
  }

  __inline__ const Uint __warn_unused __MVector_const_attr size(void) const noexcept {
    return len;
  }

  __inline__ bool __warn_unused __MVector_attr empty(void) noexcept {
    return (len == 0);
  }

  __inline__ const bool __warn_unused __MVector_const_attr empty(void) const noexcept {
    return (len == 0);
  }

  __inline__ void __MVector_attr resize(const Uint newlen) noexcept {
    if (newlen < len) {
      len = newlen;
    }
  }

  __inline__ T &__MVector_attr back(void) noexcept {
    return (len > 0) ? *(data + (len - 1)) : *data;
  }

  __inline__ const T &__MVector_const_attr back(void) const noexcept {
    return (len > 0) ? *(data + (len - 1)) : *data;
  }

  __inline__ T *__MVector_const_attr begin(void) const noexcept {
    return data;
  }

  __inline__ T *__MVector_const_attr end(void) const noexcept {
    return data + len;
  }

  __inline__ Uint index_of(const T *element) const noexcept {
    return (element >= begin() && element < end()) ? element - begin() : -1;
  }

  /* Constructors. */
  MVector(void) noexcept
      : len(0)
      , cap(10) {
    data = (T *)calloc(cap, sizeof(T));
  }

  MVector(initializer_list<T> list) noexcept(std::is_nothrow_default_constructible<T>::value)
      : len(0)
      , cap(list.size() * 2) {
    data = (T *)calloc(cap, sizeof(T));
    for (const auto &it : list) {
      push_back(it);
    }
  }

  /* Copy Constructor. */
  MVector(const MVector &other) noexcept(std::is_nothrow_copy_constructible<T>::value)
      : len(other.len)
      , cap(other.cap) {
    data = (T *)calloc(cap, sizeof(T));
    for (Uint i = 0; i < len; ++i) {
      new (data + i) T(other.data[i]); // Use copy constructor of T
    }
  }

  /* Copy Assignment Operator. */
  MVector &operator=(const MVector &other) noexcept(std::is_nothrow_copy_constructible<T>::value) {
    if (this != &other) {
      // Clean up the current object
      for (Uint i = 0; i < len; i++) {
        data[i].~T();
      }
      free(data);
      // Allocate new memory and copy elements from 'other'
      len  = other.len;
      cap  = other.cap;
      data = (T *)calloc(cap, sizeof(T));
      for (Uint i = 0; i < len; ++i) {
        new (data + i) T(other.data[i]);
      }
    }
    return *this;
  }

  /* Move Constructor. */
  MVector(MVector &&other) noexcept(std::is_nothrow_default_constructible<T>::value)
      : data(other.data)
      , len(other.len)
      , cap(other.cap) {
    other.data = nullptr;
    other.len  = 0;
    other.cap  = 0;
  }

  /* Move Assignment Operator. */
  MVector &operator=(MVector &&other) noexcept(std::is_nothrow_default_constructible<T>::value) {
    if (this != &other) {
      // Clean up the current vector
      for (Uint i = 0; i < len; i++) {
        data[i].~T();
      }
      free(data);
      // Move data from other
      data = other.data;
      len  = other.len;
      cap  = other.cap;
      // Reset the other vector
      other.data = nullptr;
      other.len  = 0;
      other.cap  = 0;
    }
    return *this;
  }

  /* Destructor. */
  ~MVector(void) noexcept(is_nothrow_destructible<T>::value) {
    for (Uint i = 0; i < len; i++) {
      data[i].~T();
    }
    free(data);
  }

  /* Operator. */
  __inline__ T &__MVector_attr operator[](const Uint index) noexcept {
    return data[index];
  }

  __inline__ const T &__MVector_const_attr operator[](const Uint index) const noexcept {
    return data[index];
  }
};

#undef MVECTOR_ATTR
#undef MVECTOR_CONST_ATTR
