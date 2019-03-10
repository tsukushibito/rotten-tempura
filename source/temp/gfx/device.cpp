#include <utility>

#include "temp/gfx/device.h"

#ifdef TEMP_GFX_API_VULKAN
#include "temp/gfx/tvk/tvk_device.h"
#endif

namespace temp {
namespace gfx {

std::unique_ptr<Device> CreateDevice(ApiType api, const void* window,
                                     std::uint32_t width,
                                     std::uint32_t height) {
  std::unique_ptr<Device> device;
  switch (api) {
#ifdef TEMP_GFX_API_VULKAN
    case ApiType::kVulkan:
      device = std::make_unique<tvk::TvkDevice>(window, width, height);
      break;
#endif
    default:
      break;
  }

  return device;
}
}  // namespace gfx
}  // namespace temp
