#pragma once
#include <cstdint>
#include <memory>

#include "temp/app/application_base.h"
#include "temp/base/define.h"
#ifdef TEMP_PLATFORM_WINDOWS

namespace temp {
namespace app {
namespace windows {

class WindowsApplication : public ApplicationBase<WindowsApplication> {
 public:
  WindowsApplication();
  ~WindowsApplication();

  std::int32_t Run();
  void Exit();
};
}  // namespace mac
using Application = windows::WindowsApplication;

}  // namespace app
}  // namespace temp
#endif
