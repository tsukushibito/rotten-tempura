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

namespace temp {
namespace math {

template <class T>
class Vector2Base {
 public:
  static const std::size_t kDimention = 2;
  static const Vector2Base kZero;
  static const Vector2Base kRight;
  static const Vector2Base kLeft;
  static const Vector2Base kUp;
  static const Vector2Base kDown;

  using iterator = typename std::array<T, kDimention>::iterator;

  Vector2Base() : Vector2Base((T)0, (T)0) {}
  explicit Vector2Base(T x, T y) {
    values_[0] = x;
    values_[1] = y;
  }

  Vector2Base(const Vector2Base&) = default;
  ~Vector2Base() = default;
  Vector2Base& operator=(const Vector2Base&) = default;

 public:
  iterator begin() { return values_.begin(); }

  iterator end() { return values_.end(); }

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

  bool operator!=(const Vector2Base& rhs) const { return !(*this == rhs); }

  Vector2Base operator+() const { return *this; }

  Vector2Base operator-() const { return Vector2Base(-x(), -y()); }

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
  T squared_magnitude() const { return x() * x() + y() * y(); }

  Vector2Base normalized() const { return *this / magnitude(); }

  void normalize() { *this /= magnitude(); }

 private:
  std::array<T, kDimention> values_;
};

template <class T>
const Vector2Base<T> Vector2Base<T>::kZero = Vector2Base<T>(0, 0);
template <class T>
const Vector2Base<T> Vector2Base<T>::kRight = Vector2Base<T>(1, 0);
template <class T>
const Vector2Base<T> Vector2Base<T>::kLeft = Vector2Base<T>(-1, 0);
template <class T>
const Vector2Base<T> Vector2Base<T>::kUp = Vector2Base<T>(0, 1);
template <class T>
const Vector2Base<T> Vector2Base<T>::kDown = Vector2Base<T>(0, -1);

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
  auto rad = from.x() * to.y() - from.y() * to.x() >= (T)0 ? unsigned_angle
                                                           : -unsigned_angle;
  return ((T)180 / ConstantsBase<T>::kPi) * rad;
}

template <class T>
T dot(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs) {
  return lhs.x() * rhs.x() + lhs.y() * rhs.y();
}

template <class T>
T distance(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs) {
  return (lhs - rhs).magnitude();
}

template <class T>
Vector2Base<T> lerp(const Vector2Base<T>& lhs, const Vector2Base<T>& rhs, T t) {
  return lhs * (1 - t) + rhs * (t);
}

using Vector2 = Vector2Base<float>;
using Vector2_64b = Vector2Base<double>;
}  // namespace math
}  // namespace temp
