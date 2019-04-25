#include "temp/base/define.h"
#ifdef TEMP_GFX_API_D3D11

#include "temp/gfx/d3d11/d3d11_device.h"
#include "temp/gfx/d3d11/d3d11_swap_chain.h"

namespace temp {
namespace gfx {
namespace d3d11 {
D3d11Device::D3d11Device(const void* window, std::uint32_t window_width,
                         std::uint32_t window_height) {
  // UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  UINT creation_flags = 0;
#if defined(_DEBUG)
  creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  D3D_FEATURE_LEVEL feature_levels[] = {
      D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3,  D3D_FEATURE_LEVEL_9_2,  D3D_FEATURE_LEVEL_9_1,
  };
  UINT feature_level_count = sizeof(feature_levels) / sizeof(feature_levels[0]);

  DXGI_SWAP_CHAIN_DESC swap_chain_desc;
  ComPtr<IDXGISwapChain> swap_chain;

  D3D11CreateDeviceAndSwapChain(
	  NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creation_flags, feature_levels, feature_level_count,
	  D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device_, &feature_level_, &context_);
}

D3d11Device::~D3d11Device() {}

SwapChain* D3d11Device::main_swap_chain() const {
	return main_swap_chain_.get();
}

std::unique_ptr<SwapChain> D3d11Device::CreateSwapChain(
    const void* window, std::uint32_t width, std::uint32_t height) const {
  return nullptr;
}

}  // namespace d3d11
}  // namespace gfx
}  // namespace temp
#endif