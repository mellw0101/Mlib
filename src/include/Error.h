#pragma once

namespace Mlib::Error
{
    [[noreturn]]
    void fatal_error(const char *from_function __attribute((__nonnull__)),
                     const char *when_calling __attribute((__nonnull__)), const char *format = "", ...);

    void non_fatal_error(bool include_errno_str, const char *from_function __attribute((__nonnull__)),
                         const char *when_calling __attribute((__nonnull__)), const char *format = "", ...);

} // namespace Mlib::Error

#define fatal_err(...)                    Mlib ::Error ::fatal_error(__func__, __VA_ARGS__)
#define non_fatal_err(...)                Mlib ::Error ::non_fatal_error(false, __func__, __VA_ARGS__)
#define non_fatal_err_with_errno_str(...) Mlib ::Error ::non_fatal_error(true, __func__, __VA_ARGS__)
