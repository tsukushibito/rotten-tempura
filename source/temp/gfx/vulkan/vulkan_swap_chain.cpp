#include "temp/core/define.h"
#ifdef TEMP_PLATFORM_WINDOWS
#define NOMINMAX
#endif
#ifdef TEMP_GFX_API_VULKAN
#include <cassert>
#include "temp/core/logger.h"
#include "temp/gfx/vulkan/vulkan_context.h"
#include "temp/gfx/vulkan/vulkan_device.h"
#include "temp/gfx/vulkan/vulkan_swap_chain.h"
#include "temp/gfx/vulkan/vulkan_utility.h"

namespace temp {
namespace gfx {
namespace vulkan {

namespace {
std::vector<VulkanSwapChain::Image> CreateSwapChainImages(
    vk::Device device, vk::SwapchainKHR swap_chain, vk::Format image_format) {
  using Image = VulkanSwapChain::Image;

  vk::ImageViewCreateInfo image_view_ci;
  image_view_ci.format = image_format;
  image_view_ci.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  image_view_ci.subresourceRange.levelCount = 1;
  image_view_ci.subresourceRange.layerCount = 1;
  image_view_ci.viewType = vk::ImageViewType::e2D;

  vk::SemaphoreCreateInfo semaphore_ci;
  vk::FenceCreateInfo fence_ci;

  std::vector<Image> images;
  auto swap_chain_images = device.getSwapchainImagesKHR(swap_chain);
  for (auto image : swap_chain_images) {
    image_view_ci.image = image;
    auto view = device.createImageViewUnique(image_view_ci);
    auto acquire_image_semaphore = device.createSemaphoreUnique(semaphore_ci);
    auto render_semaphore = device.createSemaphoreUnique(semaphore_ci);
    // auto fence = device.createFenceUnique(fence_ci);
    auto fence = vk::UniqueFence();
    images.emplace_back(Image{image, std::move(view),
                              std::move(acquire_image_semaphore),
                              std::move(render_semaphore), std::move(fence)});
  }
  return images;
}
}  // namespace

VulkanSwapChain::VulkanSwapChain(const VulkanDevice& device, const void* window,
                                 std::uint32_t width, std::uint32_t height) {
  auto vk_instance = device.instance();
  auto vk_physical_device = device.physical_device();
  auto vk_device = device.device();
  auto graphics_queue_index = device.graphics_queue_index();

  // Create target windows's surface.
  surface_ = CreateWindowSurface(vk_instance, window);

  // Cache SwapchainCreateInfo to reuse in resizing.
  auto extent = vk::Extent2D(width, height);
  swap_chain_ci_ =
      SetupSwapchainCreateInfo(vk_physical_device, graphics_queue_index,
                               *surface_, extent, vk::SwapchainKHR());

  // Create swapchain.
  swap_chain_ = vk_device.createSwapchainKHRUnique(swap_chain_ci_);

  images_ = CreateSwapChainImages(vk_device, *swap_chain_,
                                  swap_chain_ci_.imageFormat);
}

void VulkanSwapChain::Present(const Device* device) const {
  assert(device->api_type() == ApiType::kVulkan);
  auto tvk_device = static_cast<const VulkanDevice*>(device);
  auto vk_physical_device = tvk_device->physical_device();
  auto vk_device = tvk_device->device();
}

void VulkanSwapChain::Resize(const Device* device, std::uint32_t width,
                             std::uint32_t height) {
  assert(device->api_type() == ApiType::kVulkan);
  auto tvk_device = static_cast<const VulkanDevice*>(device);
  auto vk_physical_device = tvk_device->physical_device();
  auto vk_device = tvk_device->device();

  vk_device.waitIdle();

  auto surf_caps = vk_physical_device.getSurfaceCapabilitiesKHR(*surface_);

  vk::Extent2D swapchain_extent;
  if (surf_caps.currentExtent.width == -1) {
    swapchain_extent.width = width;
    swapchain_extent.height = height;
  } else {
    swapchain_extent = surf_caps.currentExtent;
  }

  swap_chain_ci_.imageExtent = surf_caps.currentExtent;
  swap_chain_ci_.oldSwapchain = *swap_chain_;

  auto old_image_count = vk_device.getSwapchainImagesKHR(*swap_chain_).size();

  swap_chain_ = vk_device.createSwapchainKHRUnique(swap_chain_ci_);
  if (swap_chain_ci_.oldSwapchain != vk::SwapchainKHR()) {
    for (auto&& image : images_) {
      image.view.reset();
      vk_device.destroyImage(image.image);
    }
  }

  auto images = vk_device.getSwapchainImagesKHR(*swap_chain_);
  if (images.size() != old_image_count) {
    std::vector<vk::Fence> fences;
    for (auto&& image : images_) {
      fences.emplace_back(*image.fence);
    }

    vk_device.waitForFences(fences, true,
                            std::numeric_limits<std::uint64_t>::max());

    // for (auto&& image : images_) {
    //     image.fence = vk_device.createFenceUnique(vk::FenceCreateInfo());
    // }

    images_ = CreateSwapChainImages(vk_device, *swap_chain_,
                                    swap_chain_ci_.imageFormat);
  }
}

std::uint32_t VulkanSwapChain::width() const {
  return swap_chain_ci_.imageExtent.width;
}

std::uint32_t VulkanSwapChain::height() const {
  return swap_chain_ci_.imageExtent.height;
}

std::uint32_t VulkanSwapChain::current_image_index() const {
  return current_image_;
}
const VulkanSwapChain::Image& VulkanSwapChain::current_image() const {
  return images_[current_image_];
}

vk::Format VulkanSwapChain::color_format() const {
  return swap_chain_ci_.imageFormat;
}

vk::SwapchainKHR VulkanSwapChain::swap_chain() const {
  return swap_chain_.get();
}

const std::uint32_t VulkanSwapChain::image_count() const {
  return images_.size();
}

const VulkanSwapChain::Image& VulkanSwapChain::image(int index) const {
  return images_[index];
}

const vk::Framebuffer VulkanSwapChain::frame_buffer(int index) const {
  return *frame_buffers_[index];
}

const vk::CommandBuffer VulkanSwapChain::command_buffer(int index) const {
  return *command_buffers_[index];
}

std::uint32_t VulkanSwapChain::AcquireNextImage(const Device* device) {
  auto tvk_device = static_cast<const VulkanDevice*>(device);
  auto vk_physical_device = tvk_device->physical_device();
  auto vk_device = tvk_device->device();

  auto result_value = vk_device.acquireNextImageKHR(
      *swap_chain_, std::numeric_limits<uint64_t>::max(),
      *images_[current_image_].acquire_image_semaphore,
      *images_[current_image_].fence);

  auto result = result_value.result;
  if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
    throw std::error_code(result);
  }
  current_image_ = result_value.value;
  return current_image_;
}

}  // namespace vulkan
}  // namespace gfx
}  // namespace temp

#endif
