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
  swap_chain_ci_ = SetupSwapchainCreateInfo(vk_physical_device, *surface_,
                                            extent, vk::SwapchainKHR());

  swap_chain_ = vk_device.createSwapchainKHRUnique(swap_chain_ci_);

  vk::SemaphoreCreateInfo semaphore_ci;
  present_complete_semaphore_ = vk_device.createSemaphoreUnique(semaphore_ci);
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

  swap_chain_ci_.imageExtent.width = width;
  swap_chain_ci_.imageExtent.width = height;
  swap_chain_ci_.oldSwapchain = *swap_chain_;

  swap_chain_ = vk_device.createSwapchainKHRUnique(swap_chain_ci_);
  if (swap_chain_ci_.oldSwapchain != vk::SwapchainKHR()) {
    for (auto&& image : images_) {
      vk_device.destroyImageView(image.view);
    }
  }
}

std::uint32_t TvkSwapChain::AcquireNextImage(const Device* device) {
  auto tvk_device = static_cast<const TvkDevice*>(device);
  auto vk_physical_device = tvk_device->physical_device();
  auto vk_device = tvk_device->device();

  auto result_value = vk_device.acquireNextImageKHR(
      *swap_chain_, std::numeric_limits<uint64_t>::max(),
      *present_complete_semaphore_, vk::Fence());

  auto result = result_value.result;
  if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
    throw std::error_code(result);
  }
  current_image_ = result_value.value;
  return current_image_;
}

}  // namespace tvk
}  // namespace gfx
}  // namespace temp

#endif