#pragma once

#include <cmath>
#include <cstdint>

#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "temp/core/assertion.h"
#include "temp/core/define.h"

#include "temp/math/constants.h"
#include "temp/math/vector3.h"
#include "temp/math/vector4.h"

namespace temp {
namespace math {
template <class T>
class Matrix44Base {
 public:
  static const std::size_t kRows = 4;
  static const std::size_t kCols = 4;
  static constexpr T kNan = std::numeric_limits<T>::quiet_NaN();
  static const Matrix44Base kZero;
  static const Matrix44Base kIdentity;
  static const Matrix44Base kNanMat;

  using iterator = T*;
  using const_iterator = const T*;
  using RowType = Vector4Base<T>;
  using ColType = Vector4Base<T>;

 public:
  Matrix44Base()
      : Matrix44Base(0, 0, 0, 0,  //
                     0, 0, 0, 0,  //
                     0, 0, 0, 0,  //
                     0, 0, 0, 0) {}
  explicit Matrix44Base(RowType row0, RowType row1, RowType row2,
                        RowType row3) {
    rows_[0] = row0;
    rows_[1] = row1;
    rows_[2] = row2;
    rows_[3] = row3;
  }
  explicit Matrix44Base(T _00, T _01, T _02, T _03,  //
                        T _10, T _11, T _12, T _13,  //
                        T _20, T _21, T _22, T _23,  //
                        T _30, T _31, T _32, T _33) {
    rows_[0][0] = _00, rows_[0][1] = _01, rows_[0][2] = _02, rows_[0][3] = _03;
    rows_[1][0] = _10, rows_[1][1] = _11, rows_[1][2] = _12, rows_[1][3] = _13;
    rows_[2][0] = _20, rows_[2][1] = _21, rows_[2][2] = _22, rows_[2][3] = _23;
    rows_[3][0] = _30, rows_[3][1] = _31, rows_[3][2] = _32, rows_[3][3] = _33;
  }

  Matrix44Base(const Matrix44Base&) = default;
  ~Matrix44Base() = default;
  Matrix44Base& operator=(const Matrix44Base&) = default;

 public:
  iterator begin() { rows_[0].begin(); }
  iterator end() { rows_[kRows].end(); }

  std::string toString() const {
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

  RowType& operator[](std::size_t index) {
    TEMP_ASSERT(index < 4, "index must be less than 4");
    return rows_[index];
  }

  const RowType& operator[](std::size_t index) const {
    TEMP_ASSERT(index < 4, "index must be less than 4");
    return rows_[index];
  }

  RowType row(std::size_t index) const {
    TEMP_ASSERT(index < 4, "index must be less than 4");
    return rows_[index];
  }
  ColType col(std::size_t index) const {
    TEMP_ASSERT(index < 4, "index must be less than 4");
    return ColType(rows_[0][index], rows_[1][index], rows_[2][index],
                   rows_[3][index]);
  }

  bool operator==(const Matrix44Base& rhs) const {
    for (auto i = 0; i < kRows; ++i) {
      if (rows_[i] != rhs[i]) return false;
    }
    return true;
  }

  bool operator!=(const Matrix44Base& rhs) const { return !(*this == rhs); }

  Matrix44Base operator+() const { return *this; }

  Matrix44Base operator-() const {
    return Matrix44Base(-rows_[0], -rows_[1], -rows_[2], -rows_[3]);
  }

  Matrix44Base& operator+=(const Matrix44Base& rhs) {
    for (auto i = 0; i < kRows; ++i) {
      rows_[i] += rhs[i];
    }
    return *this;
  }

  Matrix44Base& operator-=(const Matrix44Base& rhs) {
    for (auto i = 0; i < kRows; ++i) {
      rows_[i] -= rhs[i];
    }
    return *this;
  }

  Matrix44Base& operator*=(float rhs) {
    for (auto i = 0; i < kRows; ++i) {
      rows_[i] *= rhs;
    }
    return *this;
  }

  Matrix44Base& operator*=(const Matrix44Base rhs) {
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

  Matrix44Base& operator/=(float rhs) {
    TEMP_ASSERT(rhs != 0.0f, "rhs must not be zero");
    for (auto i = 0; i < kRows; ++i) {
      rows_[i] /= rhs;
    }
    return *this;
  }

  T cofactor(std::size_t row, std::size_t col) const {
    T m[3][3] = {};
    int i = 0;
    int j = 0;
    for (int r = 0; r < kRows; ++r) {
      if (r == row) {
        continue;
      }
      for (int c = 0; c < kCols; ++c) {
        if (c == col) {
          continue;
        }
        m[i][j] = (*this)[r][c];
        ++j;
      }
      ++i;
      j = 0;
    }

    auto m00m11m22 = m[0][0] * m[1][1] * m[2][2];
    auto m01m12m20 = m[0][1] * m[1][2] * m[2][0];
    auto m02m10m21 = m[0][2] * m[1][0] * m[2][1];
    auto m00m12m21 = m[0][0] * m[1][2] * m[2][1];
    auto m01m10m22 = m[0][1] * m[1][0] * m[2][2];
    auto m02m11m20 = m[0][2] * m[1][1] * m[2][0];

    auto d =
        m00m11m22 + m01m12m20 + m02m10m21 - m00m12m21 - m01m10m22 - m02m11m20;
    return ((row + col) % 2) == 0 ? d : -d;
  }

  T determinant() const {
    auto d = 0;
    for (int i = 0; i < kRows; ++i) {
      d += rows_[i][0] * cofactor(i, 0);
    }

    return d;
  }

  Matrix44Base inverse() const {
    auto d = determinant();
    if (d == 0) {
      return kNanMat;
    }

    Matrix44Base cofactorMatrix;
    for (int r = 0; r < kRows; ++r) {
      for (int c = 0; c < kCols; ++c) {
        cofactorMatrix[c][r] = cofactor(r, c);
      }
    }

    return cofactorMatrix / d;
  }

  Matrix44Base transpose() const {
    return Matrix44Base(rows_[0][0], rows_[1][0], rows_[2][0], rows_[3][0],  //
                        rows_[0][1], rows_[1][1], rows_[2][1], rows_[3][1],  //
                        rows_[0][2], rows_[1][2], rows_[2][2], rows_[3][2],  //
                        rows_[0][3], rows_[1][3], rows_[2][3], rows_[3][3]);
  }

 private:
  std::array<Vector4Base<T>, kRows> rows_;
};

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
std::ostream& operator<<(std::ostream& stream, const Matrix44Base<T>& mat) {
  stream << mat.toString();
  return stream;
}

template <class T>
Matrix44Base<T> operator+(const Matrix44Base<T>& lhs,
                          const Matrix44Base<T>& rhs) {
  return Matrix44Base<T>(lhs) += rhs;
}

template <class T>
Matrix44Base<T> operator-(const Matrix44Base<T>& lhs,
                          const Matrix44Base<T>& rhs) {
  return Matrix44Base<T>(lhs) -= rhs;
}

template <class T>
Matrix44Base<T> operator*(const Matrix44Base<T>& lhs, float rhs) {
  return Matrix44Base<T>(lhs) *= rhs;
}

template <class T>
Matrix44Base<T> operator*(float lhs, const Matrix44Base<T>& rhs) {
  return rhs * lhs;
}

template <class T>
Matrix44Base<T> operator*(const Matrix44Base<T>& lhs,
                          const Matrix44Base<T>& rhs) {
  return Matrix44Base<T>(lhs) *= rhs;
}

template <class T>
Vector4Base<T> operator*(const Matrix44Base<T>& lhs,
                         const Vector4Base<T>& rhs) {
  return Vector4Base<T>(dot(lhs.row(0, rhs)), dot(lhs.row(1, rhs)),
                        dot(lhs.row(2, rhs)), dot(lhs.row(3, rhs)));
}

template <class T>
Matrix44Base<T> operator/(const Matrix44Base<T>& lhs, float rhs) {
  return Matrix44Base<T>(lhs) /= rhs;
}

using Matrix44 = Matrix44Base<float>;
using Matrix44_64bit = Matrix44Base<double>;

}  // namespace math
}  // namespace temp
