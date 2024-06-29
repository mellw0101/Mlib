#pragma once

#include <queue>
#include <sys/socket.h>
#include <sys/stat.h>

#include <arpa/inet.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <netinet/ip_icmp.h>
#include <sstream>
#include <thread>
#include <unistd.h>

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

        LogMessage
        retrieve()
        {
            using namespace std;

            lock_guard<mutex> guard(mutex_);
            LogMessage        message = queue_.front();
            queue_.pop();
            return message;
        }

    private:
        std::mutex             mutex_;
        std::queue<LogMessage> queue_;
    };

    MAKE_CONSTEXPR_WRAPPER(FuncName, STRING_VIEW);
    MAKE_CONSTEXPR_WRAPPER(FileName, STRING_VIEW);
    MAKE_CONSTEXPR_WRAPPER(Line, u32);

    constexpr ARRAY<char, 256>
    make_message(STRING_VIEW str)
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

    CONSTEXPR_MAP<STRING_VIEW, u8, 5> logLevelMap = {
        {{ESC_CODE_GREEN "[INFO]" ESC_CODE_RESET, INFO},
         {ESC_CODE_CYAN "[INFO_PRIORITY]" ESC_CODE_RESET, INFO_PRIORITY},
         {ESC_CODE_YELLOW "[WARNING]" ESC_CODE_RESET, WARNING},
         {ESC_CODE_RED "[ERROR]" ESC_CODE_RESET, ERROR},
         {ESC_CODE_MAGENTA "[FUNC]" ESC_CODE_RESET, FUNC}}
    };

    class Lout
    {
    private:
        std::string_view   _output_file;
        LogLevel           _level;
        std::string        _function;
        std::string        _file;
        int                _line;
        std::ostringstream _buffer;
        std::mutex         _log_mutex;

        static Lout *_LoutInstance;

        void
        _logMessage()
        {
            using namespace std;

            //
            //  Lock the mutex to ensure thread safety.
            //
            lock_guard<mutex> guard(_log_mutex);

            //
            //  Open the outputfile in append mode.
            //
            ofstream file(_output_file.data(), ios::app);
            if (file)
            {
                file << TIME::mili() << ":" << logLevelMap[_level].key << ":" << ESC_CODE_YELLOW << "[Line:" << _line
                     << "]" << ESC_CODE_RESET << ":" << ESC_CODE_MAGENTA << "[" << _function << "]" << ESC_CODE_RESET
                     << ": " << _buffer.str() << "\n";
            }
        }


        Lout()
        {}

    public:
        Lout &
        operator<<(LogLevel logLevel)
        {
            _level = logLevel;
            return *this;
        }

        Lout &
        operator<<(const FuncName &funcName)
        {
            _function = funcName.value;
            return *this;
        }

        Lout &
        operator<<(const Line &line)
        {
            _line = line.value;
            return *this;
        }

        Lout &
        operator<<(const FileName &file_name)
        {
            _file = file_name.value;
            return *this;
        }

        Lout &
        operator<<(std::ostream &(*pf)(std::ostream &))
        {
            if (pf == static_cast<std::ostream &(*)(std::ostream &)>(std::endl))
            {
                _logMessage();
                //
                //  Reset the buffer for new messages
                //
                _buffer = std::ostringstream();
            }
            return *this;
        }

        Lout &
        operator<<(s8 c)
        {
            if (c == '\n')
            {
                _logMessage();
                _buffer = std::ostringstream(); // Reset the buffer for new messages
            }
            else
            {
                _buffer << c;
            }
            return *this;
        }

        Lout &
        operator<<(const ErrnoMsg &errno_msg)
        {
            _buffer << errno_msg.value;
            return *this;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, Lout &>::type
        operator<<(T value)
        {
            _buffer << ESC_CODE_YELLOW << value << ESC_CODE_RESET;
            return *this;
        }

        template <typename T>
        typename std::enable_if<!std::is_arithmetic<T>::value, Lout &>::type
        operator<<(const T &message)
        {
            _buffer << message;
            return *this;
        }

        static Lout &Instance();
        void         destroy();

        constexpr void
        setOutputFile(std::string_view path)
        {
            _output_file = path;
        }
    };

    inline ErrnoMsg
    Lout_errno_msg(STRING_VIEW str)
    {
        STRING s = STRING(str) + ": " + ERRNO_C_STR + " (errno: " + ERRNO_CODE_STR + ")";
        C_s8  *c = &s[0];
        return {c};
    }

    MAKE_CONSTEXPR_WRAPPER(NetworkLoggerEndl, s8)

    class NetworkLogger
    {
    private:
        s64         _socket;
        sockaddr_in _socket_address;

        bool _CONNECTED = false;
        bool _NET_DEBUG = false;

        STRINGSTREAM _buffer;

        static NetworkLogger *_NetworkLoggerInstance;
        NetworkLogger();

        u16  checksum(void *b, s32 len);
        bool ping(STRING_VIEW ip);

    public:
        void init(STRING_VIEW address, s32 port);
        void enable();
        void send_to_server(STRING_VIEW input);
        void destroy();

        NetworkLogger &operator<<(const NetworkLoggerEndl &endl);

        template <typename T>
        NetworkLogger &
        operator<<(T value)
        {
            _buffer << value;
            return Instance();
        }

        static NetworkLogger &Instance();
    };

} // namespace Mlib::Debug

//
//  Macros for logging
//
#define FUNC             Mlib::Debug::FuncName_Wrapper(__func__)
#define LINE             Mlib::Debug::Line_Wrapper(__LINE__)
#define FILE_NAME        Mlib::Debug::FileName_Wrapper(__FILENAME__)
#define LOUT             Mlib::Debug::Lout::Instance()
#define LoutI            LOUT << Mlib::Debug::INFO << FUNC << LINE
#define LoutE            LOUT << Mlib::Debug::ERROR << FUNC << LINE
#define LoutErrno(__msg) LoutE << Mlib::Debug::Lout_errno_msg(__msg) << '\n'

//
//  Macro to get the NetworkLogger instance
//
#define NETLOGGER        Mlib::Debug::NetworkLogger::Instance()
#define NETLOG_ENDL      Mlib::Debug::NetworkLoggerEndl_Wrapper('\n')
