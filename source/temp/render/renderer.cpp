#include "temp/base/define.h"

#include "temp/render/camera.h"
#include "temp/render/renderer.h"

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
  TEMP_ASSERT(false, "");
  return nullptr;
#endif
}
}  // namespace

Renderer::Renderer() {}

Renderer::~Renderer() {}

std::unique_ptr<CameraHandle> Renderer::CreateCamera() {
  return camera_manager_.CreateObject();
}

std::unique_ptr<Renderer> CreateRenderer(
    const std::shared_ptr<temp::gfx::Device>& device) {
  auto api = device->api_type();
  switch (api) {
    case gfx::ApiType::kVulkan:
      return std::unique_ptr<Renderer>(CreateVulkanRenderer(device));
      break;
    default:
      assert(false);
      return nullptr;
  }
}
}  // namespace render
}  // namespace temp