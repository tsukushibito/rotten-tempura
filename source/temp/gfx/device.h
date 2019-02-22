#pragma once
#include <memory>

#include "temp/gfx/api_type.h"

namespace temp {
namespace gfx {

class SwapChain;

class Device {
 protected:
  Device() = default;

 public:
  virtual ~Device() = default;

 public:
  virtual ApiType api_type() const = 0;

  virtual std::shared_ptr<SwapChain> default_swap_chain() const = 0;

  virtual std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window) const = 0;
};

std::unique_ptr<Device> CreateDevice(ApiType api, const void* window);

}  // namespace gfx
}  // namespace temp