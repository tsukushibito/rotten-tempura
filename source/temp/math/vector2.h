#pragma once

#include <array>
#include <sstream>
#include <string>

#include "temp/base/assertion.h"
#include "temp/base/define.h"

namespace temp {
namespace math {
//----------------------------------------
// declaration
//----------------------------------------

/**
 * @brief Vector2 class
 *
 * @tparam T element type
 */
template <class T>
class Vector2Base {
 public:
  using ElementsType = std::array<T, 2>;
  using iterator = typename ElementsType::iterator;
  using const_iterator = typename ElementsType::const_iterator;

  static const Vector2Base kZero;
  static const Vector2Base kRight;
  static const Vector2Base kLeft;
  static const Vector2Base kUp;
  static const Vector2Base kDown;

  Vector2Base();
  explicit Vector2Base(T x, T y);

  Vector2Base(const Vector2Base&) = default;
  Vector2Base& operator=(const Vector2Base&) = default;
  ~Vector2Base() = default;

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

  bool operator==(const Vector2Base& rhs) const;
  bool operator!=(const Vector2Base& rhs) const;

  Vector2Base operator+() const;
  Vector2Base operator-() const;
  Vector2Base& operator+=(const Vector2Base& rhs);
  Vector2Base& operator-=(const Vector2Base& rhs);
  Vector2Base& operator*=(float rhs);
  Vector2Base& operator/=(float rhs);

 private:
  ElementsType elements_;
};

using Vector2 = Vector2Base<float>;
using Vector2_64b = Vector2Base<double>;

//----------------------------------------
// implementation
//----------------------------------------
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
Vector2Base<T>::Vector2Base() : Vector2Base((T)0, (T)0) {}

template <class T>
Vector2Base<T>::Vector2Base(T x, T y) {
  elements_[0] = x;
  elements_[1] = y;
}

template <class T>
auto Vector2Base<T>::begin() -> iterator {
  return elements_.begin();
}

template <class T>
auto Vector2Base<T>::end() -> iterator {
  return elements_.end();
}

template <class T>
std::string Vector2Base<T>::toString() const {
  std::stringstream ss;
  ss << "Vector2(" << x() << ", " << y() << ")";
  return ss.str();
}

template <class T>
T& Vector2Base<T>::operator[](std::size_t index) {
  TEMP_ASSERT(index < 2, "index must be less than 2");
  return elements_[index];
}

template <class T>
T Vector2Base<T>::operator[](std::size_t index) const {
  TEMP_ASSERT(index < 2, "index must be less than 2");
  return elements_[index];
}

template <class T>
T& Vector2Base<T>::x() {
  return elements_[0];
}
template <class T>
T Vector2Base<T>::x() const {
  return elements_[0];
}
template <class T>
T& Vector2Base<T>::y() {
  return elements_[1];
}
template <class T>
T Vector2Base<T>::y() const {
  return elements_[1];
}

template <class T>
bool Vector2Base<T>::operator==(const Vector2Base<T>& rhs) const {
  return x() == rhs.x() && y() == rhs.y();
}

template <class T>
bool Vector2Base<T>::operator!=(const Vector2Base<T>& rhs) const {
  return !(*this == rhs);
}

template <class T>
Vector2Base<T> Vector2Base<T>::operator+() const {
  return *this;
}

template <class T>
Vector2Base<T> Vector2Base<T>::operator-() const {
  return Vector2Base(-x(), -y());
}

template <class T>
Vector2Base<T>& Vector2Base<T>::operator+=(const Vector2Base& rhs) {
  x() += rhs.x();
  y() += rhs.y();
  return *this;
}

template <class T>
Vector2Base<T>& Vector2Base<T>::operator-=(const Vector2Base& rhs) {
  x() -= rhs.x();
  y() -= rhs.y();
  return *this;
}

template <class T>
Vector2Base<T>& Vector2Base<T>::operator*=(float rhs) {
  x() *= rhs;
  y() *= rhs;
  return *this;
}

template <class T>
Vector2Base<T>& Vector2Base<T>::operator/=(float rhs) {
  TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
  x() /= rhs;
  y() /= rhs;
  return *this;
}

}  // namespace math
}  // namespace temp
