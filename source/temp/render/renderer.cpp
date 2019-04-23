#include "temp/core/define.h"

#include "temp/render/renderer.h"
#include "temp/render/camera.h"

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

Renderer::Renderer() {
  camera_table_ = std::make_shared<CameraTable>();
  camera_table_mutex_ = std::make_shared<std::mutex>();
}

Renderer::~Renderer() {}

std::unique_ptr<Camera> Renderer::CreateCamera() {
  auto&& camera = std::unique_ptr<Camera>(
      new Camera([camera_table = camera_table_,
                  mutex = camera_table_mutex_](Camera* camera) {
        std::unique_lock<std::mutex> lock(*mutex);
        camera_table->erase(camera);
      }));

  std::unique_lock<std::mutex> lock(*camera_table_mutex_);
  camera_table_->insert(camera.get());

  return std::move(camera);
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