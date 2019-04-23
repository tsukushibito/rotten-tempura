#include "temp/core/define.h"
#include "temp/render/camera.h"

namespace temp {
namespace render {

Camera::Camera(const std::function<void(Camera*)>& on_delete)
    : on_delete_(on_delete) {}

Camera::~Camera() { on_delete_(this); }

}
}