#pragma once
#include "temp/base/define.h"
#ifdef TEMP_GFX_API_D3D11

#include <cstdint>
#include <memory>

#include <d3d11.h>

#include "temp/gfx/d3d11/d3d11_device.h"
#include "temp/gfx/d3d11/d3d11_utility.h"

namespace temp {
namespace gfx {
namespace d3d11 {

class D3d11SwapChain : public SwapChain {
 public:
  explicit D3d11SwapChain(const D3d11Device& device, const void* window,
                          std::uint32_t width, std::uint32_t height);
  explicit D3d11SwapChain(const ComPtr<IDXGISwapChain>& swap_chain);

  ~D3d11SwapChain() = default;

  D3d11SwapChain(const D3d11SwapChain&) = delete;
  D3d11SwapChain& operator=(const D3d11SwapChain&) = delete;

  D3d11SwapChain(D3d11SwapChain&&) = default;
  D3d11SwapChain& operator=(D3d11SwapChain&&) = default;

  ApiType api_type() const override { return ApiType::kD3D11; }

  void Present(const Device* device) override;

  void Resize(const Device* device, std::uint32_t width,
              std::uint32_t height) override;

  std::uint32_t width() const override;
  std::uint32_t height() const override;

 private:
  ComPtr<IDXGISwapChain> swap_chain_;
  std::uint32_t width_;
  std::uint32_t height_;
};
}  // namespace d3d11
}  // namespace gfx
}  // namespace temp

#endif