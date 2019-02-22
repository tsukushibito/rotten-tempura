#pragma once

#include <tuple>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace temp {
namespace gfx {
namespace tvk {

vk::UniqueInstance CreateInstance(const std::string& app_name,
                                  const std::string& engine_name,
                                  bool enabled_validation);

vk::DebugReportCallbackEXT SetupDebugging(
    const vk::Instance& instance, const vk::DebugReportFlagsEXT& flags,
    const vk::DispatchLoaderDynamic& dispatcher);

vk::PhysicalDevice PickPhysicalDevices(
    const std::vector<vk::PhysicalDevice>& physical_devices);

vk::UniqueSurfaceKHR CreateWindowSurface(const vk::Instance& instance,
                                         const void* window);

std::tuple<vk::UniqueDevice, int, int, int> CreateLogicalDevice(
    const vk::PhysicalDevice& physical_device,
    const std::vector<vk::QueueFamilyProperties>& queue_family_properties,
    const vk::SurfaceKHR& surface, bool enabled_debug_marker);

}  // namespace tvk
}  // namespace gfx
}  // namespace temp