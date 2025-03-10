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

namespace Mlib::Debug {
  enum LogLevel { INFO, INFO_PRIORITY, WARNING, ERROR, FUNC };

  class FileHandler {
   private:
    std::string m_filename;

   public:
    /* Methods 	   */
    bool append(const std::string &text) {
      /* Open in append mode */
      std::ofstream file(m_filename, std::ios::app);
      if (!file.is_open()) {
        /* Return false if file couldn't be opened. */
        return false;
      }
      file << text; /* Append text */
      file.close();
      return true;
    }

    bool open(void) {
      /* Open in append mode. */
      std::ofstream file(m_filename, std::ios::app);
      bool          isOpen = file.is_open();
      file.close();
      return isOpen;
    }

    /* Constructor */
    explicit FileHandler(std::string filename) : m_filename(std::move(filename)) {}
  };

  class TIME {
   public:
    static const std::string get(void) {
      using namespace std;
      using namespace chrono;

      /* Get the current time */
      const auto &now       = system_clock::now();
      const auto &in_time_t = system_clock::to_time_t(now);

      /* Convert time_t to tm as local time */
      tm buf {};
      localtime_r(&in_time_t, &buf);

      /* Use stringstream to format the time */
      std::ostringstream ss;
      ss << "[" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << "]";
      return ss.str();
    }

    static std::string mili(void) {
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

      // Calculate milliseconds (now time since epoch minus time_t
      // converted back to time since epoch)
      auto since_epoch = now.time_since_epoch();
      auto s           = chrono::duration_cast<chrono::seconds>(since_epoch);
      since_epoch -= s;
      auto ms = chrono::duration_cast<chrono::milliseconds>(since_epoch);

      // Append milliseconds to the formatted string, correctly placing
      // the closing square bracket
      ss << "." << setfill('0') << setw(3) << ms.count() << "]";

      return ss.str();
    }
  };

  typedef struct {
    LogLevel    level;
    std::string function;
    int         line;
    std::string message;
    /* Include a timestamp if you prefer logging it to be handled by the logger rather than each log call */
  } LogMessage;

  typedef struct {
    std::string_view value;
  } ErrnoMsg;

  class LogQueue {
   private:
    std::mutex             mutex_;
    std::queue<LogMessage> queue_;

   public:
    void push(const LogMessage &message) {
      using namespace std;

      lock_guard<mutex> guard(mutex_);
      queue_.push(message);
    }

    bool try_pop(LogMessage &message) {
      using namespace std;

      lock_guard<mutex> guard(mutex_);
      if (queue_.empty()) {
        return false;
      }

      message = queue_.front();
      queue_.pop();
      return true;
    }

    LogMessage retrieve(void) {
      using namespace std;

      lock_guard<mutex> guard(mutex_);
      LogMessage        message = queue_.front();
      queue_.pop();
      return message;
    }
  };

  MAKE_CONSTEXPR_WRAPPER(FuncName, std::string_view);
  MAKE_CONSTEXPR_WRAPPER(FileName, std::string_view);
  MAKE_CONSTEXPR_WRAPPER(Line, Uint);

  const char *const logLevelMap[5] = {
    ESC_CODE_GREEN           "[INFO]" ESC_CODE_RESET,
    ESC_CODE_CYAN   "[INFO_PRIORITY]" ESC_CODE_RESET,
    ESC_CODE_YELLOW       "[WARNING]" ESC_CODE_RESET,
    ESC_CODE_RED            "[ERROR]" ESC_CODE_RESET,
    ESC_CODE_MAGENTA         "[FUNC]" ESC_CODE_RESET 
  };

  class Lout {
   private:
    std::string_view   _output_file;
    LogLevel           _level;
    std::string        _function;
    std::string        _file;
    int                _line;
    std::ostringstream _buffer;
    std::mutex         _log_mutex;

    static Lout *_LoutInstance;

    void _logMessage(void) {
      using namespace std;
      /* Lock the mutex to ensure thread safety. */
      lock_guard<mutex> guard(_log_mutex);
      /* Open the outputfile in append mode. */
      ofstream file(_output_file.data(), ios::app);
      if (file) {
        file << TIME::mili() << ":" << logLevelMap[_level] << ":" << ESC_CODE_YELLOW << "[Line:" << _line << "]"
             << ESC_CODE_RESET << ":" << ESC_CODE_MAGENTA << "[" << _function << "]" << ESC_CODE_RESET << ": "
             << _buffer.str() << "\n";
      }
    }

    static void destroy(void) noexcept;

    Lout(void) noexcept {}

   public:
    Lout &operator<<(const LogLevel logLevel) {
      _level = logLevel;
      return *this;
    }

    Lout &operator<<(const FuncName &funcName) {
      _function = funcName.value;
      return *this;
    }

    Lout &operator<<(const Line &line) {
      _line = line.value;
      return *this;
    }

    Lout &operator<<(const FileName &file_name) {
      _file = file_name.value;
      return *this;
    }

    Lout &operator<<(std::ostream &(*pf)(std::ostream &)) {
      if (pf == static_cast<std::ostream &(*)(std::ostream &)>(std::endl)) {
        _logMessage();
        /* Reset the buffer for new messages */
        _buffer = std::ostringstream();
      }
      return *this;
    }

    Lout &operator<<(char c) {
      if (c == '\n') {
        _logMessage();
        _buffer = std::ostringstream(); // Reset the buffer for new messages
      }
      else {
        _buffer << c;
      }
      return *this;
    }

    Lout &operator<<(const ErrnoMsg &errno_msg) {
      _buffer << errno_msg.value;
      return *this;
    }

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, Lout &>::type operator<<(T value) {
      _buffer << ESC_CODE_YELLOW << value << ESC_CODE_RESET;
      return *this;
    }

    template <typename T>
    typename std::enable_if<!std::is_arithmetic<T>::value, Lout &>::type operator<<(const T &message) {
      _buffer << message;
      return *this;
    }

    static Lout &Instance(void) noexcept;

    static __inline__ Lout *const &instanceptr(void) noexcept {
      if (!_LoutInstance) {
        LOCK_GUARD<MUTEX> lock(MUTEX);
        if (!_LoutInstance) {
          _LoutInstance = new (std::nothrow) Lout();
          if (!_LoutInstance) {
            exit(1);
          }
          atexit(destroy);
        }
      }
      return _LoutInstance;
    }

    constexpr void setOutputFile(std::string_view path) {
      _output_file = path;
    }

    void log(const LogLevel log_level, const char *from_func, const Ulong lineno, const char *format, ...) _NO_THROW;
  };

  inline ErrnoMsg Lout_errno_msg(std::string_view str) {
    std::string s = std::string(str) + ": " + strerror(errno) + " (errno: " + std::to_string(errno) + ")";
    const char *c = &s[0];
    return {c};
  }

  MAKE_CONSTEXPR_WRAPPER(NetworkLoggerEndl, char)

  class NetworkLogger {
   private:
    long         _socket;
    sockaddr_in  _socket_address;
    bool         _CONNECTED = false;
    bool         _NET_DEBUG = false;
    STRINGSTREAM _buffer;
    std::mutex   _mutex;

    static NetworkLogger *_NetworkLoggerInstance;
    Ushort checksum(void *b, int len);
    bool   ping(std::string_view ip);

    NetworkLogger(void) _NO_THROW;
    static void destroy(void) _NO_THROW;

   public:
    void init(std::string_view address, int port);
    void enable();
    void send_to_server(std::string_view input);
    void log(const char *format, ...) _NO_THROW;

    NetworkLogger &operator<<(const NetworkLoggerEndl &endl);

    template <typename T>
    NetworkLogger &operator<<(T value) {
      _buffer << value;
      return Instance();
    }

    static NetworkLogger &Instance(void) noexcept;
    static __inline__ NetworkLogger *const &instanceptr(void) noexcept {
      if (!_NetworkLoggerInstance) {
        LOCK_GUARD<MUTEX> lock(MUTEX);
        if (!_NetworkLoggerInstance) {
          _NetworkLoggerInstance = new (std::nothrow) NetworkLogger();
          if (!_NetworkLoggerInstance) {
            exit(1);
          }
          atexit(destroy);
        }
      }
      return _NetworkLoggerInstance;
    }

    static void static_log(const char *format, ...) {
      char buffer[4096];
      va_list ap;
      va_start(ap, format);
      vsnprintf(buffer, sizeof(buffer), format, ap);
      va_end(ap);
      instanceptr()->log("%s", buffer);
    }
  };
}

/* Macros for logging */
#define FUNC             Mlib::Debug::FuncName_Wrapper(__func__)
#define LINE             Mlib::Debug::Line_Wrapper(__LINE__)
#define FILE_NAME        Mlib::Debug::FileName_Wrapper(__FILENAME__)
#define LOUT             Mlib::Debug::Lout::Instance()
#define LOUTPTR          Mlib::Debug::Lout::instanceptr()
#define LoutI            LOUT << Mlib::Debug::INFO << FUNC << LINE
#define LoutE            LOUT << Mlib::Debug::ERROR << FUNC << LINE
#define LoutErrno(__msg) LoutE << Mlib::Debug::Lout_errno_msg(__msg) << '\n'
#define LOUT_logI(...)   LOUT.log(Mlib::Debug::INFO, __func__, __LINE__, __VA_ARGS__)
#define LOUT_logE(...)   LOUT.log(Mlib::Debug::ERROR, __func__, __LINE__, __VA_ARGS__)
#define LOUT_logW(...)   LOUT.log(Mlib::Debug::WARNING, __func__, __LINE__, __VA_ARGS__)
#define LOUTPTR_logI(...)   LOUTPTR->log(Mlib::Debug::INFO, __func__, __LINE__, __VA_ARGS__)
#define LOUTPTR_logE(...)   LOUTPTR->log(Mlib::Debug::ERROR, __func__, __LINE__, __VA_ARGS__)
#define LOUTPTR_logW(...)   LOUTPTR->log(Mlib::Debug::WARNING, __func__, __LINE__, __VA_ARGS__)
#define logE(...)        LOUTPTR_logE(__VA_ARGS__)
#define logI(...)        LOUTPTR_logI(__VA_ARGS__)
#define logW(...)        LOUTPTR_logW(__VA_ARGS__)

/* Macro to get the NetworkLogger instance */
#define NETLOGGER        Mlib::Debug::NetworkLogger::Instance()
#define NETLOGGERPTR     Mlib::Debug::NetworkLogger::instanceptr()
#define NLOG(...)        NETLOGGER.log(__VA_ARGS__)
#define NETLOG(...)      NETLOGGERPTR->log(__VA_ARGS__)
#define nlog(...)        NLOG(__VA_ARGS__)
#define NETLOG_ENDL      Mlib::Debug::NetworkLoggerEndl_Wrapper('\n')
