#include "temp/core/define.h"
#ifdef TEMP_PLATFORM_WINDOWS
#define NOMINMAX
#endif
#ifdef TEMP_GFX_API_VULKAN
#include <cassert>
#include "temp/core/logger.h"
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


vk::UniqueRenderPass CreateRenderPass(const vk::Device vk_device, vk::Format color_format) {
  vk::AttachmentDescription color_attachment;
  color_attachment.format = color_format;
  color_attachment.samples = vk::SampleCountFlagBits::e1;
  color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
  color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
  color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  color_attachment.initialLayout = vk::ImageLayout::eUndefined;
  color_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
  
  vk::AttachmentReference color_attachment_ref;
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;
  
  vk::SubpassDescription subpass;
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;
  
  vk::RenderPassCreateInfo render_pass_ci;
  vk::SubpassDependency subpass_dependency;
  subpass_dependency.dstSubpass = 0;
  subpass_dependency.srcStageMask =
      vk::PipelineStageFlagBits::eColorAttachmentOutput;
  subpass_dependency.srcAccessMask = vk::AccessFlagBits();
  subpass_dependency.dstStageMask =
      vk::PipelineStageFlagBits::eColorAttachmentOutput;
  subpass_dependency.dstAccessMask =
      vk::AccessFlagBits::eColorAttachmentRead |
      vk::AccessFlagBits::eColorAttachmentWrite;
  render_pass_ci.attachmentCount = 1;
  render_pass_ci.pAttachments = &color_attachment;
  render_pass_ci.subpassCount = 1;
  render_pass_ci.pSubpasses = &subpass;
  render_pass_ci.dependencyCount = 1;
  render_pass_ci.pDependencies = &subpass_dependency;
  return vk_device.createRenderPassUnique(render_pass_ci);
}

std::vector<vk::UniqueFramebuffer> CreateFrameBuffers(
    const vk::Device vk_device, const std::vector<VulkanSwapChain::Image>& images,
    vk::RenderPass render_pass, std::uint32_t width, std::uint32_t height) {
  std::vector<vk::UniqueFramebuffer> frame_buffers;

  auto image_count = images.size();
  for (auto&& image: images) {
    vk::ImageView attachments[] = {*image.view};

    vk::FramebufferCreateInfo frame_buffer_ci;
    frame_buffer_ci.renderPass = render_pass;
    frame_buffer_ci.attachmentCount = 1;
    frame_buffer_ci.pAttachments = attachments;
    frame_buffer_ci.width = width;
    frame_buffer_ci.height = height;
    frame_buffer_ci.layers = 1;

    frame_buffers.emplace_back(
        vk_device.createFramebufferUnique(frame_buffer_ci));
  }

  return std::move(frame_buffers);
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

void VulkanSwapChain::Present(const Device* device) {
  assert(device->api_type() == ApiType::kVulkan);
  auto temp_device = static_cast<const VulkanDevice*>(device);
  auto vk_device = temp_device->device();

  auto image_index = AcquireNextImage(vk_device);

  vk::PresentInfoKHR present_info;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &current_image().render_semaphore.get();
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &(*swap_chain_);
  present_info.pImageIndices = &image_index;

  auto&& iter = temp_device->queue_table().find(vk::QueueFlagBits::eGraphics);
  assert(iter != temp_device->queue_table().end());
  auto&& graphics_queue = iter->second;
  graphics_queue.presentKHR(present_info);
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

  // swap_chain_ = vk_device.createSwapchainKHRUnique(swap_chain_ci_, nullptr, tvk_device->dispatcher());
  swap_chain_ = vk_device.createSwapchainKHRUnique(swap_chain_ci_);
  if (swap_chain_ci_.oldSwapchain != vk::SwapchainKHR()) {
    for (auto&& image : images_) {
      image.view.reset();
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

    for (auto&& image : images_) {
      image.fence = vk_device.createFenceUnique(vk::FenceCreateInfo());
    }

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
	return static_cast<std::uint32_t>(images_.size());
}

const VulkanSwapChain::Image& VulkanSwapChain::image(int index) const {
  return images_[index];
}

const vk::RenderPass VulkanSwapChain::render_pass() const {
  return *render_pass_;
}

const vk::Framebuffer VulkanSwapChain::frame_buffer(int index) const {
  return *frame_buffers_[index];
}

std::uint32_t VulkanSwapChain::AcquireNextImage(const vk::Device vk_device) {

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
