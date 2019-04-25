#include "temp/base/logger.h"

namespace temp {
Logger::LogLevel Logger::level_ = Logger::LogLevel::kError;
std::mutex Logger::mutex_;
#ifdef TEMP_PLATFORM_WINDOWS
DebugStreamBuf gDebugStreamBuf;
std::ostream dout(&gDebugStreamBuf);
#endif
}  // namespace temp
