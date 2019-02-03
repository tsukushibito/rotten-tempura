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
#include "temp/math/vector3.h"

namespace temp {
namespace math {

template <class T>
class Vector4Base {
 public:
  static const std::size_t kDimention = 4;
  using iterator = typename std::array<T, kDimention>::iterator;

  Vector4Base() : Vector4Base((T)0, (T)0, (T)0, (T)0) {}
  explicit Vector4Base(T x, T y, T z, T w) {
    values_[0] = x;
    values_[1] = y;
    values_[2] = z;
    values_[3] = w;
  }
  explicit Vector4Base(const Vector3Base<T> vec3, T w)
      : Vector4Base(vec3.x(), vec3.y(), vec3.z(), w) {}

  Vector4Base(const Vector4Base&) = default;
  ~Vector4Base() = default;
  Vector4Base& operator=(const Vector4Base&) = default;

 public:
  iterator begin() { return values_.begin(); }

  iterator end() { return values_.end(); }

  std::string toString() const {
    std::stringstream ss;
    ss << "Vector4(" << x() << ", " << y() << ", " << z() << ", " << w() << ")";
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

  bool operator==(const Vector4Base& rhs) const {
    return x() == rhs.x() && y() == rhs.y() && z() == rhs.z() && w() == rhs.w();
  }

  bool operator!=(const Vector4Base& rhs) const { return !(*this == rhs); }

  Vector4Base operator+() const { return *this; }

  Vector4Base operator-() const { return Vector4Base(-x(), -y(), -z(), -w()); }

  Vector4Base& operator+=(const Vector4Base& rhs) {
    x() += rhs.x();
    y() += rhs.y();
    z() += rhs.z();
    w() += rhs.w();
    return *this;
  }

  Vector4Base& operator-=(const Vector4Base& rhs) {
    x() -= rhs.x();
    y() -= rhs.y();
    z() -= rhs.z();
    w() -= rhs.w();
    return *this;
  }

  Vector4Base& operator*=(float rhs) {
    x() *= rhs;
    y() *= rhs;
    z() *= rhs;
    w() *= rhs;
    return *this;
  }

  Vector4Base& operator/=(float rhs) {
    TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
    x() /= rhs;
    y() /= rhs;
    z() /= rhs;
    w() /= rhs;
    return *this;
  }

  T magnitude() const { return sqrtf(squared_magnitude()); }
  T squared_magnitude() const {
    return x() * x() + y() * y() + z() * z() + w() * w();
  }

  Vector4Base normalized() const { return *this / magnitude(); }

  void normalize() { *this /= magnitude(); }

 private:
  std::array<T, kDimention> values_;
};

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

using Vector4 = Vector4Base<float>;
using Vector4_64b = Vector4Base<double>;
}  // namespace math
}  // namespace temp
