#include "../include/Debug.h"
#include <cstdarg>

namespace Mlib::Debug
{
    Lout *Lout ::_LoutInstance = NULL;

    Lout &
    Lout ::Instance()
    {
        using namespace std;
        if (_LoutInstance == nullptr)
        {
            lock_guard<mutex> lock(mutex);
            _LoutInstance = new Lout();
        }
        return *_LoutInstance;
    }

    void
    Lout ::destroy()
    {
        if (_LoutInstance != nullptr)
        {
            delete _LoutInstance;
            _LoutInstance = nullptr;
        }
    }

    void
    Lout ::log(const LogLevel log_level, const char *from_func, const unsigned long lineno, const char *format, ...)
    {
        char    fbuf[4096], buf[8192];
        va_list ap;
        va_start(ap, format);
        vsnprintf(fbuf, sizeof(fbuf), format, ap);
        snprintf(buf, sizeof(buf), "%s:%s:%s[Line:%lu]%s:%s[%s]%s: %s\n", TIME::mili().c_str(),
                 &logLevelMap[log_level].key[0], ESC_CODE_YELLOW, lineno, ESC_CODE_RESET, ESC_CODE_MAGENTA, from_func,
                 ESC_CODE_RESET, fbuf);
        std::lock_guard<std::mutex> guard(_log_mutex);
        std::ofstream               file(&_output_file[0], std::ios::app);
        if (file)
        {
            file << buf;
        }
    }

    NetworkLogger *NetworkLogger ::_NetworkLoggerInstance = NULL;

    NetworkLogger ::NetworkLogger()
        : _CONNECTED(FALSE)
    {}

    static constexpr unsigned char PACKET_SIZE = 64;
    static constexpr unsigned char TIMEOUT     = 1;

    unsigned short
    NetworkLogger ::checksum(void *b, int len)
    {
        unsigned short result, *buf = (unsigned short *)b;
        unsigned int   sum = 0;
        for (sum = 0; len > 1; len -= 2)
        {
            sum += *buf++;
        }
        if (len == 1)
        {
            sum += *(unsigned char *)buf;
        }
        sum = (sum >> 16) + (sum & 0xFFFF);
        sum += (sum >> 16);
        result = ~sum;
        return result;
    }

    bool
    NetworkLogger ::ping(std::string_view ip)
    {
        struct sockaddr_in addr;
        struct icmp        icmp_hdr;
        struct timeval     timeout = {TIMEOUT, 0};
        struct sockaddr_in r_addr;

        socklen_t addr_len = sizeof(r_addr);

        s32 sockfd;
        s8  packet[PACKET_SIZE];

        if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        {
            perror("socket");
            return false;
        }

        addr.sin_family      = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.data());

        memset(&icmp_hdr, 0, sizeof(icmp_hdr));
        icmp_hdr.icmp_type  = ICMP_ECHO;
        icmp_hdr.icmp_code  = 0;
        icmp_hdr.icmp_cksum = 0;
        icmp_hdr.icmp_id    = getpid();
        icmp_hdr.icmp_seq   = 1;

        memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
        icmp_hdr.icmp_cksum = checksum(packet, sizeof(icmp_hdr));
        memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));

        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
        {
            perror("setsockopt");
            close(sockfd);
            return false;
        }

        if (sendto(sockfd, packet, sizeof(icmp_hdr), 0, (struct sockaddr *)&addr, sizeof(addr)) <= 0)
        {
            perror("sendto");
            close(sockfd);
            return false;
        }

        if (recvfrom(sockfd, packet, PACKET_SIZE, 0, (struct sockaddr *)&r_addr, &addr_len) <= 0)
        {
            if (errno == EAGAIN)
            {
                close(sockfd);
                return false;
            }
            else
            {
                perror("recvfrom");
                close(sockfd);
                return false;
            }
        }

        close(sockfd);
        return true;
    }

    NetworkLogger &
    NetworkLogger ::Instance()
    {
        using namespace std;

        if (!_NetworkLoggerInstance)
        {
            lock_guard<mutex> lock(mutex);
            _NetworkLoggerInstance = new NetworkLogger();
        }
        return *_NetworkLoggerInstance;
    }

    void
    NetworkLogger ::init(std::string_view address, int port)
    {
        if (!_NET_DEBUG)
        {
            return;
        }

        if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket");
            return;
        }

        _socket_address.sin_family = AF_INET;
        _socket_address.sin_port   = htons(port);

        if (inet_pton(AF_INET, address.data(), &_socket_address.sin_addr) < 0)
        {
            perror("inet_pton");
            return;
        }

        if (connect(_socket, (struct sockaddr *)&_socket_address, sizeof(_socket_address)) < 0)
        {
            perror("connect");
            return;
        }

        _CONNECTED = true;
    }

    void
    NetworkLogger ::enable()
    {
        _NET_DEBUG = true;
    }

    void
    NetworkLogger ::send_to_server(std::string_view input)
    {
        if (!_NET_DEBUG || !_CONNECTED)
        {
            return;
        }
        if (send(_socket, input.data(), input.length(), 0) < 0)
        {
            _CONNECTED = false;
        }
    }

    void
    NetworkLogger ::log(const char *format, ...)
    {
        if (!_NET_DEBUG || !_CONNECTED)
        {
            return;
        }
        const char   *str;
        unsigned long s_len;
        static char   buf[4096];
        va_list       ap;
        va_start(ap, format);
        vsnprintf(buf, sizeof(buf), format, ap);
        va_end(ap);
        str   = buf;
        s_len = strlen(str);
        if (send(_socket, str, s_len, 0) < 0)
        {
            _CONNECTED = false;
        }
    }

    void
    NetworkLogger ::destroy()
    {
        if (_NetworkLoggerInstance != nullptr)
        {
            delete _NetworkLoggerInstance;
            _NetworkLoggerInstance = nullptr;
        }
    }

    NetworkLogger &
    NetworkLogger ::operator<<(const NetworkLoggerEndl &endl)
    {
        send_to_server(_buffer.str());
        _buffer.str("");
        return Instance();
    }

} // namespace Mlib::Debug
