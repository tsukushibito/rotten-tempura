#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <map>
#include <set>
#include <sstream>
#include <string>

#include "temp/core/logger.h"

#include "temp/gfx/tvk/tvk_utility.h"

namespace temp {
namespace gfx {
namespace tvk {

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

VkBool32 MessageCallback(VkDebugReportFlagsEXT flags,
                         VkDebugReportObjectTypeEXT obj_type,
                         uint64_t src_object, size_t location, int32_t msg_code,
                         const char* layer_prefix, const char* msg,
                         void* user_data) {
  std::stringstream ss;
  ss << "[" << layer_prefix << "] Code " << msg_code << " : " << msg;

  if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
    TEMP_LOG_ERROR(ss.str());
  } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
    TEMP_LOG_WARNING(ss.str());
  } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    TEMP_LOG_INFO(ss.str());
  } else {
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
      instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }
    instanceCreateInfo.enabledExtensionCount =
        (uint32_t)instanceExtensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
  }
  if (enabled_validation) {
    auto layers = FilterLayers(kValidationLayerNames);
    instanceCreateInfo.enabledLayerCount = layers.size();
    instanceCreateInfo.ppEnabledLayerNames = layers.data();
  }

  return vk::createInstanceUnique(instanceCreateInfo, nullptr);
}

vk::DebugReportCallbackEXT SetupDebugging(
    const vk::Instance& instance, const vk::DebugReportFlagsEXT& flags,
    const vk::DispatchLoaderDynamic& dispatcher) {
  vk::DebugReportCallbackCreateInfoEXT info = {};
  info.pfnCallback = (PFN_vkDebugReportCallbackEXT)MessageCallback;
  info.flags = flags;
  return instance.createDebugReportCallbackEXT(info, nullptr, dispatcher);
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
    score += memory_properties.memoryHeaps[0].size / 1000000;
    candidates.insert(std::make_pair(score, device));
  }

  return candidates.begin()->second;
}

vk::UniqueSurfaceKHR CreateWindowSurface(const vk::Instance& instance,
                                         const void* window) {
#if defined(_WIN32)
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

std::tuple<vk::UniqueDevice, int, int, int> CreateLogicalDevice(
    const vk::PhysicalDevice& physical_device,
    const std::vector<vk::QueueFamilyProperties>& queue_family_properties,
    const vk::SurfaceKHR& surface, bool enabled_debug_marker) {
  int graphics_queue_index = -1;
  int present_queue_index = -1;
  int compute_queue_index = -1;
  for (int i = 0; i < queue_family_properties.size(); ++i) {
    auto& properties = queue_family_properties[i];
    if (properties.queueCount > 0) {
      if (properties.queueFlags & vk::QueueFlagBits::eGraphics) {
        graphics_queue_index = i;
      }
      if (physical_device.getSurfaceSupportKHR(i, surface)) {
        present_queue_index = i;
      }
      if (properties.queueFlags & vk::QueueFlagBits::eCompute) {
        compute_queue_index = i;
      }
    }
    if (graphics_queue_index != -1 && present_queue_index != -1 &&
        compute_queue_index != -1) {
      break;
    }
  }

  if (graphics_queue_index == -1 || present_queue_index == -1) {
    TEMP_LOG_ERROR("Not found queue family.");
    return std::forward_as_tuple(vk::UniqueDevice(), -1, -1, -1);
  }

  std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
  {
    vk::DeviceQueueCreateInfo graphics_queue_create_info;
    graphics_queue_create_info.queueFamilyIndex = graphics_queue_index;
    graphics_queue_create_info.queueCount =
        queue_family_properties[graphics_queue_index].queueCount;
    std::vector<float> queue_priorities;
    for (int i = 0; i < graphics_queue_create_info.queueCount; ++i) {
      queue_priorities.emplace_back(1.0f - 0.1f * i);
    }
    graphics_queue_create_info.pQueuePriorities = queue_priorities.data();
    queue_create_infos.emplace_back(graphics_queue_create_info);
  }

  if (graphics_queue_index != present_queue_index) {
    vk::DeviceQueueCreateInfo present_queue_create_info;
    present_queue_create_info.queueFamilyIndex = present_queue_index;
    present_queue_create_info.queueCount =
        queue_family_properties[present_queue_index].queueCount;
    std::vector<float> queue_priorities;
    for (int i = 0; i < present_queue_create_info.queueCount; ++i) {
      queue_priorities.emplace_back(1.0f - 0.1f * i);
    }
    present_queue_create_info.pQueuePriorities = queue_priorities.data();
    queue_create_infos.emplace_back(present_queue_create_info);
  }

  if (compute_queue_index != -1 &&
      graphics_queue_index != compute_queue_index &&
      present_queue_index != compute_queue_index) {
    vk::DeviceQueueCreateInfo compute_queue_create_info;
    compute_queue_create_info.queueFamilyIndex = compute_queue_index;
    compute_queue_create_info.queueCount =
        queue_family_properties[compute_queue_index].queueCount;
    std::vector<float> queue_priorities;
    for (int i = 0; i < compute_queue_create_info.queueCount; ++i) {
      queue_priorities.emplace_back(1.0f - 0.1f * i);
    }
    compute_queue_create_info.pQueuePriorities = queue_priorities.data();
    queue_create_infos.emplace_back(compute_queue_create_info);
  }

  vk::DeviceCreateInfo device_create_info;

  device_create_info.queueCreateInfoCount = queue_create_infos.size();
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

  device_create_info.enabledExtensionCount = device_extensions.size();
  device_create_info.ppEnabledExtensionNames = device_extensions.data();

  auto device = physical_device.createDeviceUnique(device_create_info);
  return std::forward_as_tuple(std::move(device), graphics_queue_index,
                               present_queue_index, compute_queue_index);
}
}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif