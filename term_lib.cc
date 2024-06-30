#include "term.hh"

#include <cassert>
#include <iostream>

using namespace std;

namespace term {

ostream &operator<<(::std::ostream &out, const Term &t) {
  if (0 == t.coefficient) {
    return out;
  }
  /* TODO: defer to Polynomial's printer when next is not NULL.
     Put Polynomial's printer in its own library on which Polynomial and Term
    both depend.gi if (nullptr != t.next.get()) { polynomial::Polynomial p(t);
    out << p;
    } */
  string printme = (t.coefficient == 1) ? "" : ::std::to_string(t.coefficient);
  switch (t.exponent) {
  case 0:
    out << printme << " ";
    break;
  case 1:
    out << printme << "x ";
    break;
  default:
    out << printme << "x^" << t.exponent << " ";
  }
  return out;
}

// Implicitly calls the move constructor?
// If we return a Term rather than a Term& from the operator, "error: use of
// deleted function ‘constexpr polynomial::Term::Term(const polynomial::Term&)’"
// So returning a Term invokes the deleted default copy constructor, while
// returning an lvalue reference m
// https://stackoverflow.com/questions/11255027/why-user-defined-move-constructor-disables-the-implicit-copy-constructor
// "If you declare move members (which legacy C++ code can't do), then we're
// going to assume that the default copy members are likely to do the wrong
// thing."
// What the generated, default move-assignment operator
//      Term &operator=(Term &&t) = default;
// does:
// [ RUN      ] TermTest.AssignmentTest
// term_lib_test.cc:84: Failure
// Expected equality of these values:
//  0
//  t.exponent
//    Which is: 3
// term_lib_test.cc:85: Failure
// Expected equality of these values:
//  0
//  t.coefficient
//    Which is: 2
// [  FAILED  ] TermTest.AssignmentTest (0 ms)
//
// Replaced with default.
/* Term &Term::operator=(Term &&t) {
  coefficient = t.coefficient;
  exponent = t.exponent;
  next = std::move(t.next);
  t.exponent = 0;
  t.coefficient = 0;
  // return this;
  // results in "polynomial_lib.cc:20:10: error: invalid initialization of
  // non-const reference of type ‘polynomial::Term&’ from an rvalue of type
  // ‘polynomial::Term*’"
  return *this;
  } */

// Only for terms with equal exponents.
// Without a move constructor:
// polynomial_lib.cc:37:10: error: use of deleted function ‘constexpr
// polynomial::Term::Term(const polynomial::Term&)’
//   return tp;
// That's odd since const lvalue references rather than rvalue references are
// passed as function parameters.
Term operator+(const Term &a, const Term &b) {
  assert(a.exponent == b.exponent);
  Term t{a.exponent, a.coefficient + b.coefficient};
  return t;
}

bool operator==(const Term &a, const Term &b) {
  // Note that operator* returns the value of the managed ptr.
  if (a.next && b.next) {
    return ((a.coefficient == b.coefficient) && (a.exponent == b.exponent) &&
            (*a.next == *b.next));
  }
  if (!a.next && !b.next) {
    return ((a.coefficient == b.coefficient) && (a.exponent == b.exponent));
  }
  // Only one next pointer is null.
  return false;
}

bool operator!=(const Term &a, const Term &b) { return !(a == b); }

} // namespace term
