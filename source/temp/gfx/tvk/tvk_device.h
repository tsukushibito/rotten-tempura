#pragma once

#include <memory>

#include "temp/gfx/device.h"
#include "temp/gfx/tvk/tvk_context.h"

namespace temp {
namespace gfx {
namespace tvk {

class Context;

class TvkDevice : public Device {
 public:
  TvkDevice() : TvkDevice(nullptr) {}
  explicit TvkDevice(const void* window);

  ~TvkDevice();

  TvkDevice(const TvkDevice&) = delete;
  TvkDevice& operator=(const TvkDevice&) = delete;

  TvkDevice(TvkDevice&& other) = default;
  TvkDevice& operator=(TvkDevice&& other) = default;

 public:
  ApiType api_type() const override { return ApiType::kVulkan; }

  SwapChain& main_swap_chain() const override { return *main_swap_chain_; }

  std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window, std::uint32_t width,
      std::uint32_t height) const override;

  vk::Instance instance() const { return context_->instance(); }
  vk::PhysicalDevice physical_device() const {
    return context_->physical_device();
  }
  vk::Device device() const { return context_->device(); }

  int graphics_queue_index() const {
    auto iter =
        context_->queue_index_table().find(vk::QueueFlagBits::eGraphics);
    if (iter == context_->queue_index_table().end()) {
      return -1;
    }
    return iter->second;
  }

 private:
  std::unique_ptr<Context> context_;
  std::unique_ptr<SwapChain> main_swap_chain_;
};
}  // namespace tvk
}  // namespace gfx
}  // namespace temp
