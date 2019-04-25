#pragma once
#include "temp/base/define.h"
#if defined(TEMP_PLATFORM_MAC)
#include "temp/app/mac/mac_application.h"
#elif defined(TEMP_PLATFORM_WINDOWS)
#include "temp/app/windows/windows_application.h"
#endif
#include "temp/app/utility.h"