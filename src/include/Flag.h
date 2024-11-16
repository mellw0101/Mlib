#pragma once
/* clang-format off */

#include "Attributes.h"
#include "Init_list.h"
#include "def.h"

namespace /* Defines. */ {
  #define __constructor(...)    __inline__ constexpr __attribute__((__always_inline__, __nodebug__, __nothrow__)) bit_flag_t(__VA_ARGS__)
  #define __ref                 __inline__ constexpr bit_flag_t & __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __void                __inline__ constexpr void __attribute__((__always_inline__, __nodebug__, __nothrow__))
  #define __Uint                __inline__ constexpr Uint __attribute__((__warn_unused_result__, __always_inline__, __nodebug__, __nothrow__, __const__))
  #define __bool                __inline__ constexpr bool __attribute__((__warn_unused_result__, __always_inline__, __nodebug__, __nothrow__, __const__))
  #define __Uchar               __inline__ constexpr Uchar __attribute__((__warn_unused_result__, __always_inline__, __nodebug__, __nothrow__, __const__))
  #define __Uchar_ref           __inline__ constexpr Uchar & __attribute__((__warn_unused_result__, __always_inline__, __nodebug__, __nothrow__))
  #define __assert_Flag_lt_Size static_assert(Flag < Size, "Flag must be lower then Size.")
}

template <Uint Size>
struct bit_flag_t {
  static_assert(((Size % 8) == 0) && (Size != 0), "Size must be a power of 8");

  __void set(Uint flag) {
    _flags(flag) |= _flag_mask(flag);
  }

  __void unset(Uint flag) {
    _flags(flag) &= ~_flag_mask(flag);
  }

  __bool is_set(Uint flag) const {
    return ((_flags(flag) & _flag_mask(flag)) != 0);
  }

  __void toggle(Uint flag) {
    _flags(flag) ^= _flag_mask(flag);
  }

  __Uint size(void) const {
    return Size;
  }

  __void clear(void) {
    for (Uint i = 0; i < (Size / 8); ++i) {
      _flag_array[i] = (Uchar)0;
    }
  }

  __Uint num_of_set_flags(void) const {
    Uint n = 0;
    for (Uint i = 0; i < Size; ++i) {
      if (is_set(i)) {
        ++n;
      }
    }
    return n;
  }

  template <Uint Flag>
  __void set(void) {
    __assert_Flag_lt_Size;
    _flags<Flag>() |= _flag_mask<Flag>();
  }
 
  template <Uint Flag>
  __void unset(void) {
    __assert_Flag_lt_Size;
    _flags<Flag>() &= ~_flag_mask<Flag>();
  }
  
  template <Uint Flag>
  __bool is_set(void) const {
    __assert_Flag_lt_Size;
    return ((_flags<Flag>() & _flag_mask<Flag>()) != 0);
  }

  template <Uint Flag>
  __void toggle(void) {
    __assert_Flag_lt_Size;
    _flags<Flag>() ^= _flag_mask<Flag>();
  }  

  template <Uint Flag>
  __void clear_and_set(void) {
    __assert_Flag_lt_Size;
    clear();
    set<Flag>();
  }

  template <Uint Flag>
  __void clear_from(void) {
    static_assert(Flag + 1 < Size, "Cannot clear from last bit.");
    Uint idx = Flag;
    while (++idx < Size) {
      unset(idx);
    }
  }

  /* Constructors. */
  __constructor(void) {
    clear();
  }

  explicit __constructor(const bit_flag_t<Size> &other) {
    for (Uint i = 0; i < (Size / 8); ++i) {
      _flag_array[i] = other._flag_array[i];
    }
  }

  explicit __constructor(const init_list<Uint> &list) {
    clear();
    int idx = 0;
    for (const auto &byte : list) {
      set(byte);
      if (++idx == Size) {
        return;
      }
    }
  }

  /* Operators. */
  __ref operator=(const init_list<Uint> &list) {
    clear();
    int idx = 0;
    for (const auto &it : list) {
      set(it);
      if (++idx == Size) {
        break;
      }
    }
    return *this;
  }

 private:
  Uchar _flag_array[Size / 8];

  __Uchar_ref _flags(Uint flag) {
    return _flag_array[flag / 8];
  }

  const __Uchar_ref _flags(Uint flag) const {
    return _flag_array[flag / 8];
  }

  template <Uint Flag>
  __Uchar_ref _flags(void) {
    __assert_Flag_lt_Size;
    return _flag_array[Flag / 8];
  }

  template <Uint Flag>
  const __Uchar_ref _flags(void) const {
    __assert_Flag_lt_Size;
    return _flag_array[Flag / 8];
  }

  __Uchar _flag_mask(Uint flag) const {
    return ((Uchar)1 << (flag % 8));
  }

  template <Uint Flag>
  __Uchar _flag_mask(void) const {
    __assert_Flag_lt_Size;
    return ((Uchar)1 << (Flag % 8));
  }
};

namespace /* Undef. */ {
  #undef __constructor
  #undef __ref
  #undef __void
  #undef __Uint
  #undef __bool
  #undef __Uchar
  #undef __Uchar_ref
  #undef __assert_Flag_lt_Size
}