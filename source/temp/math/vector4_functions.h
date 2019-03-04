#pragma once
#include <cmath>

#include <iostream>

#include "temp/math/vector4.h"

namespace temp {
namespace math {

template <class T>
std::ostream& operator<<(std::ostream& stream, const Vector4Base<T>& vec4) {
  stream << vec4.toString();
  return stream;
}

template <class T>
Vector4Base<T> operator+(const Vector4Base<T>& lhs, const Vector4Base<T>& rhs) {
  return Vector4Base<T>(lhs) += rhs;
}

template <class T>
Vector4Base<T> operator-(const Vector4Base<T>& lhs, const Vector4Base<T>& rhs) {
  return Vector4Base<T>(lhs) -= rhs;
}

template <class T>
Vector4Base<T> operator*(const Vector4Base<T>& lhs, float rhs) {
  return Vector4Base<T>(lhs) *= rhs;
}

template <class T>
Vector4Base<T> operator*(float lhs, const Vector4Base<T>& rhs) {
  return rhs * lhs;
}

template <class T>
Vector4Base<T> operator/(const Vector4Base<T>& lhs, float rhs) {
  return Vector4Base<T>(lhs) /= rhs;
}

template <class T>
T magnitude(const Vector4Base<T>& v) {
  return sqrtf(squared_magnitude(v));
}
template <class T>
T squared_magnitude(const Vector4Base<T>& v) {
  return v.x() * v.x() + v.y() * v.y() + v.z() * v.z() + v.w() * v.w();
}

template <class T>
Vector4Base<T> normalize(const Vector4Base<T>& v) {
  return v / magnitude(v);
}

template <class T>
T dot(const Vector4Base<T>& lhs, const Vector4Base<T>& rhs) {
  return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z() +
         lhs.w() * rhs.w();
}
}  // namespace math
}  // namespace temp
