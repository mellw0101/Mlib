#include "../include/Socket.h"
#include "../include/Error.h"

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <unistd.h>

void
parse_url(const char *url, char *host, char *subdomain)
{
    const char *p = strchr(url, '/');
    if (p != nullptr)
    {
        size_t host_len = p - url;
        strncpy(host, url, host_len);
        host[host_len] = '\0';
        strcpy(subdomain, p);
    }
    else
    {
        strcpy(host, url);
        strcpy(subdomain, "/");
    }
}

int
create_local_unix_socket_fd(int socket_domain, int socket_type, int socket_protocol)
{
    int fd;
    if ((fd = socket(socket_domain, socket_type, socket_protocol)) == -1)
    {
        fatal_err("socket");
    }
    return fd;
}

void
ssl_init()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void
ssl_cleanup(SSL *ssl, int fd, SSL_CTX *ctx)
{
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(fd);
    SSL_CTX_free(ctx);
    EVP_cleanup();
}

SSL_CTX *
ssl_create_ctx()
{
    const SSL_METHOD *method;
    SSL_CTX          *ssl_ctx;
    method  = SSLv23_client_method();
    ssl_ctx = SSL_CTX_new(method);
    if (ssl_ctx == nullptr)
    {
        ERR_print_errors_fp(stderr);
        fatal_err("SSL_CTX_new");
    }
    return ssl_ctx;
}

int
ssl_create_socket_fd(const char *hostname, int port)
{
    int             socket_fd;
    struct addrinfo hints, *res, *p;
    char            port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", port);
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(hostname, port_str, &hints, &res) != 0)
    {
        ERR_print_errors_fp(stderr);
        fatal_err("getaddrinfo");
    }
    for (p = res; p != nullptr; p = p->ai_next)
    {
        if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            continue;
        }
        if (connect(socket_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(socket_fd);
            continue;
        }
        break;
    }
    if (p == nullptr)
    {
        ERR_print_errors_fp(stderr);
        fatal_err("ssl_create_socket_fd", "Failed to connect");
    }
    freeaddrinfo(res);
    return socket_fd;
}

SSL *
ssl_connect(SSL_CTX *ctx, int fd)
{
    SSL *ssl;
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);
    if (SSL_connect(ssl) < 1)
    {
        ERR_print_errors_fp(stderr);
        fatal_err("SSL_connect");
    }
    return ssl;
}

void
ssl_https_request(SSL *ssl, const char *hostname, const char *subdomain)
{
    int          ret;
    int          error;
    static char  buf[4096];
    const int    subdomain_len = (subdomain) ? strlen(subdomain) : 1;
    const size_t req_len =
        strlen("GET / HTTP/1.1\r\nHost: \r\nConnection: close\r\n\r\n") + strlen(hostname) + subdomain_len;
    snprintf(buf, req_len + 1, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
             (subdomain) ? subdomain : "/", hostname);
    ret = SSL_write(ssl, buf, req_len);
    if (ret > 0)
    {
        return;
    }
    error = SSL_get_error(ssl, ret);
    switch (error)
    {
        case SSL_ERROR_ZERO_RETURN :
        {
            non_fatal_err("SSL_write", "Connection closed cleanly, but no bytes were sent");
            break;
        }
        case SSL_ERROR_WANT_WRITE :
        {
            non_fatal_err("SSL_write", "Needs to be run again");
            break;
        }
        case SSL_ERROR_WANT_READ :
        {
            non_fatal_err("SSL_write", "Needs to read data");
            break;
        }
        case SSL_ERROR_SYSCALL :
        {
            if (errno == 0)
            {
                non_fatal_err("SSL_write", "Syscall failed abruptly, unknown reason");
            }
            else
            {
                non_fatal_err_with_errno_str("SSL_write", "Syscall failed.");
            }
            break;
        }
        case SSL_ERROR_SSL :
        {
            non_fatal_err("SSL_write", "OpenSSL library error");
            ERR_print_errors_fp(stderr);
            break;
        }
        default :
        {
            non_fatal_err("SSL_write", "Unknown error");
            break;
        }
    }
}

const char *
ssl_retrieve_response(SSL *ssl, unsigned long *size)
{
    int         bytes, error, total_bytes_read = 0;
    static char buf[4096];
    char       *storage_buf = (char *)malloc(1);
    if (storage_buf == nullptr)
    {
        fatal_err("malloc");
    }
    storage_buf[0] = '\0';
    while ((bytes = SSL_read(ssl, buf, sizeof(buf) - 1)) > 0)
    {
        storage_buf = (char *)realloc(storage_buf, total_bytes_read + sizeof(buf));
        if (storage_buf == nullptr)
        {
            fatal_err("realloc");
        }
        buf[bytes] = '\0';
        strcat(storage_buf, buf);
        total_bytes_read += bytes;
    }
    if (total_bytes_read == 0)
    {
        non_fatal_err("SSL_read", "'0' bytes were read.");
        return nullptr;
    }
    if (bytes == 0)
    {
        (size) ? *size = total_bytes_read : 0;
        return storage_buf;
    }
    error = SSL_get_error(ssl, bytes);
    switch (error)
    {
        case SSL_ERROR_WANT_READ :
        {
            non_fatal_err("SSL_read", "Call SSL_read again.");
            break;
        }
        case SSL_ERROR_WANT_WRITE :
        {
            non_fatal_err("SSL_read", "Call SSL_write again.");
            break;
        }
        case SSL_ERROR_SYSCALL :
        {
            if (errno == 0)
            {
                non_fatal_err("SSL_read", "Syscall failed abruptly, unknown reason.");
            }
            else
            {
                non_fatal_err_with_errno_str("SSL_read", "Syscall failed.");
            }
            break;
        }
        case SSL_ERROR_SSL :
        {
            ERR_print_errors_fp(stdout);
            non_fatal_err("SSL_read", "SSL library error.");
            break;
        }
        default :
        {
            non_fatal_err("SSL_read", "Unknown error");
            break;
        }
    }
    return nullptr;
}

const char *
ssl_retrieve_url_data(const char *url, unsigned long *size)
{
    int              fd;
    unsigned long    total_size;
    SSL             *ssl;
    SSL_CTX         *ssl_ctx;
    static const int port = 443;
    static char      hostname[1024], subdomain[1024];
    ssl_init();
    ssl_ctx = ssl_create_ctx();
    parse_url(url, hostname, subdomain);
    fd  = ssl_create_socket_fd(hostname, port);
    ssl = ssl_connect(ssl_ctx, fd);
    ssl_https_request(ssl, hostname, subdomain);
    const char *response = ssl_retrieve_response(ssl, &total_size);
    (size) ? *size = total_size : 0;
    ssl_cleanup(ssl, fd, ssl_ctx);
    return response;
}