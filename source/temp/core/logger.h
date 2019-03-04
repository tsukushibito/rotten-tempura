#pragma once

#include <cstring>

#include <iostream>
#include <mutex>
#include <utility>

#include "temp/core/define.h"

#ifdef TEMP_PLATFORM_WINDOWS
#define FILE (strrchr(__FILE__, '¥¥') ? strrchr(__FILE__, '¥¥') + 1 : __FILE__)
#else
#define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define TEMP_COUT (std::cout << FILE << "(" << __LINE__ << "): ")

namespace temp {
namespace core {

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
  static void log__() { std::cout << std::endl; }

  template <class T, class... Args>
  static void log__(T&& arg, Args&&... args) {
    std::cout << arg;
    log__(std::forward<Args>(args)...);
  }

 private:
  static LogLevel level_;
  static std::mutex mutex_;
};

}  // namespace core
}  // namespace temp

#define TEMP_LOG_TRACE(...) \
  temp::core::Logger::trace(FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_DEBUG(...) \
  temp::core::Logger::debug(FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_INFO(...) \
  temp::core::Logger::info(FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_WARNING(...) \
  temp::core::Logger::warning(FILE, "(", __LINE__, "): ", __VA_ARGS__)

#define TEMP_LOG_ERROR(...) \
  temp::core::Logger::error(FILE, "(", __LINE__, "): ", __VA_ARGS__)
