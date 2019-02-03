#pragma once

#include <cmath>
#include <cstdint>

#include <array>
#include <iostream>
#include <sstream>
#include <string>

#include "temp/core/assertion.h"
#include "temp/core/define.h"

#include "temp/math/constants.h"
#include "temp/math/vector3.h"
#include "temp/math/vector4.h"

namespace temp {
namespace math {
template <class T>
class Matrix44Base {
 public:
  static const kRows = 4;
  static const kCols = 4;

 private:
  std::array<Vector4Base<T>, 4> rows_;
};
}  // namespace math
}  // namespace temp
