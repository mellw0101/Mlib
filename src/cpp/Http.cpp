#include "../include/Http.h"

/// @class @c HttpClient

HttpClient::HttpClient(const string& host, const string& path)
    : host_(host)
    , path_(path)
{
    struct hostent* he = gethostbyname(host_.c_str());
    if (he == nullptr)
    {
        throw std::runtime_error("Failed to resolve host");
    }
    server_.sin_family = AF_INET;
    server_.sin_port   = htons(80);
    server_.sin_addr   = *((struct in_addr*)he->h_addr);
}

string
HttpClient::get()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        throw runtime_error("Failed to create socket");
    }

    if (connect(sockfd, (struct sockaddr*)&server_, sizeof(server_)) < 0)
    {
        close(sockfd);
        throw runtime_error("Failed to connect to server");
    }

    string request = "GET " + path_ +
        " HTTP/1.1\r\n"
        "Host: " +
        host_ +
        "\r\n"
        "Connection: close\r\n\r\n";
    send(sockfd, request.c_str(), request.length(), 0);

    char         buffer[4096];
    stringstream response;
    int          bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)
    {
        response.write(buffer, bytes_received);
    }
    close(sockfd);

    return response.str();
}


/// @class @c CurlHttpClient

CurlHttpClient::CurlHttpClient()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_ = curl_easy_init();
}

CurlHttpClient::~CurlHttpClient()
{
    if (curl_)
    {
        curl_easy_cleanup(curl_);
    }
    curl_global_cleanup();
}

size_t
CurlHttpClient::writeCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string
CurlHttpClient::get(const string& url)
{
    if (!curl_)
    {
        throw std::runtime_error("Failed to initialize CURL");
    }

    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response_);

    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK)
    {
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }

    return response_;
}
