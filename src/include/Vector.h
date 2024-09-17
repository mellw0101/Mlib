#pragma once

#include <stdlib.h>
#include "Debug.h"
#include "def.h"

template <class T>
class MVector
{
    T           *data;
    unsigned int len;
    unsigned int cap;

public:
    MVector(void) noexcept
        : len(0)
        , cap(10)
    {
        cap  = 10;
        len  = 0;
        data = (T *)malloc(sizeof(T) * cap);
    }

    ~MVector(void) noexcept(std::is_nothrow_destructible<T>::value)
    {
        for (unsigned int i = 0; i < len; i++)
        {
            data[i].~T();
        }
        free(data);
    }

    __always_inline T &operator[](unsigned int index) noexcept
    {
        return data[index];
    }

    void push_back(const T &element) noexcept
    {
        if (len == cap)
        {
            cap *= 2;
            data = AREALLOC_ARRAY(data, cap);
        }
        new (data + len) T(element);
        len++;
    }

    __always_inline void reorder_from(unsigned int newstart) noexcept
    {
        if (newstart < len)
        {
            for (unsigned int i = newstart; i < len; i++)
            {
                data[i - newstart] = std::move(data[i]);
            }
        }
        len -= newstart;
    }

    __always_inline void
    erase_at(unsigned int index) noexcept(std::is_nothrow_destructible<T>::value)
    {
        if (index >= len)
        {
            return;
        }
        data[index].~T();
        for (unsigned int i = index; i < len - 1; i++)
        {
            data[i] = std::move(data[i + 1]);
        }
        len -= 1;
    }

    /* Note that this does not free any elements in the array
     * it only free`s the current array and set`s 'len' to 0
     * then mallocs the 'data' ptr again. */
    void clear(void) noexcept(std::is_nothrow_destructible<T>::value)
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

    __always_inline unsigned int size(void) const noexcept
    {
        return len;
    }

    __always_inline bool empty(void) const noexcept
    {
        return (len == 0);
    }

    __always_inline void resize(unsigned int newlen) noexcept
    {
        if (newlen < len)
        {
            len = newlen;
        }
    }

    __always_inline T *begin(void) const noexcept
    {
        return data;
    }

    __always_inline T *end(void) const noexcept
    {
        return data + len;
    }
};
