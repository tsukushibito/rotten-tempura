/**
 * @file matrix44.h
 * @author tsukushibito (tsukushibito@gmail.com)
 * @brief matrix4x4 class
 * @version 0.1
 * @date 2019-02-09
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#include <cmath>
#include <cstdint>

#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "temp/base/assertion.h"
#include "temp/base/define.h"

#include "temp/math/constants.h"
#include "temp/math/quaternion.h"
#include "temp/math/vector3.h"
#include "temp/math/vector4.h"

namespace temp {
namespace math {
//----------------------------------------
// declaration
//----------------------------------------
template <class T>
class Matrix44Base {
 public:
  using iterator = T*;              /// iterator type ailias
  using const_iterator = const T*;  /// const iterator type ailias
  using RowType = Vector4Base<T>;   /// row type ailias
  using ColType = Vector4Base<T>;   /// col type ailias

 public:
  static const std::size_t kRows = 4;  /// count of row
  static const std::size_t kCols = 4;  /// count of col
  static constexpr T kNan =
      std::numeric_limits<T>::quiet_NaN();  /// not a number
  static const Matrix44Base kZero;          /// zero matrix
  static const Matrix44Base kIdentity;      /// identity matrix
  static const Matrix44Base kNanMat;        /// nan matrix

 public:
  /**
   * @brief Create matrix from scale, rotation, and translation
   *
   * @param scale
   * @param rotation
   * @param translation
   * @return Matrix44Base
   */
  static Matrix44Base scaleRotationTranslation(  //
      const Vector3Base<T>& scale,               //
      const QuaternionBase<T>& rotation,         //
      const Vector3Base<T>& translation);

 public:
  Matrix44Base();
  explicit Matrix44Base(RowType row0, RowType row1, RowType row2, RowType row3);
  explicit Matrix44Base(T _00, T _01, T _02, T _03,  //
                        T _10, T _11, T _12, T _13,  //
                        T _20, T _21, T _22, T _23,  //
                        T _30, T _31, T _32, T _33);

  Matrix44Base(const Matrix44Base&) = default;
  Matrix44Base& operator=(const Matrix44Base&) = default;
  ~Matrix44Base() = default;

 public:
  iterator begin();
  iterator end();

  std::string toString() const;

  RowType& operator[](std::size_t index);
  const RowType& operator[](std::size_t index) const;

  RowType row(std::size_t index) const;
  ColType col(std::size_t index) const;

  bool operator==(const Matrix44Base& rhs) const;
  bool operator!=(const Matrix44Base& rhs) const;

  Matrix44Base operator+() const;
  Matrix44Base operator-() const;

  Matrix44Base& operator+=(const Matrix44Base& rhs);
  Matrix44Base& operator-=(const Matrix44Base& rhs);
  Matrix44Base& operator*=(float rhs);
  Matrix44Base& operator*=(const Matrix44Base rhs);
  Matrix44Base& operator/=(float rhs);

 private:
  std::array<Vector4Base<T>, kRows> rows_;
};

using Matrix44 = Matrix44Base<float>;
using Matrix44_64bit = Matrix44Base<double>;

//----------------------------------------
// implementation
//----------------------------------------
template <class T>
Matrix44Base<T> Matrix44Base<T>::scaleRotationTranslation(  //
    const Vector3Base<T>& scale,                            //
    const QuaternionBase<T>& rotation,                      //
    const Vector3Base<T>& translation) {
  auto s = Matrix44Base<T>(  //
      scale.x(), 0, 0, 0,    //
      0, scale.y(), 0, 0,    //
      0, 0, scale.z(), 0,    //
      0, 0, 0, 1);

  T x2 = rotation.x() * rotation.x();
  T y2 = rotation.y() * rotation.y();
  T z2 = rotation.z() * rotation.z();

  T xy = rotation.x() * rotation.y();
  T xz = rotation.x() * rotation.z();
  T yz = rotation.y() * rotation.z();
  T wx = rotation.w() * rotation.x();
  T wy = rotation.w() * rotation.y();
  T wz = rotation.w() * rotation.z();

  T m00 = 1 - (2 * y2) - (2 * z2);
  T m01 = (2 * xy) + (2 * wz);
  T m02 = (2 * xz) - (2 * wy);
  T m10 = (2 * xy) - (2 * wz);
  T m11 = 1 - (2 * x2) - (2 * z2);
  T m12 = (2 * yz) + (2 * wx);
  T m20 = (2 * xz) + (2 * wy);
  T m21 = (2 * yz) - (2 * wx);
  T m22 = 1 - (2 * x2) - (2 * y2);

  auto r = Matrix44Base<T>(  //
      m00, m01, m02, 0,      //
      m10, m11, m12, 0,      //
      m20, m21, m22, 0,      //
      0, 0, 0, 1);

  auto t = Matrix44Base<T>(      //
      0, 0, 0, translation.x(),  //
      0, 0, 0, translation.y(),  //
      0, 0, 0, translation.z(),  //
      0, 0, 0, 0);

  return t + (r * s);
}

template <class T>
const Matrix44Base<T> Matrix44Base<T>::kZero = Matrix44Base<T>();

template <class T>
const Matrix44Base<T> Matrix44Base<T>::kIdentity =
    Matrix44Base<T>(1, 0, 0, 0,   //
                    0, 1, 0, 0,   //
                    0, 0, 1, 0,   //
                    0, 0, 0, 1);  //

template <class T>
const Matrix44Base<T> Matrix44Base<T>::kNanMat =  //
    Matrix44Base<T>(kNan, kNan, kNan, kNan,       //
                    kNan, kNan, kNan, kNan,       //
                    kNan, kNan, kNan, kNan,       //
                    kNan, kNan, kNan, kNan);      //

template <class T>
Matrix44Base<T>::Matrix44Base()
    : Matrix44Base(0, 0, 0, 0,  //
                   0, 0, 0, 0,  //
                   0, 0, 0, 0,  //
                   0, 0, 0, 0) {}

template <class T>
Matrix44Base<T>::Matrix44Base(RowType row0, RowType row1, RowType row2,
                              RowType row3) {
  rows_[0] = row0;
  rows_[1] = row1;
  rows_[2] = row2;
  rows_[3] = row3;
}

template <class T>
Matrix44Base<T>::Matrix44Base(T _00, T _01, T _02, T _03,  //
                              T _10, T _11, T _12, T _13,  //
                              T _20, T _21, T _22, T _23,  //
                              T _30, T _31, T _32, T _33) {
  rows_[0][0] = _00, rows_[0][1] = _01, rows_[0][2] = _02, rows_[0][3] = _03;
  rows_[1][0] = _10, rows_[1][1] = _11, rows_[1][2] = _12, rows_[1][3] = _13;
  rows_[2][0] = _20, rows_[2][1] = _21, rows_[2][2] = _22, rows_[2][3] = _23;
  rows_[3][0] = _30, rows_[3][1] = _31, rows_[3][2] = _32, rows_[3][3] = _33;
}

template <class T>
typename Matrix44Base<T>::iterator Matrix44Base<T>::begin() {
  rows_[0].begin();
}

template <class T>
typename Matrix44Base<T>::iterator Matrix44Base<T>::end() {
  rows_[kRows].end();
}

template <class T>
std::string Matrix44Base<T>::toString() const {
  std::stringstream ss;
  ss << "Matrix44(" << std::endl << std::fixed << std::setprecision(4);
  ss << rows_[0][0] << ", " << rows_[0][1] << ", " << rows_[0][2] << ", "
     << rows_[0][3] << "," << std::endl;
  ss << rows_[1][0] << ", " << rows_[1][1] << ", " << rows_[1][2] << ", "
     << rows_[1][3] << "," << std::endl;
  ss << rows_[2][0] << ", " << rows_[2][1] << ", " << rows_[2][2] << ", "
     << rows_[2][3] << "," << std::endl;
  ss << rows_[3][0] << ", " << rows_[3][1] << ", " << rows_[3][2] << ", "
     << rows_[3][3] << ")";
  return ss.str();
}

template <class T>
typename Matrix44Base<T>::RowType& Matrix44Base<T>::operator[](
    std::size_t index) {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return rows_[index];
}

template <class T>
const typename Matrix44Base<T>::RowType& Matrix44Base<T>::operator[](
    std::size_t index) const {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return rows_[index];
}

template <class T>
typename Matrix44Base<T>::RowType Matrix44Base<T>::row(
    std::size_t index) const {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return rows_[index];
}

template <class T>
typename Matrix44Base<T>::ColType Matrix44Base<T>::col(
    std::size_t index) const {
  TEMP_ASSERT(index < 4, "index must be less than 4");
  return ColType(rows_[0][index], rows_[1][index], rows_[2][index],
                 rows_[3][index]);
}

template <class T>
bool Matrix44Base<T>::operator==(const Matrix44Base<T>& rhs) const {
  for (auto i = 0; i < kRows; ++i) {
    if (rows_[i] != rhs[i]) return false;
  }
  return true;
}

template <class T>
bool Matrix44Base<T>::operator!=(const Matrix44Base<T>& rhs) const {
  return !(*this == rhs);
}

template <class T>
Matrix44Base<T> Matrix44Base<T>::operator+() const {
  return *this;
}

template <class T>
Matrix44Base<T> Matrix44Base<T>::operator-() const {
  return Matrix44Base(-rows_[0], -rows_[1], -rows_[2], -rows_[3]);
}

template <class T>
Matrix44Base<T>& Matrix44Base<T>::operator+=(const Matrix44Base& rhs) {
  for (auto i = 0; i < kRows; ++i) {
    rows_[i] += rhs[i];
  }
  return *this;
}

template <class T>
Matrix44Base<T>& Matrix44Base<T>::operator-=(const Matrix44Base<T>& rhs) {
  for (auto i = 0; i < kRows; ++i) {
    rows_[i] -= rhs[i];
  }
  return *this;
}

template <class T>
Matrix44Base<T>& Matrix44Base<T>::operator*=(float rhs) {
  for (auto i = 0; i < kRows; ++i) {
    rows_[i] *= rhs;
  }
  return *this;
}

template <class T>
Matrix44Base<T>& Matrix44Base<T>::operator*=(const Matrix44Base<T> rhs) {
  T _00 = dot(rows_[0], rhs.col(0));
  T _01 = dot(rows_[0], rhs.col(1));
  T _02 = dot(rows_[0], rhs.col(2));
  T _03 = dot(rows_[0], rhs.col(3));

  T _10 = dot(rows_[1], rhs.col(0));
  T _11 = dot(rows_[1], rhs.col(1));
  T _12 = dot(rows_[1], rhs.col(2));
  T _13 = dot(rows_[1], rhs.col(3));

  T _20 = dot(rows_[2], rhs.col(0));
  T _21 = dot(rows_[2], rhs.col(1));
  T _22 = dot(rows_[2], rhs.col(2));
  T _23 = dot(rows_[2], rhs.col(3));

  T _30 = dot(rows_[3], rhs.col(0));
  T _31 = dot(rows_[3], rhs.col(1));
  T _32 = dot(rows_[3], rhs.col(2));
  T _33 = dot(rows_[3], rhs.col(3));

  rows_[0][0] = _00, rows_[0][1] = _01, rows_[0][2] = _02, rows_[0][3] = _03;
  rows_[1][0] = _10, rows_[1][1] = _11, rows_[1][2] = _12, rows_[1][3] = _13;
  rows_[2][0] = _20, rows_[2][1] = _21, rows_[2][2] = _22, rows_[2][3] = _23;
  rows_[3][0] = _30, rows_[3][1] = _31, rows_[3][2] = _32, rows_[3][3] = _33;

  return *this;
}

template <class T>
Matrix44Base<T>& Matrix44Base<T>::operator/=(float rhs) {
  TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
  for (auto i = 0; i < kRows; ++i) {
    rows_[i] /= rhs;
  }
  return *this;
}

}  // namespace math
}  // namespace temp
