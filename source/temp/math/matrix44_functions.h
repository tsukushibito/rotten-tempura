#pragma once
#include <cmath>
#include "temp/math/matrix44.h"

namespace temp {
namespace math {

//----------------------------------------
// declaration
//----------------------------------------

template <class T>
std::ostream& operator<<(std::ostream& stream, const Matrix44Base<T>& mat);

template <class T>
Matrix44Base<T> operator+(const Matrix44Base<T>& lhs,
                          const Matrix44Base<T>& rhs);

template <class T>
Matrix44Base<T> operator-(const Matrix44Base<T>& lhs,
                          const Matrix44Base<T>& rhs);

template <class T>
Matrix44Base<T> operator*(const Matrix44Base<T>& lhs, float rhs);

template <class T>
Matrix44Base<T> operator*(float lhs, const Matrix44Base<T>& rhs);

template <class T>
Matrix44Base<T> operator*(const Matrix44Base<T>& lhs,
                          const Matrix44Base<T>& rhs);

template <class T>
Vector4Base<T> operator*(const Matrix44Base<T>& lhs, const Vector4Base<T>& rhs);

template <class T>
Matrix44Base<T> operator/(const Matrix44Base<T>& lhs, float rhs);

template <class T>
T cofactor(const Matrix44Base<T>& mat, std::size_t row, std::size_t col);

template <class T>
T determinant(const Matrix44Base<T>& mat);

template <class T>
Matrix44Base<T> inverse(const Matrix44Base<T>& mat);

template <class T>
Matrix44Base<T> transpose(const Matrix44Base<T>& mat);

template <class T>
Vector3Base<T> transform(const Vector3Base<T>& vec3,
                         const Matrix44Base<T>& mat);

//----------------------------------------
// implementation
//----------------------------------------
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
  return Vector4Base<T>(dot(lhs.row(0), rhs), dot(lhs.row(1), rhs),
                        dot(lhs.row(2), rhs), dot(lhs.row(3), rhs));
}

template <class T>
Matrix44Base<T> operator/(const Matrix44Base<T>& lhs, float rhs) {
  return Matrix44Base<T>(lhs) /= rhs;
}

template <class T>
T cofactor(const Matrix44Base<T>& mat, std::size_t row, std::size_t col) {
  static const auto kRows = Matrix44Base<T>::kRows;
  static const auto kCols = Matrix44Base<T>::kCols;

  T m[3][3] = {};  // row行とcol列を取り除いた3x3行列
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
      m[i][j] = mat[r][c];
      ++j;
    }
    ++i;
    j = 0;
  }

  // 3x3の行列式を求める
  auto m00m11m22 = m[0][0] * m[1][1] * m[2][2];
  auto m01m12m20 = m[0][1] * m[1][2] * m[2][0];
  auto m02m10m21 = m[0][2] * m[1][0] * m[2][1];
  auto m00m12m21 = m[0][0] * m[1][2] * m[2][1];
  auto m01m10m22 = m[0][1] * m[1][0] * m[2][2];
  auto m02m11m20 = m[0][2] * m[1][1] * m[2][0];

  auto d =
      m00m11m22 + m01m12m20 + m02m10m21 - m00m12m21 - m01m10m22 - m02m11m20;

  // 正負判定して余因子を返す
  return ((row + col) % 2) == 0 ? d : -d;
}

template <class T>
T determinant(const Matrix44Base<T>& mat) {
  static const auto kRows = Matrix44Base<T>::kRows;

  // 余因子から行列式を求める
  T d = 0;
  for (int i = 0; i < kRows; ++i) {
    d += mat[i][0] * cofactor(mat, i, 0);
  }

  return d;
}

template <class T>
Matrix44Base<T> inverse(const Matrix44Base<T>& mat) {
  static const auto kRows = Matrix44Base<T>::kRows;
  static const auto kCols = Matrix44Base<T>::kCols;

  auto d = determinant(mat);
  if (d == 0) {
    // 正則行列でなければ、Nan行列を返す
    return Matrix44Base<T>::kNanMat;
  }

  // 余因子行列から逆行列を求める
  Matrix44Base<T> cofactorMatrix;
  for (int r = 0; r < kRows; ++r) {
    for (int c = 0; c < kCols; ++c) {
      cofactorMatrix[c][r] = cofactor(mat, r, c);
    }
  }

  return cofactorMatrix / d;
}

template <class T>
Matrix44Base<T> transpose(const Matrix44Base<T>& mat) {
  return Matrix44Base<T>(mat[0][0], mat[1][0], mat[2][0], mat[3][0],  //
                         mat[0][1], mat[1][1], mat[2][1], mat[3][1],  //
                         mat[0][2], mat[1][2], mat[2][2], mat[3][2],  //
                         mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
}

template <class T>
Vector3Base<T> transform(const Vector3Base<T>& vec3,
                         const Matrix44Base<T>& mat) {
  auto result = mat * Vector4Base<T>(vec3, 1);
  return Vector3Base<T>(result.x(), result.y(), result.z());
}
}  // namespace math
}  // namespace temp