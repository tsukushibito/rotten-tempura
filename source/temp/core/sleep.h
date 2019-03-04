#pragma once
#include "temp/core/define.h"
#ifdef TEMP_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace temp {
namespace core {

void sleep(int ms);

#ifdef TEMP_PLATFORM_WINDOWS
inline void sleep(int ms) { ::Sleep(ms); }
#else
inline void sleep(int ms) { ::usleep(1000 * ms); }
#endif
}  // namespace core
}  // namespace temp