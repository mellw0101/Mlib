/// @file String.h
#pragma once

#include <stdarg.h>
#include <string.h>
#include <string>

#include "def.h"
#include "Attributes.h"
#include "constexpr.hpp"

using std::pair;
using std::string;
using std::vector;

namespace Mlib::String {
  size_t                       findN(const string &str, const string &search, size_t n);
  string                       replaceAll(const string &str, const string &search, const string &replace);
  string                       replaceN(const string &str, const string &search, const string &replace, size_t n);
  vector<pair<string, string>> parse_variables(const string &input);
}

size_t __warn_unused __pure __no_debug __no_throw __no_null(1) mstrlen(const char *str) noexcept;
size_t __warn_unused __pure __no_debug __no_throw __no_null(1) mstrnlen(const char *str, size_t maxlen) noexcept;
char * __warn_unused __pure __no_debug __no_throw __no_null(1) mstrndup(const char *str, size_t maxlen) noexcept;

namespace /* Defines */ {
  #define __constructor(...) __inline__ constexpr MString(__VA_ARGS__) __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __destructor __inline__ ~MString(void) __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __ref __inline__ constexpr MString & __attribute((__always_inline__, __nothrow__, __nodebug__))
  #define __copy __inline__ MString __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  #define __bool __inline__ constexpr bool __warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  #define __Uint __inline__ constexpr bool __warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  #define __char_ref __inline__ constexpr char & __warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  #define __char_ptr __inline__ constexpr char * __warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  #define __constexpr_func(ret) __inline__ constexpr ret __warn_unused __attribute((__always_inline__, __nothrow__, __nodebug__, __const__))
  /* clang-format off */
}

namespace /* Helpers. */ {
  __constexpr_func(Uint) const_strlen(const char *str) {
    Uint i = 0;
    for (; *(str + i); ++i);
    return i;
  }

  __constexpr_func(int) const_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
      ++s1, ++s2;
    }
    return (*s1 - *s2);
  }
}

class MString {
 private:
  char *_data;
  Uint _len;
  Uint _cap;

 public:
  __constructor(const char *str = "") : _len(const_strlen(str)), _cap(_len ? (_len * 2) : 30) {
    _data = (char *)malloc(_cap);
    for (Uint i = 0; i < _len; ++i) {
      *(_data + i) = (char &&)*(str + i);
    }
    *(_data + _len) = '\0';
  }

  __constructor(const char *str, Uint len) : _len(len), _cap(len * 2) {
    _data = (char *)malloc(_cap);
    for (Uint i = 0; i < _len; ++i) {
      *(_data + i) = (char &&)*(str + i);
    }
    *(_data + _len) = '\0';
  }

  /* Copy. */
  __constructor(const MString &other) : _len(other._len), _cap(other._cap) {
    _data = (char *)malloc(_cap);
    for (Uint i = 0; i < _len; ++i) {
      *(_data + i) = (char &&)*(other._data + i);
    }
    *(_data + _len) = '\0';
  }

  /* Move. */
  __constructor(MString &&other) : _data(other._data), _len(other._len), _cap(other._cap) {
    other._data = nullptr;
    other._len = 0;
    other._cap = 0;
  }

  __destructor {
    if (_data) {
      free(_data);
      _data = nullptr;
    }
  }

  __ref operator=(const char *str) {
    _len = const_strlen(str);
    if (_len >= _cap) {
      _cap = (_len * 2);
      _data = (char *)realloc(_data, _cap);
    }
    for (Uint i = 0; i < _len; ++i) {
      *(_data + i) = (char &&)*(str + i);
    }
    *(_data + _len) = '\0';
    return *this;
  }

  __ref operator=(const MString &other) {
    if (this != &other) {
      _len = other._len;
      if (_len >= _cap) {
        _cap = (_len * 2);
        _data = (char *)realloc(_data, _cap);
      }
      for (Uint i = 0; i < _len; ++i) {
        *(_data + i) = (char &&)*(other._data + i);
      }
      *(_data + _len) = '\0';    
    }
    return *this;
  }

  __ref operator+=(const MString &other) {
    if (this != &other) {
      Uint nlen = _len + other._len;
      if (nlen >= _cap) {
        _cap = (nlen * 2);
        _data = (char *)realloc(_data, _cap);
      }
      for (Uint i = 0; i < other._len; ++i) {
        *(_data + (_len + i)) = (char &&)*(other._data + i);
      }
      *(_data + nlen) = '\0';
      _len = nlen;
    }
    return *this;
  }

  __copy operator+(const MString &other) const {
    Uint nlen = (_len + other._len);
    MString ret;
    ret._len = nlen;
    ret._cap = (nlen * 2);
    ret._data = (char *)malloc(ret._cap);
    for (Uint i = 0; i < _len; ++i) {
      *(ret._data + i) = (char &&)*(_data + i);
    }
    for (Uint i = 0; i < other._len; ++i) {
      *(ret._data + (_len + i)) = *(other._data + i);
    }
    *(ret._data + nlen) = '\0';
    return ret;
  }

  __bool operator==(const char *str) const {
    if (const_strcmp(_data, str) == 0) {
      return true;
    }
    return false;
  }

  __char_ref operator[](Uint index) {
    if (index > _len) {
      return *(_data + _len);
    }
    return *(_data + index);
  }

  const __char_ref operator[](Uint index) const {
    if (index > _len) {
      return *(_data + _len);
    }
    return *(_data + index);
  }

  __char_ptr c_str(void) {
    return _data;
  }

  const __char_ptr c_str(void) const {
    return _data;
  }

  __Uint size(void) const {
    return _len;
  }
};

namespace /* Undefs */ {
  #undef __constructor
  #undef __destructor
  #undef __ref
  #undef __copy
  #undef __bool
  #undef __Uint
  #undef __char_ref
  #undef __char_ptr
  #undef __constexpr_func
}