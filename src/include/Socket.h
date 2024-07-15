/**
    @file Socket.h
    @author Melwin Svensson [https://github.com/mellw0101]
    @version 0.1
    @date 2024-07-10 08:37:03

    @copyright Copyright (c) 2024

 */
#pragma once

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

void parse_url(const char *url, char *host, char *subdomain);
int  create_local_unix_socket_fd();

void        ssl_init();
void        ssl_cleanup(SSL *ssl, int fd, SSL_CTX *ctx);
SSL_CTX    *ssl_create_ctx();
int         ssl_create_socket_fd(const char *host, int port);
SSL        *ssl_connect(SSL_CTX *ctx, int fd);
void        ssl_https_request(SSL *ssl, const char *hostname, const char *subdomain = nullptr);
const char *ssl_retrieve_response(SSL *ssl, unsigned long *size = nullptr);
const char *ssl_retrieve_url_data(const char *url, unsigned long *size = nullptr);
