#pragma once

#include <functional>
#include <mutex>
#include <string>

#include <vulkan/vulkan.hpp>

namespace temp {
namespace gfx {
namespace tvk {

class Context {
 public:
  Context(const void* window);

 private:
  vk::UniqueInstance CreateInstance(const std::string& app_name,
                                    const std::string& engine_name,
                                    bool enabled_validation);

  void SetupDebugging(const vk::Instance& instance,
                      const vk::DebugReportFlagsEXT& flags);

  vk::UniqueSurfaceKHR CreateWindowSurface(const vk::UniqueInstance& instance,
                                           const void* window);

 private:
  vk::DispatchLoaderDynamic dispatcher_;

  vk::UniqueInstance instance_;

  std::vector<vk::PhysicalDevice> physical_devices_;
  vk::PhysicalDevice physical_device_;

  std::vector<vk::QueueFamilyProperties> queue_family_properties_;

  vk::PhysicalDeviceProperties device_properties_;

  vk::PhysicalDeviceFeatures device_features_;
  vk::PhysicalDeviceFeatures2 enabled_features2_;
  vk::PhysicalDeviceFeatures& enabled_features_ = enabled_features2_.features;

  vk::PhysicalDeviceMemoryProperties device_memory_properties_;

  vk::Device device_;

  vk::PipelineCache pipeline_cache_;

  vk::UniqueSurfaceKHR surface_;

  std::once_flag dispatcher_init_flag_;
  vk::DebugReportCallbackEXT msg_callback_;
};
}  // namespace tvk
}  // namespace gfx
}  // namespace temp
