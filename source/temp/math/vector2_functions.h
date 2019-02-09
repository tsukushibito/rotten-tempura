#pragma once

#include <cmath>

#include <iostream>

#include "temp/core/assertion.h"
#include "temp/core/define.h"

#include "temp/math/functions.h"
#include "temp/math/vector2.h"

namespace temp {
namespace math {
//----------------------------------------
// declaration
//----------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const Vector2Base<T>& vec2);

template <class T>
Vector2Base<T> operator+(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs);

template <class T>
Vector2Base<T> operator-(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs);

template <class T>
Vector2Base<T> operator*(const Vector2Base<T>& lhs, float rhs);

template <class T>
Vector2Base<T> operator*(float lhs, const Vector2Base<T>& rhs);

template <class T>
Vector2Base<T> operator/(const Vector2Base<T>& lhs, float rhs);

template <class T>
T magnitude(const Vector2Base<T>& v);

template <class T>
T squared_magnitude(const Vector2Base<T>& v);

template <class T>
Vector2Base<T> normalize(const Vector2Base<T>& v);

template <class T>
T angle(const Vector2Base<T>& from, const Vector2Base<T>& to);

template <class T>
T dot(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs);

template <class T>
T distance(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs);

template <class T>
Vector2Base<T> lerp(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs, T t);

//----------------------------------------
// implementation
//----------------------------------------
template <class T>
std::ostream& operator<<(std::ostream& stream, const Vector2Base<T>& vec2) {
  stream << vec2.toString();
  return stream;
}

template <class T>
Vector2Base<T> operator+(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs) {
  return Vector2Base<T>(lhs) += rhs;
}

template <class T>
Vector2Base<T> operator-(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs) {
  return Vector2Base<T>(lhs) -= rhs;
}

template <class T>
Vector2Base<T> operator*(const Vector2Base<T>& lhs, float rhs) {
  return Vector2Base<T>(lhs) *= rhs;
}

template <class T>
Vector2Base<T> operator*(float lhs, const Vector2Base<T>& rhs) {
  return rhs * lhs;
}

template <class T>
Vector2Base<T> operator/(const Vector2Base<T>& lhs, float rhs) {
  return Vector2Base<T>(lhs) /= rhs;
}

template <class T>
T magnitude(const Vector2Base<T>& v) {
  return sqrtf(squared_magnitude(v));
}
template <class T>
T squared_magnitude(const Vector2Base<T>& v) {
  return v.x() * v.x() + v.y() * v.y();
}

template <class T>
Vector2Base<T> normalize(const Vector2Base<T>& v) {
  return v / magnitude(v);
}

template <class T>
T angle(const Vector2Base<T>& from, const Vector2Base<T>& to) {
  auto from_magnitude_to_magnitude = magnitude(from) * magnitude(to);
  if (from_magnitude_to_magnitude == (T)0) return (T)0;

  auto cos = dot(from, to) / from_magnitude_to_magnitude;
  auto unsigned_angle = std::acos((float)cos);
  auto rad = from.x() * to.y() - from.y() * to.x() >= (T)0 ? unsigned_angle
                                                           : -unsigned_angle;
  return radianToDegree(rad);
}

template <class T>
T dot(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs) {
  return lhs.x() * rhs.x() + lhs.y() * rhs.y();
}

template <class T>
T distance(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs) {
  return magnitude(lhs - rhs);
}

template <class T>
Vector2Base<T> lerp(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs, T t) {
  return lhs * (1 - t) + rhs * (t);
}
}  // namespace math
}  // namespace temp