#pragma once

#include "temp/base/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <mutex>
#include <unordered_map>

#include "temp/render/renderer.h"

#include "temp/gfx/vulkan/vulkan_device.h"

namespace temp {
namespace render {
namespace vulkan {
class VulkanRenderer : public Renderer {
 public:
  VulkanRenderer() = delete;
  explicit VulkanRenderer(const std::shared_ptr<gfx::Device>& device);

  void Render() override;

 private:
  using VkDeviceSPtr = std::shared_ptr<gfx::vulkan::VulkanDevice>;

  VkDeviceSPtr device_;
};
}  // namespace vulkan
}  // namespace render
}  // namespace temp

#endif