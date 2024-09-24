#pragma once

template <typename T, unsigned long N>
struct param_array_t;

template <typename T, unsigned long N>
struct param_array_t
{
    __inline__ bool __attribute__((__always_inline__, __nodebug__, __nothrow__)) empty(void)
    {
        return N == 0;
    }
    param_array_t(T (&array)[N])
        : data(array)
        , size(N)
    {}
    T            *data;
    unsigned long size;
};
