#pragma once
#include "temp/core/define.h"
#ifdef TEMP_GFX_API_D3D11
#include <d3d11.h>

#include <memory>

#include "temp/gfx/d3d11/d3d11_utility.h"
#include "temp/gfx/device.h"
#include "temp/gfx/swap_chain.h"

namespace temp {
namespace gfx {
namespace d3d11 {

class D3d11SwapChain;

class D3d11Device : public Device {
 public:
  D3d11Device() : D3d11Device(nullptr, 0, 0) {}
  explicit D3d11Device(const void* window, std::uint32_t window_width,
                       std::uint32_t window_height);

  ~D3d11Device();

  D3d11Device(const D3d11Device&) = delete;
  D3d11Device& operator=(const D3d11Device&) = delete;

  D3d11Device(D3d11Device&& other) = default;
  D3d11Device& operator=(D3d11Device&& other) = default;

  ApiType api_type() const override { return ApiType::kD3D11; }

  SwapChain* main_swap_chain() const override;

  std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window, std::uint32_t width,
      std::uint32_t height) const override;

 private:
  D3D_FEATURE_LEVEL feature_level_;
  ComPtr<ID3D11Device> device_;
  ComPtr<ID3D11DeviceContext> context_;
  std::unique_ptr<D3d11SwapChain> main_swap_chain_;
};
}  // namespace d3d11
}  // namespace gfx
}  // namespace temp
#endif