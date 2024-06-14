#pragma once

namespace std {}
using namespace std;

#pragma region 'Macros'

#define ESC_CODE_RED "\033[31m"
#define ESC_CODE_GREEN "\033[32m"
#define ESC_CODE_YELLOW "\033[33m"
#define ESC_CODE_BLUE "\033[34m"
#define ESC_CODE_MAGENTA "\033[35m"
#define ESC_CODE_CYAN "\033[36m"
#define ESC_CODE_WHITE "\033[37m"
#define ESC_CODE_RESET "\033[0m"

#define FORCE_INLINE __attribute__((always_inline)) static __inline__

#define DELETE_MOVE_CONSTRUCTORS(class_name)                                                                           \
    class_name(class_name&&)            = delete;                                                                      \
    class_name& operator=(class_name&&) = delete;

#define DELETE_COPY_CONSTRUCTORS(class_name)                                                                           \
    class_name(const class_name&)            = delete;                                                                 \
    class_name& operator=(const class_name&) = delete;

#pragma endregion

using f64 = double;
using f32 = float;

using s64 = long long;
using s32 = int;
using s16 = short;
using s8  = char;

using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8  = unsigned char;
