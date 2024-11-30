/* clang-format off */
#include "../include/File.h"

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

char *read_fd_with_lock(int fd, Ulong *size) {
  /* Return early if params are invalid. */
  if (fd < 0 || !size) {
    errno = EINVAL;
    return NULL;  
  }
  struct flock lock;
  struct stat st;
  char *buffer = NULL;
  long bytes_read = 0;
  /* Init flock struct. */
  lock.l_type   = F_RDLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start  = 0;
  lock.l_len    = 0;
  /* Lock the fd. */
  if (fcntl(fd, F_SETLKW, &lock) == -1) {
    return NULL;
  }
  /* Get file stats. */
  if (fstat(fd, &st) == -1) {
    /* Unlock fd before returning apon faiure. */
    fcntl(fd, F_UNLCK, &lock);
    return NULL;
  }
  /* If file is empty return a empty buffer. */
  if (st.st_size == 0) {
    buffer = (char *)malloc(1);
    *buffer = '\0';
    *size = 0;
    fcntl(fd, F_UNLCK, &lock);
    return buffer;
  }
  buffer = (char *)malloc(st.st_size + 1);
  while (bytes_read < st.st_size) {
    long result = read(fd, (buffer + bytes_read), (st.st_size - bytes_read));
    if (result < 0) {
      free(buffer);
      *size = 0;
      fcntl(fd, F_UNLCK, &lock);
      return NULL;
    }
    bytes_read += result;
  }
  buffer[bytes_read] = '\0';
  *size = bytes_read;
  fcntl(fd, F_UNLCK, &lock);
  return buffer;
}

int write_fd_with_lock(int fd, const char *buffer, Ulong size) {
  /* Return early when params are invalid. */
  if (fd < 0 || !buffer || !size) {
    return EINVAL;
  }
  int ret = 0;
  struct flock lock;
  struct stat st;
  long bytes_written = 0, result;
  Ulong old_size;
  /* Init flock struct. */
  lock.l_type   = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start  = 0;
  lock.l_len    = 0;
  /* Lock the file. */
  if (fcntl(fd, F_SETLKW, &lock) == -1) {
    return errno;
  }
  /* Get file stats. */
  if (fstat(fd, &st) == -1) {
    fcntl(fd, F_UNLCK, &lock);
    return errno;
  }
  old_size = st.st_size;
  while (bytes_written < size) {
    result = write(fd, (buffer + bytes_written), (size - bytes_written));
    if (result < 0) {
      fcntl(fd, F_UNLCK, &lock);
      return errno;
    }
    bytes_written += result;
  }
  /* Get file stats after write is done. */
  if (fstat(fd, &st) == -1) {
    ret = errno;
  }
  /* Check that the size of the file is correct. */
  else if ((old_size + bytes_written) != st.st_size) {
    ret = EIO;
  }
  fcntl(fd, F_UNLCK, &lock);
  return ret;
}

char *read_file_with_lock(const char *file, Ulong *file_size, int *error) {
  /* Return early if file is invalid. */
  if (!file || !*file) {
    errno = EINVAL;
    return NULL;
  }
  int fd;
  Ulong size;
  char *data;
  /* Open the fd as read-only. */
  fd = open(file, O_RDONLY);
  if (fd < 0) {
    error ? (*error = errno) : 0;
    return NULL;
  }
  data = read_fd_with_lock(fd, &size);
  if (!data) {
    error ? (*error = errno) : 0;
    return NULL;
  }
  close(fd);
  file_size ? (*file_size = size) : 0;
  return data;
}

int write_file_with_lock(const char *file, int cmd, const char *buffer, Ulong size) {
  if (!file || !*file) {
    return EINVAL;
  }
  int fd, ret;
  fd = open(file, cmd);
  if (fd < 0) {
    return errno;
  }
  ret = write_fd_with_lock(fd, buffer, size);
  close(fd);
  return ret;
}
