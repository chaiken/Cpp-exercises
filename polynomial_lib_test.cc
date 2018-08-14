#include "polynomial.h"

#include "gtest/gtest.h"

using namespace std;

namespace polynomial {
namespace testing {

TEST(PolynomialTest, SyncSortTest) {
  ::std::array<int, 3> expon = {1, 2, 3};
  ::std::array<double, 3> coeffs = {1.0, 2.0, 3.0};
  SyncSortTwoArrays(&expon, &coeffs, 0);
  EXPECT_EQ(expon[0], 1);
  EXPECT_EQ(expon[1], 2);
  EXPECT_EQ(expon[2], 3);
  EXPECT_EQ(coeffs[0], 1);
  EXPECT_EQ(coeffs[2], 3);
  ::std::array<int, 3> expon1 = {3, 2, 1};
  ::std::array<double, 3> coeffs1 = {3.0, 2.0, 1.0};
  SyncSortTwoArrays(&expon1, &coeffs1, 0);
  EXPECT_EQ(expon1[0], 1);
  EXPECT_EQ(expon1[1], 2);
  EXPECT_EQ(expon1[2], 3);
  EXPECT_EQ(coeffs1[0], 1);
  EXPECT_EQ(coeffs1[1], 2);
  EXPECT_EQ(coeffs1[2], 3);
}

TEST(PolynomialTest, Constructor) {
  array<double, 3> coeffs = {1.0, 2.0, 3.0};
  array<int, 3> expon = {1, 2, 3};
  Polynomial testpoly(coeffs, expon);
  testpoly.Print();
  ASSERT_EQ(3, testpoly.Head().exponent);
  ASSERT_EQ(3.0, testpoly.Head().coefficient);
}

TEST(PolynomialTest, CopyConstructor) {
  array<double, 3> coeffs = {1.0, 2.0, 3.0};
  array<int, 3> expon = {1, 2, 3};
  Polynomial testpoly1(coeffs, expon);
  testpoly1.Print();
  Polynomial testpoly2(testpoly1);
  testpoly2.Print();
  ASSERT_EQ(testpoly2.Head().exponent, testpoly1.Head().exponent);
  ASSERT_EQ(testpoly2.Head().coefficient, testpoly1.Head().coefficient);
}

TEST(PolynomialTest, ReverseTest) {
  array<double, 3> coeffs = {1.0, 2.0, 3.0};
  array<int, 3> expon = {1, 2, 3};
  Polynomial testpoly(coeffs, expon);
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
  array<double, 1> coeffs = {1.0};
  array<int, 1> expon = {1};
  Polynomial testpoly(coeffs, expon);
  testpoly.Print();
  ASSERT_EQ(1, testpoly.Head().exponent);
  ASSERT_EQ(1.0, testpoly.Head().coefficient);
  testpoly.Reverse();
  testpoly.Print();
  ASSERT_EQ(1, testpoly.Head().exponent);
  ASSERT_EQ(1.0, testpoly.Head().coefficient);
}

TEST(PolynomialTest, AddEmptyPolynomial) {
  array<double, 3> coeffs = {1.0, 2.0, 3.0};
  array<int, 3> expon = {1, 2, 3};
  Polynomial testpoly1(coeffs, expon);
  testpoly1.Print();
  Polynomial testpoly2;
  testpoly2.Print();
  Polynomial sum(testpoly1, testpoly2);
  sum.Print();
  ASSERT_EQ(sum.Head().coefficient, testpoly1.Head().coefficient);
  ASSERT_EQ(sum.Head().exponent, testpoly1.Head().exponent);
  ASSERT_EQ(sum.Head().next->coefficient, testpoly1.Head().next->coefficient);
  ASSERT_EQ(sum.Head().next->exponent, testpoly1.Head().next->exponent);
  ASSERT_EQ(sum.Head().next->next->coefficient,
            testpoly1.Head().next->next->coefficient);
  ASSERT_EQ(sum.Head().next->next->exponent,
            testpoly1.Head().next->next->exponent);
}

TEST(PolynomialTest, DoubleAPolynomial) {
  array<double, 3> coeffs = {1.0, 2.0, 3.0};
  array<int, 3> expon = {1, 2, 3};
  Polynomial testpoly1(coeffs, expon);
  testpoly1.Print();
  Polynomial testpoly2(testpoly1);
  Polynomial sum(testpoly1, testpoly2);
  sum.Print();
  ASSERT_EQ(sum.Head().coefficient, 2 * testpoly1.Head().coefficient);
  ASSERT_EQ(sum.Head().exponent, testpoly1.Head().exponent);
  ASSERT_EQ(sum.Head().next->coefficient,
            2 * testpoly1.Head().next->coefficient);
  ASSERT_EQ(sum.Head().next->exponent, testpoly1.Head().next->exponent);
  ASSERT_EQ(sum.Head().next->next->coefficient,
            2 * testpoly1.Head().next->next->coefficient);
  ASSERT_EQ(sum.Head().next->next->exponent,
            testpoly1.Head().next->next->exponent);
}

TEST(PolynomialTest, AddIntToPolynomial) {
  array<double, 2> coeffs = {1.0, 2.0};
  array<int, 2> expon = {1, 2};
  Polynomial testpoly1(coeffs, expon);
  testpoly1.Print();
  array<double, 1> anothercoeff = {9.0};
  array<int, 1> anotherexpon = {0};
  Polynomial testpoly2(anothercoeff, anotherexpon);
  testpoly2.Print();
  Polynomial sum(testpoly1, testpoly2);
  sum.Print();
  ASSERT_EQ(sum.Head().coefficient, testpoly1.Head().coefficient);
  ASSERT_EQ(sum.Head().exponent, testpoly1.Head().exponent);
  ASSERT_EQ(sum.Head().coefficient, testpoly1.Head().coefficient);
  ASSERT_EQ(sum.Head().exponent, testpoly1.Head().exponent);
  ASSERT_EQ(sum.Head().next->coefficient, testpoly1.Head().next->coefficient);
  ASSERT_EQ(sum.Head().next->exponent, testpoly1.Head().next->exponent);
  ASSERT_EQ(sum.Head().next->next->exponent, 0);
  ASSERT_EQ(sum.Head().next->next->coefficient, 9);
}

TEST(PolynomialTest, AddTermToPolynomial) {
  array<double, 3> coeffs = {1.0, 2.0, 3.0};
  array<int, 3> expon = {1, 2, 3};
  Polynomial testpoly1(coeffs, expon);
  testpoly1.Print();
  array<double, 1> anothercoeff = {9.0};
  array<int, 1> anotherexpon = {1};
  Polynomial testpoly2(anothercoeff, anotherexpon);
  testpoly2.Print();
  Polynomial sum(testpoly1, testpoly2);
  sum.Print();
  ASSERT_EQ(sum.Head().coefficient, testpoly1.Head().coefficient);
  ASSERT_EQ(sum.Head().exponent, testpoly1.Head().exponent);
  ASSERT_EQ(sum.Head().next->coefficient, testpoly1.Head().next->coefficient);
  ASSERT_EQ(sum.Head().next->exponent, testpoly1.Head().next->exponent);
  ASSERT_EQ(sum.Head().next->next->exponent,
            testpoly1.Head().next->next->exponent);
  ASSERT_EQ(sum.Head().next->next->coefficient,
            9 + testpoly1.Head().next->next->coefficient);
}

} // namespace testing
} // namespace polynomial
