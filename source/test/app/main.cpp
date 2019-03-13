#include <cstdlib>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <temp/app/app.h>
#include <temp/core/core.h>
#include <temp/gfx/gfx.h>

#include <temp/gfx/tvk/tvk_device.h>

#include <boost/filesystem.hpp>

using namespace temp;

std::vector<char> ReadFile(const std::string& file_path) {
  std::ifstream fs(file_path, std::ios::ate | std::ios::binary);

  if (!fs.is_open()) {
    TEMP_LOG_ERROR("Failed to open file! : ", file_path);
    return std::vector<char>();
  }

  auto file_size = (std::size_t)fs.tellg();
  std::vector<char> buf(file_size);

  fs.seekg(0);
  fs.read(buf.data(), file_size);

  fs.close();

  return buf;
}

int main(int argc, char* argv[]) {
  app::Application application;

  application.on_resize_window() = [](std::uint32_t w, std::uint32_t h) {
    TEMP_LOG_TRACE("On window resize. (", w, ", ", h, ")");
  };
  auto window = application.native_window_handle();
  auto window_size = app::GetWindowViewSize(window);
  auto gfx_device = gfx::CreateDevice(gfx::ApiType::kVulkan, window,
                                      window_size.width, window_size.height);
  {
    auto vs_code = ReadFile("shader/vert.spv");
    auto fs_code = ReadFile("shader/frag.spv");

    auto tvk_device = static_cast<gfx::tvk::TvkDevice*>(gfx_device.get());
    auto vk_device = tvk_device->device();

    vk::ShaderModuleCreateInfo vs_module_ci;
    vs_module_ci.codeSize = vs_code.size();
    vs_module_ci.pCode = (std::uint32_t*)vs_code.data();
    auto vs_module = vk_device.createShaderModuleUnique(vs_module_ci);

    vk::ShaderModuleCreateInfo fs_module_ci;
    fs_module_ci.codeSize = fs_code.size();
    fs_module_ci.pCode = (std::uint32_t*)fs_code.data();
    auto ps_module = vk_device.createShaderModuleUnique(fs_module_ci);
  }

  application.Run();

  return 0;
}
