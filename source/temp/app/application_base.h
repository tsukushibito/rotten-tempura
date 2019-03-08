#pragma once
#include <cstdint>

#include <functional>

namespace temp {
namespace app {
using OnInitialize = std::function<void(void)>;
using OnUpdate = std::function<void(void)>;
using OnTerminate = std::function<void(void)>;
using OnResizeWindow = std::function<void(std::uint32_t, std::uint32_t)>;

template <class T>
class ApplicationBase {
 public:
  std::int32_t Run() { return static_cast<T>(*this)->Run(); }
  void Exit() { static_cast<T>(*this)->Exit(); }

  OnInitialize &on_initialize() { return on_initialize_; }
  OnUpdate &on_update() { return on_update_; }
  OnTerminate &on_terminate() { return on_terminate_; }
  OnResizeWindow &on_resize_window() { return on_resize_window_; }
  void *native_window_handle() const { return native_window_handle_; }

 protected:
  OnInitialize on_initialize_ = []() {};
  OnUpdate on_update_ = []() {};
  OnTerminate on_terminate_ = []() {};
  OnResizeWindow on_resize_window_ = [](std::uint32_t, std::uint32_t) {};
  void *native_window_handle_ = nullptr;
};
}  // namespace app
}  // namespace temp
