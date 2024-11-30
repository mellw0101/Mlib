#include "../include/Error.h"
#include "../include/Io.h"
#include "../include/Sys.h"

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

  void non_fatal_error(bool include_errno_str, const char *from_function, const char *when_calling, const char *format, ...) {
    static char buf[4096];
    va_list     ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    fwritef(STDERR_FILENO, "%s: from_function: [%s], when_calling: [%s],%s[%s] %s.\n", __func__, from_function,
            when_calling, (include_errno_str) ? " ERROR: " : "", (include_errno_str) ? strerror(errno) : "", buf);
  }

  void program_error(const char *prefix, const char *from_func, unsigned at_line, const char *when_calling, const char *format, ...) {
    static char buf[4096];
    va_list     ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    fwritef(STDERR_FILENO, "%s: func: ['%s'], line: ['%u'], calling: ['%s'] %s\n", prefix, from_func, at_line,
            when_calling, buf);
  }
}

const char *errno_str(int err) {
  switch (err) {
    case EPERM   : return "Operation not premitted.";
    case ENOENT  : return "No such file or directory";
    case ESRCH   : return "No such process";
    case EINTR   : return "Interrupted system call";
    case EIO     : return "I/O error";
    case ENXIO   : return "No such device or address";
    case E2BIG   : return "Argument list to long";
    case ENOEXEC : return "Exec format error";
    case EBADF   : return "Bad file number";
    case ECHILD  : return "No child process";
    case EAGAIN  : return "Try again";
    case ENOMEM  : return "Out of memory";
    case EACCES  : return "Premission denied";
    case EFAULT  : return "Bad address";
    case ENOTBLK : return "Block device required";
    case EBUSY   : return "Device or resource busy";
    case EEXIST  : return "File exists";
    case EXDEV   : return "Cross-device link";
    case ENODEV  : return "No such device";
    case ENOTDIR : return "Not a directory";
    case EISDIR  : return "Is a directory";
    case EINVAL  : return "Invalid argument";
    case ENFILE  : return "File table overflow";
    case ENOTTY  : return "Not a typewriter";
    case ETXTBSY : return "Text file busy";
    case EFBIG   : return "File to large";
    case ENOSPC  : return "No space left on device";
    case ESPIPE  : return "Illegal seek";
    case EROFS   : return "Read-only file system";
    case EMLINK  : return "Too meny links";
    case EPIPE   : return "Broken pipe";
    case EDOM    : return "Math argument out of domain";
    case ERANGE  : return "Math result not representable";
    default      : return "";
  }
}

int print_detailed_error(Uint line_number, const char *calling_function, const char *format, ...) {
  static thread_local char time[256];
  format_time(get_current_time_ms(), time, sizeof(time));
  static thread_local char buffer[16384];
  va_list va;
  va_start(va, format);
  vsnprintf(buffer, sizeof(buffer), format, va);
  va_end(va);
  return printe("[%s]:[%s]:LINE[%u]: %s\n", time, calling_function, line_number, buffer);
}