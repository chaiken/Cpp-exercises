#include "complex.h"

#include "gtest/gtest.h"

#include <cmath>

#include <iostream>

using namespace std;

namespace complex {
namespace local_testing {

TEST(ComplexLibTest, DoubleTest) {
  Complex first(3.0, 4.0);
  cout << first << endl;
  // Regular C-library sqrt.
  EXPECT_EQ(std::sqrt(25.0), first.ComplexModulus());
  -first;
  EXPECT_EQ(std::sqrt(25.0), first.ComplexModulus());
  cout << first << endl;
}

TEST(ComplexLibTest, DotProductTest) {
  Complex first(3.0, 4.0);
  Complex second(3.0, 4.0);
  double amplitude = first.ComplexModulus() * first.ComplexModulus();
  EXPECT_EQ(amplitude, DotProduct(first, second));
  -second;
  EXPECT_EQ(-1 * amplitude, DotProduct(first, second));
}

TEST(ComplexLibTest, DotProductTest2) {
  Complex first(3.0, 4.0);
  Complex second(3.0, 4.0);
  double amplitude = first.ComplexModulus() * first.ComplexModulus();
  EXPECT_EQ(amplitude, Dot(first, second));
  -second;
  EXPECT_EQ(-1 * amplitude, Dot(first, second));
}

TEST(ComplexLibTest, InnerAngleTest) {
  Complex first(3.0, 4.0);
  Complex second(3.0, 4.0);
  EXPECT_EQ(0, InnerAngle(first, second));
  -second;
  EXPECT_EQ(M_PI, InnerAngle(first, second));
  Complex third(4.0, -3.0);
  EXPECT_EQ(M_PI_2, InnerAngle(first, third));
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

// Now calling a version of the operator* overload that returns a constexpr
// value.
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
  double d = first.ComplexModulus();
  EXPECT_FALSE(3.0 == d);
  EXPECT_TRUE(5.0 == d);
}

TEST(ComplexLibTest, InequalityTest) {
  Complex first(3.0, 4.0);
  Complex second(-1.0, -1.0);
  EXPECT_TRUE(first != second);
}

TEST(ComplexLibTest, SqrtTest) {
  pair<Complex, bool> res = sqrt(Complex(0.0, 0.0));
  EXPECT_EQ(true, res.second);
  EXPECT_EQ(0.0, res.first);
  double temp = 9.0 + std::sqrt(97.0);
  double denom = std::sqrt(2.0 * temp);
  // ceil() is needed because comparing doubles is fundamentally stupid.
  res = sqrt(Complex(9.0, 4.0));
  EXPECT_EQ(true, res.second);
  EXPECT_EQ((ceil(Complex(temp / denom, 4.0 / denom).ComplexModulus())),
            ceil(res.first.ComplexModulus()));
}

TEST(ComplexLibTest, ComparisonTest) {
  EXPECT_TRUE(Complex(1.0, 2.0) < Complex(2.0, 3.0));
  EXPECT_FALSE(Complex(-2.0, -3.0) < Complex(0.0, 0.0));
}

TEST(ComplexLibTest, FunctionalComparisonTest) {
  Complex a, b;
  a = Complex(1.0, 2.0);
  b = Complex(2.0, 3.0);
  // Returns (sqrt(*this) < sqrt(b)) where this is &a.
  EXPECT_TRUE(a(b));
  a = Complex(-2.0, -3.0);
  b = Complex(0.0, 0.0);
  EXPECT_FALSE(a(b));
}

TEST(ComplexLibTest, BadSqrt) {
  Complex z{-2.0, 0.0};
  pair<Complex, bool> res = sqrt(z);
  EXPECT_EQ(false, res.second);
  EXPECT_EQ(z, res.first);
}

} // namespace local_testing
} // namespace complex
