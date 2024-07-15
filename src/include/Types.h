#pragma once

#include "def.h"

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept ConstexprArithmetic = Arithmetic<T> && requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a *b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    { a % b } -> std::convertible_to<T>; // Only for integral types
};

template <typename T1, typename T2, typename T3>
struct tri_param_t
{
    TRI_PARAM_T_ENUM;
    TRI_PARAM_T_UNION(T1, T2, T3);
    TRI_PARAM_T_MAKE_ALL_CONSTRUCTS(tri_param_t, type_1, type_2, type_3)
};

template <typename T1, typename T2>
struct double_param_t
{
    DOUBLE_PARAM_T_ENUM;
    DOUBLE_PARAM_T_UNION(T1, T2);
    DOUBLE_PARAM_T_MAKE_ALL_CONSTRUCTS
};
