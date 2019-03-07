#include "temp/core/logger.h"

namespace temp {
namespace core {
Logger::LogLevel Logger::level_ = Logger::LogLevel::kError;
std::mutex Logger::mutex_;
#ifdef TEMP_PLATFORM_WINDOWS
DebugStreamBuf gDebugStreamBuf;
std::ostream dout = std::ostream(&gDebugStreamBuf);
#endif
}  // namespace core
}  // namespace temp
