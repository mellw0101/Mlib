#include "../include/Error.h"
#include "../include/Io.h"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

namespace Mlib::Error {
  using namespace Io;

  void fatal_error(const char *from_function, const char *when_calling, const char *format, ...) {
    char    buf[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    fwritef(STDERR_FILENO, "%s: from_function: [%s], when_calling: [%s], ERROR: [%s], %s.\n", __func__, from_function,
            when_calling, strerror(errno), buf);
    exit(1);
  }

  void non_fatal_error(bool include_errno_str, const char *from_function, const char *when_calling, const char *format,
                       ...) {
    static char buf[4096];
    va_list     ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    fwritef(STDERR_FILENO, "%s: from_function: [%s], when_calling: [%s],%s[%s] %s.\n", __func__, from_function,
            when_calling, (include_errno_str) ? " ERROR: " : "", (include_errno_str) ? strerror(errno) : "", buf);
  }

  void program_error(const char *prefix, const char *from_func, unsigned at_line, const char *when_calling,
                     const char *format, ...) {
    static char buf[4096];
    va_list     ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    fwritef(STDERR_FILENO, "%s: func: ['%s'], line: ['%u'], calling: ['%s'] %s\n", prefix, from_func, at_line,
            when_calling, buf);
  }
}