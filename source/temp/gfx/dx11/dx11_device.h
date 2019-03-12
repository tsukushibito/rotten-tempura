#pragma once

#include <memory>

#include "temp/gfx/device.h"

namespace temp {
namespace gfx {
namespace dx11 {

class Dx11Device : public Device {
 public:
  Dx11Device() : Dx11Device(nullptr, 0, 0) {}
  explicit Dx11Device(const void* window, std::uint32_t window_width,
                      std::uint32_t window_height);

  ~Dx11Device();

  Dx11Device(const Dx11Device&) = delete;
  Dx11Device& operator=(const Dx11Device&) = delete;

  Dx11Device(Dx11Device&& other) = default;
  Dx11Device& operator=(Dx11Device&& other) = default;

  ApiType api_type() const override { return ApiType::kVulkan; }

  SwapChain* main_swap_chain() const override { return main_swap_chain_.get(); }

  std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window, std::uint32_t width,
      std::uint32_t height) const override;

 private:
  std::unique_ptr<SwapChain> main_swap_chain_;
};
}  // namespace dx11
}  // namespace gfx
}  // namespace temp