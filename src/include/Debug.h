#pragma once

#include <queue>
#include <sys/socket.h>
#include <sys/stat.h>

#include <Imlib2.h>
#include <X11/X.h>
#include <X11/Xauth.h>
#include <X11/keysym.h>
#include <algorithm> // For std::remove_if
#include <arpa/inet.h>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <dbus-c++-1/dbus-c++/dbus.h>
#include <dirent.h>
#include <exception>
#include <fcntl.h>
#include <features.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <ifaddrs.h>
#include <iostream>
#include <iterator>
#include <iwlib.h>
#include <map>
#include <mutex> // Added for thread safety
#include <netdb.h>
#include <netinet/in.h>
#include <png.h>
#include <pulse/pulseaudio.h>
#include <ratio>
#include <regex>
#include <spawn.h>
#include <sstream>
#include <stdbool.h>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/cdefs.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h> // For waitpid()
#include <sys/wait.h>
#include <thread>
#include <type_traits>
#include <unistd.h>
#include <unistd.h> // For fork() and exec()
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <vector>
#include <xcb/randr.h>
#include <xcb/render.h>
#include <xcb/xcb.h>
#include <xcb/xcb_cursor.h> // For cursor
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_xrm.h>
#include <xcb/xinput.h>
#include <xcb/xproto.h>

extern bool NET_DEBUG;

#include "constexpr.hpp"
#include "def.h"

namespace Mlib::Debug
{
    enum LogLevel
    {
        INFO,
        INFO_PRIORITY,
        WARNING,
        ERROR,
        FUNC
    };

    class FileHandler
    {
    private:
        std::string m_filename;

    public:
        /* Methods 	   */
        bool
        append(const std::string &text)
        {
            std::ofstream file(m_filename, std::ios::app); // Open in append mode

            if (!file.is_open())
            {
                return false; // Return false if file couldn't be opened
            }
            file << text;     // Append text
            file.close();
            return true;
        }

        bool
        open()
        {
            std::ofstream file(m_filename, std::ios::app); // Open in append mode
            bool          isOpen = file.is_open();
            file.close();
            return isOpen;
        }

        /* Constructor */
        explicit FileHandler(std::string filename)
            : m_filename(std::move(filename))
        {}
    };

    class TIME
    {
    public:
        static const std::string
        get()
        {
            using namespace std;
            using namespace chrono;

            // Get the current time
            const auto &now       = system_clock::now();
            const auto &in_time_t = system_clock::to_time_t(now);

            // Convert time_t to tm as local time
            tm buf {};
            localtime_r(&in_time_t, &buf);

            // Use stringstream to format the time
            std::ostringstream ss;
            ss << "[" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << "]";
            return ss.str();
        }

        static std::string
        mili()
        {
            using namespace std;
            using namespace chrono;

            // Get the current time point
            auto now = chrono::system_clock::now();

            // Convert to time_t for seconds and tm for local time
            auto in_time_t = chrono::system_clock::to_time_t(now);
            tm   buf {};
            localtime_r(&in_time_t, &buf);

            // Use stringstream to format the time
            std::ostringstream ss;
            ss << "[" << put_time(&buf, "%Y-%m-%d %H:%M:%S");

            // Calculate milliseconds (now time since epoch minus time_t converted back to time since epoch)
            auto since_epoch = now.time_since_epoch();
            auto s           = chrono::duration_cast<chrono::seconds>(since_epoch);
            since_epoch -= s;
            auto ms = chrono::duration_cast<chrono::milliseconds>(since_epoch);

            // Append milliseconds to the formatted string, correctly placing the closing square bracket
            ss << "." << setfill('0') << setw(3) << ms.count() << "]";

            return ss.str();
        }
    };


    typedef struct
    {
        LogLevel    level;
        std::string function;
        int         line;
        std::string message;
        // Include a timestamp if you prefer logging it to be handled by the logger rather than each log call
    } LogMessage;

    typedef struct
    {
        std::string_view value;
    } ErrnoMsg;

    class LogQueue
    {
    public:
        void
        push(const LogMessage &message)
        {
            using namespace std;

            lock_guard<mutex> guard(mutex_);
            queue_.push(message);
        }

        bool
        try_pop(LogMessage &message)
        {
            using namespace std;

            lock_guard<mutex> guard(mutex_);
            if (queue_.empty())
            {
                return false;
            }

            message = queue_.front();
            queue_.pop();
            return true;
        }

    private:
        std::mutex             mutex_;
        std::queue<LogMessage> queue_;
    };

    MAKE_CONSTEXPR_WRAPPER(FuncName, std::string_view);
    MAKE_CONSTEXPR_WRAPPER(FileName, std::string_view);
    MAKE_CONSTEXPR_WRAPPER(Line, u32);

    constexpr std::array<char, 256>
    make_message(std::string_view str)
    {
        std::array<char, 256> buffer     = {};
        auto                  str_len    = str.size();
        const char           *prefix     = ": error message (errno: 0)";
        auto                  prefix_len = std::char_traits<char>::length(prefix);

        if (str_len + prefix_len >= buffer.size())
        {
            // handle overflow error if needed
            // for now, we truncate the message to fit the buffer
            str_len = buffer.size() - prefix_len - 1;
        }

        for (size_t i = 0; i < str_len; ++i)
        {
            buffer[i] = str[i];
        }

        for (size_t i = 0; i < prefix_len; ++i)
        {
            buffer[str_len + i] = prefix[i];
        }

        return buffer;
    }

    CONSTEXPR_STRBITMAP<5> logLevelMap = {
        {{ESC_CODE_GREEN "[INFO]" ESC_CODE_RESET, INFO},
         {ESC_CODE_CYAN "[INFO_PRIORITY]" ESC_CODE_RESET, INFO_PRIORITY},
         {ESC_CODE_YELLOW "[WARNING]" ESC_CODE_RESET, WARNING},
         {ESC_CODE_RED "[ERROR]" ESC_CODE_RESET, ERROR},
         {ESC_CODE_MAGENTA "[FUNC]" ESC_CODE_RESET, FUNC}}
    };

    class Lout
    {
    private:
        std::string_view   output_file;
        LogLevel           currentLevel;
        std::string        currentFunction;
        std::string        current_file;
        int                current_line;
        std::ostringstream buffer;
        std::mutex         log_mutex;

        static Lout *LoutInstance;

        static constexpr std::string_view
        logPrefix(LogLevel level)
        {
            return logLevelMap[level].key;
        }

        void
        logMessage()
        {
            using namespace std;

            lock_guard<mutex> guard(log_mutex);

            ofstream file(output_file.data(), ios::app); // Append mode
            if (file)
            {
                file << TIME::mili() << ":" << logPrefix(currentLevel) << ":" << ESC_CODE_YELLOW
                     << "[Line:" << current_line << "]" << ESC_CODE_RESET << ":" << ESC_CODE_MAGENTA << "["
                     << currentFunction << "]" << ESC_CODE_RESET << ": " << buffer.str() << "\n";
            }
        }


        Lout()
        {}

    public:
        Lout &
        operator<<(LogLevel logLevel)
        {
            currentLevel = logLevel;
            return *this;
        }

        Lout &
        operator<<(const FuncName &funcName)
        {
            currentFunction = funcName.value;
            return *this;
        }

        Lout &
        operator<<(const Line &line)
        {
            current_line = line.value;
            return *this;
        }

        Lout &
        operator<<(const FileName &file_name)
        {
            current_file = file_name.value;
            return *this;
        }

        Lout &
        operator<<(std::ostream &(*pf)(std::ostream &))
        {
            if (pf == static_cast<std::ostream &(*)(std::ostream &)>(std::endl))
            {
                logMessage();
                //
                //  Reset the buffer for new messages
                //
                buffer = std::ostringstream();
            }
            return *this;
        }

        Lout &
        operator<<(s8 c)
        {
            if (c == '\n')
            {
                logMessage();
                buffer = std::ostringstream(); // Reset the buffer for new messages
            }
            else
            {
                buffer << c;
            }
            return *this;
        }

        Lout &
        operator<<(const ErrnoMsg &errno_msg)
        {
            buffer << errno_msg.value;
            return *this;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, Lout &>::type
        operator<<(T value)
        {
            buffer << ESC_CODE_YELLOW << value << ESC_CODE_RESET;
            return *this;
        }

        template <typename T>
        typename std::enable_if<!std::is_arithmetic<T>::value, Lout &>::type
        operator<<(const T &message)
        {
            buffer << message;
            return *this;
        }

        static Lout &Instance();
        void         Destroy();

        constexpr void
        setOutputFile(std::string_view path)
        {
            output_file = path;
        }
    };

    inline ErrnoMsg
    Lout_errno_msg(std::string_view str)
    {
        std::string s = std::string(str) + ": " + std::strerror(errno) + " (errno: " + std::to_string(errno) + ")";
        const char *c = &s[0];
        return {c};
    }

    class NetworkLogger
    {
    private:
        long             _socket;
        int              _connected;
        std::string_view _address;
        u32              _port;
        sockaddr_in      _sock_addr;

        NetworkLogger()
            : _connected(false)
        {}

    public:
        // Methods.
        void
        init(std::string_view address, s32 port)
        {
            if (NET_DEBUG == false)
            {
                return;
            }

            if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            {
                perror("socket");
                return;
            }

            _sock_addr.sin_family = AF_INET;
            if (address == _address)
            {
                _sock_addr.sin_port = htons(_port);
            }
            else
            {
                _sock_addr.sin_port = htons(port);
            }

            if (inet_pton(AF_INET, address.data(), &_sock_addr.sin_addr) < 0)
            {
                perror("inet_pton");
                return;
            }

            if (connect(_socket, (struct sockaddr *)&_sock_addr, sizeof(_sock_addr)) < 0)
            {
                perror("connect");
                return;
            }

            _connected = true;
        }

        void
        send_to_server(std::string_view input)
        {
            if (NET_DEBUG == false)
            {
                return;
            }

            if (!_connected)
            {
                return;
            }

            if (send(_socket, input.data(), input.length(), 0) < 0)
            {
                _connected = false;
                return;
            }

            char s_char('\0');
            if (send(_socket, &s_char, 1, 0) < 0)
            {
                _connected = false;
                return;
            }
        }
    };

} // namespace Mlib::Debug

#define LOUT             Mlib::Debug::Lout::Instance()

#define FUNC             Mlib::Debug::FuncName_Wrapper(__func__)
#define LINE             Mlib::Debug::Line_Wrapper(__LINE__)
#define FILE_NAME        Mlib::Debug::FileName_Wrapper(__FILENAME__)

#define LoutI            LOUT << Mlib::Debug::INFO << FUNC << LINE
#define LoutE            LOUT << Mlib::Debug::ERROR << FUNC << LINE
#define LoutErrno(__msg) LoutE << Mlib::Debug::Lout_errno_msg(__msg) << '\n'

// #define ESP_SERVER           "192.168.0.29"
// #define ESP_PORT             23
// #define NET_LOG_MSG(message) "\033[33m" + message + "\033[0m"
// #define FUNC_NAME_STR        string(__func__)
// #define NET_LOG_FUNCTION     "\033[34m(FUNCTION) " + FUNC_NAME_STR + "\033[0m"
// #define NET_LOG_WINDOW(window_name, window)                                                                      \
//     "\033[35m(FUNCTION) " + FUNC_NAME_STR + ":\033[34m (WINDOW_NAME) " + window_name + ":\033[32m (uint32_t) " + \
//         to_string(window) + "\033[0m"
// #define WINDOW(window) to_string(window)
// #define CLASS_NAME_RAW typeid(*this).name()
// #define CLASS_NAME_STR string(CLASS_NAME_RAW)
// /**
//  *
//  * @brief fetches the calling class's name and extracts a substr containing only
//  * the class name
//  *
//  * NOTE: this only works for class's with prefix '__'
//  *
//  */
// #define CLASS_NAME     CLASS_NAME_STR.substr(CLASS_NAME_STR.find("__"))
// #define NET_LOG_CLASS  "\033[35m(CLASS) " + CLASS_NAME + "\033[0m"
// // #define NET_LOG(__type) net_logger->send_to_server(__type)
// #ifdef NET_LOG_ENABLED
// #    define NET_LOG(__type) net_logger->send_to_server(__type)
// #else
// #    define NET_LOG(__type) (void)0
// #endif
// #ifdef NET_LOG_ENABLED
// #    define INIT_NET_LOG(__address)      \
//         net_logger = new __net_logger__; \
//         net_logger->init(__address);
// #else
// #    define INIT_NET_LOG(__address) (void)0
// #endif
// #define NET_LOG_CLASS_FUNCTION_START() \
//     NET_LOG(NET_LOG_CLASS + " -> " + NET_LOG_FUNCTION + " -> " + NET_LOG_MSG("Starting"))
// #define NET_LOG_CLASS_FUNCTION_DONE() \
//     NET_LOG(NET_LOG_CLASS + " -> " + NET_LOG_FUNCTION + " -> " + NET_LOG_MSG("Done!!!"))
// #define loutNUM(__variable)           "(\033[33m" << __variable << "\033[0m)"
// #define loutCFUNC(__calling_function) "calling_function(\033[35m" << __calling_function << "\033[0m)"
// #define lout_error_code(__error_code) "\033[31merror_code\033[0m" << __error_code
// #define loutCEcode(__error_code)      lout_error_code(__error_code) << " " << loutCFUNC(__calling_function)
// #define loutPath(__path)              "(\033[32m" << __path << "\033[0m)"
// #define loutUser(__user)              "(" << log_BLUE << __user << log_RESET << ")"
// #define loutEND                       '\n'
// #define Var_(_Var)                    #_Var << ' ' << _Var
