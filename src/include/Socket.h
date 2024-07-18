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

void        parse_url(const char *url, char *host, char *subdomain) __attribute_nonnull__((1, 2, 3));
const char *remove_header(const char *data, unsigned long *size) __attribute_nonnull__((1, 2)) __returns_nonnull;
int         create_local_unix_socket_fd();

void        ssl_init();
void        ssl_cleanup(SSL *ssl, int fd, SSL_CTX *ctx) __attribute_nonnull__((1, 3));
SSL_CTX    *ssl_create_ctx();
int         ssl_create_socket_fd(const char *host, int port) __attribute_nonnull__((1));
SSL        *ssl_connect(SSL_CTX *ctx, int fd) __attribute_nonnull__((1));
void        ssl_https_request(SSL *ssl, const char *hostname, const char *subdomain = nullptr);
const char *ssl_retrieve_response(SSL *ssl, unsigned long *size = nullptr);
const char *ssl_retrieve_url_data(const char *url, unsigned long *size = nullptr);
const char *ssl_download(const char *url, unsigned long *size);
