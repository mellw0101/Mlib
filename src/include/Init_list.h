#pragma once

#include <new>
#include <utility>
#include "Attributes.h"

#ifdef __attr__
#    define attr_storage __attr__
#    undef __attr__
#endif

#define __attr__ __attr(__always_inline__, __nodebug__, __nothrow__)
#ifdef __const_attr__
#    define const_attr_storage __const_attr__
#    undef __const_attr__
#endif
#define __const_attr__ __attr(__always_inline__, __nodebug__, __nothrow__, __const__)

template <typename T>
struct init_list
{
    template <typename... Args>
    __inline__ __attr__ init_list(Args &&...args) noexcept
        : _data(new(std::nothrow) T[sizeof...(args)]), _size(sizeof...(args))
    {
        int idx = 0;
        ((_data[idx++] = std::forward<Args>(args)), ...);
    }

    __inline__ ~init_list(void) noexcept
    {
        delete[] _data;
    }

    __inline__ T *__warn_unused __attr__ data(void) noexcept
    {
        return _data;
    }

    __inline__ const T *__warn_unused __const_attr__ data(void) const noexcept
    {
        return _data;
    }

    __inline__ T *__warn_unused __attr__ begin(void) noexcept
    {
        return _data;
    }

    __inline__ const T *__warn_unused __const_attr__ begin(void) const noexcept
    {
        return _data;
    }

    __inline__ T *__warn_unused __attr__ end(void) noexcept
    {
        return _data + _size;
    }

    __inline__ const T *__warn_unused __const_attr__ end(void) const noexcept
    {
        return _data + _size;
    }

    __inline__ unsigned long __warn_unused __attr__ size(void) noexcept
    {
        return _size;
    }

    __inline__ const unsigned long __warn_unused __const_attr__ size(void) const noexcept
    {
        return _size;
    }

private:
    T            *_data;
    unsigned long _size;
};

#undef __attr__
#ifdef attr_storage
#    define __attr__ attr_storage
#    undef attr_storage
#endif

#undef __const_attr__
#ifdef const_attr_storage
#    define __const_attr__ const_attr_storage
#    undef const_attr_storage
#endif
