#pragma once
#include "temp/core/define.h"
#ifdef TEMP_GFX_API_D3D11
#include <d3d11.h>
#include <wrl/client.h>

#include <memory>

#include "temp/gfx/device.h"
#include "temp/gfx/swap_chain.h"

namespace temp {
namespace gfx {
namespace d3d11 {

class D3d11Device : public Device {
 public:
  template <class T>
  using ComPtr = Microsoft::WRL::ComPtr<T>;

  D3d11Device() : D3d11Device(nullptr, 0, 0) {}
  explicit D3d11Device(const void* window, std::uint32_t window_width,
                       std::uint32_t window_height);

  ~D3d11Device();

  D3d11Device(const D3d11Device&) = delete;
  D3d11Device& operator=(const D3d11Device&) = delete;

  D3d11Device(D3d11Device&& other) = default;
  D3d11Device& operator=(D3d11Device&& other) = default;

  ApiType api_type() const override { return ApiType::kD3D11; }

  SwapChain* main_swap_chain() const override { return main_swap_chain_.get(); }

  std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window, std::uint32_t width,
      std::uint32_t height) const override;

 private:
  ComPtr<ID3D11Device> device_;
  std::unique_ptr<SwapChain> main_swap_chain_;
};
}  // namespace d3d11
}  // namespace gfx
}  // namespace temp
#endif