#include "polynomial.h"

#include "gtest/gtest.h"

using namespace std;

namespace polynomial {
namespace testing {

TEST(PolynomialTest, Constructor) {
  double coeffs[] = {1.0, 2.0, 3.0};
  int expon[] = {1, 2, 3};
  Polynomial testpoly(3, coeffs, expon);
  testpoly.Print();
  ASSERT_EQ(3, testpoly.Head().exponent);
  ASSERT_EQ(3.0, testpoly.Head().coefficient);
}

TEST(PolynomialTest, CopyConstructor) {
  double coeffs[] = {1.0, 2.0, 3.0};
  int expon[] = {1, 2, 3};
  Polynomial testpoly1(3, coeffs, expon);
  testpoly1.Print();
  Polynomial testpoly2(testpoly1);
  testpoly2.Print();
  ASSERT_EQ(testpoly2.Head().exponent, testpoly1.Head().exponent);
  ASSERT_EQ(testpoly2.Head().coefficient, testpoly1.Head().coefficient);
}

TEST(PolynomialTest, ReverseTest) {
  double coeffs[] = {1.0, 2.0, 3.0};
  int expon[] = {1, 2, 3};
  Polynomial testpoly(3, coeffs, expon);
  testpoly.Print();
  testpoly.Reverse();
  testpoly.Print();
  ASSERT_EQ(1, testpoly.Head().exponent);
  ASSERT_EQ(1.0, testpoly.Head().coefficient);
  testpoly.Reverse();
  ASSERT_EQ(3, testpoly.Head().exponent);
  ASSERT_EQ(3.0, testpoly.Head().coefficient);
}

TEST(PolynomialTest, ReverseEmpty) {
  Polynomial testpoly;
  testpoly.Print();
  testpoly.Reverse();
}

TEST(PolynomialTest, ReverseOneTerm) {
  double coeffs[] = {1.0};
  int expon[] = {1};
  Polynomial testpoly(1, coeffs, expon);
  testpoly.Print();
  ASSERT_EQ(1, testpoly.Head().exponent);
  ASSERT_EQ(1.0, testpoly.Head().coefficient);
  testpoly.Reverse();
  testpoly.Print();
  ASSERT_EQ(1, testpoly.Head().exponent);
  ASSERT_EQ(1.0, testpoly.Head().coefficient);
}
}
}
