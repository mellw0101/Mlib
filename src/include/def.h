#pragma once

namespace std {}
using namespace std;

#define ESC_CODE_RED "\033[31m"
#define ESC_CODE_GREEN "\033[32m"
#define ESC_CODE_YELLOW "\033[33m"
#define ESC_CODE_BLUE "\033[34m"
#define ESC_CODE_MAGENTA "\033[35m"
#define ESC_CODE_CYAN "\033[36m"
#define ESC_CODE_WHITE "\033[37m"
#define ESC_CODE_RESET "\033[0m"
#define ESC_CODE_GRAY "\033[90m"
#define ESC_CODE_BOLD "\033[1m"

#define ESC_CODE_UNDERLINE "\033[4m"
#define ESC_CODE_BLINK "\033[5m"
#define ESC_CODE_REVERSE "\033[7m"
#define ESC_CODE_INVISIBLE "\033[8m"
#define ESC_CODE_CLEAR "\033[2J"
#define ESC_CODE_CLEAR_LINE "\033[2K"
#define ESC_CODE_CLEAR_LINE_END "\033[K"
#define ESC_CODE_CLEAR_LINE_START "\033[1K"
#define ESC_CODE_CLEAR_LINE_ALL "\033[2K\033[1K"
#define ESC_CODE_CLEAR_LINE_ALL_END "\033[2K\033[K"
#define ESC_CODE_CLEAR_LINE_ALL_START "\033[2K\033[1K"
#define ESC_CODE_CLEAR_LINE_ALL_ALL "\033[2K\033[1K\033[K"
#define ESC_CODE_CURSOR_UP "\033[A"
#define ESC_CODE_CURSOR_DOWN "\033[B"
#define ESC_CODE_CURSOR_RIGHT "\033[C"
#define ESC_CODE_CURSOR_LEFT "\033[D"
#define ESC_CODE_CURSOR_NEXT_LINE "\033[E"
#define ESC_CODE_CURSOR_PREV_LINE "\033[F"
#define ESC_CODE_CURSOR_COLUMN "\033[G"
#define ESC_CODE_CURSOR_POSITION "\033[H"
#define ESC_CODE_CURSOR_POSITION_ALT "\033[f"
#define ESC_CODE_CURSOR_SAVE "\033[s"
#define ESC_CODE_CURSOR_RESTORE "\033[u"
#define ESC_CODE_CURSOR_HIDE "\033[?25l"
#define ESC_CODE_CURSOR_SHOW "\033[?25h"
#define ESC_CODE_CURSOR_BLINK "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT "\033[?12h"
#define ESC_CODE_CURSOR_NOBLINK_ALT "\033[?12l"
#define ESC_CODE_CURSOR_BLINK_ALT2 "\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT2 "\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT3 "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT3 "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT4 "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT4 "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT5 "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT5 "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT6 "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT6 "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT7 "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT7 "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT8 "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT8 "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT9 "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT9 "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT10 "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT10 "\033[?12;25l"
#define ESC_CODE_CURSOR_BLINK_ALT11 "\033[?12h\033[?25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT11 "\033[?12l\033[?25l"
#define ESC_CODE_CURSOR_BLINK_ALT12 "\033[?12;25h"
#define ESC_CODE_CURSOR_NOBLINK_ALT12 "\033[?12;25l"

#define ESC_CODE_CURSOR_UP_ALT "\033M"
#define ESC_CODE_CURSOR_DOWN_ALT "\033D"
#define ESC_CODE_CURSOR_RIGHT_ALT "\033C"
#define ESC_CODE_CURSOR_LEFT_ALT "\033D"
#define ESC_CODE_CURSOR_NEXT_LINE_ALT "\033E"
#define ESC_CODE_CURSOR_PREV_LINE_ALT "\033F"
#define ESC_CODE_CURSOR_COLUMN_ALT "\033G"
#define ESC_CODE_CURSOR_POSITION_ALT_ALT "\033H"

#define FORCE_INLINE __attribute__((always_inline)) static __inline__

/// @brief Bitwise operations for flags
#define FLAGMASK(flag) ((unsigned)1 << ((flag) % (sizeof(unsigned) * 8)))
#define SET(flag) FLAGS(flag) |= FLAGMASK(flag)
#define UNSET(flag) FLAGS(flag) &= ~FLAGMASK(flag)
#define ISSET(flag) ((FLAGS(flag) & FLAGMASK(flag)) != 0)
#define TOGGLE(flag) FLAGS(flag) ^= FLAGMASK(flag)
/// END:

#define DELETE_MOVE_CONSTRUCTORS(class_name)                                                                           \
    class_name(class_name&&)            = delete;                                                                      \
    class_name& operator=(class_name&&) = delete;

#define DELETE_COPY_CONSTRUCTORS(class_name)                                                                           \
    class_name(const class_name&)            = delete;                                                                 \
    class_name& operator=(const class_name&) = delete;

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
