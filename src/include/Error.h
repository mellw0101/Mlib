#pragma once

namespace Mlib::Error
{
    [[noreturn]]
    void fatal_error(const char *from_function __attribute((nonnull)), const char *when_calling __attribute((nonnull)),
                     const char *format = "", ...);

    void non_fatal_error(bool include_errno_str, const char *from_function __attribute((nonnull)),
                         const char *when_calling __attribute((nonnull)), const char *format = "", ...);

    void program_error(const char *prefix __attribute((nonnull)), const char *from_func, unsigned at_line,
                       const char *when_calling, const char *format = "", ...);

} // namespace Mlib::Error

#define fatal_err(...)                    Mlib ::Error ::fatal_error(__func__, __VA_ARGS__)
#define non_fatal_err(...)                Mlib ::Error ::non_fatal_error(false, __func__, __VA_ARGS__)
#define non_fatal_err_with_errno_str(...) Mlib ::Error ::non_fatal_error(true, __func__, __VA_ARGS__)
#define ferr(...)                         fatal_err(__VA_ARGS__)
#define nerr(...)                         non_fatal_err(__VA_ARGS__)
#define nerre(...)                        non_fatal_err_with_errno_str(__VA_ARGS__)

/**
    Define 'PROG_PREFIX' as the name of the program.
 */
#define prog_err(...)                     Mlib::Error::program_error(PROG_PREFIX, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__)
