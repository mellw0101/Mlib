#pragma once
/* clang-format off */

#include "Attributes.h"
#include "def.h"

namespace /* Defines. */ {
  #define __nothrow_default_constructor          noexcept(is_nothrow_default_constructible_v<T>)
  #define __nothrow_destructible                 noexcept(is_nothrow_destructible_v<T>)
  #define __nothrow_copy_constructible           noexcept(is_nothrow_copy_constructible_v<T>)
  #define __nothrow_move_constructible           noexcept(is_nothrow_move_constructible_v<T>)
  #define __nothrow_move_copy_constructible      noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_copy_constructible_v<T>)
  #define __nothrow_move_destruct_constructible  noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_destructible_v<T>)

  #define __ref  __inline__ constexpr MVector & __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __copy  __inline__ constexpr MVector __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __type_ref __inline__ constexpr T &__warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __type_ptr __inline__ constexpr T *__warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __bool __inline__ constexpr bool __warn_unused __attribute((__const__, __always_inline__, __nothrow__, __nodebug__))
  #define __Uint __inline__ constexpr Uint __warn_unused __attribute((__const__, __always_inline__, __nothrow__, __nodebug__))
  #define __void __inline__ constexpr void __attribute((__always_inline__, __nothrow__, __nodebug__))
}

template <class T>
class MVector {
 private:
  T   *_data;
  Uint _len;
  Uint _cap;

 public:
  template <typename ...Args>
  __void emplace_back(Args &&...args) __nothrow_move_copy_constructible {
    if (_len == _cap) {
      _cap *= 2;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
    new (_data + _len) T{std::forward<Args>(args)...};
    ++_len;
  }

  __void shrink_to_fit(void) {
    if (_len < _cap) {
      _cap = _len;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
  }

  __void insert(Uint idx, const T &value) __nothrow_move_copy_constructible {
    if (_len == _cap) {
      _cap *= 2;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
    ++_len;
    for (Uint i = _len; i > idx; --i) {
      _data[i] = (T &&)_data[i - 1];
    }
    new (_data + idx) T(value);
  }

  __void insert(const T *const &at, const T &value) __nothrow_move_copy_constructible {
    Uint idx = index_of(at);
    if (idx == (Uint)-1) {
      return;
    }
    if (_len == _cap) {
      _cap *= 2;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
    ++_len;
    for (Uint i = _len; i > idx; --i) {
      _data[i] = (T &&)_data[i - 1];
    }
    new (_data + idx) T(value);
  }

  template <typename Callback, typename ...Args>
  __void foreach(Callback &&callback, Args &&...args) {
    for (T *it = begin(); it != end(); ++it) {
      callback(it, std::forward<Args>(args)...);
    }
  }

  template <typename Callback, typename ...Args>
  __void rforeach(Callback &&callback, Args &&...args) {
    for (T *it = rbegin(); it != rend(); --it) {
      callback(it, std::forward<Args>(args)...);
    }
  }

  template <typename Callback, typename ...Args>
  const __void foreach(Callback &&callback, Args &&...args) const {
    for (const T *it = begin(); it != end(); ++it) {
      callback(it, std::forward<Args>(args)...);
    }
  }

  template <typename Callback, typename ...Args>
  const __void rforeach(Callback &&callback, Args &&...args) const {
    for (const T *it = rbegin(); it != rend(); --it) {
      callback(it, std::forward<Args>(args)...);
    }
  }

  __ref push_back(const T &element) __nothrow_copy_constructible {
    if (_len == _cap) {
      _cap *= 2;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
    new (_data + _len) T(element);
    ++_len;
    return *this;
  }

  __ref push_back(T &&element) __nothrow_move_constructible {
    if (_len == _cap) {
      _cap *= 2;
      _data = AREALLOC_ARRAY(_data, _cap);
    }
    *(_data + _len) = (T &&)element;
    ++_len;
    return *this;
  }

  __ref reorder_from(Uint newstart) __nothrow_move_destruct_constructible {
    if (newstart < _len) {
      for (Uint i = newstart; i < _len; ++i) {
        if constexpr (!is_trivially_destructible_v<T>) {
          /* Call destructor for object. */
          _data[i - newstart].~T();
        }
        /* Then move new object into the correct place. */
        _data[i - newstart] = (T &&)_data[i];
      }
      _len -= newstart;
    }
    return *this;
  }

  __ref erase_at(Uint index) __nothrow_move_destruct_constructible {
    if (index < _len) {
      if constexpr (!is_trivially_destructible_v<T>) {
        _data[index].~T();
      }
      for (Uint i = index; i < (_len - 1); ++i) {
        _data[i] = (T &&)_data[i + 1];
      }
      _len -= 1;
    }
    return *this;
  }

  __ref erase(const T *const &element) __nothrow_move_destruct_constructible {
    Uint idx = index_of(element);
    if (idx == (Uint)-1) {
      return *this;
    }
    erase_at(idx);
    return *this;
  }

  __ref erase(const T &element) __nothrow_move_destruct_constructible {
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
  __ref clear(void) __nothrow_destructible {
    if constexpr (!is_trivially_destructible_v<T>) {
      for (Uint i = 0; i < _len; i++) {
        _data[i].~T();
      }
    }
    free(_data);
    _len  = 0;
    _cap  = 10;
    _data = (T *)calloc(_cap, sizeof(T));
    return *this;
  }

  __Uint size(void) const {
    return _len;
  }

  __bool empty(void) const {
    return (_len == 0);
  }

  __ref resize(Uint newlen) __nothrow_default_constructor {
    if (newlen < _len) {
      _len = newlen;
    }
    else if (newlen > _len) {
      if (newlen > _cap) {
        _cap = newlen;
        _data = (T *)realloc(_data, (_cap * sizeof(T)));
      }
      for (Uint i = _len; i < newlen; ++i) {
        if constexpr (is_trivially_constructible_v<T>) {
          *(_data + i) = T{};
        }
        else {
          new (_data + i) T();
        }
      }
      _len = newlen;
    }
    return *this;
  }

  __ref reserve(Uint size) __nothrow_default_constructor {
    if (size > _cap) {
      Uint save_size = _len;
      resize(size);
      resize(save_size);
    }
    return *this;
  }

  __type_ref back(void) {
    return (_len > 0) ? *(_data + (_len - 1)) : *_data;
  }

  const __type_ref back(void) const {
    return (_len > 0) ? *(_data + (_len - 1)) : *_data;
  }

  __type_ptr data(void) {
    return _data;
  }

  const __type_ptr data(void) const {
    return _data;
  }

  __type_ptr begin(void) {
    return _data;
  }

  const __type_ptr begin(void) const {
    return _data;
  }

  __type_ptr end(void) {
    return (_data + _len);
  }

  const __type_ptr end(void) const {
    return (_data + _len);
  }

  __type_ptr rbegin(void) {
    return (_len > 0) ? ((_data + _len) - 1) : _data;
  }

  const __type_ptr rbegin(void) const {
    return (_len > 0) ? ((_data + _len) - 1) : _data;
  }

  __type_ptr rend(void) {
    return (_len > 0) ? (_data - 1) : _data;
  }

  const __type_ptr rend(void) const {
    return (_len > 0) ? (_data - 1) : _data;
  }

  /* Return`s the index of an element, or -1 on failure. */
  __Uint index_of(const T *const &element) const {
    return (element >= begin() && element < end()) ? (element - begin()) : (Uint)-1;
  }

  /* Return`s the index of an element, or -1 on failure. */
  __Uint index_of(const T &element) const {
    for (Uint i = 0; i < _len; ++i) {
      if (element == _data[i]) {
        return i;
      }
    }
    return (Uint)-1;
  }

  /* Constructors. */
  MVector(void) noexcept : _len(0), _cap(10) {
    _data = (T *)calloc(_cap, sizeof(T));
  }

  MVector(Uint n) __nothrow_default_constructor : _len(n), _cap(n) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (Uint i = 0; i < n; ++i) {
      if constexpr (!is_trivially_constructible_v<T>) {
        new (_data + i) T();
      }
      else {
        *(_data + i) = T{};
      }
    }
  }

  MVector(Uint n, const T &value) __nothrow_copy_constructible : _len(n), _cap(n) {
    _data = (T *)calloc(_cap, sizeof(T));
    for (Uint i = 0; i < n; ++i) {
      *(_data + i) = value;
    }
  }

  MVector(initializer_list<T> list) __nothrow_move_constructible : _len(0), _cap(list.size() * 2) {
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
      /* Use copy constructor of T */
      new (_data + i) T(*(other._data + i));
    }
  }

  /* Move Constructor. */
  MVector(MVector &&other) __nothrow_move_constructible : _data(other._data), _len(other._len), _cap(other._cap) {
    other._data = nullptr;
    other._len  = 0;
    other._cap  = 0;
  }
  
  /* Destructor. */
  ~MVector(void) __nothrow_destructible {
    if constexpr (!is_trivially_destructible_v<T>) {
      for (Uint i = 0; i < _len; ++i) {
        _data[i].~T();
      }
    }
    free(_data);
  }

  /* Copy Assignment Operator. */
  __ref operator=(const MVector &other) __nothrow_copy_constructible {
    if (this != &other) {
      /* Clean up the current object */
      if constexpr (!is_trivially_destructible_v<T>) {
        for (Uint i = 0; i < _len; ++i) {
          _data[i].~T();
        }
      }
      free(_data);
      /* Allocate new memory and copy elements from 'other' */
      _len  = other._len;
      _cap  = other._cap;
      _data = (T *)calloc(_cap, sizeof(T));
      for (Uint i = 0; i < _len; ++i) {
        new (_data + i) T(*(other._data + i));
      }
    }
    return *this;
  }

  /* Move Assignment Operator. */
  __ref operator=(MVector &&other) __nothrow_move_constructible {
    if (this != &other) {
      /* Clean up the current vector */
      if constexpr (!is_trivially_destructible_v<T>) {
        for (Uint i = 0; i < _len; ++i) {
          _data[i].~T();
        }
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

  __ref operator<<(const T &element) __nothrow_copy_constructible {
    push_back(element);
    return *this;
  }

  __ref operator<<(T &&element) __nothrow_move_constructible {
    push_back(element);
    return *this;
  }
  
  /* Operator. */
  __type_ref operator[](Uint index) {
    return _data[index];
  }

  const __type_ref operator[](Uint index) const {
    return _data[index];
  }
};

namespace /* Undef defines. */ {
  #undef __nothrow_default_constructor
  #undef __nothrow_destructible
  #undef __nothrow_copy_constructible
  #undef __nothrow_move_constructible
  #undef __nothrow_move_copy_constructible
  #undef __nothrow_move_destruct_constructible

  #undef __ref
  #undef __copy
  #undef __type_ref
  #undef __type_ptr
  #undef __bool
  #undef __Uint
  #undef __void
}
