#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "temp/core/assertion.h"
#include "temp/core/logger.h"

#include "temp/gfx/vulkan/vulkan_utility.h"

namespace temp {
namespace gfx {
namespace vulkan {

namespace {

const uint32_t kApiVersion = VK_API_VERSION_1_0;

#if !defined(__ANDROID__)
const std::vector<const char*> kValidationLayerNames = {
    "VK_LAYER_LUNARG_assistant_layer", "VK_LAYER_LUNARG_standard_validation"};
#else
const std::vector<const char*> kValidationLayerNames = {
    "VK_LAYER_GOOGLE_threading",      "VK_LAYER_LUNARG_parameter_validation",
    "VK_LAYER_LUNARG_object_tracker", "VK_LAYER_LUNARG_core_validation",
    "VK_LAYER_LUNARG_swapchain",      "VK_LAYER_GOOGLE_unique_objects"};
#endif

std::set<std::string> GetAvailableLayers() {
  std::set<std::string> result;
  auto layers = vk::enumerateInstanceLayerProperties();
  for (auto layer : layers) {
    result.insert(layer.layerName);
  }
  return result;
}

std::vector<const char*> FilterLayers(
    const std::vector<const char*>& desired_layers) {
  static std::set<std::string> valid_layer_names = GetAvailableLayers();
  std::vector<const char*> result;
  for (const auto& string : desired_layers) {
    if (valid_layer_names.count(string) != 0) {
      result.push_back(string);
    }
  }
  return result;
}

VkBool32 MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                         VkDebugUtilsMessageTypeFlagsEXT type,
                         const VkDebugUtilsMessengerCallbackDataEXT* data,
                         void* pUserData) {
  (void*)pUserData;

  std::stringstream ss;

  std::string prefix;
  if (type & VkDebugUtilsMessageTypeFlagBitsEXT::
                 VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
    prefix += "[GENERAL]";
  }
  if (type & VkDebugUtilsMessageTypeFlagBitsEXT::
                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
    prefix += "[PERFORMANCE]";
  }
  if (type & VkDebugUtilsMessageTypeFlagBitsEXT::
                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
    prefix += "[VALIDATION]";
  }

  ss << prefix << "message: " << data->pMessage;

  switch (severity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
      TEMP_LOG_ERROR(ss.str());
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
      TEMP_LOG_WARNING(ss.str());
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
      TEMP_LOG_INFO(ss.str());
      break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
      TEMP_LOG_TRACE(ss.str());
      break;
    default:
      TEMP_ASSERT(false, "severity must be error, warning, info or verbose.");
      return false;
  }

  return false;
}

}  // namespace

vk::UniqueInstance CreateInstance(const std::string& app_name,
                                  const std::string& engine_name,
                                  bool enabled_validation) {
  vk::ApplicationInfo appInfo = {};
  appInfo.pApplicationName = app_name.c_str();
  appInfo.pEngineName = engine_name.c_str();
  appInfo.apiVersion = kApiVersion;

  std::vector<const char*> instanceExtensions = {VK_KHR_SURFACE_EXTENSION_NAME};

  // Enable surface extensions depending on os
#if defined(_WIN32)
  instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
  instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
  instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
  instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
  instanceExtensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
  instanceExtensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#endif

  vk::InstanceCreateInfo instanceCreateInfo = {};
  instanceCreateInfo.pNext = NULL;
  instanceCreateInfo.pApplicationInfo = &appInfo;
  if (instanceExtensions.size() > 0) {
    if (enabled_validation) {
      instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    instanceCreateInfo.enabledExtensionCount =
        (uint32_t)instanceExtensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
  }
  std::vector<const char*> layers;
  if (enabled_validation) {
    layers = FilterLayers(kValidationLayerNames);
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    instanceCreateInfo.ppEnabledLayerNames = layers.data();
  }

  return vk::createInstanceUnique(instanceCreateInfo, nullptr);
}

vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
SetupDebugging(const vk::Instance& instance,
               const vk::DebugUtilsMessageSeverityFlagsEXT severity_flags,
               const vk::DebugUtilsMessageTypeFlagsEXT type_flags,
               const vk::DispatchLoaderDynamic& dispatcher) {
  vk::DebugUtilsMessengerCreateInfoEXT messenger_ci;
  messenger_ci.messageSeverity = severity_flags;
  messenger_ci.messageType = type_flags;
  messenger_ci.pfnUserCallback = MessageCallback;
  return instance.createDebugUtilsMessengerEXTUnique(messenger_ci, nullptr,
                                                     dispatcher);
}

vk::PhysicalDevice PickPhysicalDevices(
    const std::vector<vk::PhysicalDevice>& physical_devices) {
  assert(physical_devices.size() > 0);

  std::multimap<int, vk::PhysicalDevice> candidates;
  for (auto& device : physical_devices) {
    int score = 0;
    auto properties = device.getProperties();
    if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
      score += 1000;
    }

    score += properties.limits.maxImageDimension2D;

    auto memory_properties = device.getMemoryProperties();
    score += static_cast<int>(memory_properties.memoryHeaps[0].size) / 1000000;
    candidates.insert(std::make_pair(score, device));
  }

  return candidates.begin()->second;
}

vk::UniqueSurfaceKHR CreateWindowSurface(const vk::Instance& instance,
                                         const void* window) {
#if defined(_WIN32)
  vk::Win32SurfaceCreateInfoKHR info;
  info.hwnd = (const HWND)window;
  info.hinstance = GetModuleHandle(NULL);
  return instance.createWin32SurfaceKHRUnique(info, nullptr);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
#elif defined(_DIRECT2DISPLAY)
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
#elif defined(VK_USE_PLATFORM_XCB_KHR)
#elif defined(VK_USE_PLATFORM_IOS_MVK)
  vk::IOSSurfaceCreateInfoMVK info;
  info.pView = window;
  return instance.createIOSSurfaceMVKUnique(info, nullptr);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
  vk::MacOSSurfaceCreateInfoMVK info;
  info.pView = window;
  return instance.createMacOSSurfaceMVKUnique(info, nullptr);
#endif
}

std::tuple<vk::UniqueDevice, std::map<vk::QueueFlagBits, int>>
CreateLogicalDevice(
    const vk::PhysicalDevice& physical_device,
    const std::vector<vk::QueueFamilyProperties>& queue_family_properties,
    bool enabled_debug_marker) {
  std::map<vk::QueueFlagBits, int> queue_index_table;
  for (int i = 0; i < queue_family_properties.size(); ++i) {
    auto& properties = queue_family_properties[i];
    if (properties.queueCount > 0) {
      if (properties.queueFlags & vk::QueueFlagBits::eGraphics &&
          queue_index_table.find(vk::QueueFlagBits::eGraphics) ==
              queue_index_table.end()) {
        queue_index_table[vk::QueueFlagBits::eGraphics] = i;
      }
      if (properties.queueFlags & vk::QueueFlagBits::eCompute &&
          queue_index_table.find(vk::QueueFlagBits::eCompute) ==
              queue_index_table.end()) {
        queue_index_table[vk::QueueFlagBits::eCompute] = i;
      }
      if (properties.queueFlags & vk::QueueFlagBits::eTransfer &&
          queue_index_table.find(vk::QueueFlagBits::eTransfer) ==
              queue_index_table.end()) {
        queue_index_table[vk::QueueFlagBits::eTransfer] = i;
      }
    }
  }

  if (queue_index_table.find(vk::QueueFlagBits::eGraphics) ==
      queue_index_table.end()) {
    TEMP_LOG_ERROR("Not found queue family.");
    return std::forward_as_tuple(vk::UniqueDevice(), queue_index_table);
  }

  auto graphics_queue_index = queue_index_table[vk::QueueFlagBits::eGraphics];

  std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
  vk::DeviceQueueCreateInfo graphics_queue_create_info;
  graphics_queue_create_info.queueFamilyIndex = graphics_queue_index;
  graphics_queue_create_info.queueCount =
      queue_family_properties[graphics_queue_index].queueCount;
  std::vector<float> graphics_queue_priorities;
  for (uint32_t i = 0; i < graphics_queue_create_info.queueCount; ++i) {
    graphics_queue_priorities.emplace_back(1.0f);
  }
  graphics_queue_create_info.pQueuePriorities =
      graphics_queue_priorities.data();
  queue_create_infos.emplace_back(graphics_queue_create_info);

  auto iter = queue_index_table.find(vk::QueueFlagBits::eCompute);
  auto compute_queue_index =
      iter != queue_index_table.end() ? iter->second : -1;
  std::vector<float> compute_queue_priorities;
  if (compute_queue_index != -1 &&
      graphics_queue_index != compute_queue_index) {
    vk::DeviceQueueCreateInfo compute_queue_create_info;
    compute_queue_create_info.queueFamilyIndex = compute_queue_index;
    compute_queue_create_info.queueCount =
        queue_family_properties[compute_queue_index].queueCount;
    for (uint32_t i = 0; i < compute_queue_create_info.queueCount; ++i) {
      compute_queue_priorities.emplace_back(1.0f);
    }
    compute_queue_create_info.pQueuePriorities =
        compute_queue_priorities.data();
    queue_create_infos.emplace_back(compute_queue_create_info);
  }

  iter = queue_index_table.find(vk::QueueFlagBits::eTransfer);
  auto transfer_queue_index =
      iter != queue_index_table.end() ? iter->second : -1;
  std::vector<float> transfer_queue_priorities;
  if (transfer_queue_index != -1 &&
      graphics_queue_index != transfer_queue_index &&
      compute_queue_index != transfer_queue_index) {
    vk::DeviceQueueCreateInfo transfer_queue_create_info;
    transfer_queue_create_info.queueFamilyIndex = transfer_queue_index;
    transfer_queue_create_info.queueCount =
        queue_family_properties[transfer_queue_index].queueCount;
    for (uint32_t i = 0; i < transfer_queue_create_info.queueCount; ++i) {
      transfer_queue_priorities.emplace_back(1.0f);
    }
    transfer_queue_create_info.pQueuePriorities =
        transfer_queue_priorities.data();
    queue_create_infos.emplace_back(transfer_queue_create_info);
  }

  vk::DeviceCreateInfo device_create_info;

  device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  auto features = physical_device.getFeatures();
  device_create_info.pEnabledFeatures = &features;

  auto device_extension_properties =
      physical_device.enumerateDeviceExtensionProperties();

  std::vector<const char*> device_extensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  if (enabled_debug_marker) {
    device_extensions.emplace_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
  }

  auto end = std::remove_if(
      device_extensions.begin(), device_extensions.end(),
      [&device_extension_properties](auto& n) {
        auto r = std::find_if(device_extension_properties.begin(),
                              device_extension_properties.end(), [n](auto& p) {
                                std::string ext_name = p.extensionName;
                                return ext_name == n;
                              });
        return r == device_extension_properties.end();
      });

  device_extensions.erase(end, device_extensions.end());

  device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
  device_create_info.ppEnabledExtensionNames = device_extensions.data();

  auto device = physical_device.createDeviceUnique(device_create_info);
  return std::forward_as_tuple(std::move(device), queue_index_table);
}

vk::SwapchainCreateInfoKHR SetupSwapchainCreateInfo(
    vk::PhysicalDevice physical_device, int graphics_queue_index,
    vk::SurfaceKHR surface, const vk::Extent2D& extent,
    vk::SwapchainKHR old_swap_chain) {
  auto support =
      physical_device.getSurfaceSupportKHR(graphics_queue_index, surface);
  if (support == VK_FALSE) {
    TEMP_LOG_ERROR("Graphics queue isn't supported surface. index: ",
                   graphics_queue_index);
    return vk::SwapchainCreateInfoKHR();
  }

  auto surf_caps = physical_device.getSurfaceCapabilitiesKHR(surface);

  vk::Extent2D swapchain_extent;
  if (surf_caps.currentExtent.width == -1) {
    swapchain_extent = extent;
  } else {
    swapchain_extent = surf_caps.currentExtent;
  }

  uint32_t desired_number_of_swapchain_images = surf_caps.minImageCount + 1;
  if ((surf_caps.maxImageCount > 0) &&
      (desired_number_of_swapchain_images > surf_caps.maxImageCount)) {
    desired_number_of_swapchain_images = surf_caps.maxImageCount;
  }

  vk::SurfaceTransformFlagBitsKHR pre_transform;
  if (surf_caps.supportedTransforms &
      vk::SurfaceTransformFlagBitsKHR::eIdentity) {
    pre_transform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
  } else {
    pre_transform = surf_caps.currentTransform;
  }

  std::vector<vk::PresentModeKHR> present_modes =
      physical_device.getSurfacePresentModesKHR(surface);

  vk::PresentModeKHR present_mode = vk::PresentModeKHR::eFifo;

  auto iter = std::find(present_modes.begin(), present_modes.end(),
                        vk::PresentModeKHR::eMailbox);

  if (iter != present_modes.end()) {
    present_mode = vk::PresentModeKHR::eMailbox;
  } else {
    iter = std::find(present_modes.begin(), present_modes.end(),
                     vk::PresentModeKHR::eImmediate);
    if (iter != present_modes.end()) {
      present_mode = vk::PresentModeKHR::eImmediate;
    }
  }

  auto color_formats = physical_device.getSurfaceFormatsKHR(surface);
  vk::SurfaceFormatKHR surface_format = color_formats[0];
  if (color_formats.size() == 1 &&
      color_formats[0].format == vk::Format::eUndefined) {
    surface_format.format = vk::Format::eB8G8R8A8Unorm;
    surface_format.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
  } else {
    for (auto&& format : color_formats) {
      if (format.format == vk::Format::eB8G8R8A8Unorm &&
          format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
        surface_format = format;
        break;
      }
    }
  }

  vk::SwapchainCreateInfoKHR create_info;
  create_info.surface = surface;
  create_info.minImageCount = desired_number_of_swapchain_images;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent =
      vk::Extent2D{swapchain_extent.width, swapchain_extent.height};
  create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment |
                           vk::ImageUsageFlagBits::eTransferDst;
  create_info.preTransform = pre_transform;
  create_info.imageArrayLayers = 1;
  create_info.imageSharingMode = vk::SharingMode::eExclusive;
  create_info.queueFamilyIndexCount = 0;
  create_info.pQueueFamilyIndices = nullptr;
  create_info.presentMode = present_mode;
  create_info.oldSwapchain = old_swap_chain;
  create_info.clipped = VK_TRUE;
  create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

  return create_info;
}
}  // namespace vulkan
}  // namespace gfx
}  // namespace temp

#endif
