/**
    @file Compress.cpp
    @author Melwin Svensson [https://github.com/mellw0101]
    @version 0.1
    @date 2024-07-14 13:51:21

    @copyright Copyright (c) 2024

 */
#include "../include/Compress.h"
#include "../include/Error.h"
#include "../include/FileSys.h"
#include "../include/Io.h"

#include <archive.h>
#include <archive_entry.h>
#include <cstdio>
#include <cstring>
#include <zstd.h>

namespace Mlib::Compress
{
    using namespace FileSys;
    using namespace Io;

    void
    decompress_zst(const char *file, char **buf, unsigned long *size)
    {
        FILE          *f;
        char          *tmp_buf, *o_buf;
        unsigned long  c_size, o_size, r, r_pos = 0, t_size = 0;
        ZSTD_DStream  *d_stream;
        ZSTD_inBuffer  in;
        ZSTD_outBuffer out;
        if ((f = fopen(file, "rb")) == nullptr)
        {
            ferr("fopen");
        }
        if (file_size(&c_size, f) == false)
        {
            fclose(f);
            ferr("file_size");
        }
        if ((tmp_buf = (char *)malloc(c_size)) == nullptr)
        {
            fclose(f);
            ferr("malloc");
        }
        if ((fread(tmp_buf, 1, c_size, f)) != c_size)
        {
            fclose(f);
            free(tmp_buf);
            ferr("fread");
        }
        fclose(f);
        if ((*size = ZSTD_getFrameContentSize((const void *)tmp_buf, c_size)) == ZSTD_CONTENTSIZE_ERROR)
        {
            free(tmp_buf);
            ferr("ZSTD_getFrameContentSize");
        }
        if (o_size = ZSTD_DStreamOutSize(); (o_buf = (char *)malloc(o_size)) == nullptr)
        {
            free(tmp_buf);
            ferr("malloc");
        }
        if ((d_stream = ZSTD_createDStream()) == nullptr)
        {
            free(tmp_buf);
            free(o_buf);
            ferr("ZSTD_createDStream");
        }
        if (r = ZSTD_initDStream(d_stream); ZSTD_isError(r))
        {
            free(tmp_buf);
            free(o_buf);
            ZSTD_freeDStream(d_stream);
            ferr("ZSTD_initDStream");
        }
        if ((*buf = (char *)malloc(1)) == nullptr)
        {
            free(tmp_buf);
            free(o_buf);
            ZSTD_freeDStream(d_stream);
            ferr("malloc");
        }
        while (r_pos < c_size)
        {
            in = {tmp_buf + r_pos, c_size - r_pos, 0};
            while (in.pos < in.size)
            {
                out = {o_buf, o_size, 0};
                if (r = ZSTD_decompressStream(d_stream, &out, &in); ZSTD_isError(r))
                {
                    free(tmp_buf);
                    free(o_buf);
                    free(*buf);
                    *buf = nullptr;
                    ZSTD_freeDStream(d_stream);
                    ferr("ZSTD_decompressStream");
                }
                if ((*buf = (char *)realloc(*buf, t_size + out.pos)) == nullptr)
                {
                    free(tmp_buf);
                    free(o_buf);
                    free(*buf);
                    *buf = nullptr;
                    ZSTD_freeDStream(d_stream);
                    ferr("realloc", "total sise: %lu, out.pos: %lu", t_size, out.pos);
                }
                memcpy(*buf + t_size, o_buf, out.pos);
                t_size += out.pos;
            }
            r_pos += in.pos;
        }
        *size = t_size;
        free(tmp_buf);
        free(o_buf);
        ZSTD_freeDStream(d_stream);
    }

    void
    compress_zst(const char *in_file, const char *out_file)
    {
        FILE         *in_f, *out_f;
        char         *in_buf, *c_buf;
        unsigned long in_size, c_buf_size, c_size;
        if ((in_f = fopen(in_file, "rb")) == nullptr)
        {
            ferr("fopen", "Could not open file: ['%s'].", in_file);
        }
        g_fsize(in_f, &in_size);
        if ((in_buf = (char *)malloc(in_size)) == nullptr)
        {
            fclose(in_f);
            ferr("malloc");
        }
        if ((fread(in_buf, 1, in_size, in_f)) != in_size)
        {
            free(in_buf);
            fclose(in_f);
            ferr("fread");
        }
        fclose(in_f);
        c_buf_size = ZSTD_compressBound(in_size);
        if ((c_buf = (char *)malloc(c_buf_size)) == nullptr)
        {
            free(in_buf);
            ferr("malloc");
        }
        if (c_size = ZSTD_compress(c_buf, c_buf_size, in_buf, in_size, 1); ZSTD_isError(c_size))
        {
            free(in_buf);
            free(c_buf);
            ferr("ZSTD_compress", "Error: ['%s'].", ZSTD_getErrorName(c_size));
        }
        free(in_buf);
        if ((out_f = fopen(out_file, "wb")) == nullptr)
        {
            free(c_buf);
            ferr("fopen", "Could not open file: ['%s'].", out_file);
        }
        if ((fwrite(c_buf, 1, c_size, out_f)) != c_size)
        {
            free(c_buf);
            fclose(out_f);
            ferr("fwrite");
        }
        fclose(out_f);
        free(c_buf);
    }

    void
    extract_tar(const char *buf, const unsigned long size, const char *output, int verbose)
    {
        archive       *a, *ext;
        archive_entry *entry;
        int            r;
        static char    full_path[PATH_MAX];
        const char    *current_file;
        const void    *t_buf;
        unsigned long  t_size;
        long           offset;
        a   = archive_read_new();
        ext = archive_write_disk_new();
        archive_read_support_format_tar(a);
        if ((r = archive_read_open_memory(a, buf, size)) != ARCHIVE_OK)
        {
            archive_read_free(a);
            archive_write_free(ext);
            ferr("archive_read_open_memory", "archive_error_string: [%s]", archive_error_string(a));
        }
        while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK)
        {
            current_file = archive_entry_pathname(entry);
            snprintf(full_path, PATH_MAX, "%s/%s", output, current_file);
            verbose == 1 ? writef("%s\n", full_path) : void();
            archive_entry_set_pathname(entry, full_path);
            if ((r = archive_write_header(ext, entry)) == ARCHIVE_FATAL)
            {
                nerr("archive_write_header", "archive_error_string: ['%s']", archive_error_string(a));
                break;
            }
            while ((r = archive_read_data_block(a, &t_buf, &t_size, &offset)) == ARCHIVE_OK)
            {
                if ((r = archive_write_data_block(ext, t_buf, t_size, offset)) != 0)
                {
                    nerr("archive_write_data_block", "archive_error_string: [%s]", archive_error_string(a));
                    break;
                }
            }
            if ((r = archive_write_finish_entry(ext)) != ARCHIVE_OK)
            {
                nerr("archive_write_finish_entry", "archive_error_string: [%s]", archive_error_string(a));
                break;
            }
        }
        if (r != ARCHIVE_EOF)
        {
            nerr(__func__, "Could not fully extract archive, archive_error_string: [%s]", archive_error_string(a));
        }
        archive_read_free(a);
        archive_write_free(ext);
    }

} // namespace Mlib::Compress
