#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "temp/math/temp_math.h"

using namespace temp::math;
namespace utf = boost::unit_test;

struct F {
  F() { BOOST_TEST_MESSAGE("setup fixture"); }
  ~F() { BOOST_TEST_MESSAGE("teardown fixture"); }

  Vector2 v_0_0 = Vector2();
  Vector2 v_1_0 = Vector2(1, 0);
  Vector2 v_0_1 = Vector2(0, 1);
  Vector2 v_1_1 = Vector2(1, 1);
  Vector2 v_1_2 = Vector2(1, 2);
  Vector2 v_2_1 = Vector2(2, 1);
  Vector2 v_2_2 = Vector2(2, 2);
};

BOOST_FIXTURE_TEST_SUITE(vector2, F)

BOOST_AUTO_TEST_CASE(add, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(v_0_0 + v_0_0, v_0_0);
  BOOST_CHECK_EQUAL(v_0_0 + v_1_1, v_1_1);
  BOOST_CHECK_EQUAL(v_1_0 + v_0_1, v_1_1);
}

BOOST_AUTO_TEST_CASE(sub, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(v_0_0 - v_0_0, v_0_0);
  BOOST_CHECK_EQUAL(v_1_0 - v_1_0, v_0_0);
  BOOST_CHECK_EQUAL(v_1_1 - v_1_0, v_0_1);
  BOOST_CHECK_EQUAL(v_1_1 - v_2_2, Vector2(-1, -1));
}

BOOST_AUTO_TEST_CASE(mul, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(v_0_0 * 2, v_0_0);
  BOOST_CHECK_EQUAL(2 * v_0_0, v_0_0);
  BOOST_CHECK_EQUAL(v_1_1 * 2, v_2_2);
  BOOST_CHECK_EQUAL(v_2_2 * 0.5, v_1_1);
}

BOOST_AUTO_TEST_CASE(div, *utf::tolerance(0.00001f)) {
  BOOST_CHECK_EQUAL(v_2_2 / 1, v_2_2);
  BOOST_CHECK_EQUAL(v_2_2 / 2, v_1_1);
}

BOOST_AUTO_TEST_CASE(mag, *utf::tolerance(0.00001f)) {
  auto smag = v_1_1.squared_magnitude();
  BOOST_CHECK_EQUAL(smag, 2);
  auto mag = v_1_1.magnitude();
  BOOST_CHECK_EQUAL(mag, std::sqrt(2.0f));
}

BOOST_AUTO_TEST_CASE(nor, *utf::tolerance(0.00001f)) {
  auto vec2 = Vector2(3, 3);
  auto normalized = vec2.normalized();
  BOOST_CHECK_EQUAL(normalized.magnitude(), 1.0f);
  vec2.normalize();
  BOOST_CHECK_EQUAL(vec2.magnitude(), 1.0f);
}

BOOST_AUTO_TEST_CASE(dot_, *utf::tolerance(0.00001f)) {
  auto dot0 = dot(v_1_0, v_0_1);
  BOOST_CHECK_EQUAL(dot0, 0.0f);
  auto dot1 = dot(v_1_0, v_1_0);
  BOOST_CHECK_EQUAL(dot1, 1.0f);
  auto dot2 = dot(v_1_0, v_1_1);
  BOOST_CHECK_EQUAL(dot2, 1.0f);
}

BOOST_AUTO_TEST_SUITE_END()