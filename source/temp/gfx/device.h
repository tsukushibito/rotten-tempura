#pragma once
#include <memory>

namespace temp {
namespace gfx {
class Device {
 public:
  Device() : Device(nullptr) {}
  explicit Device(const void* window);

  Device(const Device&) = delete;
  Device& operator=(const Device&) = delete;

  Device(Device&& other) noexcept = default;
  Device& operator=(Device&& other) = default;

  ~Device();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace gfx
}  // namespace temp