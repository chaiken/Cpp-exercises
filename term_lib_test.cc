#include "term.h"

#include "gtest/gtest.h"

#include <iostream>

using namespace std;

namespace term {
namespace local_testing {

TEST(TermTest, TermConstructor) {
  Term testTerm(3, 2);
  ASSERT_EQ(3, testTerm.exponent);
  ASSERT_EQ(2, testTerm.coefficient);
  Term testTerm2;
  EXPECT_TRUE(testTerm2.empty());
  EXPECT_FALSE(testTerm.empty());
}

// Order a list of exponents, and apply the same ordering to the list of
// coefficients.
TEST(TermTest, SyncSortTest) {
  ::std::array<int, 3> expon = {1, 2, 3};
  ::std::array<double, 3> coeffs = {1.0, 2.0, 3.0};
  SyncSortTwoArrays(&expon, &coeffs, 0);
  for (int i = 0; i < static_cast<int>(expon.size()); i++) {
    EXPECT_EQ(expon[i], coeffs[i]);
    EXPECT_EQ(expon[i], i + 1);
  }
  ::std::array<int, 3> expon1 = {3, 2, 1};
  ::std::array<double, 3> coeffs1 = {3.0, 2.0, 1.0};
  SyncSortTwoArrays(&expon1, &coeffs1, 0);
  for (int i = 0; i < static_cast<int>(expon1.size()); i++) {
    EXPECT_EQ(expon1[i], coeffs1[i]);
    EXPECT_EQ(expon1[i], i + 1);
  }
}

// The copy of a Term is equal to the original.
TEST(TermTest, CopyTest) {
  Term t(3, 2);
  // term members constructor
  const Term &t2(t);
  // polynomial_lib_test.cc:41:12: error: use of deleted function ‘constexpr
  // polynomial::Term::Term(const polynomial::Term&)’ Term t2(t);
  ASSERT_EQ(t.exponent, t2.exponent);
  ASSERT_EQ(t.coefficient, t2.coefficient);
  ASSERT_EQ(t.next, t2.next);
}

TEST(TermTest, MoveTest) {
  // term members constructor
  Term t(3, 2);
  // term members constructor ?
  const Term &t2(move(t));
  ostringstream out;
  out << t2;
  EXPECT_EQ("2.000000x^3 ", out.str());
  out.str("");
  cout << "t2: " << t2 << endl;
  EXPECT_EQ(3, t2.exponent);
  EXPECT_EQ(2, t2.coefficient);
  EXPECT_EQ(3, t.exponent);
  EXPECT_EQ(2, t.coefficient);

  // term members constructor
  Term t3(5, 6);
  // term move constructor
  Term t4(move(t3));
  out << t4;
  EXPECT_EQ("6.000000x^5 ", out.str());
  out.str("");
  cout << "t4: " << t4 << endl;
  EXPECT_EQ(5, t4.exponent);
  EXPECT_EQ(6, t4.coefficient);

  // term members constructor
  // term assignment operator
  Term t5(Term(-1, -2));
  out << t5;
  EXPECT_EQ("-2.000000x^-1 ", out.str());
  cout << "t5: " << t5 << endl;
  EXPECT_EQ(-1, t5.exponent);
  EXPECT_EQ(-2, t5.coefficient);
}

TEST(TermTest, AssignmentTest) {
  Term t(3, 2);
  // https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  // "Constant lvalue references can be assigned rvalues. Since they're
  // constant, the value can't be modified through the reference and hence
  // there's no problem of modifying an rvalue. This makes possible the very
  // common C++ idiom of accepting values by constant references into functions,
  // which avoids unnecessary copying and construction of temporary objects."
  // Passing constant lvalue references therefore has the economy of passing a
  // pointer without the notational ugliness, plus the assurance that the
  // address is not null (since a reference can only be to an existing object).
  const Term &t2 = t;
  // polynomial_lib_test.cc:58:12: error: use of deleted function ‘constexpr
  // polynomial::Term::Term(const polynomial::Term&)’ Term t2(t);
  ASSERT_EQ(t.exponent, t2.exponent);
  ASSERT_EQ(t.coefficient, t2.coefficient);
  ASSERT_EQ(t.next, t2.next);
}

TEST(TermTest, AdditionTest) {
  Term t1(3, 2), t2(3, 1), t3(t1 + t2);
  ASSERT_EQ(t3.exponent, t2.exponent);
  ASSERT_EQ(t3.coefficient, t1.coefficient + t2.coefficient);
  ASSERT_EQ(t1.next, t2.next);
}

TEST(TermTest, EqualityTest) {
  Term t1(3, 2), t2(3, 2);
  ASSERT_EQ(t1, t2);
  // Make sure != gets called.
  ASSERT_FALSE(t1 != t2);
}

TEST(TermTest, CopyAssignment) {
  Term t1(3, 2), t2;
  ASSERT_FALSE(t1 == t2);
  t2 = t1;
  ASSERT_EQ(t1, t2);
}

TEST(TermTest, PrintingTest) {
  Term t1(0, 0.0);
  ostringstream out;
  out << t1;
  EXPECT_TRUE(out.str().empty());
  Term t2(0, 1.0);
  out.str().clear();
  out << t2;
  EXPECT_TRUE(out.str().find("1;"));
  out.str().clear();
  Term t3(1, 3.0);
  out << t3;
  EXPECT_TRUE(out.str().find("3x ;"));
}

} // namespace local_testing
} // namespace term
