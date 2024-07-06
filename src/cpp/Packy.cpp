#include "../include/Packy.h"

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
packy ::find_package(const char *package)
{
    const char    *url = "https://mirror.pkgbuild.com/extra/os/x86_64/";
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
                return full_package_name;
            }
        }
        found += package_len;
    }

    return nullptr;
}

int
packy ::download(const char *package)
{
    char url[256];
    char filename[256];

    const char *found_package = find_package(package);
    if (!found_package)
    {
        fprintf(stderr, "Could not find package [%s].\n", package);
        return 1;
    }

    snprintf(url, sizeof(url), "https://mirror.pkgbuild.com/extra/os/x86_64/%s", found_package);
    snprintf(filename, sizeof(filename), "%s.pkg.tar.zst", package);

    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        fprintf(stderr, "Failed to create output file [%s].\n", filename);
        return 1;
    }

    if (!curl)
    {
        fprintf(stderr, "Curl has not been initialized something has gone wrong.\nAborting.\n");
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
        return 1;
    }
    curl_easy_cleanup(curl);

    return 0;
}
