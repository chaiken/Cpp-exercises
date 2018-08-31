#include "rational.h"

#include "gtest/gtest.h"

using namespace std;

namespace rational {
namespace testing {

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
