#pragma once

#include "def.h"

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept ConstexprArithmetic = Arithmetic<T> && requires(T a, T b) {
    {
        a + b
    } -> std::convertible_to<T>;
    {
        a - b
    } -> std::convertible_to<T>;
    {
        a *b
    } -> std::convertible_to<T>;
    {
        a / b
    } -> std::convertible_to<T>;
    {
        a % b
    } -> std::convertible_to<T>; // Only for integral types
};
