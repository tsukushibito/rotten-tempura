#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "temp/gfx/tvk/tvk_context.h"

#include "temp/core/logger.h"

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

Context::Context(const void* windows) {
  instance_ = CreateInstance("tempura", "tempura", true);
  std::call_once(dispatcher_init_flag_, [&] { dispatcher_.init(*instance_); });
}

vk::UniqueInstance Context::CreateInstance(const std::string& app_name,
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

void Context::SetupDebugging(const vk::Instance& instance,
                             const vk::DebugReportFlagsEXT& flags) {
  vk::DebugReportCallbackCreateInfoEXT info = {};
  info.pfnCallback = (PFN_vkDebugReportCallbackEXT)MessageCallback;
  info.flags = flags;
  msg_callback_ =
      instance.createDebugReportCallbackEXT(info, nullptr, dispatcher_);
}

vk::UniqueSurfaceKHR Context::CreateWindowSurface(
    const vk::UniqueInstance& instance, const void* window) {
#if defined(_WIN32)
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
#elif defined(_DIRECT2DISPLAY)
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
#elif defined(VK_USE_PLATFORM_XCB_KHR)
#elif defined(VK_USE_PLATFORM_IOS_MVK)
  vk::IOSSurfaceCreateInfoMVK info;
  info.pView = window;
  return instance->createIOSSurfaceMVKUnique(info, nullptr);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
  vk::MacOSSurfaceCreateInfoMVK info;
  info.pView = window;
  return instance->createMacOSSurfaceMVKUnique(info, nullptr);
#endif
}
}  // namespace tvk
}  // namespace gfx
}  // namespace temp
