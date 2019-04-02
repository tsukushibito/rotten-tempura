#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <vulkan/vulkan.hpp>

#include "temp/gfx/vulkan/vulkan_context.h"
#include "temp/gfx/vulkan/vulkan_device.h"
#include "temp/gfx/vulkan/vulkan_swap_chain.h"
#include "temp/gfx/vulkan/vulkan_utility.h"

namespace temp {
namespace gfx {
namespace vulkan {

VulkanDevice::VulkanDevice(const void* window, std::uint32_t window_width,
                           std::uint32_t window_height)
    : context_(std::make_unique<Context>(window)) {
  main_swap_chain_ = CreateSwapChain(window, window_width, window_height);
}

VulkanDevice::~VulkanDevice() {}

std::unique_ptr<SwapChain> VulkanDevice::CreateSwapChain(
    const void* window, std::uint32_t width, std::uint32_t height) const {
  return std::make_unique<VulkanSwapChain>(*this, window, width, height);
}

vk::Instance VulkanDevice::instance() const { return context_->instance(); }

vk::PhysicalDevice VulkanDevice::physical_device() const {
  return context_->physical_device();
}

vk::Device VulkanDevice::device() const { return context_->device(); }

int VulkanDevice::graphics_queue_index() const {
  auto iter = context_->queue_index_table().find(vk::QueueFlagBits::eGraphics);
  if (iter == context_->queue_index_table().end()) {
    return -1;
  }
  return iter->second;
}
}  // namespace vulkan
}  // namespace gfx
}  // namespace temp

#endif