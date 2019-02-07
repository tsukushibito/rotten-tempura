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
#include "temp/math/functions.h"
#include "temp/math/matrix44.h"
#include "temp/math/vector3.h"
#include "temp/math/vector4.h"

namespace temp {
namespace math {
template <class T>
class QuaternionBase {
 public:
  static const QuaternionBase kIdentity;

  using iterator = typename std::array<T, 4>::iterator;

  QuaternionBase() : QuaternionBase((T)0, (T)0, (T)0, (T)0) {}
  explicit QuaternionBase(T x, T y, T z, T w) {
    values_[0] = x;
    values_[1] = y;
    values_[2] = z;
    values_[3] = w;
  }

  QuaternionBase(const QuaternionBase&) = default;
  ~QuaternionBase() = default;
  QuaternionBase& operator=(const QuaternionBase&) = default;

 public:
  iterator begin() { return values_.begin(); }

  iterator end() { return values_.end(); }

  std::string toString() const {
    std::stringstream ss;
    ss << "Quaternion(" << x() << ", " << y() << ", " << z() << ", " << w()
       << ")";
    return ss.str();
  }

  T& operator[](std::size_t index) {
    TEMP_ASSERT(index < 4, "index must be less than 4");
    return values_[index];
  }

  T operator[](std::size_t index) const {
    TEMP_ASSERT(index < 4, "index must be less than 4");
    return values_[index];
  }

  T& x() { return values_[0]; }
  T x() const { return values_[0]; }
  T& y() { return values_[1]; }
  T y() const { return values_[1]; }
  T& z() { return values_[2]; }
  T z() const { return values_[2]; }
  T& w() { return values_[3]; }
  T w() const { return values_[3]; }

  bool operator==(const QuaternionBase& rhs) const {
    return x() == rhs.x() && y() == rhs.y() && z() == rhs.z() && w() == rhs.w();
  }

  bool operator!=(const QuaternionBase& rhs) const { return !(*this == rhs); }

  QuaternionBase operator+() const { return *this; }

  QuaternionBase operator-() const {
    return QuaternionBase(-x(), -y(), -z(), -w());
  }

  QuaternionBase& operator*=(T rhs) {
    x() *= rhs;
    y() *= rhs;
    z() *= rhs;
    w() *= rhs;
    return *this;
  }

  QuaternionBase& operator/=(T rhs) {
    x() /= rhs;
    y() /= rhs;
    z() /= rhs;
    w() /= rhs;
    return *this;
  }

  T norm() const {
    return std::sqrt(w() * w() + x() * x() + y() * y() + z() * z());
  }

  QuaternionBase<T> normalized() const { return *this / norm(); }

  Vector3Base<T> eulerAngles() const {
    T x2 = x() * x();
    T y2 = y() * y();
    T z2 = z() * z();

    T xy = x() * y();
    T xz = x() * z();
    T yz = y() * z();
    T wx = w() * x();
    T wy = w() * y();
    T wz = w() * z();

    T m00 = 1 - (2 * y2) - (2 * z2);
    T m01 = (2 * xy) + (2 * wz);
    // T m02 = (2 * xz) - (2 * wy);
    T m10 = (2 * xy) - (2 * wz);
    T m11 = 1 - (2 * x2) - (2 * z2);
    // T m12 = (2 * yz) + (2 * wx);
    T m20 = (2 * xz) + (2 * wy);
    T m21 = (2 * yz) - (2 * wx);
    T m22 = 1 - (2 * x2) - (2 * y2);

    T tx, ty, tz;

    if (approximately(m21, (T)1)) {
      tx = -ConstantsBase<T>::kPi / (T)2;
      ty = 0;
      tz = atan2(m10, m00);
    } else if (approximately(m21, (T)-1)) {
      tx = ConstantsBase<T>::kPi / (T)2;
      ty = 0;
      tz = atan2(m10, m00);
    } else {
      tx = asin(-m21);
      ty = atan2(m20, m22);
      tz = atan2(m01, m11);
    }

    tx = radianToDegree(tx);
    ty = radianToDegree(ty);
    tz = radianToDegree(tz);

    return Vector3Base<T>(tx, ty, tz);
  }

 private:
  std::array<T, 4> values_;

 public:
  static QuaternionBase axisAngle(const Vector3Base<T>& axis, T degree) {
    auto v = axis.normalized();
    auto rad = degreeToRadian(degree);
    auto rad_2 = rad / (T)2;
    return QuaternionBase(v.x() * sin(rad_2), v.y() * sin(rad_2),
                          v.z() * sin(rad_2), cos(rad_2));
  }
};

template <class T>
const QuaternionBase<T> QuaternionBase<T>::kIdentity = QuaternionBase<T>(0, 0,
                                                                         0, 1);

template <class T>
std::ostream& operator<<(std::ostream& stream, const QuaternionBase<T>& quat) {
  stream << quat.toString();
  return stream;
}

template <class T>
QuaternionBase<T> operator*(QuaternionBase<T> lhs, T rhs) {
  return lhs *= rhs;
}

template <class T>
QuaternionBase<T> operator/(QuaternionBase<T> lhs, T rhs) {
  return lhs /= rhs;
}

using Quaternion = QuaternionBase<float>;
using Quaternion_64b = QuaternionBase<double>;
}  // namespace math
}  // namespace temp
