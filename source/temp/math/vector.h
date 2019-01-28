#pragma once

#include <cmath>
#include <cstdint>

#include <iostream>
#include <sstream>
#include <string>

#include "temp/core/assertion.h"
#include "temp/core/define.h"

namespace temp {
namespace math {

template <class T>
class Vector2Base {
 public:
  static const std::size_t kDimention = 2;

  Vector2Base() : Vector2Base((T)0, (T)0) {}
  explicit Vector2Base(T x, T y) {
    values_[0] = x;
    values_[1] = y;
  }

  Vector2Base(const Vector2Base&) = default;
  ~Vector2Base() = default;
  Vector2Base& operator=(const Vector2Base&) = default;

 public:
  std::string toString() const {
    std::stringstream ss;
    ss << "Vector2(" << x() << ", " << y() << ")";
    return ss.str();
  }

  T& operator[](std::size_t index) {
    TEMP_ASSERT(index < 2, "index must be less than 2");
    return values_[index];
  }

  T operator[](std::size_t index) const {
    TEMP_ASSERT(index < 2, "index must be less than 2");
    return values_[index];
  }

  T& x() { return values_[0]; }
  T x() const { return values_[0]; }
  T& y() { return values_[1]; }
  T y() const { return values_[1]; }

  bool operator==(const Vector2Base& rhs) const {
    return x() == rhs.x() && y() == rhs.y();
  }

  Vector2Base& operator+=(const Vector2Base& rhs) {
    x() += rhs.x();
    y() += rhs.y();
    return *this;
  }

  Vector2Base& operator-=(const Vector2Base& rhs) {
    x() -= rhs.x();
    y() -= rhs.y();
    return *this;
  }

  Vector2Base& operator*=(float rhs) {
    x() *= rhs;
    y() *= rhs;
    return *this;
  }

  Vector2Base& operator/=(float rhs) {
    TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
    x() /= rhs;
    y() /= rhs;
    return *this;
  }

  T magnitude() const { return sqrtf(squared_magnitude()); }
  T squared_magnitude() const { return x() * x() + y() + y(); }

  Vector2Base normalized() const { return *this / magnitude(); }

  void normalize() { *this /= magnitude(); }

 private:
  T values_[kDimention];
};

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
T angle(const Vector2Base<T>& from, const Vector2Base<T>& to) {
  auto from_magnitude_to_magnitude = from.magnitude() * to.magnitude();
  if (from_magnitude_to_magnitude == (T)0) return (T)0;

  auto cos = dot(from, to) / from_magnitude_to_magnitude;
  auto unsigned_angle = std::acos((float)cos);
  return from.x() * to.y() - from.y() * to.x() >= (T)0 ? unsigned_angle
                                                       : -unsigned_angle;
}

template <class T>
T dot(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs) {
  return lhs.x() * rhs.x() + lhs.y() + rhs.y();
}

using Vector2 = Vector2Base<float>;
using Vector2_64 = Vector2Base<double>;
}  // namespace math
}  // namespace temp