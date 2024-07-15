/**
    @file Compress.h
    @author Melwin Svensson [https://github.com/mellw0101]
    @version 0.1
    @date 2024-07-14 13:22:92

    @copyright Copyright (c) 2024

 */
#pragma once

#include "../include/Types.h"

#include <cstdio>

namespace Mlib::Compress
{
    void decompress_zst(const char *file, char **buf, unsigned long *size);

    void extract_tar(const char *data __attribute__((nonnull)), const unsigned long size, const char *output,
                     int verbose = 0);

} // namespace Mlib::Compress
