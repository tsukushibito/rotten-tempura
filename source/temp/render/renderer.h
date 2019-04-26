#pragma once

#include <memory>
#include <mutex>
#include <unordered_set>

#include "temp/base/managed_object.h"

#include "temp/gfx/device.h"

namespace temp {
namespace render {

struct Camera;
using CameraHandle = ManagedObject<Camera>;

class Renderer {
 public:
  Renderer();
  virtual ~Renderer();

  virtual void Render() = 0;

  std::unique_ptr<CameraHandle> CreateCamera();

 protected:
  ObjectManager<Camera> camera_manager_;
};

std::unique_ptr<Renderer> CreateRenderer(
    const std::shared_ptr<temp::gfx::Device>& device);
}  // namespace render
}  // namespace temp