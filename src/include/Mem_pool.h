#pragma once

#include "Debug.h"
#include "def.h"

#include <immintrin.h>
#include <malloc.h>
#include <mm_malloc.h>

template <typename Align_T>
class mem_pool_t
{
    char         *_pool;
    unsigned long _pool_size;
    unsigned long _offset;

public:
    explicit mem_pool_t(unsigned long size __align_size(alignof(Align_T))) noexcept
        : _pool_size(size)
        , _offset(0)
    {
        _pool = (char *)_mm_malloc(size, alignof(Align_T));
        if (_pool == nullptr)
        {
            logE("mem_pool_t failed to be initilized.");
            exit(1);
        }
    }

    ~mem_pool_t(void) noexcept
    {
        _mm_free(_pool);
    }

    void *alloc(unsigned long size __align_size(alignof(Align_T)),
                unsigned long      alignment = alignof(Align_T)) noexcept
    {
        unsigned long current_offset = (unsigned long)(_pool + _offset);
        unsigned long aligned_offset = (current_offset + alignment - 1) & ~(alignment - 1);
        unsigned long aligned_size   = aligned_offset - current_offset;
        if (_offset + aligned_size + size > _pool_size)
        {
            logE("mem_pool_t ran out off memory.");
            return nullptr;
        }
        _offset += aligned_size + size;
        return (void *)(_pool + aligned_offset - (unsigned long)_pool);
    }

    template <typename T>
    __inline T *__attribute((__always_inline__, __nodebug__)) alloc(
        unsigned long size      = sizeof(T),
        unsigned long alignment = alignof(Align_T)) noexcept(std::is_nothrow_destructible<T>::value)
    {
        if (alignment == 0 || (alignment & (alignment - 1)) != 0)
        {
            logE("Invalid alignment.  Must be a power of 2.");
            return nullptr;
        }
        unsigned long current_offset = (unsigned long)(_pool + _offset);
        unsigned long aligned_offset = (current_offset + alignment - 1) & ~(alignment - 1);
        unsigned long aligned_size   = aligned_offset - current_offset;
        if (_offset + aligned_size + sizeof(T) > _pool_size)
        {
            logE("mem_pool_t ran out off memory.");
            return nullptr;
        }
        _offset += aligned_size + sizeof(T);
        return (T *)(_pool + aligned_offset - (unsigned long)_pool);
    }

    __always_inline void reset(void) noexcept
    {
        _offset = 0;
    }

    __always_inline unsigned long used_memory(void) noexcept
    {
        return _offset;
    }

    __always_inline unsigned long available_memory(void) noexcept
    {
        return _pool_size - _offset;
    }
} __align_size(alignof(Align_T));

#define MB (1024 * 1024)
#define GB (1024 * MB)
