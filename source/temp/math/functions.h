#pragma once

#include <cmath>

#include <limits>

#include "temp/math/constants.h"

namespace temp {
namespace math {
template <class T>
bool approximately(T lhs, T rhs) {
  return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
}

template <class T>
T radianToDegree(T radian) {
  static const T rad2deg = (T)180 / ConstantsBase<T>::kPi;
  return rad2deg * radian;
}

template <class T>
T degreeToRadian(T degree) {
  static const T deg2rad = ConstantsBase<T>::kPi / (T)180;
  return deg2rad * degree;
}
}  // namespace math
}  // namespace temp
