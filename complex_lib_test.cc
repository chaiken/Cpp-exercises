#include "complex.h"

#include "gtest/gtest.h"

#include <cmath>

#include <iostream>

using namespace std;

namespace complex {
namespace testing {

TEST(ComplexLibTest, DoubleTest) {
  Complex first(3.0, 4.0);
  cout << first << endl;
  ;
  EXPECT_EQ(sqrt(25.0), double(first));
  -first;
  EXPECT_EQ(sqrt(25.0), double(first));
  // overloaded << operator.
  // It was not obvious that "<< endl" would still work, but type resolution
  // will choose the correct match.
  cout << first << endl;
}

TEST(ComplexLibTest, DotProductTest) {
  Complex first(3.0, 4.0);
  Complex second(3.0, 4.0);
  double amplitude = double(first) * double(first);
  EXPECT_EQ(amplitude, first.DotProduct(second));
  -second;
  EXPECT_EQ(-1 * amplitude, first.DotProduct(second));
}

TEST(ComplexLibTest, DotProductTest2) {
  Complex first(3.0, 4.0);
  Complex second(3.0, 4.0);
  double amplitude = double(first) * double(first);
  EXPECT_EQ(amplitude, Dot(first, second));
  -second;
  EXPECT_EQ(-1 * amplitude, Dot(first, second));
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

// Also tests overloaded == operator.
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

// No need to create Complex::operator=() or friend operator=(),
// as compiler-generated assignment operator works fine.
TEST(ComplexLibTest, AssignmentTest) {
  Complex first(3.0, 4.0);
  cout << first << endl;
  Complex second(-1.0, -1.0);
  // Also works: second=(first);
  cout << second << endl;
  second = first;
  cout << second << endl;
  EXPECT_TRUE(second == first);
  double d = first;
  EXPECT_FALSE(3.0 == d);
  EXPECT_TRUE(5.0 == d);
}

TEST(ComplexLibTest, InequalityTest) {
  Complex first(3.0, 4.0);
  Complex second(-1.0, -1.0);
  EXPECT_TRUE(first != second);
}

TEST(ComplexLibTest, SqrtTest) {
  EXPECT_EQ(0.0, sqrt(Complex(0.0, 0.0)));
  double temp = 9.0 + sqrt(97.0);
  double denom = sqrt(2.0 * temp);
  // ceil() is needed because comparing doubles is fundamentally stupid.
  EXPECT_EQ((ceil(Complex(temp / denom, 4.0 / denom))),
            ceil(sqrt(Complex(9.0, 4.0))));
}

} // namespace testing
} // namespace complex
