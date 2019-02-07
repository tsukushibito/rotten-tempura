#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace temp {
namespace math {
template <class T>
class ConstantsBase {
 public:
  static constexpr T kPi = M_PI;
  static constexpr T kPi_2 = M_PI_2;
  static constexpr T kPi_4 = M_PI_4;
  static constexpr T k_1_Pi = M_1_PI;
  static constexpr T k_2_Pi = M_2_PI;
};

auto& kPi = ConstantsBase<float>::kPi;
auto& kPi_2 = ConstantsBase<float>::kPi_2;
auto& kPi_4 = ConstantsBase<float>::kPi_4;
auto& k_1_Pi = ConstantsBase<float>::k_1_Pi;
auto& k_2_Pi = ConstantsBase<float>::k_2_Pi;

auto& kPi_64b = ConstantsBase<double>::kPi;
auto& kPi_2_64b = ConstantsBase<double>::kPi_2;
auto& kPi_4_64b = ConstantsBase<double>::kPi_4;
auto& k_1_Pi_64b = ConstantsBase<double>::k_1_Pi;
auto& k_2_Pi_64b = ConstantsBase<double>::k_2_Pi;
}  // namespace math
}  // namespace temp