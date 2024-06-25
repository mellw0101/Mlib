#pragma once

#include "def.h"

#include <array>
// #include <cstdint>
// #include <iostream>
// #include <stdexcept>
// #include <string>
// #include <cmath>
#include <string_view>
// #include <vector>
// #include <unordered_map>

namespace Mlib::Constexpr
{
    template <typename T1, typename T2>
    struct MapEntry
    {
        T1 key;
        T2 value;

        constexpr MapEntry(T1 key, T2 value)
            : key(key)
            , value(value)
        {}
    };

    template <typename T1, typename T2, u64 SIZE>
    struct Map
    {
        std::array<MapEntry<T1, T2>, SIZE> map;

        constexpr Map(std::initializer_list<MapEntry<T1, T2>> map, size_t size)
            : map((std::array<MapEntry<T1, T2>, map.size()>)map)
        {}

        constexpr MapEntry<T1, T2> &
        operator[](size_t index)
        {
            return map[index];
        }

        constexpr const MapEntry<T1, T2> &
        operator[](size_t index) const
        {
            return map[index];
        }
    };

    template <typename T1, typename T2, u64 Size>
    using ConstexprMap = std::array<MapEntry<T1, T2>, Size>;

    template <u64 Size>
    using ConstexprHashBitfieldMap = ConstexprMap<u64, u32, Size>;

    template <u64 Size>
    using ConstexprStrBitfieldMap = ConstexprMap<std::string_view, u32, Size>;

    constexpr u64
    fnv1a_32(const s8 *s, u64 count)
    {
        constexpr u64 fnv_prime    = 16777619u;
        constexpr u64 offset_basis = 2166136261u;

        u64 hash = offset_basis;
        for (u64 i = 0; i < count; ++i)
        {
            hash ^= s[i];
            hash *= fnv_prime;
        }
        return hash;
    }

    constexpr u64
    hash_string(const char *s)
    {
        return fnv1a_32(s, std::char_traits<s8>::length(s));
    }

    constexpr std::size_t
    operator"" _hash(const char *s, std::size_t)
    {
        return hash_string(s);
    }

    //
    //  Compile-time string comparison function
    //
    constexpr bool
    constexpr_strcmp(const s8 *str1, const s8 *str2)
    {
        while (*str1 && (*str1 == *str2))
        {
            ++str1;
            ++str2;
        }
        return (*str1 == *str2);
    }

    constexpr s32
    constexpr_strncmp(const s8 *s1, const s8 *s2, u64 n)
    {
        for (std::size_t i = 0; i < n; ++i)
        {
            if (s1[i] == '\0' && s2[i] == '\0')
            {
                return 0;
            }
            if (s1[i] == '\0')
            {
                return -1;
            }
            if (s2[i] == '\0')
            {
                return 1;
            }
            if (s1[i] < s2[i])
            {
                return -1;
            }
            if (s1[i] > s2[i])
            {
                return 1;
            }
        }
        return 0;
    }

    template <u64 N>
    constexpr void
    constexpr_strncpy(char (&dest)[N], const s8 *src, u64 count)
    {
        u64 i = 0;
        for (; i < count && src[i] != '\0'; ++i)
        {
            dest[i] = src[i];
        }
        for (; i < N; ++i)
        {
            dest[i] = '\0';
        }
    }

    template <std::size_t N>
    struct String
    {
        s8 data[N + 1] = {};

        constexpr String(const s8 *str)
        {
            constexpr_strncpy(data, str, N);
        }

        constexpr s8 &
        operator[](std::size_t i) const
        {
            return data[i];
        }

        constexpr const s8 *
        c_str() const
        {
            return data;
        }
    };

    //
    //  Deduction guide for CTAD (Class Template Argument Deduction)
    //
    template <std::size_t N>
    String(const s8 (&str)[N]) -> String<N - 1>;

    template <std::size_t N>
    constexpr bool
    operator==(const String<N> &lhs, const s8 *rhs)
    {
        return constexpr_strcmp(&lhs[0], rhs);
    }

    constexpr s32
    constexpr_tolower(s32 ch)
    {
        return (ch >= 'A' && ch <= 'Z') ? (ch + 'a' - 'A') : ch;
    }

    constexpr s32
    constexpr_strcasecmp(const s32 *s1, const s32 *s2)
    {
        while (*s1 && *s2)
        {
            const s32 c1 = constexpr_tolower(*s1);
            const s32 c2 = constexpr_tolower(*s2);
            if (c1 != c2)
            {
                return c1 - c2;
            }
            ++s1;
            ++s2;
        }
        return constexpr_tolower(*s1) - constexpr_tolower(*s2);
    }

    //
    //  Returns 0 if the strings are equal,
    //  a positive number if s1 is greater than s2,
    //  and a negative number if s1 is less than s2
    //
    constexpr int
    constexpr_strncasecmp(const s8 *s1, const s8 *s2, u64 n)
    {
        u64 i = 0;
        while (i < n && *s1 && *s2)
        {
            s32 c1 = constexpr_tolower(*s1);
            s32 c2 = constexpr_tolower(*s2);
            if (c1 != c2)
            {
                return c1 - c2;
            }
            ++s1;
            ++s2;
            ++i;
        }
        if (i < n)
        {
            if (*s1)
            {
                // s1 is longer than s2
                return 1;
            }
            if (*s2)
            {
                return -1; // s2 is longer than s1
            }
        }
        return 0;
    }
} // namespace Mlib::Constexpr

//
//  Some useful aliases for the Constexpr namespace
//
#define CONSTEXPR_STRING     constexpr Mlib::Constexpr::String
#define CONSTEXPR_ARRAY      constexpr Mlib::Constexpr::Array
#define CONSTEXPR_MAP        constexpr Mlib::Constexpr::ConstexprMap
#define CONSTEXPR_STRBITMAP  constexpr Mlib::Constexpr::ConstexprStrBitfieldMap
#define CONSTEXPR_HASHBITMAP constexpr Mlib::Constexpr::ConstexprHashBitfieldMap

//
//  Compile-time hash function
//
constexpr std::size_t
operator"" _constexpr_hash(const s8 *s, u64)
{
    return Mlib::Constexpr::hash_string(s);
}
