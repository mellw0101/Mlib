#pragma once
template <typename _T, unsigned long _Size>
struct array_t
{
    template <unsigned long _N>
    __inline__ __attribute__((__always_inline__, __nodebug__, __nothrow__))
    array_t(const _T (&array)[_N]) noexcept
    {
        static_assert((_N <= _Size), "Cannot init 'array_t' with a array bigger then '_Size'.");
        _data = array;
    }

    template <typename... Args>
    __inline__ __attribute__((__always_inline__, __nodebug__, __nothrow__))
    array_t(const Args &&...args) noexcept
    {
        int idx = 0;
        ((idx < _Size ? _data[idx++] = args : 0), ...);
    }

    __inline__ constexpr const unsigned long
        __attribute__((__always_inline__, __nodebug__, __const__, __nothrow__))
        size(void) const noexcept
    {
        return _Size;
    }

private:
    _T _data[_Size];
};