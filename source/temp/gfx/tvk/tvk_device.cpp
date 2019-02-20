#include <vector>

#include <vulkan/vulkan.hpp>

#include "temp/gfx/device.h"
#include "temp/gfx/tvk/tvk_context.h"

namespace temp {
namespace gfx {

class Device::Impl {
 public:
  Impl() : Impl(nullptr) {}
  explicit Impl(const void* window) : context_(window) {}
  ~Impl() = default;

  Impl(const Impl&) = delete;
  Impl& operator=(const Impl&) = delete;

  Impl(Impl&&) = default;
  Impl& operator=(Impl&&) = default;

 private:
  tvk::Context context_;
};

Device::Device(const void* window) : impl_(new Impl(window)) {}
Device::~Device() {}

}  // namespace gfx
}  // namespace temp
