#pragma once

#include <functional>
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
  std::vector<vk::Format> GetSupportedDepthFormats() const;

 private:
  vk::DispatchLoaderDynamic dispatcher_;

  vk::UniqueInstance instance_;

  std::vector<vk::PhysicalDevice> physical_devices_;
  vk::PhysicalDevice physical_device_;

  vk::PhysicalDeviceProperties device_properties_;
  vk::PhysicalDeviceFeatures device_features_;
  vk::PhysicalDeviceMemoryProperties device_memory_properties_;

  std::vector<vk::QueueFamilyProperties> queue_family_properties_;
  int graphics_queue_index_ = -1;
  int present_queue_index_ = -1;
  int compute_queue_index_ = -1;

  vk::UniqueDevice device_;

  vk::UniquePipelineCache pipeline_cache_;
  vk::Queue graphics_queue_;
  vk::Queue present_queue_;
  vk::Queue compute_queue_;

  vk::UniqueSurfaceKHR surface_;

  vk::DebugReportCallbackEXT msg_callback_;
};
}  // namespace tvk
}  // namespace gfx
}  // namespace temp
