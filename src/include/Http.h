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
        HttpClient(const string &host, const string &path);
        string get();

    private:
        string             host_;
        string             path_;
        struct sockaddr_in server_;
    };

    class CurlHttpClient
    {
    public:
        CurlHttpClient();
        ~CurlHttpClient();
        string get(const string &url);

    private:
        static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

        CURL       *curl_;
        std::string response_;
    };
} // namespace Mlib::Http
