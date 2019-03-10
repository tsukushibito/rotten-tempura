#pragma once
#include <cstdint>
namespace temp {
namespace app {
struct WindowViewSize {
  std::uint32_t width;
  std::uint32_t height;
};

WindowViewSize GetWindowViewSize(const void* window);

}  // namespace app
}  // namespace temp