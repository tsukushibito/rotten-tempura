#pragma once
#include <cstdint>
#include <memory>

#include "temp/gfx/device.h"

namespace temp {
namespace gfx {
class SwapChain {
 protected:
  SwapChain() = default;

 public:
  virtual ~SwapChain() = default;

 public:
  virtual ApiType api_type() const = 0;

  virtual void Present(const Device* device) const = 0;

  virtual void Resize(const Device* device, std::uint32_t width,
                      std::uint32_t height) = 0;
};
}  // namespace gfx
}  // namespace temp