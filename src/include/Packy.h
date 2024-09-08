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
    DELETE_COPY_AND_MOVE_CONSTRUCTORS(packy);
    static packy *packyInstance;
    int           verbose_lvl = 0;
    packy(void);
    ~packy(void);

public:
    static packy *Instance(void);
    const char   *retrieve_url(const char *package);
    char *find_package(const char *package, unsigned repo_mask = PACKY_REPO_ALL, unsigned *repo_index = nullptr);
    int   download(const char *p);
    void  set_verbose_lvl(int lvl);
};

#define PACKY packy::Instance()
