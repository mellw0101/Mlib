#pragma once

#include "Http.h"
#include "def.h"

#include <cstring>
#include <curl/curl.h>

using namespace Mlib::Http;

class packy
{
private:
    static packy *packyInstance;

    CURL *curl = nullptr;

    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
    static size_t write_data_to_str(void *ptr, size_t size, size_t nmemb, void *stream);

    packy();
    ~packy();

public:
    static packy *Instance();

    char *find_package(const char *package);
    int   download(const char *package);
};

#define PACKY packy::Instance()
