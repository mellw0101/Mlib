#pragma once

#include <type_traits>
#include "def.h"

template <typename T, typename... Args>
struct all_same : std::conjunction<std::is_same<std::__remove_cvref_t<T>, std::__remove_cvref_t<Args>>...> {};

template <typename T, typename... Args>
inline constexpr bool all_same_v = all_same<T, Args...>::value;

__MLIB_BEGIN_NAMESPACE_(Types, export) {
  template <typename A, typename B>
  struct is_same {
    static __inline__ constexpr bool value = false;
  };

  template <typename A>
  struct is_same<A, A> {
    static __inline__ constexpr bool value = true;
  };

  /* Using some clever deduction, so to get the correct result we declare
   * the default struct template as have type 'T' and that its = 'void'. */
  template <typename T, typename = void>
  struct has_addition_operator : std::false_type {};

  template <typename T>
  struct has_addition_operator<T, decltype(void(std::declval<T>() + std::declval<T>()))> : std::true_type {};

  template <int A, int B>
  struct Greatest_Common_Divisor {
    static const int value = Greatest_Common_Divisor<B, A % B>::value;
  };

  template <int A>
  struct Greatest_Common_Divisor<A, 0> {
    static const int value = A;
  };

  template <typename T>
  constexpr bool is_integral(void) {
    if constexpr (std::is_integral<T>::value) {
      return true;
    }
    return false;
  }
}
__END_NAMESPACE(Types)

template <typename T1, typename T2, typename T3>
struct tri_param_t {
  TRI_PARAM_T_ENUM;
  TRI_PARAM_T_UNION(T1, T2, T3);
  TRI_PARAM_T_MAKE_ALL_CONSTRUCTS(tri_param_t, type_1, type_2, type_3)
};

template <typename T1, typename T2>
struct double_param_t {
  DOUBLE_PARAM_T_ENUM;
  DOUBLE_PARAM_T_UNION(T1, T2);
  DOUBLE_PARAM_T_MAKE_ALL_CONSTRUCTS
};
