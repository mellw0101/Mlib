#pragma once

#include "def.h"

#define __MVector_attr       __attribute__((__always_inline__, __nodebug__))
#define __MVector_const_attr __attribute__((__always_inline__, __nodebug__, __const__))

template <class T>
class MVector
{
    T           *data;
    unsigned int len;
    unsigned int cap;

public:
    __inline__ void __MVector_attr push_back(const T &element) noexcept
    {
        if (len == cap)
        {
            cap *= 2;
            data = AREALLOC_ARRAY(data, cap);
        }
        new (data + len) T(element);
        len++;
    }

    __inline__ void __MVector_attr reorder_from(unsigned int newstart) noexcept
    {
        if (newstart < len)
        {
            for (unsigned int i = newstart; i < len; ++i)
            {
                data[i - newstart] = std::move(data[i]);
            }
            len -= newstart;
        }
    }

    __inline__ void __MVector_attr
    erase_at(unsigned int index) noexcept(std::is_nothrow_destructible<T>::value)
    {
        if (index < len)
        {
            data[index].~T();
            for (unsigned int i = index; i < (len - 1); ++i)
            {
                data[i] = std::move(data[i + 1]);
            }
            len -= 1;
        }
    }

    /* Note that this does not free any elements in the array
     * it only free`s the current array and set`s 'len' to 0
     * then mallocs the 'data' ptr again. */
    __inline__ void __MVector_attr clear(void) noexcept(std::is_nothrow_destructible<T>::value)
    {
        for (unsigned int i = 0; i < len; i++)
        {
            data[i].~T();
        }
        free(data);
        len  = 0;
        cap  = 10;
        data = AMALLOC_ARRAY(data, cap);
    }

    __inline__ unsigned int __warn_unused __MVector_attr size(void) noexcept
    {
        return len;
    }

    __inline__ const unsigned int __warn_unused __MVector_const_attr size(void) const noexcept
    {
        return len;
    }

    __inline__ bool __warn_unused __MVector_attr empty(void) noexcept
    {
        return (len == 0);
    }

    __inline__ const bool __warn_unused __MVector_const_attr empty(void) const noexcept
    {
        return (len == 0);
    }

    __inline__ void __MVector_attr resize(const unsigned int newlen) noexcept
    {
        if (newlen < len)
        {
            len = newlen;
        }
    }

    __inline__ T *__MVector_const_attr begin(void) const noexcept
    {
        return data;
    }

    __inline__ T *__MVector_const_attr end(void) const noexcept
    {
        return data + len;
    }

    /* Constructors. */
    MVector(void) noexcept : len(0), cap(10)
    {
        data = (T *)malloc(sizeof(T) * cap);
    }

    MVector(initializer_list<T> list) noexcept : len(0), cap(list.size() * 2)
    {
        data    = (T *)malloc(sizeof(T) * cap);
        for (const auto &it : list)
        {
            push_back(it);
        }
    }

    /* Destructor. */
    ~MVector(void) noexcept(is_nothrow_destructible<T>::value)
    {
        for (unsigned int i = 0; i < len; i++)
        {
            data[i].~T();
        }
        free(data);
    }

    /* Operator. */
    __inline__ T &__MVector_attr operator[](const unsigned int index) noexcept
    {
        return data[index];
    }

    __inline__ const T &__MVector_const_attr operator[](const unsigned int index) const noexcept
    {
        return data[index];
    }
};

#undef MVECTOR_ATTR
#undef MVECTOR_CONST_ATTR
