#pragma once

namespace Mlib::Io
{
    void fwritef(int fd, const char *format, ...);
    void writef(const char *format, ...);
    void cwrite(const char c);
    void fcwrite(int fd, const char c);

} // namespace Mlib::Io

char **get_file_lines(const char *path);
