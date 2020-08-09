#include "polynomial.h"
#include "term.h"
#include "term_vector.h"

#include "gtest/gtest.h"

#include <iostream>

using namespace std;
using namespace term;
using namespace termvector;

namespace polynomial {
namespace local_testing {

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
  ::array<int, 3> expon = {{1, 2, 3}};
  ::array<double, 3> coeffs = {{1.0, 2.0, 3.0}};
};

TEST_F(PolynomialTest, EmptyFalseTest) { EXPECT_FALSE(testpoly->empty()); }

// Does not rely on Reverse().
TEST_F(PolynomialTest, ArraysConstructor) {
  ostringstream out;
  out << *testpoly;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
  ASSERT_NE(nullptr, testpoly->head());
  ASSERT_EQ(3, testpoly->head()->exponent);
  ASSERT_EQ(3.0, testpoly->head()->coefficient);
}

// Relies on Reverse().
TEST_F(PolynomialTest, CopyConstructor) {
  // Runtime failure:
  // AddressSanitizer: stack-use-after-scope on address 0x7fffdd0ce680 at pc
  // 0x55b2243ef4c8 bp 0x7fffdd0ce5e0 sp 0x7fffdd0ce5d8 READ of size 8 at
  // 0x7fffdd0ce680 thread T0
  //    #0 0x55b2243ef4c7 in polynomial::Polynomial::Reverse()
  //    /home/alison/gitsrc/Cpp-Exercises/polynomial_lib.cc:151
  //  Polynomial testpoly2(move(*testpoly));
  // move() removes the contents of testpoly so that testpoly->head() can no
  // longer be evaluated.
  //
  // Compiler failure:
  // http://thbecker.net/articles/rvalue_references/section_03.html
  // if you implement
  // void foo(X&&);
  // but neither one of
  // void foo(X&); and void foo(X const &);
  // then, according to the final version of C++11, foo can be called on
  // r-values, but trying to call it on an l-value will trigger a compile error.
  //
  // polynomial_lib_test.cc:87:33: error: use of deleted function ‘constexpr
  // polynomial::Polynomial::Polynomial(const polynomial::Polynomial&)’
  // Polynomial testpoly2(*testpoly);
  //
  // Runtime failure:
  // Polynomial &testpoly2(*testpoly);
  // term polynomial move constructor
  // Polynomial assignment operator
  // polynomial_lib_test.cc:95:3: runtime error: member access within null
  // pointer of type 'const struct Term' polynomial_lib_test.cc:95:3: runtime
  // error: reference binding to null pointer of type 'const int' We have
  // removed the contents of testpoly and can no longer evaluate them.
  const Polynomial &testpoly2(*testpoly);
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
#ifdef DEBUG
  cout << testpoly2 << endl;
#endif
  ASSERT_EQ(testpoly2.head()->exponent, testpoly->head()->exponent);
  ASSERT_EQ(testpoly2.head()->coefficient, testpoly->head()->coefficient);
  // Runtime failure:
  // term polynomial move constructor
  // Polynomial assignment operator
  // polynomial_lib_test.cc:102:3: runtime error: member access within null
  // pointer of type 'const struct Term' polynomial_lib_test.cc:102:3: runtime
  // error: reference binding to null pointer of type 'const int' We still can't
  // remove the contents of testpoly, as testpoly2 refers to it. Polynomial
  // testpoly3(move(*testpoly)); ASSERT_EQ(testpoly3.head()->exponent,
  // testpoly2.head()->exponent); ASSERT_EQ(testpoly3.head()->coefficient,
  // testpoly2.head()->coefficient);
}

// https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
// 'The && syntax is the new rvalue reference. It does exactly what it sounds it
// does - gives us a reference to an rvalue, which is going to be destroyed
// after the call. We can use this fact to just "steal" the internals of the
// rvalue - it won't need them anyway! The operator simply switches the rvalue's
// internal buffer with its own, arranging it so the rvalue's destructor will
// release our object's own buffer, which is no longer used.'
//
// http://thbecker.net/articles/rvalue_references/section_04.html
// "If move semantics are implemented as a simple swap, then the effect of this
// is that the objects held by A and B are being exchanged between A and B.
// Nothing is being destructed yet. The object formerly held by A will of course
// be destructed eventually, namely, when B goes out of scope."
TEST_F(PolynomialTest, MoveConstructor) {
#ifdef DEBUG
  cout << "in test" << endl;
#endif
  Polynomial testpoly2(move(*testpoly));
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
#ifdef DEBUG
  cout << testpoly2 << endl;
#endif
  ASSERT_EQ(testpoly2.head()->exponent, copy2->head()->exponent);
  ASSERT_EQ(testpoly2.head()->coefficient, copy2->head()->coefficient);
  // testpoly->head() is now essentially empty, as we have actually moved the
  // contents. ASSERT_EQ(testpoly->head()->exponent, 0);
  // ASSERT_EQ(testpoly->head()->coefficient, 0);
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
#ifdef DEBUG
  cout << p << endl;
#endif
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

TEST(PolynomialSimpleTest, ShortList) {
  Polynomial simple(Term(3.0, 3));
  ASSERT_EQ(3.0, simple.head()->coefficient);
  ASSERT_EQ(3, simple.head()->exponent);
  ASSERT_EQ(nullptr, simple.head()->next);
}

TEST(PolynomialSimpleTest, PrintEmpty) {
  ostringstream out;
  Polynomial testpoly2;
  out << testpoly2;
  EXPECT_TRUE(out.str().empty());
}

TEST_F(PolynomialTest, EqualityEmptyTest) {
  Polynomial testpoly2;
  EXPECT_NE(*testpoly, testpoly2);
  EXPECT_NE(testpoly2, *testpoly);
}

TEST_F(PolynomialTest, EqualityTestOneTerm) {
  const Term aterm(1, 1.0);
  Polynomial another(aterm);
  // Should implicitly call the termlist ctor of Polynomial.
  EXPECT_NE(*testpoly, aterm);
  EXPECT_NE(*testpoly, another);
}

TEST_F(PolynomialTest, EqualityTestOneShorter) {
  ::array<int, 2> expon2 = {{3, 2}};
  ::array<double, 2> coeffs2 = {{3.0, 2.0}};
  Polynomial another(coeffs2, expon2);
  EXPECT_NE(*testpoly, another);
}

TEST_F(PolynomialTest, EqualityTestEqualLength) {
  ASSERT_TRUE(*testpoly == *copy2);
  ASSERT_FALSE(*testpoly != *copy2);
  Polynomial sum = *testpoly + *copy2;
  ASSERT_FALSE(*testpoly == sum);
  ASSERT_TRUE(*testpoly != sum);
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
  EXPECT_EQ(nullptr, testpoly.head()->next->next);
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

TEST_F(PolynomialTest, ZeroFactor) {
  const Polynomial another(*testpoly * 0);
  EXPECT_TRUE(another.empty());
}

TEST_F(PolynomialTest, MultiplicationTest) {
  const Polynomial another(*testpoly * 2);
  EXPECT_EQ(2 * testpoly->head()->coefficient, another.head()->coefficient);
  EXPECT_EQ(testpoly->head()->exponent, another.head()->exponent);
  EXPECT_EQ(2 * testpoly->head()->next->coefficient,
            another.head()->next->coefficient);
  EXPECT_EQ(2 * testpoly->head()->next->next->coefficient,
            another.head()->next->next->coefficient);
}

TEST_F(PolynomialTest, PlusMinusTest) {
  const Term aterm(4.0, 4);
  const Polynomial another(*testpoly + aterm);
  ASSERT_EQ(aterm, *(another.head()));

  Polynomial testpoly2(*testpoly - Term(3.0, 3));
  array<double, 2> coeffs{{1.0, 2.0}};
  array<int, 2> exps{{1, 2}};
  Polynomial testpoly3(coeffs, exps);
  EXPECT_TRUE(testpoly2 == testpoly3);

  Term temp(3, 3.0);
  Polynomial shortpoly1(temp), shortpoly2(temp);
  ASSERT_EQ(shortpoly1, shortpoly2);
}

TEST_F(PolynomialTest, PolynomialSumIsEmpty) {
  array<double, 3> negcoeffs{{-1.0, -2.0, -3.0}};
  Polynomial another(negcoeffs, expon);
  EXPECT_NE(*testpoly, another);
  Polynomial sum = *testpoly + another;
  EXPECT_TRUE(sum.empty());
}

TEST_F(PolynomialTest, ReverseTest) {
#ifdef DEBUG
  cout << "testpoly: " << *testpoly << endl;
#endif
  testpoly->Reverse();
#ifdef DEBUG
  cout << "testpoly reversed: " << *testpoly << endl;
#endif
  ASSERT_EQ(1, testpoly->head()->exponent);
  ASSERT_EQ(1.0, testpoly->head()->coefficient);
  testpoly->Reverse();
  ASSERT_EQ(3, testpoly->head()->exponent);
  ASSERT_EQ(3.0, testpoly->head()->coefficient);
#ifdef DEBUG
  cout << "testpoly reversed a second time: " << *testpoly << endl;
#endif
  Polynomial testpoly2(coeffs, expon);
  EXPECT_EQ(testpoly2, *testpoly);
}

TEST_F(PolynomialTest, ReverseEmpty) {
  Polynomial testpoly;
  EXPECT_EQ(nullptr, testpoly.head());
#ifdef DEBUG
  cout << testpoly << endl;
#endif
  testpoly.Reverse();
}

TEST_F(PolynomialTest, ReverseOneTerm) {
  Term temp(3, 3.0);
  Polynomial shortpoly1(temp), shortpoly2(temp);
  ASSERT_EQ(shortpoly1, shortpoly2);
  shortpoly1.Reverse();
  ASSERT_EQ(shortpoly1, shortpoly2);
}

TEST_F(PolynomialTest, MoveAssignmentTest) {
  //  ::array<int, 3> expon = {{-1, -2, -3}};
  //  ::array<double, 3> coeffs = {{4.0, 5.0, 6.0}};
  //  Polynomial testpoly2(coeffs, expon);
  //  ostringstream out;
  //  out << testpoly2;
  //  ASSERT_EQ("4.000000x^-1 + 5.000000x^-2 + 6.000000x^-3 ", out.str());
  Polynomial testpoly2;
  testpoly2 = move(*testpoly);
  ostringstream out;
  out << testpoly2;
  ASSERT_EQ("3.000000x^3 + 2.000000x^2 + x ", out.str());
  ASSERT_EQ(3, testpoly2.head()->exponent);
  ASSERT_EQ(3.0, testpoly2.head()->coefficient);
}

TEST_F(PolynomialTest, CopyAssignmentTest) {
  // Copy to an uninitialized Polynomial.
  Polynomial testpoly2;
  testpoly2 = *testpoly;
  ASSERT_EQ(*testpoly, testpoly2);

  // Copy to an initialized Polynomial.
  Polynomial testpoly3;
#ifdef DEBUG
  cout << "testpoly: " << *testpoly << endl;
#endif
  testpoly3 = *testpoly;
  testpoly3.Reverse();
#ifdef DEBUG
  cout << "testpoly3 is reversed *testpoly: " << testpoly3 << endl;
#endif
  testpoly->Reverse();
#ifdef DEBUG
  cout << "testpoly reversed: " << *testpoly << endl;
#endif
  testpoly2 = *testpoly;
  ASSERT_EQ(testpoly3, testpoly2);
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
#ifdef DEBUG
  cout << "Empty polynomial: " << testpoly2 << endl;
#endif
  Polynomial sum = *testpoly + testpoly2;
#ifdef DEBUG
  cout << "Sum: " << sum << endl;
#endif
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
#ifdef DEBUG
  cout << "This polynomial is an int: " << testpoly2 << endl;
#endif
  Polynomial sum = *testpoly + testpoly2;
#ifdef DEBUG
  cout << sum << endl;
  cout << "Sum: " << sum << endl;
#endif
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
#ifdef DEBUG
  cout << testpoly2 << endl;
#endif
  Polynomial sum = *testpoly + testpoly2;
#ifdef DEBUG
  cout << sum << endl;
#endif
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
#ifdef DEBUG
  cout << "testpoly is " << *testpoly << endl;
#endif
  Polynomial sum = *testpoly + *testpoly;
#ifdef DEBUG
  cout << "testpoly added to itself is " << sum << endl;
#endif
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

TEST_F(PolynomialTest, AddTwoDifferentPolynomials) {
  ::array<int, 3> expon = {{4, 5, 6}};
  ::array<double, 3> coeffs = {{4.0, 5.0, 6.0}};
  Polynomial testpoly2(coeffs, expon);
  ASSERT_TRUE(testpoly2 != *testpoly);
  Polynomial testpoly3 = *testpoly + testpoly2;
  term::Term aterm(6, 6.0);
  EXPECT_EQ(aterm, *(testpoly3.head()));
}

TEST_F(PolynomialTest, EmptyTrueTest) {
  Polynomial sum = *testpoly - *testpoly;
  cout << sum << endl;
  EXPECT_TRUE(sum.empty());
  EXPECT_EQ(nullptr, sum.head());
}

using PolynomialDeathTest = PolynomialTest;

TEST_F(PolynomialDeathTest, EmptyInputs) {
  array<int, 0> expon2;
  array<double, 0> coeffs2;
  EXPECT_EXIT(
      Polynomial(coeffs2, expon2), testing::KilledBySignal(SIGABRT),
      "Cannot create a polynomial from empty coefficients or exponent.");
}

} // namespace local_testing
} // namespace polynomial
