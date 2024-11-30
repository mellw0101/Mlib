#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define Ulong unsigned long
#define null __null

char *read_fd_with_lock(int fd, Ulong *size);
int write_fd_with_lock(int fd, const char *buffer, Ulong size);
int write_file_with_lock(const char *file, int cmd, const char *buffer, Ulong size);

#ifdef __cplusplus
char *read_file_with_lock(const char *file, Ulong *file_size = NULL, int *error = NULL);
#else
char *read_file_with_lock(const char *file, Ulong *file_size, int *error);
#endif

#ifdef __cplusplus
}
#endif