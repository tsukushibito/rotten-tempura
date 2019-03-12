#include "temp/core/define.h"
#ifdef TEMP_GFX_API_D3D11

#include "temp/gfx/dx11/dx11_device.h"

namespace temp {
namespace gfx {
namespace dx11 {
Dx11Device::Dx11Device(const void* window, std::uint32_t window_width,
                       std::uint32_t window_height) {}

Dx11Device::~Dx11Device() {}

std::unique_ptr<SwapChain> Dx11Device::CreateSwapChain(
    const void* window, std::uint32_t width, std::uint32_t height) const {
  return nullptr;
}

}  // namespace dx11
}  // namespace gfx
}  // namespace temp
#endif