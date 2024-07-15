#pragma once

#include "FileSys.h"
#include "Http.h"
#include "constexpr.hpp"
#include "def.h"

#include <archive.h>
#include <archive_entry.h>
#include <cstring>
#include <curl/curl.h>
#include <zstd.h>

using namespace Mlib::Http;

#define PACKY_REPO_CORE     (1 << 0)
#define PACKY_REPO_EXTRA    (1 << 1)
#define PACKY_REPO_MULTILIB (1 << 2)

#define PACKY_REPO_ALL      PACKY_REPO_CORE | PACKY_REPO_EXTRA | PACKY_REPO_MULTILIB

class packy
{
private:
    static packy *packyInstance;
    CURL         *curl = nullptr;

    int verbose_lvl = 0;

    static size_t write_data(void *, size_t, size_t, FILE *);

    // void extract_tar(const char *, size_t, const char *);

    const char *retrieve_url(const char *package);

    packy();
    ~packy();

public:
    static packy *Instance();

    char *find_package(const char *package, u32 repo_mask = PACKY_REPO_ALL, u32 *repo_index = nullptr);
    int   download(const char *package);
    void  set_verbose_lvl(int lvl);
};

#define PACKY packy::Instance()
