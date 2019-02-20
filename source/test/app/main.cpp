#include <cstdlib>
#include <iostream>

#include <temp/app/app.h>
#include <temp/gfx/gfx.h>

using namespace temp;

int main(int argc, char* argv[]) {
  std::cout << std::getenv("VK_ICD_FILENAMES") << std::endl;
  std::cout << std::getenv("VK_LAYER_PATH") << std::endl;

  app::Application application;

  auto window = application.native_window_handle();
  gfx::Device device(window);

  application.Run();

  return 0;
}