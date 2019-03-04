#pragma once

#include <iostream>

#include "temp/math/constants.h"
#include "temp/math/functions.h"
#include "temp/math/quaternion.h"
#include "temp/math/vector3.h"

namespace temp {
namespace math {

//----------------------------------------
// declaration
//----------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const QuaternionBase<T>& quat);

template <class T>
QuaternionBase<T> operator*(const QuaternionBase<T>& lhs, T rhs);

template <class T>
QuaternionBase<T> operator*(T lhs, const QuaternionBase<T>& rhs);

template <class T>
QuaternionBase<T> operator*(const QuaternionBase<T>& lhs,
                            const QuaternionBase<T>& rhs);

template <class T>
QuaternionBase<T> operator/(const QuaternionBase<T>& lhs, T rhs);

template <class T>
T magnitude(const QuaternionBase<T>& q);

template <class T>
QuaternionBase<T> normalize(const QuaternionBase<T>& q);

template <class T>
QuaternionBase<T> conjugated(const QuaternionBase<T>& q);

template <class T>
QuaternionBase<T> inverse(const QuaternionBase<T>& q);

template <class T>
Vector3Base<T> eulerAngles(const QuaternionBase<T>& q);

template <class T>
Vector3Base<T> rotate(const Vector3Base<T>& vec3,
                      const QuaternionBase<T>& quat);

//----------------------------------------
// implementation
//----------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const QuaternionBase<T>& quat) {
  stream << quat.toString();
  return stream;
}

template <class T>
QuaternionBase<T> operator*(const QuaternionBase<T>& lhs, T rhs) {
  return QuaternionBase<T>(lhs) *= rhs;
}

template <class T>
QuaternionBase<T> operator*(T lhs, const QuaternionBase<T>& rhs) {
  return rhs * lhs;
}

template <class T>
QuaternionBase<T> operator*(const QuaternionBase<T>& lhs,
                            const QuaternionBase<T>& rhs) {
  return QuaternionBase<T>(lhs) *= rhs;
}

template <class T>
QuaternionBase<T> operator/(const QuaternionBase<T>& lhs, T rhs) {
  return QuaternionBase<T>(lhs) /= rhs;
}

template <class T>
T magnitude(const QuaternionBase<T>& q) {
  return std::sqrt(q.w() * q.w() + q.x() * q.x() + q.y() * q.y() +
                   q.z() * q.z());
}

template <class T>
QuaternionBase<T> normalize(const QuaternionBase<T>& q) {
  return q / magnitude(q);
}

template <class T>
QuaternionBase<T> conjugated(const QuaternionBase<T>& q) {
  return QuaternionBase<T>(-q.x(), -q.y(), -q.z(), q.w());
}

template <class T>
QuaternionBase<T> inverse(const QuaternionBase<T>& q) {
  auto mag = magnitude(q);
  return conjugated(q) / (mag * mag);
}

template <class T>
Vector3Base<T> eulerAngles(const QuaternionBase<T>& q) {
  T x2 = q.x() * q.x();
  T y2 = q.y() * q.y();
  T z2 = q.z() * q.z();

  T xy = q.x() * q.y();
  T xz = q.x() * q.z();
  T yz = q.y() * q.z();
  T wx = q.w() * q.x();
  T wy = q.w() * q.y();
  T wz = q.w() * q.z();

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

template <class T>
Vector3Base<T> rotate(const Vector3Base<T>& vec3,
                      const QuaternionBase<T>& quat) {
  QuaternionBase<T> temp(vec3.x(), vec3.y(), vec3.z(), 0);
  auto result = quat * temp * conjugated(quat);
  return Vector3Base<T>(result.x(), result.y(), result.z());
}

}  // namespace math
}  // namespace temp
