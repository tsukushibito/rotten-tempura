#include "temp/render/renderer.h"

#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN
#include "temp/render/vulkan/vulkan_renderer.h"
#endif

namespace temp {
namespace render {

namespace {
Renderer* CreateVulkanRenderer(
    const std::shared_ptr<temp::gfx::Device>& device) {
#ifdef TEMP_GFX_API_VULKAN
  return new vulkan::VulkanRenderer(device);
#else
  assert(false);
  return nullptr;
#endif
}
}  // namespace

std::unique_ptr<Renderer> CreateRenderer(
    const std::shared_ptr<temp::gfx::Device>& device) {
  auto api = device->api_type();
  switch (api) {
    case gfx::ApiType::kVulkan:
      return std::make_unique<Renderer>(CreateVulkanRenderer(device));
      break;
    default:
      assert(false);
      return nullptr;
  }
}
}  // namespace render
}  // namespace temp