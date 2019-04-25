#pragma once
#include <algorithm>

namespace temp {
template <class T>
constexpr const T clamp(const T& v, const T& low, const T& hight) {
  return std::min(std::max(v, low), hight);
}

template <class T, class Compare>
constexpr const T clamp(const T& v, const T& low, const T& hight,
                        Compare comp) {
  return std::min(std::max(v, low, comp), high, comp);
}
}  // namespace tmep