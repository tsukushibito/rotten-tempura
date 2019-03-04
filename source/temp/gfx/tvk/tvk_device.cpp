#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include <vulkan/vulkan.hpp>

#include "temp/gfx/tvk/tvk_context.h"
#include "temp/gfx/tvk/tvk_device.h"
#include "temp/gfx/tvk/tvk_swap_chain.h"
#include "temp/gfx/tvk/tvk_utility.h"

namespace temp {
namespace gfx {
namespace tvk {

TvkDevice::TvkDevice(const void* window)
    : context_(std::make_unique<Context>(window)) {
  auto surface = CreateWindowSurface(context_->instance(), window);
}

TvkDevice::~TvkDevice() {}

std::unique_ptr<SwapChain> TvkDevice::CreateSwapChain(
    const void* window, std::uint32_t width, std::uint32_t height) const {
  return std::make_unique<TvkSwapChain>(*this, window, width, height);
}
}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif