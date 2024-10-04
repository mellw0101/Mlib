#pragma once

#include "Attributes.h"
#include "Init_list.h"
#include "def.h"

#define __bit_flag_t_attr     __attribute__((__always_inline__, __nodebug__, __nothrow__))
#define __assert_Flag_lt_Size static_assert(Flag < Size, "Flag must be lower then Size.")

template <Uint Size>
struct bit_flag_t {
  static_assert((Size % 8 == 0) && Size != 0, "Size must be a power of 8");

  __inline__ void __bit_flag_t_attr set(const Uint flag) noexcept {
    _flags(flag) |= _flag_mask(flag);
  }

  __inline__ void __bit_flag_t_attr set(const Uint flag) const noexcept {
    _flags(flag) |= _flag_mask(flag);
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr set(void) noexcept {
    __assert_Flag_lt_Size;
    _flags<Flag>() |= _flag_mask<Flag>();
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr set(void) const noexcept {
    __assert_Flag_lt_Size;
    _flags<Flag>() |= _flag_mask<Flag>();
  }

  __inline__ void __bit_flag_t_attr unset(const Uint flag) noexcept {
    _flags(flag) &= ~_flag_mask(flag);
  }

  __inline__ void __bit_flag_t_attr unset(const Uint flag) const noexcept {
    _flags(flag) &= ~_flag_mask(flag);
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr unset(void) noexcept {
    __assert_Flag_lt_Size;
    _flags<Flag>() &= ~_flag_mask<Flag>();
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr unset(void) const noexcept {
    __assert_Flag_lt_Size;
    _flags<Flag>() &= ~_flag_mask<Flag>();
  }

  __inline__ bool __warn_unused __bit_flag_t_attr is_set(const Uint flag) noexcept {
    return ((_flags(flag) & _flag_mask(flag)) != 0);
  }

  __inline__ bool __warn_unused __bit_flag_t_attr is_set(const Uint flag) const noexcept {
    return ((_flags(flag) & _flag_mask(flag)) != 0);
  }

  template <Uint Flag>
  __inline__ constexpr bool __warn_unused __bit_flag_t_attr is_set(void) noexcept {
    __assert_Flag_lt_Size;
    return ((_flags<Flag>() & _flag_mask<Flag>()) != 0);
  }

  template <Uint Flag>
  __inline__ constexpr bool __warn_unused __bit_flag_t_attr is_set(void) const noexcept {
    __assert_Flag_lt_Size;
    return ((_flags<Flag>() & _flag_mask<Flag>()) != 0);
  }

  __inline__ void __bit_flag_t_attr toggle(const Uint flag) noexcept {
    _flags(flag) ^= _flag_mask(flag);
  }

  __inline__ void __bit_flag_t_attr toggle(const Uint flag) const noexcept {
    _flags(flag) ^= _flag_mask(flag);
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr toggle(void) noexcept {
    __assert_Flag_lt_Size;
    _flags<Flag>() ^= _flag_mask<Flag>();
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr toggle(void) const noexcept {
    __assert_Flag_lt_Size;
    _flags<Flag>() ^= _flag_mask<Flag>();
  }

  __inline__ constexpr Uint __warn_unused __bit_flag_t_attr size(void) noexcept {
    return Size;
  }

  __inline__ constexpr Uint __warn_unused __bit_flag_t_attr size(void) const noexcept {
    return Size;
  }

  __inline__ constexpr void __bit_flag_t_attr clear(void) noexcept {
    for (Uint i = 0; i < (Size / 8); ++i) {
      _flag_array[i] = (Uchar)0;
    }
  }

  __inline__ constexpr void __bit_flag_t_attr clear(void) const noexcept {
    for (Uint i = 0; i < (Size / 8); ++i) {
      _flag_array[i] = (Uchar)0;
    }
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr clear_and_set(void) noexcept {
    __assert_Flag_lt_Size;
    clear();
    set<Flag>();
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr clear_and_set(void) const noexcept {
    __assert_Flag_lt_Size;
    clear();
    set<Flag>();
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr clear_from(void) noexcept {
    static_assert(Flag + 1 < Size, "Cannot clear from last bit.");
    Uint idx = Flag;
    while (++idx < Size) {
      unset(idx);
    }
  }

  template <Uint Flag>
  __inline__ constexpr void __bit_flag_t_attr clear_from(void) const noexcept {
    static_assert(Flag + 1 < Size, "Cannot clear from last bit.");
    Uint idx = Flag;
    while (++idx < Size) {
      unset(idx);
    }
  }

  /* Constructors. */
  __inline__ __bit_flag_t_attr bit_flag_t(void) noexcept {
    clear();
  };

  explicit __inline__ __bit_flag_t_attr bit_flag_t(const bit_flag_t<Size> &other) noexcept {
    for (Uint i = 0; i < (Size / 8); ++i) {
      _flag_array[i] = other._flag_array[i];
    }
  }

  explicit __inline__ __bit_flag_t_attr bit_flag_t(const init_list<Uint> &list) noexcept {
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
  __inline__ __bit_flag_t_attr bit_flag_t &operator=(const init_list<Uint> &list) noexcept {
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

  __inline__ Uchar __warn_unused __bit_flag_t_attr _flag_mask(const Uint flag) noexcept {
    return ((Uchar)1 << (flag % 8));
  }

  __inline__ constexpr Uchar __warn_unused __bit_flag_t_attr _flag_mask(const Uchar flag) const noexcept {
    return ((Uchar)1 << (flag % 8));
  }

  template <Uint Flag>
  __inline__ Uchar &__warn_unused __bit_flag_t_attr _flags(void) noexcept {
    __assert_Flag_lt_Size;
    return _flag_array[Flag / 8];
  }

  template <Uint Flag>
  __inline__ constexpr const Uchar &__warn_unused __bit_flag_t_attr _flags(void) const noexcept {
    __assert_Flag_lt_Size;
    return _flag_array[Flag / 8];
  }

  __inline__ Uchar &__warn_unused __bit_flag_t_attr _flags(const Uint flag) noexcept {
    return _flag_array[flag / 8];
  }

  __inline__ constexpr const Uchar &__warn_unused __bit_flag_t_attr _flags(const Uint flag) const noexcept {
    return _flag_array[flag / 8];
  }

  template <Uint Flag>
  __inline__ constexpr Uchar __warn_unused __bit_flag_t_attr _flag_mask(void) noexcept {
    __assert_Flag_lt_Size;
    return ((Uchar)1 << (Flag % 8));
  }

  template <Uint Flag>
  __inline__ constexpr Uchar __warn_unused __bit_flag_t_attr _flag_mask(void) const noexcept {
    __assert_Flag_lt_Size;
    return ((Uchar)1 << (Flag % 8));
  }
};

#undef __bit_flag_t_attr
#undef __assert_Flag_lt_Size
