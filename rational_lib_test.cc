#include "rational.h"

#include "gtest/gtest.h"

using namespace std;

namespace rational {
namespace testing {

TEST(RationalLibTest, GCDSimple) {
  EXPECT_EQ(4, GCD(540,8));
}

TEST(RationalLibTest, GCDWithOne) {
  EXPECT_EQ(1, GCD(1,540));
}

TEST(RationalLibTest, GCDWithZero) {
  EXPECT_EQ(0, GCD(0,540));
}

TEST(RationalLibTest, GCDWithEquals) {
  EXPECT_EQ(12, GCD(12,12));
}

TEST(RationalLibTest, IntGreater) {
  int i = 10, j = 5;
  EXPECT_EQ(i, Greater(i, j));
}

TEST(RationalLibTest, FloatDoubleGreater) {
  float x = 7.0;
  double y = 14.5;
  EXPECT_EQ(y, Greater(x, y));
}

TEST(RationalLibTest, RationalGreater) {
  Rational w(10), z(3.5), zmax(10);
  w.Print();
  z.Print();
  EXPECT_EQ(zmax, Greater(w, z));
  zmax.Print();
}

TEST(RationalLibTest, RationalIntGreater) {
  int i = 10;
  Rational z(3.5);
  // error: call of overloaded ‘Greater(int&, rational::Rational&)’ is ambiguous
  // EXPECT_EQ(i, Greater(i,z));
  EXPECT_EQ(i, Greater(static_cast<Rational>(i),z));
}

} // namespace testing
} // namespace rational
