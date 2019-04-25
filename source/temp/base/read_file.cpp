#include <fstream>

#include "temp/base/read_file.h"

namespace temp {

std::vector<char> ReadFile(const std::string& file_path) {
  std::ifstream fs(file_path, std::ios::ate | std::ios::binary);

  if (!fs.is_open()) {
    TEMP_LOG_ERROR("Failed to open file! : ", file_path);
    fs.close();
    return std::vector<char>();
  }

  auto file_size = (std::size_t)fs.tellg();
  std::vector<char> buf(file_size);

  fs.seekg(0);
  fs.read(buf.data(), file_size);

  fs.close();

  return buf;
}

}  // namespace temp
