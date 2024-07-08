#include "../include/Term.h"
#include "../include/def.h"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace Mlib::Term
{

    void
    clear_screen()
    {
        printf(ESC_CODE_CLEAR ESC_CODE_CURSOR_POSITION);
        fflush(stdout);
    }

    bool
    term_size(size_t *colums, size_t *rows, size_t *x_pixel, size_t *y_pixel)
    {
        winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) < 0)
        {
            fprintf(stderr, "ioctl: ERROR: %s\n", strerror(errno));
            return false;
        }
        (colums != nullptr) ? *colums = w.ws_col : 0;
        (rows != nullptr) ? *rows = w.ws_row : 0;
        (x_pixel != nullptr) ? (*x_pixel = w.ws_xpixel) : 0;
        (y_pixel != nullptr) ? (*y_pixel = w.ws_ypixel) : 0;
        return true;
    }

    void
    move_cursor(unsigned short x, unsigned short y)
    {
        printf("\033[%u;%uH", y, x);
        fflush(stdout);
    }

    void
    printf_xy(short x, short y, const char *fmt, ...)
    {
        move_cursor(x, y);
        char    msg[1024];
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(msg, sizeof(fmt), fmt, ap);
        va_end(ap);
        printf("%s", msg);
        fflush(stdout);
    }

    void
    printf_center(const char *fmt, ...)
    {
        size_t colums, rows;
        if (!term_size(&colums, &rows))
        {
            fprintf(stderr, "term_size: Could not retrive term size, ERROR: [%s].\n", ERRNO_C_STR);
            return;
        }

        char    buf[1024];
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        size_t x_pos = ((colums - strlen(buf)) / 2);
        size_t y_pos = (rows / 2);
        move_cursor(x_pos, y_pos);
        fprintf(stdout, "%s", buf);
        fflush(stdout);
    }

    int
    open_tty_as_fd(const char *tty)
    {
        return open(tty, O_RDWR | O_NOCTTY);
    }

} // namespace Mlib::Term
