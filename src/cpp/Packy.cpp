#include "../include/Packy.h"
#include <cassert>
#include "../include/Compress.h"
#include "../include/Error.h"
#include "../include/Socket.h"
#include "../include/def.h"

using namespace Mlib;
using namespace FileSys;
using namespace Compress;

#define PROG_PREFIX "packy"

constexpr_map<unsigned int, STRING_VIEW, 5> PackyUrlMap = {
    {{PACKY_REPO_CORE, "ftp.lysator.liu.se/pub/archlinux/core/os/x86_64/"},
     {PACKY_REPO_MULTILIB, "ftp.lysator.liu.se/pub/archlinux/multilib/os/x86_64/"},
     {PACKY_REPO_CORE, "mirror.pkgbuild.com/core/os/x86_64/"},
     {PACKY_REPO_EXTRA, "mirror.pkgbuild.com/extra/os/x86_64/"},
     {PACKY_REPO_MULTILIB, "mirror.pkgbuild.com/multilib/os/x86_64/"}}
};

packy *packy::packyInstance = nullptr;

const char *
packy ::retrieve_url(const char *package)
{
    static char  url[PATH_MAX];
    unsigned int repo_index;
    const char  *found;
    if ((found = this->find_package(package, PACKY_REPO_ALL, &repo_index)) == nullptr)
    {
        prog_err("find_package", "Could not find package: ['%s'].", package);
        return nullptr;
    }
    snprintf(url, sizeof(url), "%s%s", &PackyUrlMap[repo_index].value[0], found);
    return url;
}

packy ::packy()
{}

packy ::~packy()
{
    if (this->packyInstance != nullptr)
    {
        delete this->packyInstance;
        this->packyInstance = nullptr;
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
    int           i = 0;
    unsigned long package_len, ext_len;
    const char   *found, *ext, *end;
    static char   full_package_name[PATH_MAX];
    ext         = ".pkg.tar.zst";
    package_len = strlen(package);
    ext_len     = strlen(ext);
    for (const auto Entry : PackyUrlMap)
    {
        if ((repo_mask & Entry.key) == false)
        {
            continue;
        }
        if ((found = ssl_retrieve_url_data(&Entry.value[0])) == nullptr)
        {
            prog_err("ssl_retrieve_url_data", "Failed to get data from: ['%s']", &Entry.value[0]);
            return nullptr;
        }
        while ((found = strstr(found, package)) != nullptr)
        {
            if (*(found - 1) == '"')
            {
                if ((end = strstr(found, ext)) != nullptr)
                {
                    end += ext_len;
                    memmove(full_package_name, found, end - found);
                    full_package_name[end - found] = '\0';
                    repo_index ? *repo_index = i : 0;
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
    unsigned long size, new_size;
    char         *buf;
    static char   file[PATH_MAX];
    const char   *url, *response, *ext;
    if ((url = this->retrieve_url(package)) == nullptr)
    {
        prog_err("retrieve_url");
        return 1;
    }
    if ((response = ssl_retrieve_url_data(url, &size)) == nullptr)
    {
        prog_err("ssl_retrieve_url_data");
        return 1;
    }
    response = remove_header(response, &size);
    ext      = ".pkg.tar.zst";
    snprintf(file, sizeof(file), "%s%s", package, ext);
    write_to_file(response, &size, file);
    decompress_zst(file, &buf, &new_size);
    extract_tar(buf, new_size, package, this->verbose_lvl);
    return 0;
}

void
packy ::set_verbose_lvl(int lvl)
{
    this->verbose_lvl = (lvl < 0) ? 0 : (lvl > 1) ? 1 : lvl;
}
