#pragma once

#include <memory>
#include <mutex>
#include <unordered_set>

#include "temp/gfx/device.h"

namespace temp {
namespace render {

struct Camera;

class Renderer {
 public:
  Renderer();
  virtual ~Renderer();

  virtual void Render() = 0;

  std::unique_ptr<Camera> CreateCamera();

 private:
  using CameraTable = std::unordered_set<Camera*>;

  std::shared_ptr<CameraTable> camera_table_;
  std::shared_ptr<std::mutex> camera_table_mutex_;
};

std::unique_ptr<Renderer> CreateRenderer(
    const std::shared_ptr<temp::gfx::Device>& device);
}  // namespace render
}  // namespace temp