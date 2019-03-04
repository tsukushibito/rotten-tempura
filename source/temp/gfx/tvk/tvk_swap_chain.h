#pragma once
#include <vulkan/vulkan.hpp>

#include "temp/gfx/swap_chain.h"
#include "temp/gfx/tvk/tvk_device.h"

namespace temp {
namespace gfx {
namespace tvk {

class TvkSwapChain : public SwapChain {
 public:
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

 private:
  void CreateSwapChain() const;

  vk::UniqueSurfaceKHR surface_;
  vk::SwapchainCreateInfoKHR create_info_;
  vk::UniqueSwapchainKHR swap_chain_;
  vk::Queue queue_;

  int current_image_ = 0;
};

}  // namespace tvk
}  // namespace gfx
}  // namespace temp