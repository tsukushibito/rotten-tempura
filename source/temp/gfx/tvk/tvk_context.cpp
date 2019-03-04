#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "temp/core/logger.h"

#include "temp/gfx/tvk/tvk_context.h"
#include "temp/gfx/tvk/tvk_utility.h"

namespace temp {
namespace gfx {
namespace tvk {

Context::Context(const void* window) {
  instance_ = CreateInstance("tempura", "tempura", true);

  dispatcher_.init(*instance_);

  auto flags = vk::DebugReportFlagBitsEXT::eInformation |
               vk::DebugReportFlagBitsEXT::eWarning |
               vk::DebugReportFlagBitsEXT::ePerformanceWarning |
               vk::DebugReportFlagBitsEXT::eError |
               vk::DebugReportFlagBitsEXT::eDebug;
  msg_callback_ = SetupDebugging(*instance_, flags, dispatcher_);

  physical_devices_ = instance_->enumeratePhysicalDevices();
  physical_device_ = PickPhysicalDevices(physical_devices_);
  device_properties_ = physical_device_.getProperties();
  device_features_ = physical_device_.getFeatures();
  device_memory_properties_ = physical_device_.getMemoryProperties();
  queue_family_properties_ = physical_device_.getQueueFamilyProperties();

  auto device_and_queue_indices =
      CreateLogicalDevice(physical_device_, queue_family_properties_, false);

  device_ = std::move(std::get<0>(device_and_queue_indices));
  if ((VkDevice)(*device_) == VK_NULL_HANDLE) {
    TEMP_LOG_ERROR("Could not create vulkan device!");
    std::abort();
  }
  queue_index_table_ = std::get<1>(device_and_queue_indices);

  dispatcher_.init(*instance_, *device_);

  pipeline_cache_ =
      device_->createPipelineCacheUnique(vk::PipelineCacheCreateInfo());

  queue_table_[vk::QueueFlagBits::eGraphics] =
      device_->getQueue(queue_index_table_[vk::QueueFlagBits::eGraphics], 0);

  auto iter = queue_index_table_.find(vk::QueueFlagBits::eCompute);
  if (iter != queue_index_table_.end()) {
    queue_table_[vk::QueueFlagBits::eCompute] =
        device_->getQueue(iter->second, 0);
  }

  iter = queue_index_table_.find(vk::QueueFlagBits::eTransfer);
  if (iter != queue_index_table_.end()) {
    queue_table_[vk::QueueFlagBits::eTransfer] =
        device_->getQueue(iter->second, 0);
  }
}

std::vector<vk::Format> Context::GetSupportedDepthFormats() const {
  std::vector<vk::Format> depth_formats = {
      vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat,
      vk::Format::eD24UnormS8Uint, vk::Format::eD16UnormS8Uint,
      vk::Format::eD16Unorm};

  std::vector<vk::Format> supported;

  for (auto& format : depth_formats) {
    vk::FormatProperties format_props;
    format_props = physical_device_.getFormatProperties(format);

    if (format_props.optimalTilingFeatures &
        vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
      supported.push_back(format);
    }
  }

  return supported;
}
}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif
