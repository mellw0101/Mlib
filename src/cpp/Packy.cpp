#include "../include/Packy.h"

using namespace Mlib::FileSys;

CONSTEXPR_MAP<u32, STRING_VIEW, 3> PackyUrlMap = {
    {{PACKY_REPO_CORE, "https://mirror.pkgbuild.com/core/os/x86_64/"},
     {PACKY_REPO_EXTRA, "https://mirror.pkgbuild.com/extra/os/x86_64/"},
     {PACKY_REPO_MULTILIB, "https://mirror.pkgbuild.com/multilib/os/x86_64/"}}
};

packy *packy::packyInstance = nullptr;

size_t
packy ::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

size_t
packy ::write_data_to_str(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t total_size = size * nmemb;
    strncat(static_cast<char *>(stream), static_cast<const char *>(ptr), total_size);
    return total_size;
}

void
packy ::decompress_zst(const char *file_path, char **data_buf, size_t *data_size)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        PERROR(fopen);
        return;
    }

    size_t compressed_size;
    if (!file_size(&compressed_size, file))
    {
        fprintf(stderr, "Error: Failed to retrive file size for file: [%s].\n", file_path);
        fclose(file);
        return;
    }

    char *tmp_data_buf = (char *)malloc(compressed_size);
    if (!tmp_data_buf)
    {
        PERROR(malloc);
        fclose(file);
        return;
    }

    const size_t read_size = fread(tmp_data_buf, 1, compressed_size, file);
    if (read_size != compressed_size)
    {
        PERROR(fread);
        fprintf(stderr, "Error reading file: [%s], expected [%zu] bytes, read [%zu] bytes.\n", file_path,
                compressed_size, read_size);
        free(tmp_data_buf);
        fclose(file);
        return;
    }
    fclose(file);

    *data_size = ZSTD_getFrameContentSize((const void *)tmp_data_buf, compressed_size);
    if (*data_size == ZSTD_CONTENTSIZE_ERROR)
    {
        fprintf(stderr, "Error: Data size is wrong, 'ZSTD_getFrameContentSize' Failed for file: [%s].\n", file_path);
        free(tmp_data_buf);
        return;
    }

    const size_t output_buffer_size = ZSTD_DStreamOutSize();
    char        *output_buffer      = (char *)malloc(output_buffer_size);
    if (output_buffer == nullptr)
    {
        PERROR(malloc);
        free(tmp_data_buf);
        return;
    }

    ZSTD_DStream *d_stream = ZSTD_createDStream();
    if (!d_stream)
    {
        fprintf(stderr, "Error: Failed to create 'ZSTD_DStream'.\n");
        free(tmp_data_buf);
        free(output_buffer);
        return;
    }

    const size_t init_result = ZSTD_initDStream(d_stream);
    if (ZSTD_isError(init_result))
    {
        fprintf(stderr, "Error: 'ZSTD_initDStream' Failed.\n");
        free(tmp_data_buf);
        free(output_buffer);
        ZSTD_freeDStream(d_stream);
        return;
    }

    *data_buf = (char *)malloc(1);
    if (*data_buf == nullptr)
    {
        PERROR(malloc);
        free(tmp_data_buf);
        free(output_buffer);
        ZSTD_freeDStream(d_stream);
        return;
    }

    size_t total_size = 0;
    size_t read_pos   = 0;

    while (read_pos < compressed_size)
    {
        ZSTD_inBuffer input = {tmp_data_buf + read_pos, compressed_size - read_pos, 0};

        while (input.pos < input.size)
        {
            ZSTD_outBuffer output = {output_buffer, output_buffer_size, 0};

            size_t result = ZSTD_decompressStream(d_stream, &output, &input);
            if (ZSTD_isError(result))
            {
                fprintf(stderr, "Error: 'ZSTD_decompressStream' Failed, ErrorName: [%s].\n", ZSTD_getErrorName(result));
                free(tmp_data_buf);
                free(output_buffer);
                ZSTD_freeDStream(d_stream);
                free(*data_buf);
                *data_buf = nullptr;
                return;
            }
            *data_buf = (char *)realloc(*data_buf, total_size + output.pos);
            if (*data_buf == nullptr)
            {
                PERROR(realloc);
                free(tmp_data_buf);
                free(output_buffer);
                ZSTD_freeDStream(d_stream);
                return;
            }
            memcpy(*data_buf + total_size, output_buffer, output.pos);
            total_size += output.pos;
        }
        read_pos += input.pos;
    }
    *data_size = total_size;

    free(tmp_data_buf);
    free(output_buffer);
    ZSTD_freeDStream(d_stream);
}

void
packy ::extract_tar(const char *data, size_t size, const char *output_path)
{
    int            result = 0;
    archive       *a      = archive_read_new();
    archive       *ext    = archive_write_disk_new();
    archive_entry *entry;

    archive_read_support_format_tar(a);
    result = archive_read_open_memory(a, data, size);
    if (result != ARCHIVE_OK)
    {
        fprintf(
            stderr, "Error: 'archive_read_open_memory' Failed, archive_error_string: [%s].\n", archive_error_string(a));
        archive_read_free(a);
        archive_write_free(ext);
        return;
    }

    while ((result = archive_read_next_header(a, &entry)) == ARCHIVE_OK)
    {
        const char *current_file = archive_entry_pathname(entry);
        char        full_output_path[PATH_MAX];
        snprintf(full_output_path, sizeof(full_output_path), "%s/%s", output_path, current_file);

        if (this->verbose_lvl == 1)
        {
            printf("%s\n", full_output_path);
        }

        archive_entry_set_pathname(entry, full_output_path);
        result = archive_write_header(ext, entry);
        if (result == ARCHIVE_FATAL)
        {
            fprintf(stderr, "Error: 'archive_write_header' Failed for file: [%s], archive_error_string: [%s]",
                    full_output_path, archive_error_string(a));
            break;
        }

        const void *buff;
        size_t      buff_size;
        int64_t     offset;

        while ((result = archive_read_data_block(a, &buff, &buff_size, &offset)) == ARCHIVE_OK)
        {
            result = archive_write_data_block(ext, buff, buff_size, offset);
            if (result != ARCHIVE_OK)
            {
                fprintf(stderr,
                        "Error: 'archive_write_data_block' Failed for file: [%s], archive_error_string: [%s].\n",
                        full_output_path, archive_error_string(a));
                break;
            }
        }

        result = archive_write_finish_entry(ext);
        if (result != ARCHIVE_OK)
        {
            fprintf(stderr, "Error: 'archive_write_finish_entry' Failed for file: [%s], archive_error_string: [%s].\n",
                    full_output_path, archive_error_string(a));
            break;
        }
    }

    if (result != ARCHIVE_EOF)
    {
        fprintf(stderr, "Error: failed to fully extract folder: [%s], archive_error_string: [%s].\n", output_path,
                archive_error_string(a));
    }

    archive_read_free(a);
    archive_write_free(ext);
}

const char *
packy ::retrieve_url(const char *package)
{
    static char url[256];

    u32 repo_index;

    const char *found = find_package(package, PACKY_REPO_ALL, &repo_index);
    if (found == nullptr)
    {
        fprintf(stderr, "packy: Could not find package: [%s].\n", package);
        return nullptr;
    }

    snprintf(url, sizeof(url), "%s%s", &PackyUrlMap[repo_index].value[0], found);

    return url;
}

packy ::packy()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

packy ::~packy()
{
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    if (packyInstance != nullptr)
    {
        delete packyInstance;
        packyInstance = nullptr;
    }
}

packy *
packy ::Instance()
{
    if (packyInstance == nullptr)
    {
        packyInstance = new packy();
    }
    return packyInstance;
}

char *
packy ::find_package(const char *package, u32 repo_mask, u32 *repo_index)
{
    int i = 0;
    for (const auto PackyUrlMapEntry : PackyUrlMap)
    {
        if ((repo_mask & PackyUrlMapEntry.key) == false)
        {
            continue;
        }
        const char *url = &PackyUrlMapEntry.value[0];

        CurlHttpClient curlhttp;
        std::string    repo = curlhttp.get(url);

        const char *found       = repo.c_str();
        const char *ext         = ".pkg.tar.zst";
        size_t      package_len = strlen(package);
        size_t      ext_len     = strlen(ext);

        while ((found = strstr(found, package)) != nullptr)
        {
            if (*(found - 1) == '"')
            {
                const char *end = strstr(found, ext);
                if (end)
                {
                    end += ext_len;
                    static char full_package_name[256];
                    strncpy(full_package_name, found, end - found);
                    full_package_name[end - found] = '\0';
                    if (repo_index != nullptr)
                    {
                        *repo_index = i;
                    }
                    return full_package_name;
                }
            }
            found += package_len;
        }
        ++i;
    }
    return nullptr;
}

int
packy ::download(const char *package)
{
    const char *url = retrieve_url(package);
    if (url == nullptr)
    {
        fprintf(stderr, "packy: ERROR: Failed to find package: [%s].\n", package);
        return 1;
    }

    char filename[256];
    snprintf(filename, sizeof(filename), "%s.pkg.tar.zst", package);

    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        fprintf(stderr, "packy: ERROR: Failed to create output file [%s].\n", filename);
        return 1;
    }

    if (!curl)
    {
        fprintf(stderr, "packy: ERROR: Curl has not been initialized something has gone wrong.\nAborting.\n");
        return 1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl Error: %s.\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        fclose(fp);
        return 1;
    }
    curl_easy_cleanup(curl);
    fclose(fp);

    char  *decompress_data;
    size_t decompress_size;

    decompress_zst(filename, &decompress_data, &decompress_size);
    extract_tar(decompress_data, decompress_size, package);

    return 0;
}

void
packy ::set_verbose_lvl(int lvl)
{
    if (lvl < 0)
    {
        lvl = 0;
    }
    else if (lvl > 1)
    {
        lvl = 1;
    }
    this->verbose_lvl = lvl;
}
