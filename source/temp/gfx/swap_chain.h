#pragma once
#include <memory>

#include "temp/gfx/device.h"

namespace temp {
namespace gfx {
class SwapChain {
 protected:
  SwapChain() = default;

 public:
  virtual ~SwapChain() = default;

 public:
  virtual ApiType api_type() const = 0;

  virtual void Present() const = 0;
};
}  // namespace gfx
}  // namespace temp