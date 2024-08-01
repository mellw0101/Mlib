#include "../include/Io.h"
#include "../include/Error.h"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

namespace Mlib::Io
{
    void
    fwritef(int fd, const char *format, ...)
    {
        static char buf[1024];
        va_list     ap;
        va_start(ap, format);
        vsnprintf(buf, sizeof(buf), format, ap);
        va_end(ap);
        unsigned long length = strlen(buf);
        write(fd, &buf, length);
    }

    void
    writef(const char *format, ...)
    {
        static char buf[4096];
        va_list     ap;
        va_start(ap, format);
        vsnprintf(buf, sizeof(buf), format, ap);
        va_end(ap);
        const size_t  len    = strlen(buf);
        const ssize_t wr_len = write(STDOUT_FILENO, &buf, len);
        if (wr_len != len || wr_len == -1)
        {
            fprintf(stderr, "%s: 'write' Failed, ERROR: [%s].\n", __func__, strerror(errno));
            exit(1);
        }
    }

    void
    cwrite(const char c)
    {
        long len = write(STDOUT_FILENO, &c, 1);
        if (len != 1 || len == -1)
        {
            ferr("write", "%zi (len), was not == 1", len);
        }
    }

    void
    fcwrite(int fd, const char c)
    {
        ssize_t len = write(fd, &c, 1);
        if (len != 1 || len == -1)
        {
            fatal_err("write", "%zi (len), was not == 1", len);
        }
    }

} // namespace Mlib::Io

char **
get_file_lines(const char *path)
{
    unsigned long size, len, i, cap;
    char         *buf, *line, **lines;
    FILE         *f;
    if ((f = fopen(path, "rb")) == NULL)
    {
        nerr("fopen");
        return NULL;
    }
    i = 0, cap = 10;
    lines = (char **)malloc(sizeof(char *) * cap);
    while ((len = getline(&buf, &size, f)) != EOF)
    {
        if (i == cap)
        {
            cap *= 2;
            lines = (char **)realloc(lines, sizeof(char *) * cap);
        }
        if (buf[len - 1] == '\n')
        {
            buf[--len] = '\0';
        }
        line = (char *)malloc(len);
        strcpy(line, buf);
        lines[i++] = line;
    }
    lines[i] = NULL;
    fclose(f);
    return lines;
}
