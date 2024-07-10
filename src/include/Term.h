#pragma once

#include <any>
#include <functional>
#include "def.h"

namespace Mlib::Term
{
    enum Color
    {
        RED     = (1 << 0),
        GREEN   = (1 << 1),
        YELLOW  = (1 << 2),
        BLUE    = (1 << 3),
        MAGENTA = (1 << 4),
        CYAN    = (1 << 5),
        WHITE   = (1 << 6)
    };

    struct rgb_code_t
    {
        const unsigned short r;
        const unsigned short g;
        const unsigned short b;
        rgb_code_t(const unsigned short r, const unsigned short g, const unsigned short b)
            : r(r)
            , g(g)
            , b(b)
        {}
    };

    struct function_t
    {
        std::function<void()> action;
    };

    template <typename Function, typename... Args>
    function_t
    make_function_t(Function &&function, Args &&...args)
    {
        return {bind(std::forward<Function>(function), std::forward<Args>(args)...)};
    }

    [[noreturn]]
    void fatal_error(const char *from_function __attribute((__nonnull__)),
                     const char *when_calling __attribute((__nonnull__)), const char *format = "", ...);

    void fwritef(int fd, const char *format, ...);
    void writef(const char *format, ...);
    void cwrite(const char c);
    void fcwrite(int fd, const char c);
    template <typename T, size_t N>
    ssize_t
    nread(int fd, T (&buffer)[N])
    {
        ssize_t len = read(fd, buffer, (sizeof(T) * N));
        if (len > 0)
        {
            len /= sizeof(T);
        }
        return len;
    }
    void        retrieve_current_row_colum_position(int fd, unsigned short *row, unsigned short *colum,
                                                    bool in_raw_mode = true);
    void        clear_screen();
    bool        term_size(size_t *colums, size_t *rows, size_t *x_pixel = nullptr, size_t *y_pixel = nullptr);
    void        move_cursor(const unsigned short row, const unsigned short colum);
    void        printf_xy(const unsigned short x, const unsigned short y, const char *fmt, ...);
    void        printf_center(const char *fmt, ...);
    int         open_tty_as_fd(const char *tty = "/dev/tty" /* <- Default`s to calling tty. */);
    void        hide_cursor(bool hide);
    void        set_color(Color fg, Color bg, bool light);
    void        set_color_rgb(bool bg, unsigned short r, unsigned short g, unsigned short b);
    void        reset_color();
    int         read_char_from_fd(int fd);
    bool        prompt(const char *format, ...);
    const char *prompt_raw(int fd, const char **wanted_answers, const rgb_code_t fg, const rgb_code_t bg,
                           const unsigned short row, const unsigned short colum, const char *format, ...);
    int         fork_function(pid_t *pid, function_t *function);

} // namespace Mlib::Term

#define fatal_err(...) Mlib ::Term ::fatal_error(__func__, __VA_ARGS__)
#define PIPE_READ      0
#define PIPE_WRITE     1
