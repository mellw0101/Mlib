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

#define TRI_PARAM_T_ENUM   \
    enum type_t            \
    {                      \
        TYPE_1 = (1 << 0), \
        TYPE_2 = (1 << 1), \
        TYPE_3 = (1 << 2), \
                           \
    } type

#define TRI_PARAM_T_UNION(T1, T2, T3) \
    union                             \
    {                                 \
        T1 union_1;                   \
        T2 union_2;                   \
        T3 union_3;                   \
    }

#define PARAM_T_CONSTRUCT(OBJ, T, N, NAME) \
    OBJ(T NAME)                            \
        : type(TYPE_##N)                   \
        , union_##N(NAME)                  \
    {}

#define TRI_PARAM_T_MAKE_ALL_CONSTRUCTS(OBJ, NAME_1, NAME_2, NAME_3) \
    PARAM_T_CONSTRUCT(OBJ, T1, 1, NAME_1)                            \
    PARAM_T_CONSTRUCT(OBJ, T2, 2, NAME_2)                            \
    PARAM_T_CONSTRUCT(OBJ, T3, 3, NAME_3)

template <typename T1, typename T2, typename T3>
struct tri_param_t
{
    TRI_PARAM_T_ENUM;
    TRI_PARAM_T_UNION(T1, T2, T3);
    TRI_PARAM_T_MAKE_ALL_CONSTRUCTS(tri_param_t, type_1, type_2, type_3)
};

#define DOUBLE_PARAM_T_ENUM \
    enum type_t             \
    {                       \
        TYPE_1 = (1 << 0),  \
        TYPE_2 = (1 << 1),  \
                            \
    } type

#define DOUBLE_PARAM_T_UNION(T1, T2) \
    union                            \
    {                                \
        T1 union_1;                  \
        T2 union_2;                  \
    }

#define DOUBLE_PARAM_T_MAKE_ALL_CONSTRUCTS           \
    PARAM_T_CONSTRUCT(double_param_t, T1, 1, type_1) \
    PARAM_T_CONSTRUCT(double_param_t, T2, 2, type_2)

template <typename T1, typename T2>
struct double_param_t
{
    DOUBLE_PARAM_T_ENUM;
    DOUBLE_PARAM_T_UNION(T1, T2);
    DOUBLE_PARAM_T_MAKE_ALL_CONSTRUCTS
};

#define OUTPUT_OBJ_3WAY(name, T1, T2, T3, n1, n2, n3) \
    struct name                                       \
    {                                                 \
        TRI_PARAM_T_ENUM;                             \
        TRI_PARAM_T_UNION(T1, T2, T3);                \
        PARAM_T_CONSTRUCT(name, T1, 1, type_1)        \
        PARAM_T_CONSTRUCT(name, T2, 2, type_2)        \
        PARAM_T_CONSTRUCT(name, T3, 3, type_3)        \
    }
