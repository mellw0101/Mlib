#pragma once

#include "Attributes.h"
#include "Debug.h"
#include "def.h"

#include <immintrin.h>
#include <malloc.h>
#include <mm_malloc.h>

#define __mem_pool_t_attr __attr(__always_inline__, __nodebug__, __nothrow__)
template <Ulong Alignment>
class mem_pool_t {
  static_assert((Alignment % 2) == 0 && Alignment != 0, "Alignment must be a power of 2.");

  char *_pool;
  Ulong _pool_size;
  Ulong _offset;

 public:
  explicit mem_pool_t(Ulong size) noexcept
      : _pool_size(size)
      , _offset(0) {
    _pool = (char *)_mm_malloc(size, Alignment);
    if (_pool == nullptr) {
      logE("mem_pool_t failed to be initilized.");
      exit(1);
    }
  }

  ~mem_pool_t(void) noexcept {
    _mm_free(_pool);
  }

  __inline__ __ptr<void> __warn_unused __mem_pool_t_attr alloc(Ulong size, Ulong alignment) noexcept {
    Ulong current_offset = (Ulong)(_pool + _offset);
    Ulong aligned_offset = (current_offset + alignment - 1) & ~(alignment - 1);
    Ulong aligned_size   = aligned_offset - current_offset;
    if (_offset + aligned_size + size > _pool_size) {
      logE("mem_pool_t ran out off memory.");
      return nullptr;
    }
    _offset += aligned_size + size;
    return (void *)(_pool + aligned_offset - (Ulong)_pool);
  }

  template <typename T, typename... Args>
  __inline__ T *__warn_unused __mem_pool_t_attr alloc(unsigned long size      = sizeof(T),
                                                      unsigned long alignment = alignof(T),
                                                      Args &&...args) noexcept(std::is_nothrow_destructible<T>::value) {
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
      logE("Invalid alignment.  Must be a power of 2.");
      return nullptr;
    }
    unsigned long current_offset = (unsigned long)(_pool + _offset);
    unsigned long aligned_offset = (current_offset + alignment - 1) & ~(alignment - 1);
    unsigned long aligned_size   = aligned_offset - current_offset;
    if (_offset + aligned_size + size > _pool_size) {
      logE("mem_pool_t ran out off memory.");
      return nullptr;
    }
    _offset += aligned_size + size;
    void *mem_location = (void *)(_pool + aligned_offset - (unsigned long)_pool);
    return new (mem_location) T(std::forward<Args>(args)...);
  }

  template <typename T>
  __inline__ T *__warn_unused __mem_pool_t_attr
  alloc(Ulong size = sizeof(T), Ulong alignment = alignof(T)) noexcept(std::is_nothrow_destructible<T>::value) {
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
      logE("Invalid alignment.  Must be a power of 2.");
      return nullptr;
    }
    unsigned long current_offset = (unsigned long)(_pool + _offset);
    unsigned long aligned_offset = (current_offset + alignment - 1) & ~(alignment - 1);
    unsigned long aligned_size   = aligned_offset - current_offset;
    if (_offset + aligned_size + size > _pool_size) {
      logE("mem_pool_t ran out off memory.");
      return nullptr;
    }
    _offset += aligned_size + size;
    void *mem_location = (void *)(_pool + aligned_offset - (unsigned long)_pool);
    return new (mem_location) T();
    // return (T *)(_pool + aligned_offset - (unsigned long)_pool);
  }

  __inline__ void __mem_pool_t_attr reset(void) noexcept {
    _offset = 0;
  }

  __inline__ unsigned long __mem_pool_t_attr used_memory(void) noexcept {
    return _offset;
  }

  __inline__ unsigned long __mem_pool_t_attr available_memory(void) noexcept {
    return _pool_size - _offset;
  }
};
#undef __mem_pool_t_attr
#define MB (1024 * 1024)
#define GB (1024 * MB)
