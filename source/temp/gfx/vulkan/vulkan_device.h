#pragma once

#include <memory>
#include <vector>
#include <map>

#include <vulkan/vulkan.hpp>

#include "temp/gfx/device.h"

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

  std::vector<vk::Format> GetSupportedDepthFormats() const;

  std::unique_ptr<SwapChain> CreateSwapChain(
      const void* window, std::uint32_t width,
      std::uint32_t height) const override;

  const vk::DispatchLoaderDynamic& dispatcher() const;
  vk::Instance instance() const;
  vk::PhysicalDevice physical_device() const;
  vk::Device device() const;
  const std::map<vk::QueueFlagBits, int>& queue_index_table() const;
  int graphics_queue_index() const;
  const std::map<vk::QueueFlagBits, vk::Queue>& queue_table() const;

 private:
  vk::DispatchLoaderDynamic dispatcher_;

  vk::UniqueInstance instance_;

  std::vector<vk::PhysicalDevice> physical_devices_;
  vk::PhysicalDevice physical_device_;

  vk::PhysicalDeviceProperties device_properties_;
  vk::PhysicalDeviceFeatures device_features_;
  vk::PhysicalDeviceMemoryProperties device_memory_properties_;

  std::vector<vk::QueueFamilyProperties> queue_family_properties_;
  std::map<vk::QueueFlagBits, int> queue_index_table_;

  vk::UniqueDevice device_;

  std::map<vk::QueueFlagBits, vk::Queue> queue_table_;

  vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
      messenger_;

  std::unique_ptr<SwapChain> main_swap_chain_;
};
}  // namespace vulkan
}  // namespace gfx
}  // namespace temp
