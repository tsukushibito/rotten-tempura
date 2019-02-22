#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <vulkan/vulkan.hpp>

#include "temp/gfx/tvk/tvk_context.h"
#include "temp/gfx/tvk/tvk_device.h"

namespace temp {
namespace gfx {
namespace tvk {

TvkDevice::TvkDevice(const void* window)
    : context_(std::make_shared<Context>(window)) {}

}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif