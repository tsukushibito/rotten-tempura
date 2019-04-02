#pragma once

#include <memory>

#include "temp/gfx/device.h"

namespace temp {
namespace render {
class Renderer {
 public:
  virtual void Render() = 0;
};

std::unique_ptr<Renderer> CreateRenderer(
    const std::shared_ptr<temp::gfx::Device>& device);
}  // namespace render
}  // namespace temp