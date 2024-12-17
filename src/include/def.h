#pragma once

#include <array>
#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <deque>
#include <exception>
#include <fstream>
#include <future>
#include <ios>
#include <limits>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "Attributes.h"

#define ESC_CODE_RED                     "\033[31m"
#define ESC_CODE_GREEN                   "\033[32m"
#define ESC_CODE_YELLOW                  "\033[33m"
#define ESC_CODE_BLUE                    "\033[34m"
#define ESC_CODE_MAGENTA                 "\033[35m"
#define ESC_CODE_CYAN                    "\033[36m"
#define ESC_CODE_WHITE                   "\033[37m"
#define ESC_CODE_RESET                   "\033[0m"
#define ESC_CODE_GRAY                    "\033[90m"
#define ESC_CODE_BOLD                    "\033[1m"

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

#define RW_FOR_ALL                       (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

using std::initializer_list;
using std::is_nothrow_copy_constructible;
using std::is_nothrow_copy_constructible_v;
using std::is_nothrow_default_constructible;
using std::is_nothrow_default_constructible_v;
using std::is_nothrow_destructible;
using std::is_nothrow_destructible_v;
using std::is_nothrow_move_constructible;
using std::is_nothrow_move_constructible_v;
using std::is_trivially_destructible;
using std::is_trivially_destructible_v;
using std::is_trivially_constructible;
using std::is_trivially_constructible_v;

constexpr auto ESC_CODE_TURN_ON_BRACKETED_PASTE  = "\x1B[?2004h";
constexpr auto ESC_CODE_TURN_OFF_BRACKETED_PASTE = "\x1B[?2004l";

#define FORCE_INLINE                  __attribute__((always_inline)) static __inline__
#define _UNUSED                       __attribute__((unused))
#define _NORETURN                     __attribute__((noreturn))

#define STRLITERAL(x)                 #x

#define CONCATENATE(prefix, header)   STRINGIFY(prefix / header)
#define PREFIX_HEADER(prefix, header) CONCATENATE(prefix, header)

#define DELETE_MOVE_CONSTRUCTORS(class_name)     \
  class_name(class_name &&)            = delete; \
  class_name &operator=(class_name &&) = delete

#define DELETE_COPY_CONSTRUCTORS(class_name)          \
  class_name(const class_name &)            = delete; \
  class_name &operator=(const class_name &) = delete

#define DELETE_COPY_AND_MOVE_CONSTRUCTORS(class_name) \
  DELETE_COPY_CONSTRUCTORS(class_name);               \
  DELETE_MOVE_CONSTRUCTORS(class_name)

#define DEL_CM_CONSTRUCTORS(class) DELETE_COPY_AND_MOVE_CONSTRUCTORS(class)

#define DEPRECATED(msg)            [[deprecated(#msg)]]
#define NO_DISCARD                 [[__nodiscard__]]
#define NO_RETURN                  [[__noreturn__]]

#define RE_CAST(type, value)       reinterpret_cast<type>(value)

// Macro to enforce that a parameter is a compile-time constant
#define __ENFORCE_CONSTANT_PARAM(param) \
  do {                                  \
    if (!__builtin_constant_p(param)) { \
      __builtin_trap();                 \
    }                                   \
  }                                     \
  while (0)

#define __COMPILE_ERROR(msg) __attribute__((__error__(msg)))

//
//  Some macros for ease of use for errno handling.
//
#define ERRNO_C_STR          std::strerror(errno)
#define ERRNO_STR            std::string(ERRNO_C_STR)
#define ERRNO_CODE_STR       std::to_string(errno)
#define PERROR(__FUNCTION)   std::perror(#__FUNCTION)

/// Included for @c size_t and @c ssize_t
#include <cstdint>
#include <sys/types.h>

using f64 = double;
using f32 = float;

using s64 = int64_t;
using s32 = int32_t;
using s16 = int16_t;
using s8  = char;

using SigS8 = int8_t;

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8  = uint8_t;

using c32 = char32_t;
using c16 = char16_t;

template <typename T>
using NUMERIC_LIMITS = std::numeric_limits<T>;

//
//  Constants for the maximum and minimum values of the integer types.
//
constexpr f64 f64_MAX = NUMERIC_LIMITS<f64>::max();
constexpr f64 f64_MIN = NUMERIC_LIMITS<f64>::min();
constexpr f64 f64_EPS = 2.2204460492503131e-16; //

constexpr f32 f32_MAX = NUMERIC_LIMITS<f32>::max();
constexpr f32 f32_MIN = NUMERIC_LIMITS<f32>::min();
constexpr f32 f32_EPS = 1.192092896e-07;

constexpr s64 s64_MAX = NUMERIC_LIMITS<s64>::max();
constexpr s64 s64_MIN = NUMERIC_LIMITS<s64>::min();
constexpr s32 s32_MAX = NUMERIC_LIMITS<s32>::max();
constexpr s32 s32_MIN = NUMERIC_LIMITS<s32>::min();
constexpr s16 s16_MAX = NUMERIC_LIMITS<s16>::max();
constexpr s16 s16_MIN = NUMERIC_LIMITS<s16>::min();
constexpr s8  s8_MAX  = NUMERIC_LIMITS<s8>::max();
constexpr s8  s8_MIN  = NUMERIC_LIMITS<s8>::min();

constexpr u64 u64_MAX = NUMERIC_LIMITS<u64>::max();
constexpr u64 u64_MIN = NUMERIC_LIMITS<u64>::min();
constexpr u32 u32_MAX = NUMERIC_LIMITS<u32>::max();
constexpr u32 u32_MIN = NUMERIC_LIMITS<u32>::min();
constexpr u16 u16_MAX = NUMERIC_LIMITS<u16>::max();
constexpr u16 u16_MIN = NUMERIC_LIMITS<u16>::min();
constexpr u8  u8_MAX  = NUMERIC_LIMITS<u8>::max();
constexpr u8  u8_MIN  = NUMERIC_LIMITS<u8>::min();

constexpr auto FAILURE = 1;
constexpr auto SUCCESS = 0;

constexpr u64 operator"" _KB(unsigned long long value) {
  return value * 1024;
}

#define __FILENAME__ (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)

#define MAKE_CONSTEXPR_WRAPPER(_name, _type)     \
  struct _name {                                 \
    _type value;                                 \
    constexpr _name(_type val)                   \
        : value(val) {                           \
    }                                            \
  };                                             \
  constexpr _name _name##_Wrapper(_type value) { \
    return _name {value};                        \
  }

#define DIE(_Msg)       \
  do {                  \
    perror(_Msg);       \
    exit(EXIT_FAILURE); \
  }                     \
  while (false)

#define VISIBILITY(V) __attribute__((__visibility__(#V)))
#define _EXPORT       VISIBILITY(default)
#define _HIDDEN       VISIBILITY(hidden)
#define ERROR_CODE    std::error_code
#define _NO_THROW     noexcept
#define FS            std::filesystem
#define CHRONO        std::chrono

#define P_FORWARD     std::forward
#define COPY_N        std::copy_n

template <typename T>
using C_ = const T;

template <typename T>
using __ptr = T *;

using C_s64 = const s64;
using C_s32 = const s32;
using C_s16 = const s16;
using C_s8  = const s8;

using C_SigS8 = const SigS8;

using C_u64 = const u64;
using C_u32 = const u32;
using C_u16 = const u16;
using C_u8  = const u8;

using C_c32 = const c32;
using C_c16 = const c16;

using CFuncPtr = void (*)();

using C_f64 = const f64;
using C_f32 = const f32;

//  Typedefs for the standard library types.
//
//  This is vital for the codebase to be portable.
//  While we greatly improve readebility by redusing
//  namespace pollution, we also make the codebase
//  portable by using these typedefs.
//

//
//  C++ Standard string.
//  Should only be used for string manipulation.
//  For string views, use @c STRING_VIEW
//  For constant standard library strings, use @c C_STRING.
//  For all uses of string where no manipulation is required,
//  @c 'C_s8 *' ( const char * ) should be used.
//  This is to prevent the overhead of the string class.
//
using STRING = class std::basic_string<char>;
//
//  C++ Standard constant string.
//  Represents a const STRING.
//  @see STRING
//
using C_STRING = const STRING;
//
//  C++ Standard string_view.
//  Should be used for string views.
//  This is to prevent the overhead of the string class.
//  A string_view is a non-owning reference to a string.
//  It is a lightweight object that is used to refer to a
//  sequence of characters.
//
//  @note:
//  - SHOULD NOT BE USED FOR STRING MANIPULATION OF ANY KIND AS,
//  - IT DOES NOT OWN THE DATA IT ONLY HOLDS A 'VIEW' OF IT.
//
using STRING_VIEW = class std::basic_string_view<char>;
//
//  C++ Standard stringstream.
//  Should be used for string building.
//
using STRINGSTREAM   = class std::basic_stringstream<char>;
using C_STRINGSTREAM = const STRINGSTREAM;

using FILEBUF   = class std::basic_filebuf<char>;
using C_FILEBUF = const FILEBUF;

using IFSTREAM = class std::basic_ifstream<char>;

using IOS = class std::basic_ios<char>;

template <typename T>
using VECTOR = std::vector<T>;
template <typename T>
using C_VECTOR = const VECTOR<T>;
template <typename T, u64 SIZE>
using ARRAY = std::array<T, SIZE>;
template <typename T, u64 SIZE>
using C_ARRAY = const ARRAY<T, SIZE>;
template <typename T0, typename T1>
using PAIR = std::pair<T0, T1>;
template <typename T0, typename T1>
using C_PAIR = const PAIR<T0, T1>;
template <typename T0, typename T1>
using MAP = std::map<T0, T1>;
template <typename T0, typename T1>
using C_MAP = const MAP<T0, T1>;
template <typename T>
using SET = std::set<T>;
template <typename T>
using C_SET = const SET<T>;
template <typename T>
using FUTURE = std::future<T>;
template <typename _Functor, typename... _ArgTypes>
using INVOKE_RESULT = struct std::invoke_result<_Functor, _ArgTypes...>;
template <typename _Signature>
using PACKAGED_TASK = std::packaged_task<_Signature>;
//
//  represents a point in time as measured by a clock
//
template <typename T>
using TIME_POINT = CHRONO::time_point<T>;
//
//  -***-
//
//  ALIAS to 'std ::chrono ::high_resolution_clock'
//  -***-
//
//  Highest-resolution clock.
//  This is the clock "with the shortest tick period."
//  Alias to std::system_clock until higher-than-nanosecond
//  definitions become feasible.
//  @ingroup chrono
//
using HIGH_RES_CLOCK = CHRONO::high_resolution_clock;
using EXCEPTION      = class std::exception;
template <typename T, typename Period>
using Duration       = CHRONO::duration<T, Period>;
using Milli = std::milli;
using CEXCEPTION     = const EXCEPTION;
using RUNTIME_ERROR  = class std::runtime_error;
using CRUNTIME_ERROR = const RUNTIME_ERROR;
template <typename _Mutex>
using UNIQUE_LOCK = class std::unique_lock<_Mutex>;
template <typename _Mutex>
using LOCK_GUARD         = class std::lock_guard<_Mutex>;
using CONDITION_VARIABLE = class std::condition_variable;
using MUTEX              = class std::mutex;
using THREAD             = class std::thread;

template <typename _Tp, typename _Sequence = std::deque<_Tp>>
using QUEUE = class std::queue<_Tp, _Sequence>;

decltype(auto) operator"" _hash(const char *s, unsigned long);

#define CLAMBDA              [](auto &&...args) _NO_THROW -> decltype(auto)
#define CLAMBDA_REF          [](auto &&...args) _NO_THROW -> decltype(auto) &
#define CLAMBDA_CREF         [](auto &&...args) _NO_THROW -> decltype(auto) const &
#define CLAMBDA_RREF         [](auto &&...args) _NO_THROW -> decltype(auto) &&
#define CLAMBDA_CRREF        [](auto &&...args) _NO_THROW -> decltype(auto) const &&
#define CLAMBDA_NORET        [](auto &&...args) _NO_THROW -> void
#define CLAMBDA_RET(T)       [](auto &&...args) _NO_THROW -> T
#define CLAMBDA_RET_REF(T)   [](auto &&...args) _NO_THROW -> T &
#define CLAMBDA_RET_CREF(T)  [](auto &&...args) _NO_THROW -> T const &
#define CLAMBDA_RET_RREF(T)  [](auto &&...args) _NO_THROW -> T &&
#define CLAMBDA_RET_CRREF(T) [](auto &&...args) _NO_THROW -> T const &&

#define TRI_PARAM_T_ENUM \
  enum type_t {          \
    TYPE_1 = (1 << 0),   \
    TYPE_2 = (1 << 1),   \
    TYPE_3 = (1 << 2),   \
                         \
  } type

#define TRI_PARAM_T_UNION(T1, T2, T3) \
  union                               \
  {                                   \
    T1 union_1;                       \
    T2 union_2;                       \
    T3 union_3;                       \
  }

#define PARAM_T_CONSTRUCT(OBJ, T, N, NAME) \
  OBJ(T NAME)                              \
      : type(TYPE_##N)                     \
      , union_##N(NAME) {                  \
  }

#define TRI_PARAM_T_MAKE_ALL_CONSTRUCTS(OBJ, NAME_1, NAME_2, NAME_3) \
  PARAM_T_CONSTRUCT(OBJ, T1, 1, NAME_1)                              \
  PARAM_T_CONSTRUCT(OBJ, T2, 2, NAME_2)                              \
  PARAM_T_CONSTRUCT(OBJ, T3, 3, NAME_3)

#define DOUBLE_PARAM_T_ENUM \
  enum type_t {             \
    TYPE_1 = (1 << 0),      \
    TYPE_2 = (1 << 1),      \
                            \
  } type

#define DOUBLE_PARAM_T_UNION(T1, T2) \
  union                              \
  {                                  \
    T1 union_1;                      \
    T2 union_2;                      \
  }

#define DOUBLE_PARAM_T_MAKE_ALL_CONSTRUCTS         \
  PARAM_T_CONSTRUCT(double_param_t, T1, 1, type_1) \
  PARAM_T_CONSTRUCT(double_param_t, T2, 2, type_2)

#define OUTPUT_OBJ_3WAY(name, T1, T2, T3, n1, n2, n3) \
  struct name {                                       \
    TRI_PARAM_T_ENUM;                                 \
    TRI_PARAM_T_UNION(T1, T2, T3);                    \
    PARAM_T_CONSTRUCT(name, T1, 1, type_1)            \
    PARAM_T_CONSTRUCT(name, T2, 2, type_2)            \
    PARAM_T_CONSTRUCT(name, T3, 3, type_3)            \
  }

#define BOOL_STR(statement) statement ? "TRUE" : "FALSE"
#define SIZEOF_BIT(type)    (sizeof(type) * 8)

template <typename T>
__inline__ __warn_unused constexpr unsigned long __attribute__((__always_inline__, __nodebug__, __const__))
__bit_sizeof(void) noexcept {
  return (sizeof(T) * 8);
}
#define bit_sizeof(type)     __bit_sizeof<type>()

#define SSE_SIMD_WIDTH(type) (128 / SIZEOF_BIT(type))
template <typename T>
__inline__ __warn_unused constexpr unsigned long __attribute__((__always_inline__, __nodebug__, __const__))
sse_simd_width(void) noexcept {
  return SSE_SIMD_WIDTH(T);
}
#define sizeof_sse_simd(type)     sse_simd_width<type>()

#define AMALLOC(obj)              (decltype(obj))malloc(sizeof(*obj))
#define AUTO_OBJ_MALLOC(obj)      obj = AMALLOC(obj)
#define ACALLOC_ARRAY(obj, size)  (decltype(obj))calloc(size, sizeof(*obj)) 
#define AMALLOC_ARRAY(obj, size)  (decltype(obj))malloc(sizeof(*obj) * size)
#define AREALLOC_ARRAY(obj, size) (decltype(obj))realloc(obj, sizeof(*obj) * size)

#define LOG_AMALLOC(obj)      \
  do {                        \
    obj = AMALLOC(obj);       \
    if (!obj) {               \
      logE("Malloc Failed."); \
      exit(1);                \
    }                         \
  }                           \
  while (0)

#define LOG_AMALLOC_HANDLE(obj, handler) \
  do {                                   \
    obj = AMALLOC(obj);                  \
    if (!obj) {                          \
      logE("Malloc Failed.");            \
      handler();                         \
    }                                    \
  }                                      \
  while (0)

#define LOG_AMALLOC_ARRAY(obj, size) \
  do {                               \
    obj = AMALLOC_ARRAY(obj, size);  \
    if (!obj) {                      \
      logE("Malloc Failed.");        \
      exit(1);                       \
    }                                \
  }                                  \
  while (0)

#define LOG_AMALLOC_ARRAY_HANDLE(obj, size, handler) \
  do {                                               \
    obj = AMALLOC_ARRAY(obj, size);                  \
    if (!obj) {                                      \
      logE("Malloc Failed.");                        \
      handler();                                     \
    }                                                \
  }                                                  \
  while (0)

#define LOG_AREALLOC_ARRAY(obj, size) \
  do {                                \
    obj = AREALLOC_ARRAY(obj, size);  \
    if (!obj) {                       \
      logE("Realloc Failed.");        \
      exit(1);                        \
    }                                 \
  }                                   \
  while (0)

#define LOG_AREALLOC_ARRAY_HANDLE(obj, size, handler) \
  do {                                                \
    obj = AREALLOC_ARRAY(obj, size);                  \
    if (!obj) {                                       \
      logE("Realloc Failed.");                        \
      handler();                                      \
    }                                                 \
  }                                                   \
  while (0)

#define __MLIB_BEGIN_NAMESPACE_(name, vis) \
  namespace __type_##vis Mlib {            \
    inline namespace name

#define __END_NAMESPACE(name) }

#ifdef Uchar
  #undef Uchar
#endif
#ifdef Ushort
  #undef Ushort
#endif
#ifdef Uint
  #undef Uint
#endif
#ifdef Ulong
  #undef Ulong
#endif
#ifdef UlongMIN
  #undef UlongMIN
#endif
#ifdef UlongMAX
  #undef UlongMAX
#endif
#ifdef PTR_SIZE
  #undef PTR_SIZE
#endif
#define Uchar   unsigned char
#define Ushort  unsigned short int
#define Uint    unsigned int
#ifndef __WORDSIZE
  #if defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(__aarch64__) || defined(_M_ARM64)
    #define __WORDSIZE 64
  #elif defined(_WIN32)
    #define __WORDSIZE 32
  #endif
#endif
#if __WORDSIZE == 64
  #if (defined(__x86_64__) && !defined(__ILP32__)) || defined(__aarch64__) || defined(_M_ARM64)
    #define Ulong   unsigned long int
    #define UlongMIN (0UL)
    #define UlongMAX (18446744073709551615UL)
  #else
    #define Ulong   unsigned long long int
    #define UlongMIN (0ULL)
    #define UlongMAX (18446744073709551615ULL)
  #endif
#else
  #define Ulong   unsigned long long int
  #define UlongMIN (0ULL)
  #define UlongMAX (18446744073709551615ULL)
#endif
#define PTR_SIZE __WORDSIZE

/* Null def. */
// #ifdef NULL
//   #undef NULL
// #endif
/* Bool def. */
#ifdef TRUE
  #undef TRUE
#endif
#ifdef FALSE
  #undef FALSE
#endif
#ifdef __cplusplus
  // #define NULL nullptr
  #define TRUE true
  #define FALSE false
#else
  // #define NULL __null
  #define TRUE 1
  #define FALSE 0
#endif