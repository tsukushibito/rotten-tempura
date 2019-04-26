#pragma once

#include <functional>
#include <memory>

#include "temp/math/matrix44.h"
#include "temp/math/quaternion.h"
#include "temp/math/vector3.h"

#include "temp/gfx/color.h"
#include "temp/gfx/swap_chain.h"

namespace temp {

template <class T>
class ObjectManager;

namespace render {

struct Camera {
  friend class ObjectManager<Camera>;

 public:
  enum class ProjectionType {
    kOrthographic,
    kPerspective,
  };

  enum class ClearMode {
    kSolidColor,
    kDepthOnly,
    kNone,
  };

  math::Vector3 position;
  math::Quaternion rotation;
  ClearMode clear_mode;
  gfx::Color clear_color;
  ProjectionType projection_type;
  float near_clip;
  float far_clip;
  float field_of_view;
  float orthographic_size;
  float aspect_ratio;
  std::shared_ptr<gfx::SwapChain> swap_chain;
};

}  // namespace render
}  // namespace temp