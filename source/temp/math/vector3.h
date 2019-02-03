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
#include "temp/math/vector2.h"

namespace temp {
namespace math {

template <class T>
class Vector3Base {
 public:
  static const std::size_t kDimention = 3;
  static const Vector3Base kZero;
  static const Vector3Base kRight;
  static const Vector3Base kLeft;
  static const Vector3Base kUp;
  static const Vector3Base kDown;
  static const Vector3Base kForward;
  static const Vector3Base kBackward;

  using iterator = typename std::array<T, kDimention>::iterator;

  Vector3Base() : Vector3Base((T)0, (T)0, (T)0) {}
  explicit Vector3Base(T x, T y, T z) {
    values_[0] = x;
    values_[1] = y;
    values_[2] = z;
  }
  explicit Vector3Base(const Vector2Base<T> vec2, T z)
      : Vector3Base(vec2.x(), vec2.y(), z) {}

  Vector3Base(const Vector3Base&) = default;
  ~Vector3Base() = default;
  Vector3Base& operator=(const Vector3Base&) = default;

 public:
  iterator begin() { return values_.begin(); }

  iterator end() { return values_.end(); }

  std::string toString() const {
    std::stringstream ss;
    ss << "Vector3(" << x() << ", " << y() << ", " << z() << ")";
    return ss.str();
  }

  T& operator[](std::size_t index) {
    TEMP_ASSERT(index < 3, "index must be less than 3");
    return values_[index];
  }

  T operator[](std::size_t index) const {
    TEMP_ASSERT(index < 3, "index must be less than 3");
    return values_[index];
  }

  T& x() { return values_[0]; }
  T x() const { return values_[0]; }
  T& y() { return values_[1]; }
  T y() const { return values_[1]; }
  T& z() { return values_[2]; }
  T z() const { return values_[2]; }

  bool operator==(const Vector3Base& rhs) const {
    return x() == rhs.x() && y() == rhs.y() && z() == rhs.z();
  }

  bool operator!=(const Vector3Base& rhs) const { return !(*this == rhs); }

  Vector3Base operator+() const { return *this; }

  Vector3Base operator-() const { return Vector3Base(-x(), -y(), -z()); }

  Vector3Base& operator+=(const Vector3Base& rhs) {
    x() += rhs.x();
    y() += rhs.y();
    z() += rhs.z();
    return *this;
  }

  Vector3Base& operator-=(const Vector3Base& rhs) {
    x() -= rhs.x();
    y() -= rhs.y();
    z() -= rhs.z();
    return *this;
  }

  Vector3Base& operator*=(float rhs) {
    x() *= rhs;
    y() *= rhs;
    z() *= rhs;
    return *this;
  }

  Vector3Base& operator/=(float rhs) {
    TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
    x() /= rhs;
    y() /= rhs;
    z() /= rhs;
    return *this;
  }

  T magnitude() const { return sqrtf(squared_magnitude()); }
  T squared_magnitude() const { return x() * x() + y() * y() + z() * z(); }

  Vector3Base normalized() const { return *this / magnitude(); }

  void normalize() { *this /= magnitude(); }

 private:
  std::array<T, kDimention> values_;
};

template <class T>
const Vector3Base<T> Vector3Base<T>::kZero = Vector3Base<T>(0, 0, 0);
template <class T>
const Vector3Base<T> Vector3Base<T>::kRight = Vector3Base<T>(1, 0, 0);
template <class T>
const Vector3Base<T> Vector3Base<T>::kLeft = Vector3Base<T>(-1, 0, 0);
template <class T>
const Vector3Base<T> Vector3Base<T>::kUp = Vector3Base<T>(0, 1, 0);
template <class T>
const Vector3Base<T> Vector3Base<T>::kDown = Vector3Base<T>(0, -1, 0);
template <class T>
const Vector3Base<T> Vector3Base<T>::kForward = Vector3Base<T>(0, 0, 1);
template <class T>
const Vector3Base<T> Vector3Base<T>::kBackward = Vector3Base<T>(0, 0, -1);

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
T angle_rad(const Vector3Base<T>& from, const Vector3Base<T>& to,
            const Vector3Base<T>& axis) {
  auto unsigned_angle = unsigned_angle_rad(from, to);
  auto c = cross(from, to);
  auto angle = dot(axis, c) >= 0 ? unsigned_angle : -unsigned_angle;
  return angle;
}

template <class T>
T angle_deg(const Vector3Base<T>& from, const Vector3Base<T>& to,
            const Vector3Base<T>& axis) {
  return ((T)180 / kPi) * angle_rad(from, to, axis);
}

template <class T>
T unsigned_angle_rad(const Vector3Base<T>& from, const Vector3Base<T>& to) {
  auto from_magnitude_to_magnitude = from.magnitude() * to.magnitude();
  if (from_magnitude_to_magnitude == (T)0) {
    return (T)0;
  }

  auto cos = dot(from, to) / from_magnitude_to_magnitude;
  T unsigned_angle = std::acos((T)cos);

  return unsigned_angle;
}

template <class T>
T unsigned_angle_deg(const Vector3Base<T>& from, const Vector3Base<T>& to) {
  return ((T)180 / kPi) * unsigned_angle_rad(from, to);
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
  return (lhs - rhs).magnitude();
}

template <class T>
Vector3Base<T> lerp(const Vector3Base<T>& lhs, const Vector3Base<T>& rhs, T t) {
  return lhs * (1 - t) + rhs * (t);
}

using Vector3 = Vector3Base<float>;
using Vector3_64b = Vector3Base<double>;

}  // namespace math
}  // namespace temp
