#pragma once

#include <memory>

#include "temp/gfx/device.h"
#include "temp/gfx/vulkan/vulkan_context.h"

namespace temp {
namespace gfx {
namespace vulkan {

class VulkanDevice : public Device {
 public:
  VulkanDevice() : VulkanDevice(nullptr, 0, 0) {}
  explicit VulkanDevice(const void* window, std::uint32_t window_width,
                        std::uint32_t window_height);

  ~VulkanDevice();

  VulkanDevice(const VulkanDevice&) = delete;
  VulkanDevice& operator=(const VulkanDevice&) = delete;

  VulkanDevice(VulkanDevice&& other) = default;
  VulkanDevice& operator=(VulkanDevice&& other) = default;

  ApiType api_type() const override { return ApiType::kVulkan; }

  SwapChain* main_swap_chain() const override { return main_swap_chain_.get(); }

  std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window, std::uint32_t width,
      std::uint32_t height) const override;

  vk::Instance instance() const;
  vk::PhysicalDevice physical_device() const;
  vk::Device device() const;
  int graphics_queue_index() const;

  const Context& context() const { return *context_; }

 private:
  std::unique_ptr<Context> context_;
  std::unique_ptr<SwapChain> main_swap_chain_;
};
}  // namespace vulkan
}  // namespace gfx
}  // namespace temp
