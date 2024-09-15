#pragma once

#include <stdlib.h>

template <class T>
class MVector
{
    T           *data;
    unsigned int len;
    unsigned int cap;

public:
    MVector(void)
        : len(0)
        , cap(10)
    {
        cap  = 10;
        len  = 0;
        data = (T *)malloc(sizeof(T) * cap);
    }

    ~MVector(void)
    {
        for (unsigned int i = 0; i < len; i++)
        {
            data[i].~T();
        }
        free(data);
    }

    T &operator[](unsigned int index)
    {
        return data[index];
    }

    void push_back(const T &element)
    {
        if (len == cap)
        {
            cap *= 2;
            data = (T *)realloc(data, sizeof(T) * cap);
        }
        new (data + len) T(element);
        len++;
    }

    unsigned int size(void)
    {
        return len;
    }

    inline bool empty(void)
    {
        return (len == 0);
    }

    T *begin(void)
    {
        return data;
    }

    T *end(void)
    {
        return data + len;
    }
};
