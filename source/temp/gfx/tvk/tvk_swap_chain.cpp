#include "temp/core/define.h"
#ifdef TEMP_GFX_API_VULKAN

#include "temp/gfx/tvk/tvk_context.h"
#include "temp/gfx/tvk/tvk_device.h"
#include "temp/gfx/tvk/tvk_swap_chain.h"
#include "temp/gfx/tvk/tvk_utility.h"

namespace temp {
namespace gfx {
namespace tvk {

TvkSwapChain::TvkSwapChain(const TvkDevice& device, const void* window,
                           std::uint32_t width, std::uint32_t height) {
  auto vk_instance = device.instance();
  auto vk_physical_device = device.physical_device();
  auto vk_device = device.device();

  surface_ = CreateWindowSurface(vk_instance, window);

  auto extent = vk::Extent2D(width, height);
  create_info_ = SetupSwapchainCreateInfo(vk_physical_device, *surface_, extent,
                                          vk::SwapchainKHR());

  swap_chain_ = vk_device.createSwapchainKHRUnique(create_info_);
}

void TvkSwapChain::Present(const Device* device) const {
  assert(device->api_type() == ApiType::kVulkan);
  auto tvk_device = static_cast<const TvkDevice*>(device);
  auto vk_physical_device = tvk_device->physical_device();
  auto vk_device = tvk_device->device();
}

void TvkSwapChain::Resize(const Device* device, std::uint32_t width,
                          std::uint32_t height) {
  assert(device->api_type() == ApiType::kVulkan);
  auto tvk_device = static_cast<const TvkDevice*>(device);
  auto vk_physical_device = tvk_device->physical_device();
  auto vk_device = tvk_device->device();

  create_info_.imageExtent.width = width;
  create_info_.imageExtent.width = height;
  create_info_.oldSwapchain = *swap_chain_;

  swap_chain_ = vk_device.createSwapchainKHRUnique(create_info_);
  if (create_info_.oldSwapchain != vk::SwapchainKHR()) {
    for (auto&& image : images_) {
      vk_device.destroyImageView(image.view);
    }
  }
}

void TvkSwapChain::CreateSwapChain() const {}
}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif