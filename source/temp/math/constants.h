#pragma once
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

namespace temp {
namespace math {

template <class T>
class ConstantsBase {
 public:
  static constexpr T kPi = (T)M_PI;
  static constexpr T kPi_2 = (T)M_PI_2;
  static constexpr T kPi_4 = (T)M_PI_4;
  static constexpr T k_1_Pi = (T)M_1_PI;
  static constexpr T k_2_Pi = (T)M_2_PI;
};

extern const float kPi;
extern const float kPi_2;
extern const float kPi_4;
extern const float k_1_Pi;
extern const float k_2_Pi;

extern const double kPi_64b;
extern const double kPi_2_64b;
extern const double kPi_4_64b;
extern const double k_1_Pi_64b;
extern const double k_2_Pi_64b;

}  // namespace math
}  // namespace temp