#include "temp/base/define.h"
#ifdef TEMP_PLATFORM_WINDOWS
#include "Windows.h"
#include "temp/app/utility.h"
namespace temp {
namespace app {
WindowViewSize GetWindowViewSize(const void* window) {
  auto hwnd = (const HWND)(window);
  RECT rect;
  auto result = GetClientRect(hwnd, &rect);
  if (result == FALSE) {
    return WindowViewSize();
  }

  return WindowViewSize{(std::uint32_t)(rect.right - rect.left),
                        (std::uint32_t)(rect.bottom - rect.top)};
}
}  // namespace app
}  // namespace temp

#endif