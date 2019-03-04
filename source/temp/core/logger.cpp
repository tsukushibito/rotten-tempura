#include "temp/core/logger.h"

namespace temp {
namespace core {
Logger::LogLevel Logger::level_ = Logger::LogLevel::kError;
std::mutex Logger::mutex_;
}  // namespace core
}  // namespace temp
