#pragma once

#include "Attributes.h"
#include "def.h"

#include <cstdlib>
#include <type_traits>

#define __MVector_attr       __attribute__((__always_inline__, __nodebug__))
#define __MVector_const_attr __attribute__((__always_inline__, __nodebug__, __const__))

/* clang-format off */

namespace /* Defines. */ {
  #define __nothrow_default_constructor noexcept(is_nothrow_default_constructible<T>::value)
  #define __nothrow_destruct            noexcept(is_nothrow_destructible<T>::value)
  #define __nothrow_copy_constructible  noexcept(is_nothrow_copy_constructible<T>::value)
  #define __nothrow_move_constructible  noexcept(std::is_nothrow_move_constructible<T>::value)

  #define __ref  __inline__ constexpr MVector & __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __type_ref __inline__ constexpr T &__warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __type_ptr __inline__ constexpr T *__warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __bool __inline__ constexpr bool __warn_unused __attribute((__const__, __always_inline__, __nothrow__, __nodebug__))
  #define __Uint __inline__ constexpr Uint __warn_unused __attribute((__const__, __always_inline__, __nothrow__, __nodebug__))
}

template <class T>
class MVector {
 private:
  T   *_data;
  Uint _len;
  Uint _cap;

 public:
  __ref push_back(const T &element) __nothrow_default_constructor {
    if (_len == _cap) {
      _cap *= 2;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
    new (_data + _len) T(element);
    ++_len;
    return *this;
  }

  __ref push_back(T &&element) __nothrow_default_constructor {
    if (_len == _cap) {
      _cap *= 2;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
    *(_data + _len) = (T &&)element;
    ++_len;
    return *this;
  }

  __ref reorder_from(Uint newstart) {
    if (newstart < _len) {
      for (Uint i = newstart; i < _len; ++i) {
        _data[i - newstart] = (T &&)_data[i];
      }
      _len -= newstart;
    }
    return *this;
  }

  __ref erase_at(Uint index) __nothrow_destruct {
    if (index < _len) {
      _data[index].~T();
      for (Uint i = index; i < (_len - 1); ++i) {
        _data[i] = (T &&)_data[i + 1];
      }
      _len -= 1;
    }
    return *this;
  }

  __ref erase(const T *const &element) __nothrow_destruct {
    Uint idx = index_of(element);
    if (idx == (Uint)-1) {
      return *this;
    }
    erase_at(idx);
    return *this;
  }

  /* Note that this does not free any elements in the array
   * it only free`s the current array and set`s '_len' to 0
   * then mallocs the '_data' ptr again. */
  __ref clear(void) __nothrow_destruct {
    for (Uint i = 0; i < _len; i++) {
      _data[i].~T();
    }
    free(_data);
    _len  = 0;
    _cap  = 10;
    _data = AMALLOC_ARRAY(_data, _cap);
    return *this;
  }

  __Uint size(void) const {
    return _len;
  }

  __bool empty(void) const {
    return (_len == 0);
  }

  __ref resize(Uint newlen) {
    if (newlen < _len) {
      _len = newlen;
    }
    return *this;
  }

  __type_ref back(void) const {
    return (_len > 0) ? *(_data + (_len - 1)) : *_data;
  }

  __type_ptr data(void) const {
    return _data;
  }

  __type_ptr begin(void) const {
    return _data;
  }

  __type_ptr end(void) const {
    return (_data + _len);
  }

  __Uint index_of(const T *element) const {
    return (element >= begin() && element < end()) ? element - begin() : (Uint)-1;
  }

  /* Constructors. */
  MVector(void) noexcept : _len(0), _cap(10) {
    _data = (T *)calloc(_cap, sizeof(T));
  }

  MVector(Uint n) noexcept : _len(n), _cap(n) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (Uint i = 0; i < n; ++i) {
      _data[i] = T{};
    }
  }

  MVector(Uint n, const T &value) : _len(n), _cap(n) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (Uint i = 0; i < n; ++i) {
      *(_data + i) = value;
    }
  }

  MVector(initializer_list<T> list) __nothrow_default_constructor : _len(0), _cap(list.size() * 2) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (const auto &it : list) {
      push_back(it);
    }
  }

  MVector(const T *array, Uint size) __nothrow_default_constructor : _len(0), _cap(size + 1) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (Uint i = 0; i < size; ++i) {
      push_back(array[i]);
    }
  }

  template<Uint N>
  MVector(const T(&array)[N]) __nothrow_default_constructor : _len(0), _cap(N + 1) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (Uint i = 0; i < N; ++i) {
      push_back(array[i]);
    }
  }

  /* Copy Constructor. */
  MVector(const MVector &other) __nothrow_copy_constructible : _len(other._len), _cap(other._cap) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (Uint i = 0; i < _len; ++i) {
      // Use copy constructor of T
      new (_data + i) T(*(other._data + i));
    }
  }

  /* Copy Assignment Operator. */
  __ref operator=(const MVector &other) __nothrow_copy_constructible {
    if (this != &other) {
      /* Clean up the current object */
      for (Uint i = 0; i < _len; i++) {
        _data[i].~T();
      }
      free(_data);
      /* Allocate new memory and copy elements from 'other' */
      _len  = other._len;
      _cap  = other._cap;
      _data = (T *)calloc(_cap, sizeof(T));
      for (Uint i = 0; i < _len; ++i) {
        new (_data + i) T(other._data[i]);
      }
    }
    return *this;
  }

  /* Move Constructor. */
  MVector(MVector &&other) __nothrow_default_constructor : _data(other._data), _len(other._len), _cap(other._cap) {
    other._data = nullptr;
    other._len  = 0;
    other._cap  = 0;
  }

  /* Move Assignment Operator. */
  __ref operator=(MVector &&other) __nothrow_move_constructible {
    if (this != &other) {
      /* Clean up the current vector */
      for (Uint i = 0; i < _len; i++) {
        _data[i].~T();
      }
      free(_data);
      /* Move _data from other */
      _data = other._data;
      _len  = other._len;
      _cap  = other._cap;
      /* Reset the other vector */
      other._data = nullptr;
      other._len  = 0;
      other._cap  = 0;
    }
    return *this;
  }

  /* Destructor. */
  ~MVector(void) __nothrow_destruct {
    for (Uint i = 0; i < _len; ++i) {
      _data[i].~T();
    }
    free(_data);
  }

  /* Operator. */
  __type_ref operator[](Uint index) const {
    return _data[index];
  }
};

namespace /* Undef defines. */ {
  #undef __nothrow_default_constructor
  #undef __nothrow_destruct

  #undef __ref
  #undef __bool
  #undef __Uint
}

#undef __MVector_attr
#undef __MVector_const_attr
