#include "../include/Debug.h"


namespace Mlib::Debug
{
    Lout *Lout::_LoutInstance = nullptr;

    Lout &
    Lout::Instance()
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
    Lout::Destroy()
    {
        if (_LoutInstance != nullptr)
        {
            delete _LoutInstance;
            _LoutInstance = nullptr;
        }
    }

    NetworkLogger *NetworkLogger::_NetworkLoggerInstance = nullptr;

    NetworkLogger *
    NetworkLogger::Instance()
    {
        using namespace std;

        if (_NetworkLoggerInstance == nullptr)
        {
            lock_guard<mutex> lock(mutex);
            _NetworkLoggerInstance = new NetworkLogger();
        }
        return _NetworkLoggerInstance;
    }

    void
    NetworkLogger::init(std::string_view address, s32 port)
    {
        if (_NET_DEBUG == false)
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
    NetworkLogger::enable()
    {
        _NET_DEBUG = true;
    }

    void
    NetworkLogger::send_to_server(std::string_view input)
    {
        if (!_NET_DEBUG || !_CONNECTED)
        {
            return;
        }

        if (send(_socket, input.data(), input.length(), 0) < 0)
        {
            _CONNECTED = false;
            return;
        }
    }

    void
    NetworkLogger::destroy()
    {
        if (_NetworkLoggerInstance != nullptr)
        {
            delete _NetworkLoggerInstance;
            _NetworkLoggerInstance = nullptr;
        }
    }

} // namespace Mlib::Debug
