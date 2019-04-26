#include "temp/base/define.h"
#ifdef TEMP_GFX_API_D3D11
#include <ios>

#include "temp/base/logger.h"

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
  ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
  swap_chain_desc.BufferDesc.Width = (UINT)window_width;
  swap_chain_desc.BufferDesc.Width = (UINT)window_height;
  swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
  swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
  swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swap_chain_desc.BufferDesc.ScanlineOrdering =
      DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  swap_chain_desc.SampleDesc.Count = 1;
  swap_chain_desc.SampleDesc.Quality = 0;
  swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swap_chain_desc.BufferCount = 3;
  swap_chain_desc.OutputWindow = (HWND)window;
  swap_chain_desc.Windowed = true;
  swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  ComPtr<IDXGISwapChain> swap_chain;

  auto result = D3D11CreateDeviceAndSwapChain(
      NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creation_flags, feature_levels,
      feature_level_count, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain,
      &device_, &feature_level_, &context_);

  if (result != S_OK) {
    TEMP_LOG_ERROR("D3D11CreateDeviceAndSwapChain failed. error: 0x", std::hex,
                   result);
  }
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