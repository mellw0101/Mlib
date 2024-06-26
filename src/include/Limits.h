#pragma once

constexpr auto NR_OPEN = 1024;

constexpr auto NGROUPS_MAX    = 65536;  /* supplemental group IDs are available */
constexpr auto ARG_MAX        = 131072; /* # bytes of args + environ for exec() */
constexpr auto LINK_MAX       = 127;    /* # links a file may have */
constexpr auto MAX_CANON      = 255;    /* size of the canonical input queue */
constexpr auto MAX_INPUT      = 255;    /* size of the type-ahead buffer */
constexpr auto NAME_MAX       = 255;    /* # chars in a file name */
constexpr auto PATH_MAX       = 4096;   /* # chars in a path name including nul */
constexpr auto PIPE_BUF       = 4096;   /* # bytes in atomic write to a pipe */
constexpr auto XATTR_NAME_MAX = 255;    /* # chars in an extended attribute name */
constexpr auto XATTR_SIZE_MAX = 65536;  /* size of an extended attribute value (64k) */
constexpr auto XATTR_LIST_MAX = 65536;  /* size of extended attribute namelist (64k) */

constexpr auto RTSIG_MAX = 32;
