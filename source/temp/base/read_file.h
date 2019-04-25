#pragma once

#include <string>
#include <vector>

#include "temp/base/logger.h"

namespace temp {

std::vector<char> ReadFile(const std::string& file_path);

}  // namespace temp
