#pragma once

#include <cmath>
#include <cstdint>

#include <array>
#include <sstream>
#include <string>

#include "temp/base/assertion.h"
#include "temp/base/define.h"

#include "temp/math/constants.h"
#include "temp/math/functions.h"
#include "temp/math/matrix44.h"
#include "temp/math/vector3.h"
#include "temp/math/vector4.h"

namespace temp {
namespace math {
//----------------------------------------
// declaration
//----------------------------------------
template <class T>
class QuaternionBase {
 public:
  using ElementsType = std::array<T, 4>;
  using iterator = typename ElementsType::iterator;

  static const QuaternionBase kIdentity;

  static QuaternionBase axisAngle(const Vector3Base<T>& axis, T degree);

  QuaternionBase();
  explicit QuaternionBase(T x, T y, T z, T w);

  QuaternionBase(const QuaternionBase&) = default;
  QuaternionBase& operator=(const QuaternionBase&) = default;
  ~QuaternionBase() = default;

 public:
  iterator begin();
  iterator end();

  std::string toString() const;

  T& operator[](std::size_t index);
  T operator[](std::size_t index) const;

  T& x();
  T x() const;
  T& y();
  T y() const;
  T& z();
  T z() const;
  T& w();
  T w() const;

  bool operator==(const QuaternionBase& rhs) const;
  bool operator!=(const QuaternionBase& rhs) const;
  QuaternionBase operator+() const;
  QuaternionBase operator-() const;
  QuaternionBase& operator*=(T rhs);
  QuaternionBase& operator*=(const QuaternionBase& rhs);
  QuaternionBase& operator/=(T rhs);

 private:
  std::array<T, 4> elements_;
};

using Quaternion = QuaternionBase<float>;
using Quaternion_64b = QuaternionBase<double>;

template <class T>
const QuaternionBase<T> QuaternionBase<T>::kIdentity = QuaternionBase<T>(0, 0,
                                                                         0, 1);

//----------------------------------------
// implementation
//----------------------------------------
template <class T>
QuaternionBase<T> QuaternionBase<T>::axisAngle(const Vector3Base<T>& axis,
                                               T degree) {
  auto v = normalize(axis);
  auto rad = degreeToRadian(degree);
  auto rad_2 = rad / (T)2;
  return QuaternionBase<T>(v.x() * sin(rad_2), v.y() * sin(rad_2),
                           v.z() * sin(rad_2), cos(rad_2));
}

template <class T>
QuaternionBase<T>::QuaternionBase() : QuaternionBase((T)0, (T)0, (T)0, (T)0) {}

template <class T>
QuaternionBase<T>::QuaternionBase(T x, T y, T z, T w) {
  elements_[0] = x;
  elements_[1] = y;
  elements_[2] = z;
  elements_[3] = w;
}

template <class T>
typename QuaternionBase<T>::iterator QuaternionBase<T>::begin() {
  return elements_.begin();
}
template <class T>
typename QuaternionBase<T>::iterator QuaternionBase<T>::end() {
  return elements_.end();
}

template <class T>
std::string QuaternionBase<T>::toString() const {
  std::stringstream ss;
  ss << "Quaternion(" << x() << ", " << y() << ", " << z() << ", " << w()
     << ")";
  return ss.str();
}

template <class T>
T& QuaternionBase<T>::operator[](std::size_t index) {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return elements_[index];
}

template <class T>
T QuaternionBase<T>::operator[](std::size_t index) const {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return elements_[index];
}

template <class T>
T& QuaternionBase<T>::x() {
  return elements_[0];
}
template <class T>
T QuaternionBase<T>::x() const {
  return elements_[0];
}
template <class T>
T& QuaternionBase<T>::y() {
  return elements_[1];
}
template <class T>
T QuaternionBase<T>::y() const {
  return elements_[1];
}
template <class T>
T& QuaternionBase<T>::z() {
  return elements_[2];
}
template <class T>
T QuaternionBase<T>::z() const {
  return elements_[2];
}
template <class T>
T& QuaternionBase<T>::w() {
  return elements_[3];
}
template <class T>
T QuaternionBase<T>::w() const {
  return elements_[3];
}

template <class T>
bool QuaternionBase<T>::operator==(const QuaternionBase<T>& rhs) const {
  return x() == rhs.x() && y() == rhs.y() && z() == rhs.z() && w() == rhs.w();
}

template <class T>
bool QuaternionBase<T>::operator!=(const QuaternionBase<T>& rhs) const {
  return !(*this == rhs);
}

template <class T>
QuaternionBase<T> QuaternionBase<T>::operator+() const {
  return *this;
}

template <class T>
QuaternionBase<T> QuaternionBase<T>::operator-() const {
  return QuaternionBase(-x(), -y(), -z(), -w());
}

template <class T>
QuaternionBase<T>& QuaternionBase<T>::operator*=(T rhs) {
  x() *= rhs;
  y() *= rhs;
  z() *= rhs;
  w() *= rhs;
  return *this;
}

template <class T>
QuaternionBase<T>& QuaternionBase<T>::operator*=(const QuaternionBase& rhs) {
  auto l_w = w();
  auto l_x = x();
  auto l_y = y();
  auto l_z = z();
  w() = l_w * rhs.w() - l_x * rhs.x() - l_y * rhs.y() - l_z * rhs.z();
  x() = l_x * rhs.w() + l_w * rhs.x() - l_z * rhs.y() + l_y * rhs.z();
  y() = l_y * rhs.w() + l_z * rhs.x() + l_w * rhs.y() - l_x * rhs.z();
  z() = l_z * rhs.w() - l_y * rhs.x() + l_x * rhs.y() + l_w * rhs.z();
  return *this;
}

template <class T>
QuaternionBase<T>& QuaternionBase<T>::operator/=(T rhs) {
  x() /= rhs;
  y() /= rhs;
  z() /= rhs;
  w() /= rhs;
  return *this;
}

}  // namespace math
}  // namespace temp
