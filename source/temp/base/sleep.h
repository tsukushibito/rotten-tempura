#pragma once
#include "temp/base/define.h"
#ifdef TEMP_PLATFORM_WINDOWS
#define NOMINMAX
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace temp {

inline void sleep(int ms) {
#ifdef TEMP_PLATFORM_WINDOWS
  ::Sleep(ms);
#else
  ::usleep(1000 * ms);
#endif
}

}  // namespace temp