#pragma once
#include <cstdint>

#include <functional>

namespace temp {
namespace app {
template <class T>
class ApplicationBase {
 public:
  std::int32_t Run() { return static_cast<T>(*this)->Run(); }
  void Exit() { static_cast<T>(*this)->Exit(); }

  std::function<void(void)> &on_initialize() { return on_initialize_; }
  std::function<void(void)> &on_update() { return on_update_; }
  std::function<void(void)> &on_terminate() { return on_terminate_; }
  void *native_window_handle() const { return native_window_handle_; }

 protected:
  std::function<void(void)> on_initialize_ = []() {};
  std::function<void(void)> on_update_ = []() {};
  std::function<void(void)> on_terminate_ = []() {};
  void *native_window_handle_ = nullptr;
};
}  // namespace app
}  // namespace temp
