#include "../include/Packy.h"
#include "../include/Compress.h"
#include "../include/Socket.h"

using namespace Mlib;
using namespace FileSys;
using namespace Compress;

CONSTEXPR_MAP<unsigned int, STRING_VIEW, 3> PackyUrlMap = {
    {{PACKY_REPO_CORE, "mirror.pkgbuild.com/core/os/x86_64/"},
     {PACKY_REPO_EXTRA, "mirror.pkgbuild.com/extra/os/x86_64/"},
     {PACKY_REPO_MULTILIB, "mirror.pkgbuild.com/multilib/os/x86_64/"}}
};

packy *packy::packyInstance = nullptr;

size_t
packy ::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

const char *
packy ::retrieve_url(const char *package)
{
    static char  url[256];
    unsigned int repo_index;
    const char  *found = find_package(package, PACKY_REPO_ALL, &repo_index);
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
packy ::find_package(const char *package, unsigned repo_mask, unsigned *repo_index)
{
    int i = 0;
    for (const auto PackyUrlMapEntry : PackyUrlMap)
    {
        if ((repo_mask & PackyUrlMapEntry.key) == false)
        {
            continue;
        }
        const char *found = ssl_retrieve_url_data(&PackyUrlMapEntry.value[0]);
        if (!found)
        {
            return nullptr;
        }
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
    extract_tar(decompress_data, decompress_size, package, this->verbose_lvl);

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
