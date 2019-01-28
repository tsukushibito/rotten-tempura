#pragma once
#include <iostream>

#include "temp/core/define.h"

#if defined(TEMP_PLATFORM_WINDOWS)
#include <Windows.h>
#include <sstream>
#endif

namespace temp {
inline void ReportAssertion(const char *msg, const char *file,
                            std::size_t line) {
#ifdef TEMP_PLATFORM_WINDOWS
  std::stringstream log;
  log << file << " : line[" << line << "] : " << msg << std::endl;
  OutputDebugStringA(log.str().c_str());
  // DebugBreak();
#else
  std::cout << file << " : line[" << line << "] : " << msg << std::endl;
  __builtin_trap();
#endif
}
}  // namespace temp

#define TEMP_ASSERT(expr, msg) \
  (void)((!!(expr)) || (temp::ReportAssertion(msg, __FILE__, __LINE__), 0))
