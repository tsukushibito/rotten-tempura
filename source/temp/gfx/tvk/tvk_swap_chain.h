#pragma once
#include <vulkan/vulkan.hpp>

#include "temp/gfx/swap_chain.h"
#include "temp/gfx/tvk/tvk_device.h"

namespace temp {
namespace gfx {
namespace tvk {

class TvkSwapChain : public SwapChain {
 public:
  struct Image {
    vk::Image image;
    vk::ImageView view;
    vk::Fence fence;
  };

  explicit TvkSwapChain(const TvkDevice& device, const void* window,
                        std::uint32_t width, std::uint32_t height);
  ~TvkSwapChain() = default;

  TvkSwapChain(const TvkSwapChain&) = delete;
  TvkSwapChain& operator=(const TvkSwapChain&) = delete;

  TvkSwapChain(TvkSwapChain&&) = default;
  TvkSwapChain& operator=(TvkSwapChain&&) = default;

 public:
  ApiType api_type() const override { return ApiType::kVulkan; }

  void Present(const Device* device) const override;

  void Resize(const Device* device, std::uint32_t width,
              std::uint32_t height) override;

  std::uint32_t AcquireNextImage(const Device* device);

 private:
  vk::UniqueSurfaceKHR surface_;
  vk::Format color_format_;
  vk::ColorSpaceKHR color_space_;
  vk::SwapchainCreateInfoKHR swap_chain_ci_;
  vk::UniqueSwapchainKHR swap_chain_;
  vk::UniqueSemaphore present_complete_semaphore_;
  std::uint32_t current_image_;
  std::vector<Image> images_;
  vk::Queue queue_;
};

}  // namespace tvk
}  // namespace gfx
}  // namespace temp
