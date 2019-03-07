#pragma once

#if defined(__alpha__) || defined(__ia64__) || defined(__x86_64__) || \
    defined(_WIN64) || defined(__LP64__) || defined(__LLP64__)
#define TEMP_64BIT
#endif

#if defined _WIN32
#define TEMP_PLATFORM_WINDOWS
#ifdef TEMP_EXPORT
#define TEMP_DECLSPEC __declspec(dllexport)
#else
#define TEMP_DECLSPEC __declspec(dllimport)
#define TEMP_GFX_API_VULKAN
#define TEMP_GFX_API_OPENGL
#define TEMP_GFX_API_D3D12
#define TEMP_GFX_API_D3D11
#pragma warning(disable : 4566)
#endif

#elif defined __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IOS
#define TEMP_PLATFORM_IOS
#elif TARGET_OS_MAC
#define TEMP_PLATFORM_MAC
#endif
#define TEMP_DECLSPEC
#define TEMP_GFX_API_VULKAN

#elif defined __gnu_linux__
#define TEMP_PLATFORM_LINUX
#define TEMP_DECLSPEC
#define TEMP_GFX_API_VULKAN
#define TEMP_GFX_API_OPENGL

#elif defined __ANDROID__
#define TEMP_PLATFORM_ANDROID
#define TEMP_DECLSPEC
#define TEMP_GFX_API_VULKAN
#define TEMP_GFX_API_OPENGL

#endif
