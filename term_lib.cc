#include "term.h"

#include <cassert>
#include <iostream>

using namespace std;

namespace term {

ostream &operator<<(::std::ostream &out, const Term &t) {
  if (0 == t.coefficient) {
    return out;
  }
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
Term &Term::operator=(Term &&t) {
#ifdef DEBUG
  cout << "term move assignment operator\n" << endl;
#endif
  swap(coefficient, t.coefficient);
  swap(exponent, t.exponent);
  // return this;
  // results in "polynomial_lib.cc:20:10: error: invalid initialization of
  // non-const reference of type ‘polynomial::Term&’ from an rvalue of type
  // ‘polynomial::Term*’"
  return *this;
}

Term &Term::operator=(const Term &t) {
  coefficient = t.coefficient;
  exponent = t.exponent;
  return *this;
}

// Only for terms with equal exponents.
// Without a move constructor:
// polynomial_lib.cc:37:10: error: use of deleted function ‘constexpr
// polynomial::Term::Term(const polynomial::Term&)’
//   return tp;
// That's odd since const lvalue references rather than rvalue references are
// passed as function parameters.
Term operator+(const Term &a, const Term &b) {
#ifdef DEBUG
  cout << "term operator+()" << endl;
#endif
  assert(a.exponent == b.exponent);
  struct termprops tp;
  tp.exp = a.exponent;
  tp.coeff = a.coefficient + b.coefficient;
  tp.next = 0;
  return tp;
}

bool operator==(const Term &a, const Term &b) {
  return ((a.coefficient == b.coefficient) && (a.exponent == b.exponent));
}

bool operator!=(const Term &a, const Term &b) { return !(a == b); }

} // namespace term
