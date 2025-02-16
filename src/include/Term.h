#pragma once

#include <functional>
#include <iostream>
#include <termios.h>

#include "def.h"

namespace Mlib::Term {
  enum Color {
    RED     = (1 << 0),
    GREEN   = (1 << 1),
    YELLOW  = (1 << 2),
    BLUE    = (1 << 3),
    MAGENTA = (1 << 4),
    CYAN    = (1 << 5),
    WHITE   = (1 << 6)
  };

  struct rgb_code_t {
    Ushort r;
    Ushort g;
    Ushort b;
    rgb_code_t(const Ushort r, const Ushort g, const Ushort b)
        : r(r)
        , g(g)
        , b(b) {
    }
    rgb_code_t() {};
  };

  struct function_t {
    std::function<void()> action;
  };

  template <typename Function, typename... Args>
  function_t make_function_t(Function &&function, Args &&...args) {
    return {bind(std::forward<Function>(function), std::forward<Args>(args)...)};
  }

  int hex_sti(const char *str);
  template <typename T, size_t N>
  ssize_t nread(int fd, T (&buffer)[N]) {
    ssize_t len = read(fd, buffer, (sizeof(T) * N));
    if (len > 0) {
      len /= sizeof(T);
    }
    return len;
  }
  void retrieve_current_row_colum_position(int fd, Ushort *row, Ushort *colum, bool in_raw_mode = true);
  const char *retrieve_current_rgb_colors(bool bg = true);
  void        clear_screen();
  bool        term_size(size_t *colums, size_t *rows, size_t *x_pixel = NULL, size_t *y_pixel = NULL);
  void        move_cursor(const Ushort row, const Ushort colum);
  void        printf_xy(const Ushort x, const Ushort y, const char *fmt, ...);
  void        printf_center(const char *fmt, ...);
  int         open_tty_as_fd(const char *tty = "/dev/tty" /* <- Default`s to calling tty. */);
  void        hide_cursor(bool hide);
  void        set_color(Color fg, Color bg, bool light);
  void        set_color_rgb(bool bg, Ushort r, Ushort g, Ushort b);
  void        make_entire_line_color(const Ushort r, const Ushort g, const Ushort b);
  void        clear_line(bool from_start);
  void        reset_color();
  int         read_char_from_fd(int fd);
  bool        prompt(const char *format, ...);
  const char *prompt_raw(int fd, const char **wanted_answers, const rgb_code_t *fg, const rgb_code_t *bg, Ushort row, Ushort colum, const char *format, ...);
  int         fork_function(pid_t *pid, function_t *function);
  /**
    Returns the old terminal settings
    @param fd
    @return termios
   */
  termios setup_raw_term(int fd);
  int read_kbinput(int fd);
}

#define PIPE_READ  0
#define PIPE_WRITE 1
