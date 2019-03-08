#include <cstdlib>
#include <iostream>

#include <temp/app/app.h>
#include <temp/core/core.h>
#include <temp/gfx/gfx.h>

using namespace temp;

int main(int argc, char* argv[]) {
  app::Application application;

  application.on_resize_window() = [](std::uint32_t w, std::uint32_t h) {
    TEMP_LOG_TRACE("On window resize. (", w, ", ", h, ")");
  };
  auto window = application.native_window_handle();
  auto device = gfx::CreateDevice(gfx::ApiType::kVulkan, window);

  application.Run();

  return 0;
}
