#pragma once
#include "temp/base/define.h"

namespace temp {
namespace gfx {
enum class ApiType {
#ifdef TEMP_GFX_API_VULKAN
  kVulkan,
#endif
#ifdef TEMP_GFX_API_OPENGL
  kOpenGL,
#endif
#ifdef TEMP_GFX_API_D3D12
  kD3D12,
#endif
#ifdef TEMP_GFX_API_D3D11
  kD3D11,
#endif
};
}  // namespace gfx
}  // namespace temp