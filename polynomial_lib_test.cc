#include "polynomial.h"
#include "term.h"
#include "term_vector.h"

#include "gtest/gtest.h"

#include <iostream>

using namespace std;
using namespace term;
using namespace termvector;

namespace polynomial {
namespace testing {

TEST(TermVectorTest, ArrayConstructorTest) {
  array<int, 3> expon = {{1, 2, 3}};
  array<double, 3> coeffs = {{1.0, 2.0, 3.0}};
  TermVector tv(coeffs, expon);
  EXPECT_EQ(3, tv.size());
  cout << "head: " << *(tv.begin()) << endl;
  int i = 0;
  while (i < tv.size()) {
    EXPECT_EQ(1.0 * (i + 1), tv.at(i).coefficient);
    EXPECT_EQ(i + 1, tv.at(i).exponent);
    cout << "i: " << i << ", tv[i] " << tv.at(i) << endl;
    i++;
  }
}

TEST(TermVectorTest, VectorConstructorTest) {
  array<int, 3> expon = {{1, 2, 3}};
  array<double, 3> coeffs = {{1.0, 2.0, 3.0}};
  vector<double> dv;
  vector<int> iv;
  for (int i = 0; i < 3; i++) {
    dv.push_back(coeffs[i]);
    iv.push_back(expon[i]);
  }
  TermVector tv(dv, iv);
  EXPECT_EQ(3, tv.size());
  cout << "head: " << *(tv.begin()) << endl;
  int i = 0;
  while (i < tv.size()) {
    EXPECT_EQ(1.0 * (i + 1), tv.at(i).coefficient);
    EXPECT_EQ(i + 1, tv.at(i).exponent);
    cout << "i: " << i << ", tv[i] " << tv.at(i) << endl;
    i++;
  }
}

class PolynomialSimpleTest : public ::testing::Test {};

class PolynomialTest : public ::testing::Test {
public:
  PolynomialTest() {
    testpoly = new Polynomial(coeffs, expon);
    copy2 = new Polynomial(coeffs, expon);
  }
  ~PolynomialTest() {
    delete testpoly;
    delete copy2;
  }
  Polynomial *testpoly, *copy2;
  ::std::array<int, 3> expon = {{1, 2, 3}};
  ::std::array<double, 3> coeffs = {{1.0, 2.0, 3.0}};
};

// Does not rely on Reverse().
TEST_F(PolynomialTest, ArraysConstructor) {
  ostringstream out;
  out << *testpoly;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
  ASSERT_EQ(3, testpoly->head()->exponent);
  ASSERT_EQ(3.0, testpoly->head()->coefficient);
}

// Relies on Reverse().
TEST_F(PolynomialTest, CopyConstructor) {
  // AddressSanitizer: stack-use-after-scope on address 0x7fffdd0ce680 at pc
  // 0x55b2243ef4c8 bp 0x7fffdd0ce5e0 sp 0x7fffdd0ce5d8 READ of size 8 at
  // 0x7fffdd0ce680 thread T0
  //    #0 0x55b2243ef4c7 in polynomial::Polynomial::Reverse()
  //    /home/alison/gitsrc/Cpp-Exercises/polynomial_lib.cc:151
  //  Polynomial testpoly2(move(*testpoly));
  const Polynomial &testpoly2(*testpoly);
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
  cout << testpoly2 << endl;
  ASSERT_EQ(testpoly2.head()->exponent, testpoly->head()->exponent);
  ASSERT_EQ(testpoly2.head()->coefficient, testpoly->head()->coefficient);
}

TEST(PolynomialSimpleTest, ListConstructor) {
  Term *head = new Term(1, 2.0);
  double j = 4.0;
  for (int i = 2; i < 6; i++, j *= 2.0) {
    Term *t = new Term(i, j, head);
    head = t;
  }
  // Polynomial(head) results in "no known conversion for argument 1 from
  // ‘polynomial::Term*’ to ‘const polynomial::Term&’"
  Polynomial p(*head);
  ASSERT_EQ(p.head()->exponent, 5);
  ASSERT_EQ(p.head()->coefficient, 32.0);
  ASSERT_EQ(p.head()->next->exponent, 4);
  ASSERT_EQ(p.head()->next->coefficient, 16.0);
  ostringstream out;
  out << p;
  cout << p << endl;
  ASSERT_EQ(
      "32.000000x^5 + 16.000000x^4 + 8.000000x^3 + 4.000000x^2 + 2.000000x ",
      out.str());

  Term *t = head;
  while (nullptr != t) {
    Term *save = t;
    t = t->next;
    delete save;
  }
}

TEST_F(PolynomialTest, MoveConstructor) {
  cout << "in test" << endl;
  Polynomial testpoly2(move(*testpoly));
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
  cout << testpoly2 << endl;
  ASSERT_EQ(testpoly2.head()->exponent, copy2->head()->exponent);
  ASSERT_EQ(testpoly2.head()->coefficient, copy2->head()->coefficient);
  // testpoly->head() is now essentially empty, as we have actually moved the
  // contents. ASSERT_EQ(testpoly->head()->exponent, 0);
  // ASSERT_EQ(testpoly->head()->coefficient, 0);
}

TEST_F(PolynomialTest, ReverseTest) {
  cout << *testpoly << endl;
  testpoly->Reverse();
  cout << *testpoly << endl;
  ASSERT_EQ(1, testpoly->head()->exponent);
  ASSERT_EQ(1.0, testpoly->head()->coefficient);
  testpoly->Reverse();
  ASSERT_EQ(3, testpoly->head()->exponent);
  ASSERT_EQ(3.0, testpoly->head()->coefficient);
}

TEST_F(PolynomialTest, ReverseEmpty) {
  Polynomial testpoly;
  cout << testpoly << endl;
  testpoly.Reverse();
}

TEST_F(PolynomialTest, ReverseOneTerm) {
  cout << *testpoly << endl;
  ASSERT_EQ(3, testpoly->head()->exponent);
  ASSERT_EQ(3.0, testpoly->head()->coefficient);
  testpoly->Reverse();
  cout << *testpoly << endl;
  ASSERT_EQ(1, testpoly->head()->exponent);
  ASSERT_EQ(1.0, testpoly->head()->coefficient);
}

TEST_F(PolynomialTest, AssignmentTest) {
  //  ::std::array<int, 3> expon = {{-1, -2, -3}};
  //  ::std::array<double, 3> coeffs = {{4.0, 5.0, 6.0}};
  //  Polynomial testpoly2(coeffs, expon);
  //  ostringstream out;
  //  out << testpoly2;
  //  ASSERT_EQ("4.000000x^-1 + 5.000000x^-2 + 6.000000x^-3 ", out.str());
  const Polynomial &testpoly2 = *testpoly;
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
  ASSERT_EQ(3, testpoly2.head()->exponent);
  ASSERT_EQ(3.0, testpoly2.head()->coefficient);
}

TEST_F(PolynomialTest, TermVectorCtorTest) {
  TermVector tv(coeffs, expon);
  Polynomial poly(tv);
  ostringstream out;
  out << poly;
  ASSERT_EQ("x + 2.000000x^2 + 3.000000x^3 ", out.str());
  ASSERT_EQ(1, poly.head()->exponent);
  ASSERT_EQ(1.0, poly.head()->coefficient);
}

TEST_F(PolynomialTest, AddEmptyPolynomial) {
  Polynomial testpoly2;
  cout << "Empty polynomial: " << testpoly2 << endl;
  Polynomial sum = *testpoly + testpoly2;
  cout << "Sum: " << sum << endl;
  EXPECT_EQ(sum.head()->coefficient, copy2->head()->coefficient);
  EXPECT_EQ(sum.head()->exponent, copy2->head()->exponent);
  EXPECT_EQ(sum.head()->next->coefficient, copy2->head()->next->coefficient);
  EXPECT_EQ(sum.head()->next->exponent, copy2->head()->next->exponent);
  EXPECT_EQ(sum.head()->next->next->coefficient,
            copy2->head()->next->next->coefficient);
  EXPECT_EQ(sum.head()->next->next->exponent,
            copy2->head()->next->next->exponent);
}

TEST_F(PolynomialTest, AddIntToPolynomial) {
  array<double, 1> anothercoeff = {9.0};
  array<int, 1> anotherexpon = {0};
  Polynomial testpoly2(anothercoeff, anotherexpon);
  cout << "This polynomial is an int: " << testpoly2 << endl;
  Polynomial sum = *testpoly + testpoly2;
  cout << sum << endl;
  cout << "Sum: " << sum << endl;
  EXPECT_EQ(sum.head()->coefficient, copy2->head()->coefficient);
  EXPECT_EQ(sum.head()->exponent, copy2->head()->exponent);
  EXPECT_EQ(sum.head()->next->coefficient, copy2->head()->next->coefficient);
  EXPECT_EQ(sum.head()->next->exponent, copy2->head()->next->exponent);
  EXPECT_EQ(sum.head()->next->next->coefficient,
            copy2->head()->next->next->coefficient);
  EXPECT_EQ(sum.head()->next->next->exponent,
            copy2->head()->next->next->exponent);
}

TEST_F(PolynomialTest, AddTermToPolynomial) {
  array<double, 1> anothercoeff = {9.0};
  array<int, 1> anotherexpon = {1};
  Polynomial testpoly2(anothercoeff, anotherexpon);
  cout << testpoly2 << endl;
  Polynomial sum = *testpoly + testpoly2;
  cout << sum << endl;
  ASSERT_EQ(sum.head()->coefficient, testpoly->head()->coefficient);
  ASSERT_EQ(sum.head()->exponent, testpoly->head()->exponent);
  ASSERT_EQ(sum.head()->next->coefficient, testpoly->head()->next->coefficient);
  ASSERT_EQ(sum.head()->next->exponent, testpoly->head()->next->exponent);
  ASSERT_EQ(sum.head()->next->next->exponent,
            testpoly->head()->next->next->exponent);
  ASSERT_EQ(sum.head()->next->next->coefficient,
            9 + testpoly->head()->next->next->coefficient);
}

TEST_F(PolynomialTest, DoubleAPolynomial) {
  cout << "testpoly is " << *testpoly << endl;
  Polynomial sum = *testpoly + *testpoly;
  cout << "testpoly added to itself is " << sum << endl;
  ASSERT_EQ(sum.head()->coefficient, 2 * testpoly->head()->coefficient);
  ASSERT_EQ(sum.head()->exponent, testpoly->head()->exponent);
  ASSERT_EQ(sum.head()->next->coefficient,
            2 * testpoly->head()->next->coefficient);
  ASSERT_EQ(sum.head()->next->exponent, testpoly->head()->next->exponent);
  ASSERT_EQ(sum.head()->next->next->coefficient,
            2 * testpoly->head()->next->next->coefficient);
  ASSERT_EQ(sum.head()->next->next->exponent,
            testpoly->head()->next->next->exponent);
}

} // namespace testing
} // namespace polynomial
