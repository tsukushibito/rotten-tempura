#pragma once
#include <vulkan/vulkan.hpp>

#include "temp/gfx/swap_chain.h"
#include "temp/gfx/vulkan/vulkan_device.h"

namespace temp {
namespace gfx {
namespace vulkan {

class VulkanSwapChain : public SwapChain {
 public:
  struct Image {
    vk::Image image;
    vk::UniqueImageView view;
    vk::UniqueSemaphore acquire_image_semaphore;
    vk::UniqueSemaphore render_semaphore;
    vk::UniqueFence fence;
  };

  explicit VulkanSwapChain(const VulkanDevice& device, const void* window,
                           std::uint32_t width, std::uint32_t height);
  ~VulkanSwapChain() = default;

  VulkanSwapChain(const VulkanSwapChain&) = delete;
  VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;

  VulkanSwapChain(VulkanSwapChain&&) = default;
  VulkanSwapChain& operator=(VulkanSwapChain&&) = default;

  ApiType api_type() const override { return ApiType::kVulkan; }

  void Present(const Device* device) override;

  void Resize(const Device* device, std::uint32_t width,
              std::uint32_t height) override;

  std::uint32_t width() const override;
  std::uint32_t height() const override;

  std::uint32_t current_image_index() const;

  const Image& current_image() const;

  vk::Format color_format() const;

  vk::SwapchainKHR swap_chain() const;

  const std::uint32_t image_count() const;

  const Image& image(int index) const;

  const vk::RenderPass render_pass() const;

  const vk::Framebuffer frame_buffer(int index) const;

  std::uint32_t AcquireNextImage(const vk::Device vk_device);
private:

  void CreateRenderPass(const vk::Device device);

  void CreateFrameBuffers(const vk::Device device);

  vk::UniqueSurfaceKHR surface_;
  vk::Format color_format_;
  vk::ColorSpaceKHR color_space_;
  vk::SwapchainCreateInfoKHR swap_chain_ci_;
  vk::UniqueSwapchainKHR swap_chain_;
  std::uint32_t current_image_ = 0;
  std::vector<Image> images_;
  vk::UniqueRenderPass render_pass_;

  std::vector<vk::UniqueFramebuffer> frame_buffers_;
};

}  // namespace vulkan
}  // namespace gfx
}  // namespace temp
