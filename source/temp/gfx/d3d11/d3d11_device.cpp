#include "temp/core/define.h"
#ifdef TEMP_GFX_API_D3D11

#include "temp/gfx/d3d11/d3d11_device.h"

namespace temp {
namespace gfx {
namespace d3d11 {
D3d11Device::D3d11Device(const void* window, std::uint32_t window_width,
                         std::uint32_t window_height) {}

D3d11Device::~D3d11Device() {}

std::unique_ptr<SwapChain> D3d11Device::CreateSwapChain(
    const void* window, std::uint32_t width, std::uint32_t height) const {
  return nullptr;
}

}  // namespace d3d11
}  // namespace gfx
}  // namespace temp
#endif