#include "../include/Term.h"
#include "../include/def.h"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace Mlib::Term
{
    void
    writef(int fd, const char *format, ...)
    {
        char    buf[1024];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buf, sizeof(buf), format, ap);
        va_end(ap);
        unsigned long length = strlen(buf);
        write(fd, &buf, length);
    }

    void
    retrieve_current_row_colum_position(int fd, unsigned short *row, unsigned short *colum, bool in_raw_mode)
    {
        termios      old_term;
        char         buf[30];
        unsigned int i = 0;
        if (!in_raw_mode)
        {
            termios new_term;
            if (tcgetattr(fd, &old_term))
            {
                fprintf(stderr, "%s: 'tcgetattr' Failed, ERROR: [%s].\n", __func__, strerror(errno));
                exit(1);
            }
            new_term = old_term;
            new_term.c_lflag &= ~(ICANON | ECHO);
            if (tcsetattr(fd, TCSANOW, &new_term))
            {
                fprintf(stderr, "%s: 'tcsetattr' Failed, ERROR: [%s].\n", __func__, strerror(errno));
                exit(1);
            }
        }
        write(fd, "\033[6n", 4);
        for (; i < sizeof(buf) - 1; i++)
        {
            if (read(fd, &buf[i], 1) != 1)
            {
                break;
            }
            if (buf[i] == 'R')
            {
                break;
            }
        }
        buf[i] = '\0';
        if (!in_raw_mode)
        {
            if (tcsetattr(fd, TCSANOW, &old_term))
            {
                fprintf(stderr, "%s: 'tcsetattr' Failed, ERROR: [%s].\n", __func__, strerror(errno));
                exit(1);
            }
        }
        if (buf[0] != '\033' || buf[1] != '[')
        {
            fprintf(stderr, "%s: Something went wrong retriving data buffer, [buf[0] == '%c'], [buf[1] == '%c'].\n",
                    __func__, buf[0], buf[1]);
            exit(1);
        }
        if (sscanf(buf + 2, "%hu;%hu", row, colum) != 2)
        {
            fprintf(stderr, "%s: 'sscanf' Failed, ERROR: [%s].\n", __func__, strerror(errno));
            exit(1);
        }
    }

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
            fprintf(stderr, "ioctl: ERROR: [%s].\n", strerror(errno));
            return false;
        }
        (colums != nullptr) ? *colums = w.ws_col : 0;
        (rows != nullptr) ? *rows = w.ws_row : 0;
        (x_pixel != nullptr) ? (*x_pixel = w.ws_xpixel) : 0;
        (y_pixel != nullptr) ? (*y_pixel = w.ws_ypixel) : 0;
        return true;
    }

    void
    move_cursor(const unsigned short x, const unsigned short y)
    {
        printf("\033[%u;%uH", y, x);
        fflush(stdout);
    }

    void
    printf_xy(const unsigned short x, const unsigned short y, const char *fmt, ...)
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

    void
    hide_cursor(bool hide)
    {
        if (hide)
        {
            printf(ESC_CODE_CURSOR_HIDE);
        }
        else
        {
            printf(ESC_CODE_CURSOR_SHOW);
        }
        fflush(stdout);
    }

    void
    set_color(Color fg, Color bg, bool light)
    {
        if (fg == 0 && bg == 0)
        {
            return;
        }

        char str[20] = "\033[";
        if (bg != 0 && light)
        {
            strcat(str, "10");
        }
        else if (bg != 0 && !light)
        {
            strcat(str, "4");
        }

        if (bg & RED)
        {
            strcat(str, "1");
        }
        else if (bg & GREEN)
        {
            strcat(str, "2");
        }
        else if (bg & YELLOW)
        {
            strcat(str, "3");
        }
        else if (bg & BLUE)
        {
            strcat(str, "4");
        }
        else if (bg & MAGENTA)
        {
            strcat(str, "5");
        }
        else if (bg & CYAN)
        {
            strcat(str, "6");
        }
        else if (bg & WHITE)
        {
            strcat(str, "7");
        }

        if (bg != 0)
        {
            strcat(str, ";");
        }

        if (fg != 0 && light)
        {
            strcat(str, "9");
        }
        else if (fg != 0 && !light)
        {
            strcat(str, "3");
        }

        if (fg & RED)
        {
            strcat(str, "1");
        }
        else if (fg & GREEN)
        {
            strcat(str, "2");
        }
        else if (fg & YELLOW)
        {
            strcat(str, "3");
        }
        else if (fg & BLUE)
        {
            strcat(str, "4");
        }
        else if (fg & MAGENTA)
        {
            strcat(str, "5");
        }
        else if (fg & CYAN)
        {
            strcat(str, "6");
        }
        else if (fg & WHITE)
        {
            strcat(str, "7");
        }

        strcat(str, "m");
        printf("%s", str);
        fflush(stdout);
    }

    void
    set_color_rgb(bool bg, unsigned short r, unsigned short g, unsigned short b)
    {
        if (bg)
        {
            printf("\033[48;2;%u;%u;%um", r, g, b);
        }
        else
        {
            printf("\033[38;2;%u;%u;%um", r, g, b);
        }
        fflush(stdout);
    }

    void
    reset_color()
    {
        printf(ESC_CODE_RESET);
        fflush(stdout);
    }

    int
    read_char_from_fd(int fd)
    {
        int     c;
        ssize_t len = read(fd, &c, 1);
        if (len != 1 && len == -1)
        {
            fprintf(stderr, "%s: 'read' Failed, the read length was not 1.\n", __func__);
            exit(EXIT_FAILURE);
        }
        return c;
    }

    bool
    prompt(const char *format, ...)
    {
        char    c;
        char    prompt_str[1024];
        va_list ap;
        va_start(ap, format);
        vsnprintf(prompt_str, sizeof(prompt_str), format, ap);
        va_end(ap);
        while (true)
        {
            printf("%s [Yy/Nn]: ", prompt_str);
            fflush(stdout);
            c = read_char_from_fd(STDIN_FILENO);
            switch (c)
            {
                case 'N' :
                case 'n' :
                {
                    for (; c != '\n'; c = read_char_from_fd(STDIN_FILENO))
                    {
                        fflush(stdin);
                    }
                    return false;
                }
                case 'Y' :
                case 'y' :
                {
                    for (; c != '\n'; c = read_char_from_fd(STDIN_FILENO))
                    {
                        fflush(stdin);
                    }
                    return true;
                }
                default :
                {
                    printf("Only [Yy/Nn] is allowed.\n");
                    for (; c != '\n'; c = read_char_from_fd(STDIN_FILENO))
                    {
                        fflush(stdin);
                    }
                    break;
                }
            }
        }
    }

    const char *
    prompt_raw(int fd, const char **wanted_answers, rgb_code_t fg, rgb_code_t bg, const unsigned short x,
               const unsigned short y, const char *format, ...)
    {
        char    prompt_str[1024], read_buf[1024];
        int     c, i, prompt_len;
        va_list ap;
        va_start(ap, format);
        vsnprintf(prompt_str, sizeof(prompt_str), format, ap);
        va_end(ap);
        prompt_len = strlen(prompt_str) + 3;
        while (true)
        {
            move_cursor(x, y);
            set_color_rgb(false, fg.r, fg.g, fg.b);
            set_color_rgb(true, bg.r, bg.g, bg.b);
            writef(fd, "%s: ", prompt_str);
            i = 0;
            while ((c = read_char_from_fd(fd)) != '\r')
            {
                if (c == 127)
                {
                    if (i == 0)
                    {
                        continue;
                    }
                    i--;
                    move_cursor(x + prompt_len + i, y);
                    write(fd, " ", 1);
                    move_cursor(x + prompt_len + i, y);
                    continue;
                }
                write(fd, &c, 1);
                read_buf[i++] = c;
            }
            read_buf[i]        = '\0';
            const char *answer = read_buf;
            if (wanted_answers == nullptr)
            {
                return answer;
            }
            for (const char **wa = wanted_answers; *wa; ++wa)
            {
                if (strcmp(*wa, answer) == 0)
                {
                    reset_color();
                    return answer;
                }
            }
            move_cursor(x + prompt_len, y);
            for (int j = 0; answer[j]; j++)
            {
                printf(" ");
                fflush(stdout);
            }
        }
    }

} // namespace Mlib::Term
