#pragma once

#include <map>
#include <tuple>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace temp {
namespace gfx {
namespace vulkan {

vk::UniqueInstance CreateInstance(const std::string& app_name,
                                  const std::string& engine_name,
                                  bool enabled_validation);

vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
SetupDebugging(const vk::Instance& instance,
               const vk::DebugUtilsMessageSeverityFlagsEXT severity_flags,
               const vk::DebugUtilsMessageTypeFlagsEXT type_flags,
               const vk::DispatchLoaderDynamic& dispatcher);

vk::PhysicalDevice PickPhysicalDevices(
    const std::vector<vk::PhysicalDevice>& physical_devices);

vk::UniqueSurfaceKHR CreateWindowSurface(const vk::Instance& instance,
                                         const void* window);

std::tuple<vk::UniqueDevice, std::map<vk::QueueFlagBits, int>>
CreateLogicalDevice(
    const vk::PhysicalDevice& physical_device,
    const std::vector<vk::QueueFamilyProperties>& queue_family_properties,
    bool enabled_debug_marker);

vk::SwapchainCreateInfoKHR SetupSwapchainCreateInfo(
    vk::PhysicalDevice physical_device, int graphics_queue_index,
    vk::SurfaceKHR surface, const vk::Extent2D& extent,
    vk::SwapchainKHR old_swap_chain);

}  // namespace vulkan
}  // namespace gfx
}  // namespace temp