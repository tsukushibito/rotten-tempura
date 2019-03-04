#pragma once

#include <cmath>

#include <iostream>

#include "temp/core/assertion.h"
#include "temp/core/define.h"

#include "temp/math/functions.h"
#include "temp/math/vector3.h"

namespace temp {
namespace math {
//----------------------------------------
// declaration
//----------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const Vector3Base<T>& vec3);

template <class T>
Vector3Base<T> operator+(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs);

template <class T>
Vector3Base<T> operator-(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs);

template <class T>
Vector3Base<T> operator*(const Vector3Base<T>& lhs, float rhs);

template <class T>
Vector3Base<T> operator*(float lhs, const Vector3Base<T>& rhs);

template <class T>
Vector3Base<T> operator/(const Vector3Base<T>& lhs, float rhs);

template <class T>
T magnitude(const Vector3Base<T>& v);

template <class T>
T squared_magnitude(const Vector3Base<T>& v);

template <class T>
Vector3Base<T> normalize(const Vector3Base<T>& v);

template <class T>
T angle(const Vector3Base<T>& from, const Vector3Base<T>& to,
        const Vector3Base<T>& axis);

template <class T>
T unsigned_angle(const Vector3Base<T>& from, const Vector3Base<T>& to);

template <class T>
T dot(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs);

template <class T>
Vector3Base<T> cross(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs);

template <class T>
T distance(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs);

template <class T>
Vector3Base<T> lerp(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs, T t);

//----------------------------------------
// implementation
//----------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const Vector3Base<T>& vec3) {
  stream << vec3.toString();
  return stream;
}

template <class T>
Vector3Base<T> operator+(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs) {
  return Vector3Base<T>(lhs) += rhs;
}

template <class T>
Vector3Base<T> operator-(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs) {
  return Vector3Base<T>(lhs) -= rhs;
}

template <class T>
Vector3Base<T> operator*(const Vector3Base<T>& lhs, float rhs) {
  return Vector3Base<T>(lhs) *= rhs;
}

template <class T>
Vector3Base<T> operator*(float lhs, const Vector3Base<T>& rhs) {
  return rhs * lhs;
}

template <class T>
Vector3Base<T> operator/(const Vector3Base<T>& lhs, float rhs) {
  return Vector3Base<T>(lhs) /= rhs;
}

template <class T>
T magnitude(const Vector3Base<T>& v) {
  return sqrtf(squared_magnitude(v));
}

template <class T>
T squared_magnitude(const Vector3Base<T>& v) {
  return v.x() * v.x() + v.y() * v.y() + v.z() * v.z();
}

template <class T>
Vector3Base<T> normalize(const Vector3Base<T>& v) {
  return v / magnitude(v);
}

template <class T>
T angle(const Vector3Base<T>& from, const Vector3Base<T>& to,
        const Vector3Base<T>& axis) {
  auto uangle = unsigned_angle(from, to);
  auto c = cross(from, to);
  auto angle = dot(axis, c) >= 0 ? uangle : -uangle;
  return angle;
}

template <class T>
T unsigned_angle(const Vector3Base<T>& from, const Vector3Base<T>& to) {
  auto from_magnitude_to_magnitude = magnitude(from) * magnitude(to);
  if (from_magnitude_to_magnitude == (T)0) {
    return (T)0;
  }

  auto cos = dot(from, to) / from_magnitude_to_magnitude;
  T unsigned_angle = std::acos((T)cos);

  return radianToDegree(unsigned_angle);
}

template <class T>
T dot(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs) {
  return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

template <class T>
Vector3Base<T> cross(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs) {
  return Vector3Base<T>(lhs.y() * rhs.z() - rhs.y() * lhs.z(),
                        -(lhs.x() * rhs.z() - rhs.x() * lhs.z()),
                        lhs.x() * rhs.y() - rhs.x() * lhs.y());
}

template <class T>
T distance(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs) {
  return magnitude(lhs - rhs);
}

template <class T>
Vector3Base<T> lerp(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs, T t) {
  return lhs * (1 - t) + rhs * (t);
}
}  // namespace math
}  // namespace temp