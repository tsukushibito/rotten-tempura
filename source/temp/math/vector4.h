#pragma once

#include <cmath>
#include <cstdint>

#include <array>
#include <iostream>
#include <sstream>
#include <string>

#include "temp/core/assertion.h"
#include "temp/core/define.h"

#include "temp/math/vector3.h"

namespace temp {
namespace math {

//----------------------------------------
// declaration
//----------------------------------------
template <class T>
class Vector4Base {
 public:
  using ElementsType = std::array<T, 4>;
  using iterator = typename ElementsType::iterator;
  using const_iterator = typename ElementsType::const_iterator;

  Vector4Base();
  explicit Vector4Base(T x, T y, T z, T w);
  explicit Vector4Base(const Vector3Base<T> vec3, T w);

  Vector4Base(const Vector4Base&) = default;
  ~Vector4Base() = default;
  Vector4Base& operator=(const Vector4Base&) = default;

 public:
  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

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

  bool operator==(const Vector4Base& rhs) const;
  bool operator!=(const Vector4Base& rhs) const;

  Vector4Base operator+() const;
  Vector4Base operator-() const;
  Vector4Base& operator+=(const Vector4Base& rhs);
  Vector4Base& operator-=(const Vector4Base& rhs);
  Vector4Base& operator*=(float rhs);
  Vector4Base& operator/=(float rhs);

 private:
  ElementsType elements_;
};

using Vector4 = Vector4Base<float>;
using Vector4_64b = Vector4Base<double>;

//----------------------------------------
// implementation
//----------------------------------------
template <class T>
Vector4Base<T>::Vector4Base() : Vector4Base((T)0, (T)0, (T)0, (T)0) {}
template <class T>
Vector4Base<T>::Vector4Base(T x, T y, T z, T w) {
  elements_[0] = x;
  elements_[1] = y;
  elements_[2] = z;
  elements_[3] = w;
}
template <class T>
Vector4Base<T>::Vector4Base(const Vector3Base<T> vec3, T w)
    : Vector4Base(vec3.x(), vec3.y(), vec3.z(), w) {}

template <class T>
auto Vector4Base<T>::begin() -> iterator {
  return elements_.begin();
}

template <class T>
auto Vector4Base<T>::end() -> iterator {
  return elements_.end();
}

template <class T>
std::string Vector4Base<T>::toString() const {
  std::stringstream ss;
  ss << "Vector4(" << x() << ", " << y() << ", " << z() << ", " << w() << ")";
  return ss.str();
}

template <class T>
T& Vector4Base<T>::operator[](std::size_t index) {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return elements_[index];
}

template <class T>
T Vector4Base<T>::operator[](std::size_t index) const {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return elements_[index];
}

template <class T>
T& Vector4Base<T>::x() {
  return elements_[0];
}
template <class T>
T Vector4Base<T>::x() const {
  return elements_[0];
}
template <class T>
T& Vector4Base<T>::y() {
  return elements_[1];
}
template <class T>
T Vector4Base<T>::y() const {
  return elements_[1];
}
template <class T>
T& Vector4Base<T>::z() {
  return elements_[2];
}
template <class T>
T Vector4Base<T>::z() const {
  return elements_[2];
}
template <class T>
T& Vector4Base<T>::w() {
  return elements_[3];
}
template <class T>
T Vector4Base<T>::w() const {
  return elements_[3];
}

template <class T>
bool Vector4Base<T>::operator==(const Vector4Base& rhs) const {
  return x() == rhs.x() && y() == rhs.y() && z() == rhs.z() && w() == rhs.w();
}

template <class T>
bool Vector4Base<T>::operator!=(const Vector4Base& rhs) const {
  return !(*this == rhs);
}

template <class T>
Vector4Base<T> Vector4Base<T>::operator+() const {
  return *this;
}

template <class T>
Vector4Base<T> Vector4Base<T>::operator-() const {
  return Vector4Base(-x(), -y(), -z(), -w());
}

template <class T>
Vector4Base<T>& Vector4Base<T>::operator+=(const Vector4Base& rhs) {
  x() += rhs.x();
  y() += rhs.y();
  z() += rhs.z();
  w() += rhs.w();
  return *this;
}

template <class T>
Vector4Base<T>& Vector4Base<T>::operator-=(const Vector4Base& rhs) {
  x() -= rhs.x();
  y() -= rhs.y();
  z() -= rhs.z();
  w() -= rhs.w();
  return *this;
}

template <class T>
Vector4Base<T>& Vector4Base<T>::operator*=(float rhs) {
  x() *= rhs;
  y() *= rhs;
  z() *= rhs;
  w() *= rhs;
  return *this;
}

template <class T>
Vector4Base<T>& Vector4Base<T>::operator/=(float rhs) {
  TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
  x() /= rhs;
  y() /= rhs;
  z() /= rhs;
  w() /= rhs;
  return *this;
}

}  // namespace math
}  // namespace temp
