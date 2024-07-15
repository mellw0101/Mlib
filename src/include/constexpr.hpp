#pragma once

#include "def.h"

namespace Mlib::Constexpr
{
    constexpr bool
    is_sorted(C_s32 *arr, u64 size)
    {
        for (u64 i = 1; i < size; ++i)
        {
            if (arr[i - 1] > arr[i])
            {
                return false;
            }
        }
        return true;
    }

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

    template <typename T1, typename T2, u64 Size>
    using Map = ARRAY<MapEntry<T1, T2>, Size>;

    // template <typename T>
    // constexpr u64
    // fnv1a_32(std::span<const T> data)
    // {
    //     constexpr u64 fnv_prime    = 16777619u;
    //     constexpr u64 offset_basis = 2166136261u;

    //     u64 hash = offset_basis;
    //     for (const auto &byte : data)
    //     {
    //         hash ^= byte;
    //         hash *= fnv_prime;
    //     }
    //     return hash;
    // }

    // constexpr u64
    // hash_string(const std::string_view &s)
    // {
    //     return fnv1a_32(std::as_bytes(std::span {s.data(), s.size()}));
    // }

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
    strcmp(const s8 *str1, const s8 *str2) _NO_THROW
    {
        while (*str1 && (*str1 == *str2))
        {
            ++str1;
            ++str2;
        }
        return (*str1 - *str2);
    }

    constexpr s32
    strncmp(const s8 *s1, const s8 *s2, u64 n) _NO_THROW
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

    constexpr s8 *
    strcpy(s8 *dest, C_s8 *src) _NO_THROW
    {
        u64 i = 0;
        for (; src[i] != '\0'; ++i)
        {
            dest[i] = src[i];
        }
        dest[i] = '\0';
        return dest;
    }

    constexpr s8 *
    strncpy(s8 *dest, C_s8 *src, C_u64 n) _NO_THROW
    {
        u64 i = 0;
        for (; i < n && src[i] != '\0'; ++i)
        {
            dest[i] = src[i];
        }
        for (; i < n; ++i)
        {
            dest[i] = '\0';
        }
        return dest;
    }

    constexpr u64
    strlen(C_s8 *str) _NO_THROW
    {
        u64 i = 0;
        for (; str[i]; ++i)
        {}
        return i;
    }

    // template <u64 N>
    // class String
    // {
    // public:
    //     constexpr String(C_s8 (&str)[N + 1])
    //     {
    //         std::copy_n(str, N + 1, data);
    //     }

    //     constexpr C_PTR<s8>
    //     c_str() const
    //     {
    //         return data;
    //     }

    //     constexpr u64
    //     size() const
    //     {
    //         return N;
    //     }

    // private:
    //     char data[N + 1] {};
    // };

    template <u64 N>
    struct String
    {
        s8 data[N + 1] {};

        constexpr String(C_<s8> (&str)[N + 1])
        {
            std::copy_n(str, N + 1, data);
        }

        constexpr s8 &
        operator[](u64 i) const
        {
            return const_cast<s8 &>(data[i]);
        }

        constexpr C_s8 *
        c_str() const
        {
            return data;
        }

        constexpr u64
        size() const
        {
            return N;
        }
    };

    template <u64 N>
    String(C_s8 (&)[N]) -> String<N - 1>;

    template <u64 N>
    constexpr bool
    operator==(const String<N> &lhs, const s8 *rhs)
    {
        return strcmp(&lhs[0], rhs);
    }

    constexpr s32
    tolower(s32 ch) _NO_THROW
    {
        return (ch >= 'A' && ch <= 'Z') ? (ch + 'a' - 'A') : ch;
    }

    constexpr int
    atoi(const char *str) _NO_THROW
    {
        int result = 0;
        while (*str >= '0' && *str <= '9')
        {
            result = result * 10 + (*str - '0');
            ++str;
        }
        return result;
    }

    constexpr s32
    strcasecmp(const s8 *s1, const s8 *s2) _NO_THROW
    {
        while (*s1 && *s2)
        {
            const s32 c1 = tolower(static_cast<u8>(*s1));
            const s32 c2 = tolower(static_cast<u8>(*s2));
            if (c1 != c2)
            {
                return c1 - c2;
            }
            ++s1;
            ++s2;
        }
        return tolower(static_cast<u8>(*s1)) - tolower(static_cast<u8>(*s2));
    }

    //
    //  Returns 0 if the strings are equal,
    //  a positive number if s1 is greater than s2,
    //  and a negative number if s1 is less than s2
    //
    constexpr s32
    strncasecmp(C_s8 *s1, C_s8 *s2, u64 n) _NO_THROW
    {
        u64 i = 0;
        while (i < n && *s1 && *s2)
        {
            s32 c1 = tolower(*s1);
            s32 c2 = tolower(*s2);
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
                //
                //  's1' is longer than 's2'
                //
                return 1;
            }
            if (*s2)
            {
                //
                //  's2' is longer than 's1'
                //
                return -1;
            }
        }
        return 0;
    }

    constexpr char *
    strchr(char *str, char ch) _NO_THROW
    {
        while (*str)
        {
            if (*str == ch)
            {
                return str;
            }
            ++str;
        }
        return nullptr;
    }

    constexpr const s8 *
    strchr(C_s8 *str, s8 ch) _NO_THROW
    {
        while (*str)
        {
            if (*str == ch)
            {
                return str;
            }
            ++str;
        }
        return nullptr;
    }

    constexpr const char *
    strstr(const char *haystack, const char *needle) _NO_THROW
    {
        if (!*needle)
        {
            return haystack;
        }
        for (const char *h = haystack; *h; ++h)
        {
            const char *n     = needle;
            const char *start = h;
            while (*start && *n && *start == *n)
            {
                ++start;
                ++n;
            }
            if (!*n)
            {
                return h;
            }
        }
        return nullptr;
    }

    //
    //  Helper function to compare two characters case-insensitively
    //
    static constexpr bool char_equal_ignore_case(s8 a, s8 b) _NO_THROW _HIDDEN;
    static constexpr bool
    char_equal_ignore_case(s8 a, s8 b) _NO_THROW
    {
        return tolower(a) == tolower(b);
    }

    //
    //  Helper function to check if a string starts with another string, case-insensitively
    //
    static constexpr bool starts_with_ignore_case(C_s8 *str, C_s8 *prefix) _NO_THROW _HIDDEN;
    static constexpr bool
    starts_with_ignore_case(C_s8 *str, C_s8 *prefix) _NO_THROW
    {
        while (*prefix)
        {
            if (!char_equal_ignore_case(*str++, *prefix++))
            {
                return false;
            }
        }
        return true;
    }

    //
    //  The constexpr version of strcasestr
    //
    constexpr C_s8 *
    strcasestr(C_s8 *haystack, C_s8 *needle) _NO_THROW
    {
        if (!*needle)
        {
            return haystack;
        }
        while (*haystack)
        {
            if (starts_with_ignore_case(haystack, needle))
            {
                return haystack;
            }
            ++haystack;
        }
        return nullptr;
    }

    constexpr s64
    strtoll(C_s8 *str, s8 **endptr = nullptr, s32 base = 10) _NO_THROW
    {
        if (base < 2 || base > 36)
        {
            return s64_MAX;
        }

        C_s8 *ptr = str;
        while (std::isspace(*ptr))
        {
            ++ptr;
        }

        bool negative = false;
        if (*ptr == '-')
        {
            negative = true;
            ++ptr;
        }
        else if (*ptr == '+')
        {
            ++ptr;
        }

        s64   result     = 0;
        C_s64 maxDivBase = s64_MAX / base;
        while (*ptr != '\0')
        {
            int digit = -1;
            if (std::isdigit(*ptr))
            {
                digit = *ptr - '0';
            }
            else if (std::isalpha(*ptr))
            {
                digit = Constexpr::tolower(*ptr) - 'a' + 10;
            }

            if (digit < 0 || digit >= base)
            {
                break;
            }

            if (result > maxDivBase || (result == maxDivBase && digit > s64_MAX % base))
            {
                return s64_MAX;
            }

            result = result * base + digit;
            ++ptr;
        }

        if (endptr)
        {
            *endptr = const_cast<s8 *>(ptr);
        }

        return negative ? -result : result;
    }

    constexpr s8 *
    strcat(s8 *dest, C_s8 *src) _NO_THROW
    {
        s8 *start = dest;
        while (*dest)
        {
            ++dest;
        }
        while ((*dest++ = *src++))
            ;
        return start;
    }

    constexpr u64
    num_digits(s32 num) _NO_THROW
    {
        u64 digits = (num <= 0) ? 1 : 0;
        while (num)
        {
            num /= 10;
            ++digits;
        }
        return digits;
    }

    constexpr void
    itoa(s32 num, s8 *buffer) _NO_THROW
    {
        s8 *p = buffer;
        if (num < 0)
        {
            *p++ = '-';
            num  = -num;
        }
        do
        {
            *p++ = '0' + (num % 10);
            num /= 10;
        }
        while (num);
        *p = '\0';

        for (s8 *q = (*buffer == '-') ? buffer + 1 : buffer; q < --p; ++q)
        {
            s8 temp = *q;
            *q      = *p;
            *p      = temp;
        }
    }

    constexpr C_s8 *
    strrchr(C_s8 *str, C_s8 ch) _NO_THROW
    {
        C_s8 *last_occurrence = nullptr;
        while (*str)
        {
            if (*str == ch)
            {
                last_occurrence = str;
            }
            ++str;
        }
        return last_occurrence;
    }

    constexpr s8 *
    strrchr(s8 *str, C_s8 ch) _NO_THROW
    {
        s8 *last_occurrence = nullptr;
        while (*str)
        {
            if (*str == ch)
            {
                last_occurrence = str;
            }
            ++str;
        }
        return last_occurrence;
    }

    constexpr C_s8 *
    strpbrk(C_s8 *str, C_s8 *accept) _NO_THROW
    {
        while (*str)
        {
            C_s8 *a = accept;
            while (*a)
            {
                if (*str == *a)
                {
                    return str;
                }
                ++a;
            }
            ++str;
        }
        return nullptr;
    }

    namespace Chars
    {
        constexpr bool
        isblank(s8 ch) _NO_THROW
        {
            return ch == ' ' || ch == '\t';
        }

        //
        //  Helper function to check if a character is a combining character
        //
        static constexpr bool is_combining_character(c32 ucs) _HIDDEN;
        static constexpr bool
        is_combining_character(c32 ucs)
        {
            // Comprehensive Unicode combining characters ranges
            return (ucs >= 0x0300 && ucs <= 0x036F) || (ucs >= 0x1AB0 && ucs <= 0x1AFF) ||
                   (ucs >= 0x1DC0 && ucs <= 0x1DFF) || (ucs >= 0x20D0 && ucs <= 0x20FF) ||
                   (ucs >= 0xFE20 && ucs <= 0xFE2F) || (ucs >= 0x0591 && ucs <= 0x05BD) || (ucs == 0x05BF) ||
                   (ucs >= 0x05C1 && ucs <= 0x05C2) || (ucs >= 0x05C4 && ucs <= 0x05C5) || (ucs == 0x05C7) ||
                   (ucs >= 0x0610 && ucs <= 0x061A) || (ucs >= 0x064B && ucs <= 0x065F) || (ucs == 0x0670) ||
                   (ucs >= 0x06D6 && ucs <= 0x06DC) || (ucs >= 0x06DF && ucs <= 0x06E4) ||
                   (ucs >= 0x06E7 && ucs <= 0x06E8) || (ucs >= 0x06EA && ucs <= 0x06ED) ||
                   (ucs >= 0x0711 && ucs <= 0x073F) || (ucs >= 0x07A6 && ucs <= 0x07B0) ||
                   (ucs >= 0x07EB && ucs <= 0x07F3) || (ucs >= 0x0816 && ucs <= 0x0819) ||
                   (ucs >= 0x081B && ucs <= 0x0823) || (ucs >= 0x0825 && ucs <= 0x0827) ||
                   (ucs >= 0x0829 && ucs <= 0x082D) || (ucs >= 0x0859 && ucs <= 0x085B) ||
                   (ucs >= 0x08D3 && ucs <= 0x08E1) || (ucs >= 0x08E3 && ucs <= 0x0903) ||
                   (ucs >= 0x093A && ucs <= 0x093C) || (ucs >= 0x093E && ucs <= 0x094F) ||
                   (ucs >= 0x0951 && ucs <= 0x0957) || (ucs >= 0x0962 && ucs <= 0x0963) ||
                   (ucs >= 0x0981 && ucs <= 0x0983) || (ucs == 0x09BC) || (ucs == 0x09BE) ||
                   (ucs >= 0x09C0 && ucs <= 0x09C4) || (ucs >= 0x09C7 && ucs <= 0x09C8) ||
                   (ucs >= 0x09CB && ucs <= 0x09CD) || (ucs == 0x09D7) || (ucs >= 0x09E2 && ucs <= 0x09E3) ||
                   (ucs == 0x09FE) || (ucs >= 0x0A01 && ucs <= 0x0A03) || (ucs == 0x0A3C) || (ucs == 0x0A3E) ||
                   (ucs >= 0x0A40 && ucs <= 0x0A42) || (ucs >= 0x0A47 && ucs <= 0x0A48) ||
                   (ucs >= 0x0A4B && ucs <= 0x0A4D) || (ucs >= 0x0A51 && ucs <= 0x0A52) ||
                   (ucs >= 0x0A70 && ucs <= 0x0A71) || (ucs >= 0x0A75 && ucs <= 0x0A82) ||
                   (ucs >= 0x0ABC && ucs <= 0x0ACD) || (ucs >= 0x0AE2 && ucs <= 0x0AE3) || (ucs == 0x0AFA) ||
                   (ucs >= 0x0B01 && ucs <= 0x0B03) || (ucs >= 0x0B3C && ucs <= 0x0B4D) ||
                   (ucs >= 0x0B56 && ucs <= 0x0B57) || (ucs >= 0x0B62 && ucs <= 0x0B63) ||
                   (ucs >= 0x0B82 && ucs <= 0x0BC0) || (ucs >= 0x0BCD && ucs <= 0x0C4A) ||
                   (ucs >= 0x0C55 && ucs <= 0x0C56) || (ucs >= 0x0C62 && ucs <= 0x0C63) ||
                   (ucs >= 0x0CBC && ucs <= 0x0CC8) || (ucs >= 0x0CCA && ucs <= 0x0CCD) ||
                   (ucs >= 0x0CE2 && ucs <= 0x0CE3) || (ucs >= 0x0D00 && ucs <= 0x0D0C) ||
                   (ucs >= 0x0D3B && ucs <= 0x0D4D) || (ucs >= 0x0D62 && ucs <= 0x0D63) ||
                   (ucs >= 0x0DCA && ucs <= 0x0DD6) || (ucs >= 0x0E31 && ucs <= 0x0E3A) ||
                   (ucs >= 0x0E47 && ucs <= 0x0E4E) || (ucs >= 0x0EB1 && ucs <= 0x0EBC) ||
                   (ucs >= 0x0EC8 && ucs <= 0x0ECD) || (ucs >= 0x0F18 && ucs <= 0x0F39) ||
                   (ucs >= 0x0F71 && ucs <= 0x0F84) || (ucs >= 0x0F86 && ucs <= 0x0F87) ||
                   (ucs >= 0x0F8D && ucs <= 0x0F97) || (ucs >= 0x0F99 && ucs <= 0x0FBC) ||
                   (ucs >= 0x102B && ucs <= 0x103E) || (ucs >= 0x1056 && ucs <= 0x1059) ||
                   (ucs >= 0x105E && ucs <= 0x1060) || (ucs >= 0x1062 && ucs <= 0x1064) ||
                   (ucs >= 0x1067 && ucs <= 0x106D) || (ucs >= 0x1071 && ucs <= 0x1074) ||
                   (ucs >= 0x1082 && ucs <= 0x108D) || (ucs >= 0x108F && ucs <= 0x109A) ||
                   (ucs >= 0x135D && ucs <= 0x135F) || (ucs >= 0x1712 && ucs <= 0x1714) ||
                   (ucs >= 0x1732 && ucs <= 0x1734) || (ucs >= 0x1752 && ucs <= 0x1753) ||
                   (ucs >= 0x1772 && ucs <= 0x1773) || (ucs >= 0x17B4 && ucs <= 0x17D3) ||
                   (ucs >= 0x180B && ucs <= 0x180D) || (ucs >= 0x1885 && ucs <= 0x1886) ||
                   (ucs >= 0x18A9 && ucs <= 0x1923) || (ucs >= 0x1927 && ucs <= 0x1928) ||
                   (ucs >= 0x1932 && ucs <= 0x1935) || (ucs >= 0x1939 && ucs <= 0x193B) ||
                   (ucs >= 0x1A17 && ucs <= 0x1A1B) || (ucs >= 0x1A56 && ucs <= 0x1A60) ||
                   (ucs >= 0x1A62 && ucs <= 0x1A7C) || (ucs >= 0x1B00 && ucs <= 0x1B04) ||
                   (ucs >= 0x1B34 && ucs <= 0x1B44) || (ucs >= 0x1B6B && ucs <= 0x1B73) ||
                   (ucs >= 0x1B80 && ucs <= 0x1B82) || (ucs >= 0x1BA2 && ucs <= 0x1BA5) ||
                   (ucs >= 0x1BA8 && ucs <= 0x1BAA) || (ucs >= 0x1BAB && ucs <= 0x1BAD) ||
                   (ucs >= 0x1BE6 && ucs <= 0x1BF3) || (ucs >= 0x1C24 && ucs <= 0x1C37) ||
                   (ucs >= 0x1C36 && ucs <= 0x1C37) || (ucs >= 0x1CD0 && ucs <= 0x1CD2) ||
                   (ucs >= 0x1CD4 && ucs <= 0x1CE8) || (ucs >= 0x1CED && ucs <= 0x1CF7) ||
                   (ucs >= 0x1CF9 && ucs <= 0x1CFA) || (ucs >= 0x1DC0 && ucs <= 0x1DFF) ||
                   (ucs >= 0x20D0 && ucs <= 0x20FF) || (ucs >= 0x2CEF && ucs <= 0x2CF1) ||
                   (ucs >= 0x2D7F && ucs <= 0x2DEB) || (ucs >= 0x2DEB && ucs <= 0x2DFF) ||
                   (ucs >= 0x302A && ucs <= 0x302F) || (ucs >= 0x3099 && ucs <= 0x309A) ||
                   (ucs >= 0xA66F && ucs <= 0xA672) || (ucs >= 0xA674 && ucs <= 0xA67D) ||
                   (ucs >= 0xA69E && ucs <= 0xA6A0) || (ucs >= 0xA6F0 && ucs <= 0xA6F1) ||
                   (ucs >= 0xA8E0 && ucs <= 0xA8F1) || (ucs >= 0xA8FF && ucs <= 0xA92D) ||
                   (ucs >= 0xA92D && ucs <= 0xA92F) || (ucs >= 0xA953 && ucs <= 0xA96B) ||
                   (ucs >= 0xA9B3 && ucs <= 0xA9B3) || (ucs >= 0xA9B6 && ucs <= 0xA9B9) ||
                   (ucs >= 0xA9BC && ucs <= 0xA9BD) || (ucs >= 0xA9E5 && ucs <= 0xA9E5) ||
                   (ucs >= 0xAA29 && ucs <= 0xAA2E) || (ucs >= 0xAA31 && ucs <= 0xAA32) ||
                   (ucs >= 0xAA35 && ucs <= 0xAA36) || (ucs >= 0xAA43 && ucs <= 0xAA4C) ||
                   (ucs >= 0xAA4C && ucs <= 0xAA4C) || (ucs >= 0xAA7C && ucs <= 0xAA7C) ||
                   (ucs >= 0xAAB0 && ucs <= 0xAAB0) || (ucs >= 0xAAB2 && ucs <= 0xAAB4) ||
                   (ucs >= 0xAAB7 && ucs <= 0xAAB8) || (ucs >= 0xAABE && ucs <= 0xAABF) ||
                   (ucs >= 0xAAC1 && ucs <= 0xAAC1) || (ucs >= 0xAAEC && ucs <= 0xAAED) ||
                   (ucs >= 0xAAF6 && ucs <= 0xAB5B) || (ucs >= 0xAB5B && ucs <= 0xAB60) ||
                   (ucs >= 0xAB65 && ucs <= 0xAB65) || (ucs >= 0xABE3 && ucs <= 0xABE4) ||
                   (ucs >= 0xABE6 && ucs <= 0xABE7) || (ucs >= 0xABE9 && ucs <= 0xABEA) ||
                   (ucs >= 0xABEC && ucs <= 0xABED) || (ucs >= 0xFB1E && ucs <= 0xFE00) ||
                   (ucs >= 0xFE00 && ucs <= 0xFE0F) || (ucs >= 0xFE20 && ucs <= 0xFE2F) ||
                   (ucs >= 0x101FD && ucs <= 0x101FD) || (ucs >= 0x102E0 && ucs <= 0x102E0) ||
                   (ucs >= 0x10376 && ucs <= 0x1037A) || (ucs >= 0x10A01 && ucs <= 0x10A03) ||
                   (ucs >= 0x10A05 && ucs <= 0x10A06) || (ucs >= 0x10A0C && ucs <= 0x10A0F) ||
                   (ucs >= 0x10A38 && ucs <= 0x10A3A) || (ucs >= 0x10A3F && ucs <= 0x10AE5) ||
                   (ucs >= 0x10D24 && ucs <= 0x10D27) || (ucs >= 0x10EAB && ucs <= 0x10EAC) ||
                   (ucs >= 0x10F46 && ucs <= 0x10F50) || (ucs >= 0x11000 && ucs <= 0x11002) ||
                   (ucs >= 0x11038 && ucs <= 0x11046) || (ucs >= 0x1107F && ucs <= 0x11082) ||
                   (ucs >= 0x110B0 && ucs <= 0x110B2) || (ucs >= 0x110B7 && ucs <= 0x110B8) ||
                   (ucs >= 0x11100 && ucs <= 0x11102) || (ucs >= 0x11127 && ucs <= 0x11134) ||
                   (ucs >= 0x11173 && ucs <= 0x11173) || (ucs >= 0x11180 && ucs <= 0x11182) ||
                   (ucs >= 0x111B3 && ucs <= 0x111C0) || (ucs >= 0x111C9 && ucs <= 0x111CC) ||
                   (ucs >= 0x1122C && ucs <= 0x11237) || (ucs >= 0x112DF && ucs <= 0x112EA) ||
                   (ucs >= 0x11301 && ucs <= 0x11303) || (ucs >= 0x1133C && ucs <= 0x11340) ||
                   (ucs >= 0x11357 && ucs <= 0x11357) || (ucs >= 0x11362 && ucs <= 0x11363) ||
                   (ucs >= 0x114B0 && ucs <= 0x114C3) || (ucs >= 0x115AF && ucs <= 0x115B5) ||
                   (ucs >= 0x115B8 && ucs <= 0x115C0) || (ucs >= 0x11630 && ucs <= 0x11640) ||
                   (ucs >= 0x116AB && ucs <= 0x116B7) || (ucs >= 0x1171D && ucs <= 0x1172B) ||
                   (ucs >= 0x1182C && ucs <= 0x1183A) || (ucs >= 0x119D1 && ucs <= 0x119E1) ||
                   (ucs >= 0x11A01 && ucs <= 0x11A0A) || (ucs >= 0x11A33 && ucs <= 0x11A39) ||
                   (ucs >= 0x11C2F && ucs <= 0x11C36) || (ucs >= 0x11C38 && ucs <= 0x11C40) ||
                   (ucs >= 0x11C92 && ucs <= 0x11CA7) || (ucs >= 0x11D31 && ucs <= 0x11D36) ||
                   (ucs >= 0x11D3A && ucs <= 0x11D3A) || (ucs >= 0x11D3C && ucs <= 0x11D3D) ||
                   (ucs >= 0x11D3F && ucs <= 0x11D45) || (ucs >= 0x11D47 && ucs <= 0x11D48) ||
                   (ucs >= 0x11D90 && ucs <= 0x11D91) || (ucs >= 0x11D95 && ucs <= 0x11D95) ||
                   (ucs >= 0x11F00 && ucs <= 0x11F10) || (ucs >= 0x16AF0 && ucs <= 0x16AF5) ||
                   (ucs >= 0x16B30 && ucs <= 0x16B36) || (ucs >= 0x16F51 && ucs <= 0x16F87) ||
                   (ucs >= 0x16FF0 && ucs <= 0x16FF1) || (ucs >= 0x1BC9D && ucs <= 0x1BC9E) ||
                   (ucs >= 0x1C2C && ucs <= 0x1C33) || (ucs >= 0x1C36 && ucs <= 0x1C37) ||
                   (ucs >= 0x1CD0 && ucs <= 0x1CD2) || (ucs >= 0x1CD4 && ucs <= 0x1CE0) ||
                   (ucs >= 0x1CE2 && ucs <= 0x1CE8) || (ucs >= 0x1CED && ucs <= 0x1CED) ||
                   (ucs >= 0x1CF4 && ucs <= 0x1CF4) || (ucs >= 0x1CF8 && ucs <= 0x1CF9) ||
                   (ucs >= 0x1DC0 && ucs <= 0x1DFF) || (ucs >= 0x20D0 && ucs <= 0x20F0) ||
                   (ucs >= 0x2CEF && ucs <= 0x2CF1) || (ucs >= 0x2D7F && ucs <= 0x2D7F) ||
                   (ucs >= 0x2DE0 && ucs <= 0x2DFF) || (ucs >= 0x302A && ucs <= 0x302D) ||
                   (ucs >= 0x3099 && ucs <= 0x309A) || (ucs >= 0xA66F && ucs <= 0xA672) ||
                   (ucs >= 0xA674 && ucs <= 0xA67D) || (ucs >= 0xA69E && ucs <= 0xA69F) ||
                   (ucs >= 0xA6F0 && ucs <= 0xA6F1) || (ucs >= 0xA8E0 && ucs <= 0xA8F1) ||
                   (ucs >= 0xA8FF && ucs <= 0xA8FF) || (ucs >= 0xA926 && ucs <= 0xA92D) ||
                   (ucs >= 0xA947 && ucs <= 0xA953) || (ucs >= 0xA980 && ucs <= 0xA982) ||
                   (ucs >= 0xA9B3 && ucs <= 0xA9B3) || (ucs >= 0xA9B6 && ucs <= 0xA9B9) ||
                   (ucs >= 0xA9BC && ucs <= 0xA9BD) || (ucs >= 0xA9E5 && ucs <= 0xA9E5) ||
                   (ucs >= 0xAA29 && ucs <= 0xAA2E) || (ucs >= 0xAA31 && ucs <= 0xAA32) ||
                   (ucs >= 0xAA35 && ucs <= 0xAA36) || (ucs >= 0xAA43 && ucs <= 0xAA43) ||
                   (ucs >= 0xAA4C && ucs <= 0xAA4C) || (ucs >= 0xAA7C && ucs <= 0xAA7C) ||
                   (ucs >= 0xAAB0 && ucs <= 0xAAB0) || (ucs >= 0xAAB2 && ucs <= 0xAAB4) ||
                   (ucs >= 0xAAB7 && ucs <= 0xAAB8) || (ucs >= 0xAABE && ucs <= 0xAABF) ||
                   (ucs >= 0xAAC1 && ucs <= 0xAAC1) || (ucs >= 0xAAEC && ucs <= 0xAAED) ||
                   (ucs >= 0xAAF6 && ucs <= 0xAAF6) || (ucs >= 0xABE5 && ucs <= 0xABE5) ||
                   (ucs >= 0xABE8 && ucs <= 0xABE8) || (ucs >= 0xABED && ucs <= 0xABED) ||
                   (ucs >= 0xFB1E && ucs <= 0xFB1E) || (ucs >= 0xFE00 && ucs <= 0xFE0F) ||
                   (ucs >= 0xFE20 && ucs <= 0xFE2F) || (ucs >= 0x101FD && ucs <= 0x101FD) ||
                   (ucs >= 0x102E0 && ucs <= 0x102E0) || (ucs >= 0x10376 && ucs <= 0x1037A) ||
                   (ucs >= 0x10A01 && ucs <= 0x10A03) || (ucs >= 0x10A05 && ucs <= 0x10A06) ||
                   (ucs >= 0x10A0C && ucs <= 0x10A0F) || (ucs >= 0x10A38 && ucs <= 0x10A3A) ||
                   (ucs >= 0x10A3F && ucs <= 0x10A3F) || (ucs >= 0x11001 && ucs <= 0x11001) ||
                   (ucs >= 0x11038 && ucs <= 0x11046) || (ucs >= 0x1107F && ucs <= 0x11082) ||
                   (ucs >= 0x110B3 && ucs <= 0x110B6) || (ucs >= 0x110B9 && ucs <= 0x110BA) ||
                   (ucs >= 0x11100 && ucs <= 0x11102) || (ucs >= 0x11127 && ucs <= 0x1112B) ||
                   (ucs >= 0x1112D && ucs <= 0x11134) || (ucs >= 0x11173 && ucs <= 0x11173) ||
                   (ucs >= 0x11180 && ucs <= 0x11181) || (ucs >= 0x111B6 && ucs <= 0x111BE) ||
                   (ucs >= 0x111C9 && ucs <= 0x111CC) || (ucs >= 0x1122F && ucs <= 0x11231) ||
                   (ucs >= 0x11234 && ucs <= 0x11234) || (ucs >= 0x11236 && ucs <= 0x11237) ||
                   (ucs >= 0x112DF && ucs <= 0x112DF) || (ucs >= 0x112E3 && ucs <= 0x112EA) ||
                   (ucs >= 0x11300 && ucs <= 0x11301) || (ucs >= 0x1133B && ucs <= 0x1133B) ||
                   (ucs >= 0x1133E && ucs <= 0x1133F) || (ucs >= 0x11357 && ucs <= 0x11357) ||
                   (ucs >= 0x11366 && ucs <= 0x1136C) || (ucs >= 0x11370 && ucs <= 0x11374) ||
                   (ucs >= 0x114B3 && ucs <= 0x114B8) || (ucs >= 0x114BA && ucs <= 0x114BA) ||
                   (ucs >= 0x114BF && ucs <= 0x114C0) || (ucs >= 0x114C2 && ucs <= 0x114C3) ||
                   (ucs >= 0x115B2 && ucs <= 0x115B5) || (ucs >= 0x115BC && ucs <= 0x115BD) ||
                   (ucs >= 0x115BF && ucs <= 0x115C0) || (ucs >= 0x115DC && ucs <= 0x115DD) ||
                   (ucs >= 0x11633 && ucs <= 0x1163A) || (ucs >= 0x1163D && ucs <= 0x1163D) ||
                   (ucs >= 0x1163F && ucs <= 0x11640) || (ucs >= 0x116AB && ucs <= 0x116AB) ||
                   (ucs >= 0x116AD && ucs <= 0x116AD) || (ucs >= 0x116B0 && ucs <= 0x116B5) ||
                   (ucs >= 0x116B7 && ucs <= 0x116B7) || (ucs >= 0x1171D && ucs <= 0x1171F) ||
                   (ucs >= 0x11722 && ucs <= 0x11725) || (ucs >= 0x11727 && ucs <= 0x1172B) ||
                   (ucs >= 0x1182F && ucs <= 0x11837) || (ucs >= 0x11839 && ucs <= 0x1183A) ||
                   (ucs >= 0x11A01 && ucs <= 0x11A0A) || (ucs >= 0x11A33 && ucs <= 0x11A38) ||
                   (ucs >= 0x11A3B && ucs <= 0x11A3E) || (ucs >= 0x11A47 && ucs <= 0x11A47) ||
                   (ucs >= 0x11A51 && ucs <= 0x11A56) || (ucs >= 0x11A59 && ucs <= 0x11A5B) ||
                   (ucs >= 0x11A8A && ucs <= 0x11A96) || (ucs >= 0x11A98 && ucs <= 0x11A99) ||
                   (ucs >= 0x11C30 && ucs <= 0x11C36) || (ucs >= 0x11C38 && ucs <= 0x11C3D) ||
                   (ucs >= 0x11C3F && ucs <= 0x11C3F) || (ucs >= 0x11C92 && ucs <= 0x11CA7) ||
                   (ucs >= 0x11CAA && ucs <= 0x11CB0) || (ucs >= 0x11CB2 && ucs <= 0x11CB3) ||
                   (ucs >= 0x11CB5 && ucs <= 0x11CB6) || (ucs >= 0x16AF0 && ucs <= 0x16AF4) ||
                   (ucs >= 0x16B30 && ucs <= 0x16B36) || (ucs >= 0x16F8F && ucs <= 0x16F92) ||
                   (ucs >= 0x1BC9D && ucs <= 0x1BC9E) || (ucs >= 0x1D167 && ucs <= 0x1D169) ||
                   (ucs >= 0x1D17B && ucs <= 0x1D182) || (ucs >= 0x1D185 && ucs <= 0x1D18B) ||
                   (ucs >= 0x1D1AA && ucs <= 0x1D1AD) || (ucs >= 0x1D242 && ucs <= 0x1D244) ||
                   (ucs >= 0x1DA00 && ucs <= 0x1DA36) || (ucs >= 0x1DA3B && ucs <= 0x1DA6C) ||
                   (ucs >= 0x1DA75 && ucs <= 0x1DA75) || (ucs >= 0x1DA84 && ucs <= 0x1DA84) ||
                   (ucs >= 0x1DA9B && ucs <= 0x1DA9F) || (ucs >= 0x1DAA1 && ucs <= 0x1DAAF) ||
                   (ucs >= 0x1E000 && ucs <= 0x1E006) || (ucs >= 0x1E008 && ucs <= 0x1E018) ||
                   (ucs >= 0x1E01B && ucs <= 0x1E021) || (ucs >= 0x1E023 && ucs <= 0x1E024) ||
                   (ucs >= 0x1E026 && ucs <= 0x1E02A) || (ucs >= 0x1E8D0 && ucs <= 0x1E8D6) ||
                   (ucs >= 0x1E944 && ucs <= 0x1E94A) || (ucs >= 0xE0100 && ucs <= 0xE01EF);
        }

        //
        //  The constexpr version of wcwidth
        //
        //  TODO : DOES NOT WORK YET
        //
        constexpr s32
        wcwidth(char32_t ucs)
        {
            // Control characters
            if (ucs == 0)
            {
                return 0;
            }
            if (ucs < 32 || (ucs >= 0x7F && ucs < 0xA0))
            {
                return -1;
            }

            // Combining characters
            if (is_combining_character(ucs))
            {
                return 0;
            }

            // Wide characters
            if ((ucs >= 0x1100 && ucs <= 0x115F) || // Hangul Jamo init. consonants
                (ucs >= 0x2329 && ucs <= 0x232A) || (ucs >= 0x2E80 && ucs <= 0xA4CF && ucs != 0x303F) || // CJK ... Yi
                (ucs >= 0xAC00 && ucs <= 0xD7A3) || // Hangul Syllables
                (ucs >= 0xF900 && ucs <= 0xFAFF) || // CJK Compatibility Ideographs
                (ucs >= 0xFE10 && ucs <= 0xFE19) || // Vertical forms
                (ucs >= 0xFE30 && ucs <= 0xFE6F) || // CJK Compatibility Forms
                (ucs >= 0xFF00 && ucs <= 0xFF60) || // Fullwidth Forms
                (ucs >= 0xFFE0 && ucs <= 0xFFE6) || (ucs >= 0x20000 && ucs <= 0x2FFFD) ||
                (ucs >= 0x30000 && ucs <= 0x3FFFD))
            {
                return 2;
            }

            //
            //  All other characters are assumed to be of width 1
            //
            return 1;
        }
    } // namespace Chars

} // namespace Mlib::Constexpr

//
//  Some useful aliases for the Constexpr namespace
//
#define CONSTEXPR_STRING                            constexpr Mlib::Constexpr::String
#define CONSTEXPR_ARRAY                             constexpr std::array
#define CONSTEXPR_MAP                               constexpr Mlib::Constexpr::Map

#define constexpr_strcasecmp(_Str1, _Str2)          Mlib::Constexpr::strcasecmp(_Str1, _Str2)
#define constexpr_strncasecmp(_Str1, _Str2, _Count) Mlib::Constexpr::strncasecmp(_Str1, _Str2, _Count)
#define constexpr_strchr(_Str, _Ch)                 Mlib::Constexpr::strchr(_Str, _Ch)
#define constexpr_strcpy(_Dest, _Src)               Mlib::Constexpr::strcpy(_Dest, _Src)
#define constexpr_strcmp(_Str1, _Str2)              Mlib::Constexpr::strcmp(_Str1, _Str2)
#define constexpr_tolower(_Ch)                      Mlib::Constexpr::tolower(_Ch)
#define constexpr_atoi(_Str)                        Mlib::Constexpr::atoi(_Str)
#define constexpr_strlen(_Str)                      Mlib::Constexpr::strlen(_Str)
#define constexpr_strncmp(_Str1, _Str2, _Count)     Mlib::Constexpr::strncmp(_Str1, _Str2, _Count)
#define constexpr_strstr(_Haystack, _Needle)        Mlib::Constexpr::strstr(_Haystack, _Needle)
#define constexpr_strcasestr(_Haystack, _Needle)    Mlib::Constexpr::strcasestr(_Haystack, _Needle)
#define constexpr_strncpy(_Dest, _Src, _N)          Mlib::Constexpr::strncpy(_Dest, _Src, _N)
#define constexpr_strtoll(_Str, _End, _Base)        Mlib::Constexpr::strtoll(_Str, _End, _Base)
#define constexpr_strcat(_Dest, _Src)               Mlib::Constexpr::strcat(_Dest, _Src)
#define constexpr_strrchr(_Str, _Ch)                Mlib::Constexpr::strrchr(_Str, _Ch)
#define constexpr_strpbrk(_Str, _Accept)            Mlib::Constexpr::strpbrk(_Str, _Accept)

#define constexpr_isblank(_Ch)                      Mlib::Constexpr::Chars::isblank(_Ch)
#define constexpr_wcwidth(_Ucs)                     Mlib::Constexpr::Chars::wcwidth(_Ucs)

//
//  Compile-time hash function
//
constexpr u64
operator"" _constexpr_hash(const s8 *s, u64)
{
    return Mlib::Constexpr::hash_string(s);
}
