#pragma once


#pragma region /** @def Macros */
#pragma region /** @def Color Macros */


#define ESC_CODE_RED     "\033[31m"
#define ESC_CODE_GREEN   "\033[32m"
#define ESC_CODE_YELLOW  "\033[33m"
#define ESC_CODE_BLUE    "\033[34m"
#define ESC_CODE_MAGENTA "\033[35m"
#define ESC_CODE_CYAN    "\033[36m"
#define ESC_CODE_WHITE   "\033[37m"
#define ESC_CODE_RESET   "\033[0m"
#define ESC_CODE_GRAY    "\033[90m"
#define ESC_CODE_BOLD    "\033[1m"


#pragma endregion /** END: Color Macros */
#pragma region    /** @def ANSI cursor Macros */


#define ESC_CODE_UNDERLINE               "\033[4m"
#define ESC_CODE_BLINK                   "\033[5m"
#define ESC_CODE_REVERSE                 "\033[7m"
#define ESC_CODE_INVISIBLE               "\033[8m"
#define ESC_CODE_CLEAR                   "\033[2J"
#define ESC_CODE_CLEAR_LINE              "\033[2K"
#define ESC_CODE_CLEAR_LINE_END          "\033[K"
#define ESC_CODE_CLEAR_LINE_START        "\033[1K"
#define ESC_CODE_CLEAR_LINE_ALL          "\033[2K\033[1K"
#define ESC_CODE_CLEAR_LINE_ALL_END      "\033[2K\033[K"
#define ESC_CODE_CLEAR_LINE_ALL_START    "\033[2K\033[1K"
#define ESC_CODE_CLEAR_LINE_ALL_ALL      "\033[2K\033[1K\033[K"
#define ESC_CODE_CURSOR_UP               "\033[A"
#define ESC_CODE_CURSOR_DOWN             "\033[B"
#define ESC_CODE_CURSOR_RIGHT            "\033[C"
#define ESC_CODE_CURSOR_LEFT             "\033[D"
#define ESC_CODE_CURSOR_NEXT_LINE        "\033[E"
#define ESC_CODE_CURSOR_PREV_LINE        "\033[F"
#define ESC_CODE_CURSOR_COLUMN           "\033[G"
#define ESC_CODE_CURSOR_POSITION         "\033[H"
#define ESC_CODE_CURSOR_POSITION_ALT     "\033[f"
#define ESC_CODE_CURSOR_SAVE             "\033[s"
#define ESC_CODE_CURSOR_RESTORE          "\033[u"
#define ESC_CODE_CURSOR_HIDE             "\033[?25l"
#define ESC_CODE_CURSOR_SHOW             "\033[?25h"
#define ESC_CODE_CURSOR_BLINK            "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK          "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT        "\033[?12h"
#define ESC_CODE_CURSOR_NOBLINK_ALT      "\033[?12l"
#define ESC_CODE_CURSOR_BLINK_ALT2       "\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT2     "\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT3       "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT3     "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT4       "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT4     "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT5       "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT5     "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT6       "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT6     "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT7       "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT7     "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT8       "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT8     "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT9       "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT9     "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT10      "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT10    "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT11      "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT11    "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT12      "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT12    "\033[?12;25l"
#define ESC_CODE_CURSOR_UP_ALT           "\033M"
#define ESC_CODE_CURSOR_DOWN_ALT         "\033D"
#define ESC_CODE_CURSOR_RIGHT_ALT        "\033C"
#define ESC_CODE_CURSOR_LEFT_ALT         "\033D"
#define ESC_CODE_CURSOR_NEXT_LINE_ALT    "\033E"
#define ESC_CODE_CURSOR_PREV_LINE_ALT    "\033F"
#define ESC_CODE_CURSOR_COLUMN_ALT       "\033G"
#define ESC_CODE_CURSOR_POSITION_ALT_ALT "\033H"


#pragma endregion /** END: ANSI cursor Macros */


#define FORCE_INLINE       __attribute__((always_inline)) static __inline__
#define UNUSED             __attribute__((unused))
#define NORETURN           __attribute__((noreturn))

#define STRLITERAL(x)      #x

#define FLAGMASK(flag)     ((unsigned)1 << ((flag) % (sizeof(unsigned) * 8)))
#define SET(flag)          FLAGS(flag) |= FLAGMASK(flag)
#define UNSET(flag)        FLAGS(flag) &= ~FLAGMASK(flag)
#define ISSET(flag)        ((FLAGS(flag) & FLAGMASK(flag)) != 0)
#define TOGGLE(flag)       FLAGS(flag) ^= FLAGMASK(flag)
#define ISBITSET(var, bit) ((var & bit) == true)

#define DELETE_MOVE_CONSTRUCTORS(class_name)       \
    class_name(class_name &&)            = delete; \
    class_name &operator=(class_name &&) = delete;

#define DELETE_COPY_CONSTRUCTORS(class_name)            \
    class_name(const class_name &)            = delete; \
    class_name &operator=(const class_name &) = delete;

#define DELETE_COPY_AND_MOVE_CONSTRUCTORS(class_name) \
    DELETE_COPY_CONSTRUCTORS(class_name)              \
    DELETE_MOVE_CONSTRUCTORS(class_name)

#define DEPRECATED(msg)      [[deprecated(#msg)]]

#define RE_CAST(type, value) reinterpret_cast<type>(value)

#define ERRNO_STR            std::string(std::strerror(errno))

/// Included for @c size_t and @c ssize_t
#include <cstdint>
#include <sys/types.h>

using f64 = double;
using f32 = float;

using s64 = int_least64_t;
using s32 = int_least32_t;
using s16 = int_least16_t;
using s8  = char;

using u128 = unsigned long long;
using u64  = uint_least64_t;
using u32  = uint_least32_t;
using u16  = uint_least16_t;
using u8   = uint_least8_t;

#pragma endregion /** END: Typedefs */

//
//  Constants for the maximum and minimum values of the integer types.
//
constexpr f64 f64_MAX = 1.7976931348623157e+308;
constexpr f64 f64_MIN = 2.2250738585072014e-308;
constexpr f32 f32_MAX = 3.402823466e+38;
constexpr f32 f32_MIN = 1.175494351;
constexpr f32 f32_EPS = 1.192092896e-07;
constexpr u64 u64_MAX = 0xFFFFFFFFFFFFFFFF;
constexpr u64 u64_MIN = 0x0000000000000000;
constexpr s64 s64_MAX = 0x7FFFFFFFFFFFFFFF;
constexpr s64 s64_MIN = 0x8000000000000000;
constexpr u32 u32_MAX = 0xFFFFFFFF;
constexpr u32 u32_MIN = 0x00000000;
constexpr s32 s32_MAX = 0x7FFFFFFF;
constexpr s32 s32_MIN = 0x80000000;
constexpr u16 u16_MAX = 0xFFFF;
constexpr u16 u16_MIN = 0x0000;
constexpr s16 s16_MAX = 0x7FFF;
constexpr s16 s16_MIN = 0x8000;
constexpr u8  u8_MAX  = 0xFF;
constexpr u8  u8_MIN  = 0x00;
constexpr s8  s8_MAX  = 0x7F;
constexpr s8  s8_MIN  = 0x80;

constexpr u64
operator"" _KB(u128 value)
{
    return value * 1024;
}

#define __FILENAME__ (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)

#define MAKE_CONSTEXPR_WRAPPER(_name, _type)     \
    struct _name                                 \
    {                                            \
        _type value;                             \
        constexpr _name(_type val)               \
            : value(val)                         \
        {}                                       \
    };                                           \
    constexpr _name _name##_Wrapper(_type value) \
    {                                            \
        return _name {value};                    \
    }
