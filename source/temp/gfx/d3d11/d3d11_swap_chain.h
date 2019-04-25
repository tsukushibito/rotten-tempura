#pragma once
#include <cstdint>
#include <memory>

#include "temp/gfx/device.h"

namespace temp {
namespace gfx {
namespace d3d11 {

class D3d11SwapChain {
 public:
  D3d11SwapChain() {}
  ~D3d11SwapChain() = default;

 public:
  virtual ApiType api_type() const = 0;

  virtual void Present(const Device* device) = 0;

  virtual void Resize(const Device* device, std::uint32_t width,
                      std::uint32_t height) = 0;

  virtual std::uint32_t width() const = 0;
  virtual std::uint32_t height() const = 0;
};
}  // namespace d3d11
}  // namespace gfx
}  // namespace temp
