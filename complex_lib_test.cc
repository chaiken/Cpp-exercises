#include "complex.h"

#include "gtest/gtest.h"

#include <cmath>

#include <iostream>

using namespace std;

namespace complex {
namespace testing {

TEST(ComplexLibTest, DoubleTest) {
  Complex first(3.0, 4.0);
  first.print();
  EXPECT_EQ(sqrt(25.0), double(first));
  -first;
  EXPECT_EQ(sqrt(25.0), double(first));
  // overloaded << operator.
  cout << first;
}

TEST(ComplexLibTest, DotProductTest) {
  Complex first(3.0, 4.0);
  Complex second(3.0, 4.0);
  double amplitude = double(first) * double(first);
  EXPECT_EQ(amplitude, first.DotProduct(second));
  -second;
  EXPECT_EQ(-1 * amplitude, first.DotProduct(second));
}

TEST(ComplexLibTest, InnerAngleTest) {
  Complex first(3.0, 4.0);
  Complex second(3.0, 4.0);
  EXPECT_EQ(0, first.InnerAngle(second));
  -second;
  EXPECT_EQ(M_PI, first.InnerAngle(second));
  Complex third(4.0, -3.0);
  EXPECT_EQ(M_PI_2, first.InnerAngle(third));
}

} // namespace testing
} // namespace complex
