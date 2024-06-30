#include "polynomial.hh"
#include "term.hh"
#include "term_vector.hh"

#include "gtest/gtest.h"

#include <iostream>

using namespace std;
using namespace term;
using namespace termvector;

namespace polynomial {

TEST(TermVectorTest, ArrayConstructorTest) {
  array<int, 3> expon = {{1, 2, 3}};
  array<double, 3> coeffs = {{1.0, 2.0, 3.0}};
  TermVector tv(coeffs, expon);
  EXPECT_EQ(3, tv.size());
#ifdef DEBUG
  cout << "head: " << *(tv.begin()) << endl;
#endif
  int i = 0;
  while (i < tv.size()) {
    EXPECT_EQ(1.0 * (i + 1), tv.at(i).coefficient);
    EXPECT_EQ(i + 1, tv.at(i).exponent);
#ifdef DEBUG
    cout << "i: " << i << ", tv[i] " << tv.at(i) << endl;
#endif
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
#ifdef DEBUG
  cout << "head: " << *(tv.begin()) << endl;
#endif
  int i = 0;
  while (i < tv.size()) {
    EXPECT_EQ(1.0 * (i + 1), tv.at(i).coefficient);
    EXPECT_EQ(i + 1, tv.at(i).exponent);
#ifdef DEBUG
    cout << "i: " << i << ", tv[i] " << tv.at(i) << endl;
#endif
    i++;
  }
}

TEST(PolynomialSimpleTest, TermVectorCtorTest) {
  std::array<int, 3> expon = {1, 2, 3};
  std::array<double, 3> coeffs = {1.0, 2.0, 3.0};

  TermVector tv(coeffs, expon);
  Polynomial poly(tv);
  ostringstream out;
  out << poly;
  ASSERT_EQ("x + 2.000000x^2 + 3.000000x^3 ", out.str());
  ASSERT_EQ(1, poly.head()->exponent);
  ASSERT_EQ(1.0, poly.head()->coefficient);
}

TEST(PolynomialSimpleTest, PrependTest) {
  // An anonymous Term is not const and doesn't match Polynomial list
  // constructor.
  const term::Term t(1.0, 1);
  Polynomial testpoly(t);
  testpoly.Prepend(std::unique_ptr<term::Term>(new Term(2.0, 2)));
  ASSERT_EQ(2, testpoly.head()->exponent);
  ASSERT_EQ(2.0, testpoly.head()->coefficient);
}

TEST(PolynomialSimpleTest, ReverseEmpty) {
  Polynomial testpoly;
  EXPECT_EQ(nullptr, testpoly.head());
#ifdef DEBUG
  cout << testpoly << endl;
#endif
  testpoly.Reverse();
}

TEST(PolynomialSimpleTest, ReverseOneTerm) {
  Term temp(3, 3.0);
  Polynomial shortpoly1(temp), shortpoly2(temp);
  EXPECT_EQ(shortpoly1, shortpoly2);
  shortpoly1.Reverse();
  EXPECT_EQ(shortpoly1, shortpoly2);
}

TEST(PolynomialSimpleTest, ShortList) {
  Polynomial simple(Term(3.0, 3));
  EXPECT_EQ(3.0, simple.head()->coefficient);
  EXPECT_EQ(3, simple.head()->exponent);
  EXPECT_EQ(nullptr, simple.head()->next);
}

TEST(PolynomialSimpleTest, ListConstructor) {
  std::unique_ptr<term::Term> head(new Term(1, 2.0));
  double j = 4.0;
  for (int i = 2; i < 6; i++, j *= 2.0) {
    std::unique_ptr<term::Term> t(new Term(i, j));
    t->next = std::unique_ptr<term::Term>(head.release());
    head = std::unique_ptr<term::Term>(t.release());
  }
  // Polynomial(head) results in "no known conversion for argument 1 from
  // ‘polynomial::Term*’ to ‘const polynomial::Term&’"
  Polynomial p(*head);
  EXPECT_EQ(p.head()->exponent, 5);
  EXPECT_EQ(p.head()->coefficient, 32.0);
  EXPECT_EQ(p.head()->next->exponent, 4);
  EXPECT_EQ(p.head()->next->coefficient, 16.0);
  ostringstream out;
  out << p;
  EXPECT_EQ(
      "32.000000x^5 + 16.000000x^4 + 8.000000x^3 + 4.000000x^2 + 2.000000x ",
      out.str());
#ifdef DEBUG
  cout << p << endl;
#endif
}

TEST(PolynomialSimpleTest, PrintEmpty) {
  ostringstream out;
  Polynomial testpoly2;
  out << testpoly2;
  EXPECT_TRUE(out.str().empty());
}

TEST(PolynomialSimpleTest, RemoveEmptyTermsEmpty) {
  Polynomial testpoly, testpoly1;
  EXPECT_TRUE(testpoly.empty());
  testpoly.RemoveEmptyTerms();
  EXPECT_TRUE(testpoly.empty());
  EXPECT_EQ(testpoly, testpoly1);
}

TEST(PolynomialSimpleTest, RemoveEmptyTermsHasNoEffectWithoutEmpty) {
  const array<int, 3> expon = {{3, 2, 1}};
  const array<double, 3> coeffs = {{3.0, 2.0, 1.0}};
  const TermVector tv(coeffs, expon);
  EXPECT_EQ(3, tv.size());
  Polynomial testpoly(tv);
  Polynomial testpolyprime(tv);
#ifdef DEBUG
  cout << "Before " << testpoly << endl;
#endif
  testpoly.RemoveEmptyTerms();
#ifdef DEBUG
  cout << "After " << testpoly << endl;
#endif
  EXPECT_EQ(testpoly, testpolyprime);
}

TEST(PolynomialSimpleTest, RemoveEmptyTermsRemovesEmptyEnd) {
  const array<int, 3> expon = {{3, 2, 1}};
  const array<double, 3> coeffs = {{0.0, 2.0, 1.0}};
  const TermVector tv(coeffs, expon);
  Polynomial testpoly(tv);
#ifdef DEBUG
  cout << "Before " << testpoly << endl;
#endif
  EXPECT_EQ(0, testpoly.head()->next->next->coefficient);
  EXPECT_EQ(3, testpoly.head()->next->next->exponent);
  testpoly.RemoveEmptyTerms();
#ifdef DEBUG
  cout << "After " << testpoly << endl;
#endif
  EXPECT_EQ(1.0, testpoly.head()->coefficient);
  EXPECT_EQ(2.0, testpoly.head()->next->coefficient);
  EXPECT_EQ(2, testpoly.head()->next->exponent);
  EXPECT_EQ(nullptr, testpoly.head()->next->next.get());
}

TEST(PolynomialSimpleTest, RemoveEmptyTermsRemovesEmptyMiddle) {
  const array<int, 3> expon = {{3, 2, 1}};
  const array<double, 3> coeffs = {{3.0, 0.0, 1.0}};
  const TermVector tv(coeffs, expon);
  Polynomial testpoly(tv);
#ifdef DEBUG
  cout << "Before " << testpoly << endl;
#endif
  EXPECT_EQ(0, testpoly.head()->next->coefficient);
  EXPECT_NE(nullptr, testpoly.head()->next->next);
  testpoly.RemoveEmptyTerms();
#ifdef DEBUG
  cout << "After " << testpoly << endl;
#endif
  EXPECT_EQ(3.0, testpoly.head()->next->coefficient);
  EXPECT_EQ(3, testpoly.head()->next->exponent);
  EXPECT_EQ(nullptr, testpoly.head()->next->next);
}

TEST(PolynomialSimpleTest, RemoveEmptyTermsRemovesEmptyHead) {
  const array<int, 3> expon = {{3, 2, 1}};
  const array<double, 3> coeffs = {{1.0, 2.0, 0.0}};
  const TermVector tv(coeffs, expon);
  Polynomial testpoly(tv);
#ifdef DEBUG
  cout << "Before " << testpoly << endl;
#endif
  EXPECT_EQ(0, testpoly.head()->coefficient);
  EXPECT_NE(nullptr, testpoly.head()->next->next);
  testpoly.RemoveEmptyTerms();
#ifdef DEBUG
  cout << "After " << testpoly << endl;
#endif
  EXPECT_EQ(2, testpoly.head()->exponent);
  EXPECT_EQ(2.0, testpoly.head()->coefficient);
  EXPECT_EQ(nullptr, testpoly.head()->next->next);
}

struct PolynomialTest : public ::testing::Test {
  PolynomialTest() : testpoly(coeffs, expon), copy2(coeffs, expon) {}

  // Initializing the arrays after the Polynomials does not result in a compiler
  // warning, but instead in garbage in the Polynomials.
  std::array<int, 3> expon = {1, 2, 3};
  std::array<double, 3> coeffs = {1.0, 2.0, 3.0};
  Polynomial testpoly, copy2;
};

TEST_F(PolynomialTest, EmptyFalseTest) { EXPECT_FALSE(testpoly.empty()); }

TEST_F(PolynomialTest, ArraysConstructor) {
  ostringstream out;
  out << testpoly;
  ASSERT_NE(nullptr, testpoly.head());
  EXPECT_EQ(3, testpoly.head()->exponent);
  EXPECT_EQ(3.0, testpoly.head()->coefficient);
  EXPECT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
}

// Okay since array constructor does not depend on Reverse().
TEST_F(PolynomialTest, ReverseTest) {
#ifdef DEBUG
  cerr << "testpoly: " << testpoly << endl;
#endif
  testpoly.Reverse();
#ifdef DEBUG
  cerr << "testpoly reversed: " << testpoly << endl;
#endif
  EXPECT_EQ(1, testpoly.head()->exponent);
  EXPECT_EQ(1.0, testpoly.head()->coefficient);
  testpoly.Reverse();
  EXPECT_EQ(3, testpoly.head()->exponent);
  EXPECT_EQ(3.0, testpoly.head()->coefficient);
#ifdef DEBUG
  cout << "testpoly reversed a second time: " << testpoly << endl;
#endif
}

// https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
// 'The && syntax is the new rvalue reference. It does exactly what it sounds it
// does - gives us a reference to an rvalue, which is going to be destroyed
// after the call. We can use this fact to just "steal" the internals of the
// rvalue - it won't need them anyway! The operator simply switches the rvalue's
// internal buffer with its own, arranging it so the rvalue's destructor will
// release our object's own buffer, which is no longer used.'
//
// http://thbecker.net/articles/rvalue_references/section_04.hhtml
// "If move semantics are implemented as a simple swap, then the effect of this
// is that the objects held by A and B are being exchanged between A and B.
// Nothing is being destructed yet. The object formerly held by A will of course
// be destructed eventually, namely, when B goes out of scope."
TEST_F(PolynomialTest, MoveConstructor) {
#ifdef DEBUG
  cout << "in test" << endl;
#endif
  Polynomial testpoly2(move(testpoly));
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
#ifdef DEBUG
  cout << testpoly2 << endl;
#endif
  ASSERT_EQ(testpoly2.head()->exponent, copy2.head()->exponent);
  ASSERT_EQ(testpoly2.head()->coefficient, copy2.head()->coefficient);
  // testpoly.head() is now essentially empty, as we have actually moved the
  // contents. ASSERT_EQ(testpoly.head()->exponent, 0);
  // ASSERT_EQ(testpoly.head()->coefficient, 0);
}

TEST_F(PolynomialTest, EqualityEmptyTest) {
  Polynomial testpoly2;
  EXPECT_NE(testpoly, testpoly2);
  EXPECT_NE(testpoly2, testpoly);
}

TEST_F(PolynomialTest, EqualityTestOneTerm) {
  const Term aterm(3, 3.0);
  Polynomial another(aterm);
  EXPECT_NE(testpoly, another);
  EXPECT_NE(another, testpoly);
  // Should implicitly call the term ctor of Polynomial.
  EXPECT_NE(testpoly, aterm);
}

TEST_F(PolynomialTest, EqualityTestOneShorter) {
  std::array<int, 2> expon2 = {3, 2};
  std::array<double, 2> coeffs2 = {3.0, 2.0};
  Polynomial another(coeffs2, expon2);
  EXPECT_NE(testpoly, another);
  std::array<int, 8> expon3 = {2, 3, 4, 5, 6, 7, 8, 9};
  std::array<double, 8> coeffs3 = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  Polynomial yetanother(coeffs3, expon3);
  EXPECT_NE(yetanother, testpoly);
}

TEST_F(PolynomialTest, EqualityTestEqualLength) {
  ASSERT_EQ(testpoly, copy2);
  ASSERT_FALSE(testpoly != copy2);
  Polynomial sum = testpoly + copy2;
  ASSERT_NE(testpoly, sum);
  ASSERT_TRUE(testpoly != sum);
}

TEST_F(PolynomialTest, ZeroFactor) {
  const Polynomial another(testpoly * 0);
  EXPECT_TRUE(another.empty());
}

TEST_F(PolynomialTest, MultiplicationTest) {
  const Polynomial another(testpoly * 2);
  EXPECT_EQ(2 * testpoly.head()->coefficient, another.head()->coefficient);
  EXPECT_EQ(testpoly.head()->exponent, another.head()->exponent);
  EXPECT_EQ(2 * testpoly.head()->next->coefficient,
            another.head()->next->coefficient);
  EXPECT_EQ(2 * testpoly.head()->next->next->coefficient,
            another.head()->next->next->coefficient);
}

TEST_F(PolynomialTest, MultiplicationByZeroTest) {
  const Polynomial another(testpoly * 0);
  EXPECT_TRUE(another.empty());
}

TEST_F(PolynomialTest, PlusMinusTest) {
  const Term aterm(4.0, 4);
  const Polynomial another(testpoly + aterm);
  EXPECT_EQ(aterm.coefficient, another.head()->coefficient);
  EXPECT_EQ(aterm.exponent, another.head()->exponent);
  EXPECT_NE(aterm.next, another.head()->next);

  Polynomial testpoly2(testpoly - Term(3.0, 3));
  array<double, 2> coeffs{{1.0, 2.0}};
  array<int, 2> exps{{1, 2}};
  Polynomial testpoly3(coeffs, exps);
  EXPECT_TRUE(testpoly2 == testpoly3);
}

TEST_F(PolynomialTest, PolynomialSumIsEmpty) {
  array<double, 3> negcoeffs{{-1.0, -2.0, -3.0}};
  Polynomial another(negcoeffs, expon);
  EXPECT_NE(testpoly, another);
  Polynomial sum = testpoly + another;
  EXPECT_TRUE(sum.empty());
}

TEST_F(PolynomialTest, MoveAssignmentTest) {
  //  ::array<int, 3> expon = {{-1, -2, -3}};
  //  ::array<double, 3> coeffs = {{4.0, 5.0, 6.0}};
  //  Polynomial testpoly2(coeffs, expon);
  //  ostringstream out;
  //  out << testpoly2;
  //  ASSERT_EQ("4.000000x^-1 + 5.000000x^-2 + 6.000000x^-3 ", out.str());
  Polynomial testpoly2;
  testpoly2 = move(testpoly);
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
  ASSERT_EQ(3, testpoly2.head()->exponent);
  ASSERT_EQ(3.0, testpoly2.head()->coefficient);
}

TEST_F(PolynomialTest, AddEmptyPolynomial) {
  Polynomial testpoly2;
#ifdef DEBUG
  cout << "Empty polynomial: " << testpoly2 << endl;
#endif
  Polynomial sum = testpoly + testpoly2;
#ifdef DEBUG
  cout << "Sum: " << sum << endl;
#endif
  EXPECT_EQ(sum.head()->coefficient, copy2.head()->coefficient);
  EXPECT_EQ(sum.head()->exponent, copy2.head()->exponent);
  EXPECT_EQ(sum.head()->next->coefficient, copy2.head()->next->coefficient);
  EXPECT_EQ(sum.head()->next->exponent, copy2.head()->next->exponent);
  EXPECT_EQ(sum.head()->next->next->coefficient,
            copy2.head()->next->next->coefficient);
  EXPECT_EQ(sum.head()->next->next->exponent,
            copy2.head()->next->next->exponent);
}

TEST_F(PolynomialTest, AddIntToPolynomial) {
  array<double, 1> anothercoeff = {9.0};
  array<int, 1> anotherexpon = {0};
  Polynomial testpoly2(anothercoeff, anotherexpon);
#ifdef DEBUG
  cout << "This polynomial is an int: " << testpoly2 << endl;
#endif
  Polynomial sum = testpoly + testpoly2;
#ifdef DEBUG
  cout << sum << endl;
  cout << "Sum: " << sum << endl;
#endif
  EXPECT_EQ(sum.head()->coefficient, copy2.head()->coefficient);
  EXPECT_EQ(sum.head()->exponent, copy2.head()->exponent);
  EXPECT_EQ(sum.head()->next->coefficient, copy2.head()->next->coefficient);
  EXPECT_EQ(sum.head()->next->exponent, copy2.head()->next->exponent);
  EXPECT_EQ(sum.head()->next->next->coefficient,
            copy2.head()->next->next->coefficient);
  EXPECT_EQ(sum.head()->next->next->exponent,
            copy2.head()->next->next->exponent);
}

TEST_F(PolynomialTest, AddTermToPolynomial) {
  array<double, 1> anothercoeff = {9.0};
  array<int, 1> anotherexpon = {1};
  Polynomial testpoly2(anothercoeff, anotherexpon);
#ifdef DEBUG
  cout << testpoly2 << endl;
#endif
  Polynomial sum = testpoly + testpoly2;
#ifdef DEBUG
  cout << sum << endl;
#endif
  ASSERT_EQ(sum.head()->coefficient, testpoly.head()->coefficient);
  ASSERT_EQ(sum.head()->exponent, testpoly.head()->exponent);
  ASSERT_EQ(sum.head()->next->coefficient, testpoly.head()->next->coefficient);
  ASSERT_EQ(sum.head()->next->exponent, testpoly.head()->next->exponent);
  ASSERT_EQ(sum.head()->next->next->exponent,
            testpoly.head()->next->next->exponent);
  ASSERT_EQ(sum.head()->next->next->coefficient,
            9 + testpoly.head()->next->next->coefficient);
}

TEST_F(PolynomialTest, DoubleAPolynomial) {
#ifdef DEBUG
  cout << "testpoly is " << testpoly << endl;
#endif
  Polynomial sum = testpoly + testpoly;
#ifdef DEBUG
  cout << "testpoly added to itself is " << sum << endl;
#endif
  ASSERT_EQ(sum.head()->coefficient, 2 * testpoly.head()->coefficient);
  ASSERT_EQ(sum.head()->exponent, testpoly.head()->exponent);
  ASSERT_EQ(sum.head()->next->coefficient,
            2 * testpoly.head()->next->coefficient);
  ASSERT_EQ(sum.head()->next->exponent, testpoly.head()->next->exponent);
  ASSERT_EQ(sum.head()->next->next->coefficient,
            2 * testpoly.head()->next->next->coefficient);
  ASSERT_EQ(sum.head()->next->next->exponent,
            testpoly.head()->next->next->exponent);
}

TEST_F(PolynomialTest, AddTwoDifferentPolynomials) {
  ::array<int, 3> expon = {{4, 5, 6}};
  ::array<double, 3> coeffs = {{4.0, 5.0, 6.0}};
  Polynomial testpoly2(coeffs, expon);
  ASSERT_TRUE(testpoly2 != testpoly);
  Polynomial testpoly3 = testpoly + testpoly2;
  term::Term aterm(6, 6.0);
  EXPECT_EQ(aterm.coefficient, testpoly3.head()->coefficient);
  EXPECT_EQ(aterm.exponent, testpoly3.head()->exponent);
  EXPECT_NE(aterm.next, testpoly3.head()->next);
}

TEST_F(PolynomialTest, EmptyTrueTest) {
  Polynomial sum = testpoly - testpoly;
  cout << sum << endl;
  EXPECT_TRUE(sum.empty());
  EXPECT_EQ(nullptr, sum.head());
}

TEST(PolynomialDeathTest, EmptyInputs) {
  array<int, 0> expon2;
  array<double, 0> coeffs2;
  EXPECT_THROW(Polynomial(coeffs2, expon2), invalid_argument);
}

} // namespace polynomial
