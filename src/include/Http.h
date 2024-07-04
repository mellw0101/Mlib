/// @file Http.h
#pragma once

#include <arpa/inet.h>
#include <curl/curl.h>
#include <iostream>
#include <netdb.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "def.h"
using namespace std;

namespace Mlib::Http
{
    class HttpClient
    {
    public:
        HttpClient(C_STRING &host, C_STRING &path);
        string get();

    private:
        string      host_;
        string      path_;
        sockaddr_in server_;
    };

    class CurlHttpClient
    {
    public:
        CurlHttpClient();
        ~CurlHttpClient();
        string get(const string &url);

    private:
        static u64 writeCallback(void *contents, u64 size, u64 nmemb, void *userp);

        CURL  *curl_;
        STRING response_;
    };
} // namespace Mlib::Http
