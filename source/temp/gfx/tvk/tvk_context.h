#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <tuple>

#include <vulkan/vulkan.hpp>

namespace temp {
namespace gfx {
namespace tvk {

class Context {
 public:
  Context(const void* window);

 public:
  const vk::Instance& instance() const { return *instance_; }

  std::vector<vk::Format> GetSupportedDepthFormats() const;

  vk::PhysicalDevice physical_device() const { return physical_device_; }

  vk::Device device() const { return *device_; }

  const std::map<vk::QueueFlagBits, int>& queue_index_table() const {
    return queue_index_table_;
  }

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

  vk::UniquePipelineCache pipeline_cache_;
  std::map<vk::QueueFlagBits, vk::Queue> queue_table_;

  vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic> messenger_;
};
}  // namespace tvk
}  // namespace gfx
}  // namespace temp
