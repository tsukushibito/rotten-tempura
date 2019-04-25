#pragma once

#include <cstring>

#include <iostream>
#include <mutex>
#include <streambuf>
#include <utility>

#include "temp/base/define.h"

#ifdef TEMP_PLATFORM_WINDOWS
#define NOMINMAX
#include <Windows.h>
#include <sstream>
#define TEMP_FILE \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define TEMP_FILE \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define TEMP_COUT (std::cout << TEMP_FILE << "(" << __LINE__ << "): ")

namespace temp {

#ifdef TEMP_PLATFORM_WINDOWS
class DebugStreamBuf : public std::stringbuf {
 public:
  ~DebugStreamBuf() { sync(); }

  int sync() override {
    OutputDebugStringA(str().c_str());
    str("");
    return 0;
  }
};

extern std::ostream dout;
#endif

class Logger {
 public:
  enum class LogLevel {
    kTrace,
    kDebug,
    kInfo,
    kWarning,
    kError,
  };

 public:
  static void setLogLevel(LogLevel level) { level_ = level; }

  static LogLevel getLogLevel() { return level_; }

  template <class... Args>
  static void trace(Args&&... args) {
    if (getLogLevel() >= LogLevel::kTrace) {
      std::unique_lock<std::mutex> lock(mutex_);
      log__("[trace]", std::forward<Args>(args)...);
    }
  }

  template <class... Args>
  static void debug(Args&&... args) {
    if (getLogLevel() >= LogLevel::kDebug) {
      std::unique_lock<std::mutex> lock(mutex_);
      log__("[debug]", std::forward<Args>(args)...);
    }
  }

  template <class... Args>
  static void info(Args&&... args) {
    if (getLogLevel() >= LogLevel::kInfo) {
      std::unique_lock<std::mutex> lock(mutex_);
      log__("[info]", std::forward<Args>(args)...);
    }
  }

  template <class... Args>
  static void warning(Args&&... args) {
    if (getLogLevel() >= LogLevel::kWarning) {
      std::unique_lock<std::mutex> lock(mutex_);
      log__("[warning]", std::forward<Args>(args)...);
    }
  }

  template <class... Args>
  static void error(Args&&... args) {
    if (getLogLevel() >= LogLevel::kError) {
      std::unique_lock<std::mutex> lock(mutex_);
      log__("[error]", std::forward<Args>(args)...);
    }
  }

 private:
  static void log__() {
    std::cout << std::endl;
#ifdef TEMP_PLATFORM_WINDOWS
    dout << std::endl;
#endif
  }

  template <class T, class... Args>
  static void log__(T&& arg, Args&&... args) {
    std::cout << arg;
#ifdef TEMP_PLATFORM_WINDOWS
    dout << arg;
#endif
    log__(std::forward<Args>(args)...);
  }

 private:
  static LogLevel level_;
  static std::mutex mutex_;
};

}  // namespace temp

#define TEMP_LOG_TRACE(...) \
  temp::Logger::trace(TEMP_FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_DEBUG(...) \
  temp::Logger::debug(TEMP_FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_INFO(...) \
  temp::Logger::info(TEMP_FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_WARNING(...) \
  temp::Logger::warning(TEMP_FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_ERROR(...) \
  temp::Logger::error(TEMP_FILE, "(", __LINE__, "): ", __VA_ARGS__)
