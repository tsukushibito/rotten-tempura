#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <tuple>

#include <vulkan/vulkan.hpp>

namespace temp {
namespace gfx {
namespace vulkan {

class Context {
 public:
  Context(const void* window);

 public:
  const vk::Instance& instance() const { return *instance_; }

  std::vector<vk::Format> GetSupportedDepthFormats() const;

  vk::PhysicalDevice physical_device() const;
  vk::Device device() const;
  const std::map<vk::QueueFlagBits, int>& queue_index_table() const;
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
};
}  // namespace vulkan
}  // namespace gfx
}  // namespace temp
