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

  virtual SwapChain* main_swap_chain() const = 0;

  virtual std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window, std::uint32_t width, std::uint32_t height) const = 0;
};

std::shared_ptr<Device> CreateDevice(ApiType api, const void* window,
                                     std::uint32_t width, std::uint32_t height);

}  // namespace gfx
}  // namespace temp