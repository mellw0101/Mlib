#pragma once

#include "def.h"

namespace Mlib::Term
{

    void clear_screen();
    bool term_size(size_t *colums, size_t *rows, size_t *x_pixel = nullptr, size_t *y_pixel = nullptr);
    void move_cursor(unsigned short x, unsigned short y);
    void printf_xy(short x, short y, const char *fmt, ...);
    void printf_center(const char *fmt, ...);
    int  open_tty_as_fd(const char *tty = "/dev/tty" /* <- Default`s to calling tty. */);

} // namespace Mlib::Term
