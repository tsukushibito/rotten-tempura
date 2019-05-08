#pragma once

#include <memory>
#include <mutex>
#include <unordered_set>

#include "temp/base/object_manager.h"
#include "temp/gfx/device.h"

namespace temp {
namespace render {

struct Camera;

class Renderer {
 public:
  Renderer();
  virtual ~Renderer();

  virtual void Render() = 0;

  ObjectManager<Camera>::CreateType CreateCamera();

 protected:
  std::shared_ptr<ObjectManager<Camera>> camera_manager_;
};

std::unique_ptr<Renderer> CreateRenderer(
    const std::shared_ptr<temp::gfx::Device>& device);
}  // namespace render
}  // namespace temp