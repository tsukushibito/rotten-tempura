#pragma once
#include <vulkan/vulkan.hpp>

#include "temp/gfx/swap_chain.h"

namespace temp {
namespace gfx {
namespace tvk {

class Context;

class TvkSwapChain : public SwapChain {
 public:
  explicit TvkSwapChain(std::shared_ptr<Context>& context);
  ~TvkSwapChain() = default;

  TvkSwapChain(const TvkSwapChain&) = delete;
  TvkSwapChain& operator=(const TvkSwapChain&) = delete;

  TvkSwapChain(TvkSwapChain&&) = default;
  TvkSwapChain& operator=(TvkSwapChain&&) = default;

 public:
  void Present() const override;

 private:
  std::shared_ptr<Context> context_;
};

}  // namespace tvk
}  // namespace gfx
}  // namespace temp