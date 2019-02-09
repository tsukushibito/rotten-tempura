#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "temp/core/debug_out.h"
#include "temp/math/temp_math.h"

using namespace temp::math;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(vector2)

BOOST_AUTO_TEST_CASE(add, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector2() + Vector2(), Vector2());
  BOOST_CHECK_EQUAL(Vector2(0.0f, 1.0f) + Vector2(1.0f, 0.0f),
                    Vector2(1.0f, 1.0f));
}

BOOST_AUTO_TEST_CASE(sub, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector2(1.0f, 1.0f) - Vector2(2.0f, 2.0f), Vector2(-1, -1));
}

BOOST_AUTO_TEST_CASE(mul, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector2() * 2, Vector2());
  BOOST_CHECK_EQUAL(2 * Vector2(), Vector2());
  BOOST_CHECK_EQUAL(Vector2(1.0f, 2.0f) * 2, Vector2(2.0f, 4.0f));
  BOOST_CHECK_EQUAL(Vector2(2.0f, 1.0f) * 0.5, Vector2(1.0f, 0.5f));
}

BOOST_AUTO_TEST_CASE(div, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector2() / 1, Vector2());
  BOOST_CHECK_EQUAL(Vector2(2.0f, 1.0f) / 2, Vector2(1.0f, 0.5f));
}

BOOST_AUTO_TEST_CASE(mag, *utf::tolerance(0.00001f)) {
  auto smag = squared_magnitude(Vector2(1.0f, 1.0f));
  BOOST_CHECK_EQUAL(smag, 2);
  auto mag = magnitude(Vector2(1.0f, 1.0f));
  BOOST_CHECK_EQUAL(mag, std::sqrt(2.0f));
}

BOOST_AUTO_TEST_CASE(nor, *utf::tolerance(0.00001f)) {
  auto vec2 = Vector2(3.0f, 3.0f);
  auto normalized = normalize(vec2);
  BOOST_CHECK_EQUAL(magnitude(normalized), 1.0f);
  vec2 = normalize(vec2);
  BOOST_CHECK_EQUAL(magnitude(vec2), 1.0f);
}

BOOST_AUTO_TEST_CASE(dot_, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(dot(Vector2(1, 0), Vector2(0, 1)), 0.0f);
  BOOST_CHECK_EQUAL(dot(Vector2(1, 0), Vector2(1, 0)), 1.0f);
  BOOST_CHECK_EQUAL(dot(Vector2(1, 0), Vector2(-1, 0)), -1.0f);
}

BOOST_AUTO_TEST_CASE(dist, *utf::tolerance(0.00001f)) {
  auto d = distance(Vector2(0, 3), Vector2(4, 0));
  BOOST_CHECK_EQUAL(d, 5);
}

BOOST_AUTO_TEST_CASE(ang, *utf::tolerance(0.00001f)) {
  auto deg = angle(Vector2(1.0f, 0.0f), Vector2(0.0f, 1.0f));
  BOOST_CHECK_EQUAL(deg, 90.0f);
}

BOOST_AUTO_TEST_CASE(ler, *utf::tolerance(0.00001f)) {
  auto r = lerp(Vector2(), Vector2(2.0f, 4.0f), 0.5f);
  BOOST_CHECK_EQUAL(r, Vector2(1.0f, 2.0f));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(vector3)

BOOST_AUTO_TEST_CASE(add, *utf::tolerance(0.00001f)) {
  auto v0 = Vector3();
  auto v1 = Vector3(1.0f, 2.0f, 3.0f);
  auto v2 = Vector3(3.0f, 4.0f, 5.0f);
  auto r = v1 + v2;
  BOOST_CHECK_EQUAL(r, Vector3(4.0f, 6.0f, 8.0f));
}

BOOST_AUTO_TEST_CASE(sub, *utf::tolerance(0.00001f)) {
  auto v0 = Vector3();
  auto v1 = Vector3(1.0f, 2.0f, 3.0f);
  auto v2 = Vector3(3.0f, 4.0f, 5.0f);
  auto r = v2 - v1;
  BOOST_CHECK_EQUAL(r, Vector3(2.0f, 2.0f, 2.0f));
}

BOOST_AUTO_TEST_CASE(mul, *utf::tolerance(0.00001f)) {
  auto v1 = Vector3(1.0f, 2.0f, 3.0f);
  auto r = v1 * 2.0f;
  BOOST_CHECK_EQUAL(r, Vector3(2.0f, 4.0f, 6.0f));
  auto v2 = Vector3(3.0f, 4.0f, 5.0f);
  r = 2.0f * v2;
  BOOST_CHECK_EQUAL(r, Vector3(6.0f, 8.0f, 10.0f));
}

BOOST_AUTO_TEST_CASE(div, *utf::tolerance(0.00001f)) {
  auto v1 = Vector3(1.0f, 2.0f, 3.0f);
  auto r = v1 / 2.0f;
  BOOST_CHECK_EQUAL(r, Vector3(0.5f, 1.0f, 1.5f));
}

BOOST_AUTO_TEST_CASE(mag, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(squared_magnitude(Vector3(1.0f, 1.0f, 1.0f)), 3.0f);
  BOOST_CHECK_EQUAL(magnitude(Vector3(1.0f, 1.0f, 1.0f)), std::sqrt(3.0f));
}

BOOST_AUTO_TEST_CASE(nor, *utf::tolerance(0.00001f)) {
  auto root3 = std::sqrt(3.0f);
  BOOST_CHECK_EQUAL(normalize(Vector3(1.0f, 1.0f, 1.0f)),
                    Vector3(1.0f / root3, 1.0f / root3, 1.0f / root3));
  BOOST_CHECK_EQUAL(normalize(Vector3(3.0f, 2.0f, std::sqrt(3.0f))),
                    Vector3(3.0f / 4.0f, 0.5f, std::sqrt(3.0f) / 4.0f));
}

BOOST_AUTO_TEST_CASE(ang, *utf::tolerance(0.00001f)) {
  auto deg =
      unsigned_angle(Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
  BOOST_CHECK_EQUAL(deg, 90.0f);
  deg = angle(Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f),
              Vector3(0, 1, 0));
  BOOST_CHECK_EQUAL(deg, -90.0f);
}

BOOST_AUTO_TEST_CASE(dot_, *utf::tolerance(0.00001f)) {
  auto d = dot(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
  BOOST_CHECK_EQUAL(d, 0.0f);
}

BOOST_AUTO_TEST_CASE(cross_, *utf::tolerance(0.00001f)) {
  auto c = cross(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
  BOOST_CHECK_EQUAL(c, Vector3(0.0f, 0.0f, 1.0f));
}

BOOST_AUTO_TEST_CASE(dist, *utf::tolerance(0.00001f)) {
  auto d = distance(Vector3(1.0f, 3.0f, 2.0f), Vector3(0.0f, -2.0f, 1.0f));
  BOOST_CHECK_EQUAL(d, std::sqrt(27.0f));
}

BOOST_AUTO_TEST_CASE(ler, *utf::tolerance(0.00001f)) {
  auto r = lerp(Vector3(), Vector3(2.0f, 4.0f, 3.0f), 0.5f);
  BOOST_CHECK_EQUAL(r, Vector3(1.0f, 2.0f, 1.5f));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(vector4)

BOOST_AUTO_TEST_CASE(add, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector4() + Vector4(), Vector4());
  BOOST_CHECK_EQUAL(Vector4(1, 0, 2, 3) + Vector4(0, 2, 1, 0),
                    Vector4(1, 2, 3, 3));
}

BOOST_AUTO_TEST_CASE(sub, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector4(1, 1, 1, 1) - Vector4(2, 2, 2, 2),
                    Vector4(-1, -1, -1, -1));
}

BOOST_AUTO_TEST_CASE(mul, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector4() * 2, Vector4());
  BOOST_CHECK_EQUAL(2 * Vector4(), Vector4());
  BOOST_CHECK_EQUAL(Vector4(1, 2, 3, 4) * 2, Vector4(2, 4, 6, 8));
  BOOST_CHECK_EQUAL(Vector4(2, 5, 4, 1) * 0.5, Vector4(1, 2.5f, 2, 0.5f));
}

BOOST_AUTO_TEST_CASE(div, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(Vector4() / 1, Vector4());
  BOOST_CHECK_EQUAL(Vector4(2, 1, 0, 4) / 2, Vector4(1, 0.5, 0, 2));
}

BOOST_AUTO_TEST_CASE(mag, *utf::tolerance(0.00001f)) {
  auto smag = squared_magnitude(Vector4(1, 1, 1, 1));
  BOOST_CHECK_EQUAL(smag, 4);
  auto mag = magnitude(Vector4(1, 1, 1, 1));
  BOOST_CHECK_EQUAL(mag, 2);
}

BOOST_AUTO_TEST_CASE(nor, *utf::tolerance(0.00001f)) {
  auto vec4 = Vector4(3, 3, 1, 1);
  auto normalized = normalize(vec4);
  BOOST_TEST(magnitude(normalized) == 1.0f);
}

BOOST_AUTO_TEST_CASE(foreach_) {
  auto vec0 = Vector4(4, 3, 2, 1);
  auto vec1 = Vector4(4, 3, 2, 1);
  BOOST_CHECK_EQUAL_COLLECTIONS(vec0.begin(), vec0.end(), vec1.begin(),
                                vec1.end());
}

BOOST_AUTO_TEST_CASE(dir) {
  auto d = dot(Vector2::kRight, Vector2::kUp);
  BOOST_CHECK_EQUAL(d, 0.0f);
  auto c = cross(Vector3::kRight, Vector3::kUp);
  BOOST_CHECK_EQUAL(c, Vector3::kForward);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(matrix)
BOOST_AUTO_TEST_CASE(add) {
  auto mat0 = Matrix44::kZero;
  auto mat1 = Matrix44::kIdentity;
  mat0 += mat1;
  BOOST_CHECK_EQUAL(mat0, mat1);
}

BOOST_AUTO_TEST_CASE(det) {
  auto mat0 = Matrix44::kZero;
  auto mat1 = Matrix44::kIdentity;
  auto mat2 = Matrix44(  //
      1, 1, 1, -1,       //
      1, 1, -1, 1,       //
      1, -1, 1, 1,       //
      -1, 1, 1, 1);
  auto mat3 = Matrix44(  //
      3, 1, 1, 2,        //
      5, 1, 3, 4,        //
      2, 0, 1, 0,        //
      1, 3, 2, 1);
  BOOST_CHECK_EQUAL(determinant(mat0), 0);
  BOOST_CHECK_EQUAL(determinant(mat1), 1);
  BOOST_CHECK_EQUAL(determinant(mat2), -16);
  BOOST_CHECK_EQUAL(determinant(mat3), -22);
}

BOOST_AUTO_TEST_CASE(inv) {
  auto mat0 = Matrix44(  //
      3, 1, 1, 2,        //
      5, 1, 3, 4,        //
      2, 0, 1, 0,        //
      1, 3, 2, 1);
  auto id = inverse(mat0) * mat0;
  auto offset = Matrix44(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
  auto mat1 = id + offset;
  auto mat2 = Matrix44::kIdentity + offset;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      BOOST_CHECK_CLOSE_FRACTION(mat1[i][j], mat2[i][j], 0.00001f);
    }
  }
}

BOOST_AUTO_TEST_CASE(transpose_) {
  auto mat0 = Matrix44(  //
      3, 1, 1, 2,        //
      5, 1, 3, 4,        //
      2, 0, 1, 0,        //
      1, 3, 2, 1);
  auto mat1 = Matrix44(  //
      3, 5, 2, 1,        //
      1, 1, 0, 3,        //
      1, 3, 1, 2,        //
      2, 4, 0, 1);
  BOOST_CHECK_EQUAL(transpose(mat0), mat1);
}

BOOST_AUTO_TEST_CASE(transform_) {
  auto scale = Vector3(1, 2, 1);
  auto rotation = Quaternion::axisAngle(Vector3(0, 1, 0), 45);
  auto translation = Vector3(0, -2, -sqrt(2.0f));
  auto mat = Matrix44::scaleRotationTranslation(scale, rotation, translation);
  auto pos = Vector3(1, 1, 1);
  pos = transform(pos, mat);
  pos += Vector3(1, 1, 1);
  BOOST_CHECK_CLOSE_FRACTION(pos.x(), 1, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(pos.y(), 1, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(pos.z(), 1, 0.00001f);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(quat)
BOOST_AUTO_TEST_CASE(mag) {
  auto quat = Quaternion(1, 2, 3, 4);
  BOOST_CHECK_CLOSE(magnitude(quat), sqrt(1.0f + 4.0f + 9.0f + 16.0f),
                    0.00001f);
}
BOOST_AUTO_TEST_CASE(normalize_) {
  auto quat = Quaternion(1, 2, 3, 4);
  auto nl = normalize(quat);
  auto nr = quat / magnitude(quat);
  BOOST_CHECK_CLOSE_FRACTION(nl.x(), nr.x(), 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(nl.y(), nr.y(), 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(nl.z(), nr.z(), 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(nl.w(), nr.w(), 0.00001f);
}
BOOST_AUTO_TEST_CASE(inv) {
  auto quat = Quaternion(1, 2, 3, 4);
  auto inv = inverse(quat);
  auto il = quat * inv;
  auto ir = Quaternion::kIdentity;
  BOOST_CHECK_CLOSE_FRACTION(il.x(), ir.x(), 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(il.y(), ir.y(), 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(il.z(), ir.z(), 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(il.w(), ir.w(), 0.00001f);
}
BOOST_AUTO_TEST_CASE(euler) {
  auto quat = Quaternion::axisAngle(Vector3(1, 0, 0), 90);
  auto euler = eulerAngles(quat);
  BOOST_CHECK_CLOSE_FRACTION(euler.x(), 90, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(euler.y(), 0, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(euler.z(), 0, 0.00001f);
  quat.x() = 0.5319757f;
  quat.y() = 0.02226007f;
  quat.z() = 0.2005621f;
  quat.w() = 0.8223631f;
  euler = eulerAngles(quat);
  BOOST_CHECK_CLOSE_FRACTION(euler.x(), 60, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(euler.y(), 30, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(euler.z(), 45, 0.00001f);
}
BOOST_AUTO_TEST_CASE(rot) {
  auto quat = Quaternion::axisAngle(Vector3(0, 1, 0), 90);
  auto pos = Vector3(1, 0, 0);
  pos = rotate(pos, quat);
  BOOST_CHECK_CLOSE_FRACTION(pos.x(), 0, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(pos.y(), 0, 0.00001f);
  BOOST_CHECK_CLOSE_FRACTION(pos.z(), -1, 0.00001f);
}
BOOST_AUTO_TEST_SUITE_END()
