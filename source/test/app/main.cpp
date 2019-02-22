#include <cstdlib>
#include <iostream>

#include <temp/app/app.h>
#include <temp/gfx/gfx.h>

using namespace temp;

int main(int argc, char* argv[]) {
  app::Application application;

  auto window = application.native_window_handle();
  auto device = gfx::CreateDevice(gfx::ApiType::kVulkan, window);

  application.Run();

  return 0;
}
