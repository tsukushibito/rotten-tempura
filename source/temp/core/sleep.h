#pragma once
#include "temp/core/define.h"
#ifdef TEMP_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace temp {
namespace core {

inline void sleep(int ms) {
#ifdef TEMP_PLATFORM_WINDOWS
  ::Sleep(ms);
#else
  ::usleep(1000 * ms);
#endif
}

}  // namespace core
}  // namespace temp