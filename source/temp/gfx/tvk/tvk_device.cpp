#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <vulkan/vulkan.hpp>

#include "temp/gfx/tvk/tvk_context.h"
#include "temp/gfx/tvk/tvk_device.h"
#include "temp/gfx/tvk/tvk_swap_chain.h"
#include "temp/gfx/tvk/tvk_utility.h"

namespace temp {
namespace gfx {
namespace tvk {

TvkDevice::TvkDevice(const void* window, std::uint32_t window_width,
                     std::uint32_t window_height)
    : context_(std::make_unique<Context>(window)) {
  main_swap_chain_ = CreateSwapChain(window, window_width, window_height);
}

TvkDevice::~TvkDevice() {}

std::unique_ptr<SwapChain> TvkDevice::CreateSwapChain(
    const void* window, std::uint32_t width, std::uint32_t height) const {
  return std::make_unique<TvkSwapChain>(*this, window, width, height);
}

vk::Instance TvkDevice::instance() const { return context_->instance(); }

vk::PhysicalDevice TvkDevice::physical_device() const {
  return context_->physical_device();
}

vk::Device TvkDevice::device() const { return context_->device(); }

int TvkDevice::graphics_queue_index() const {
  auto iter = context_->queue_index_table().find(vk::QueueFlagBits::eGraphics);
  if (iter == context_->queue_index_table().end()) {
    return -1;
  }
  return iter->second;
}
}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif