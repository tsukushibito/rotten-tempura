#pragma once

#include <memory>

#include "temp/gfx/device.h"

namespace temp {
namespace render {
class Renderer {
 public:
 private:
};

std::unique_ptr<Renderer> CreateRenderer(
    const std::shared_ptr<temp::gfx::Device>& device);
}  // namespace render
}  // namespace temp