#pragma once

#include <memory>
#include <functional>

#include "temp/math/matrix44.h"
#include "temp/math/quaternion.h"
#include "temp/math/vector3.h"

#include "temp/gfx/swap_chain.h"

namespace temp {
namespace render {

struct Camera {
  friend class Renderer;

 public:
  enum class ProjectionType {
    kOrthographic,
    kPerspective,
  };

 private:
  explicit Camera(const std::function<void(Camera*)>& on_delete);

 public:
  ~Camera();

  math::Vector3 position;
  math::Quaternion rotation;
  ProjectionType projection_type;
  float near_clip;
  float far_clip;
  float field_of_view;
  float orthographic_size;
  float aspect_ratio;
  std::shared_ptr<gfx::SwapChain> swap_chain;

 private:
  std::function<void(Camera*)> on_delete_;
};

}  // namespace render
}  // namespace temp