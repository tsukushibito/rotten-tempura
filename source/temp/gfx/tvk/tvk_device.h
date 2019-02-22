#pragma once

#include <memory>

#include "temp/gfx/device.h"

namespace temp {
namespace gfx {
namespace tvk {

class Context;

class TvkDevice : public Device {
 public:
  TvkDevice() : TvkDevice(nullptr) {}
  explicit TvkDevice(const void* window);

  TvkDevice(const TvkDevice&) = delete;
  TvkDevice& operator=(const TvkDevice&) = delete;

  TvkDevice(TvkDevice&& other) = default;
  TvkDevice& operator=(TvkDevice&& other) = default;

  ~TvkDevice() = default;

 public:
  ApiType api_type() const override { return ApiType::kVulkan; }

  std::shared_ptr<SwapChain> default_swap_chain() const override {
    return nullptr;
  }

  std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window) const override {
    return nullptr;
  }

 private:
  std::shared_ptr<Context> context_;
};
}  // namespace tvk
}  // namespace gfx
}  // namespace temp
