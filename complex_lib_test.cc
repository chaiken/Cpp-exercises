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

TEST(ComplexLibTest, AddTest) {
  Complex first(3.0, 4.0);
  cout << first << endl;
  Complex second(1.0, 2.0);
  cout << second << endl;
  Complex third(4.0, 6.0);
  cout << third << endl;
  Complex fourth = first + second;
  cout << fourth << endl;
  EXPECT_TRUE(third == fourth);
}

TEST(ComplexLibTest, SubtractTest) {
  Complex first(3.0, 4.0);
  Complex third;
  cout << third << endl;
  Complex fourth = first - first;
  cout << fourth << endl;
  EXPECT_TRUE(third == fourth);
}

TEST(ComplexLibTest, MultiplyTest) {
  Complex first(3.0, 4.0);
  Complex second(1.0 / 3.0, 1.0 / 4.0);
  Complex third = first * second;
  Complex fourth(1.0, 1.0);
  EXPECT_TRUE(third == fourth);
}

TEST(ComplexLibTest, AddDoubleTest) {
  Complex first(3.0, 4.0);
  Complex second = first + -3.0;
  Complex third = 3.0 + second;
  EXPECT_FALSE(first == second);
  EXPECT_TRUE(first == third);
}

} // namespace testing
} // namespace complex
