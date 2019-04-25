#pragma once

#include <cmath>

#include <array>
#include <sstream>
#include <string>

#include "temp/base/assertion.h"
#include "temp/base/define.h"

#include "temp/math/constants.h"
#include "temp/math/vector2.h"

namespace temp {
namespace math {

template <class T>
class Vector3Base {
 public:
  using ElementsType = std::array<T, 3>;
  using iterator = typename ElementsType::iterator;
  using const_iterator = typename ElementsType::const_iterator;

  static const Vector3Base kZero;
  static const Vector3Base kRight;
  static const Vector3Base kLeft;
  static const Vector3Base kUp;
  static const Vector3Base kDown;
  static const Vector3Base kForward;
  static const Vector3Base kBackward;

  Vector3Base();
  explicit Vector3Base(T x, T y, T z);
  explicit Vector3Base(const Vector2Base<T> vec2, T z);

  Vector3Base(const Vector3Base&) = default;
  Vector3Base& operator=(const Vector3Base&) = default;
  ~Vector3Base() = default;

 public:
  iterator begin();
  iterator end();

  const iterator begin() const;
  const iterator end() const;

  std::string toString() const;
  T& operator[](std::size_t index);
  T operator[](std::size_t index) const;

  T& x();
  T x() const;
  T& y();
  T y() const;
  T& z();
  T z() const;

  bool operator==(const Vector3Base& rhs) const;
  bool operator!=(const Vector3Base& rhs) const;
  Vector3Base operator+() const;
  Vector3Base operator-() const;
  Vector3Base& operator+=(const Vector3Base& rhs);
  Vector3Base& operator-=(const Vector3Base& rhs);
  Vector3Base& operator*=(float rhs);
  Vector3Base& operator/=(float rhs);

 private:
  ElementsType elements_;
};

using Vector3 = Vector3Base<float>;
using Vector3_64b = Vector3Base<double>;

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
Vector3Base<T>::Vector3Base() : Vector3Base((T)0, (T)0, (T)0) {}
template <class T>
Vector3Base<T>::Vector3Base(T x, T y, T z) {
  elements_[0] = x;
  elements_[1] = y;
  elements_[2] = z;
}
template <class T>
Vector3Base<T>::Vector3Base(const Vector2Base<T> vec2, T z)
    : Vector3Base(vec2.x(), vec2.y(), z) {}

template <class T>
auto Vector3Base<T>::begin() -> iterator {
  return elements_.begin();
}

template <class T>
auto Vector3Base<T>::end() -> iterator {
  return elements_.end();
}

template <class T>
std::string Vector3Base<T>::toString() const {
  std::stringstream ss;
  ss << "Vector3(" << x() << ", " << y() << ", " << z() << ")";
  return ss.str();
}

template <class T>
T& Vector3Base<T>::operator[](std::size_t index) {
  TEMP_ASSERT(index < 3, "index must be less than 3");
  return elements_[index];
}

template <class T>
T Vector3Base<T>::operator[](std::size_t index) const {
  TEMP_ASSERT(index < 3, "index must be less than 3");
  return elements_[index];
}

template <class T>
T& Vector3Base<T>::x() {
  return elements_[0];
}
template <class T>
T Vector3Base<T>::x() const {
  return elements_[0];
}
template <class T>
T& Vector3Base<T>::y() {
  return elements_[1];
}
template <class T>
T Vector3Base<T>::y() const {
  return elements_[1];
}
template <class T>
T& Vector3Base<T>::z() {
  return elements_[2];
}
template <class T>
T Vector3Base<T>::z() const {
  return elements_[2];
}

template <class T>
bool Vector3Base<T>::operator==(const Vector3Base& rhs) const {
  return x() == rhs.x() && y() == rhs.y() && z() == rhs.z();
}

template <class T>
bool Vector3Base<T>::operator!=(const Vector3Base& rhs) const {
  return !(*this == rhs);
}

template <class T>
Vector3Base<T> Vector3Base<T>::operator+() const {
  return *this;
}

template <class T>
Vector3Base<T> Vector3Base<T>::operator-() const {
  return Vector3Base(-x(), -y(), -z());
}

template <class T>
Vector3Base<T>& Vector3Base<T>::operator+=(const Vector3Base& rhs) {
  x() += rhs.x();
  y() += rhs.y();
  z() += rhs.z();
  return *this;
}

template <class T>
Vector3Base<T>& Vector3Base<T>::operator-=(const Vector3Base& rhs) {
  x() -= rhs.x();
  y() -= rhs.y();
  z() -= rhs.z();
  return *this;
}

template <class T>
Vector3Base<T>& Vector3Base<T>::operator*=(float rhs) {
  x() *= rhs;
  y() *= rhs;
  z() *= rhs;
  return *this;
}

template <class T>
Vector3Base<T>& Vector3Base<T>::operator/=(float rhs) {
  TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
  x() /= rhs;
  y() /= rhs;
  z() /= rhs;
  return *this;
}

}  // namespace math
}  // namespace temp
