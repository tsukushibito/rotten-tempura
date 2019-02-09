#pragma once

#include <cstring>

#include <iostream>

#include "temp/core/define.h"

#ifdef TEMP_PLATFORM_WINDOWS
#define FILE (strrchr(__FILE__, '¥¥') ? strrchr(__FILE__, '¥¥') + 1 : __FILE__)
#else
#define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define dout (std::cout << FILE << "(" << __LINE__ << "): ")