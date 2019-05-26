#ifndef TERM_H
#define TERM_H

#include "term_impl.h"

#include <array>
#include <iostream>

namespace term {

class Term;

struct termprops {
  int exp;
  double coeff;
  Term *next;
};

class Term {
public:
  Term() : exponent(0), coefficient(0), next(0){};
  Term(int e, double c, Term *n = 0) : exponent(e), coefficient(c), next(n) {
    //    ::std::cout << "term members constructor" << ::std::endl;
  }
  // Implicitly called by operator+().
  Term(const struct termprops tp)
      : exponent(tp.exp), coefficient(tp.coeff), next(tp.next) {
    ::std::cout << "term struct constructor" << ::std::endl;
  }
  // clang-format off
  //Term(Term &&t) : exponent(t.exponent), coefficient(t.coefficient), next(t.next) {
  //      ::std::cout << "term move constructor" << ::std::endl;
  //  }
  // clang-format on
  // Initialize the object empty, then move() contents in.
  // Copied from code by Brian Silverman.
  Term(Term &&t) : Term() {
    ::std::cout << "term move constructor" << ::std::endl;
    *this = ::std::move(t);
  }

  // Following
  // https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  Term &operator=(Term &&t);
  // Must be a friend, non-member function because operator<<() takes only one
  // parameter, so we cannot override with a function that takes two.
  friend ::std::ostream &operator<<(::std::ostream &out, const Term &t);
  friend Term operator+(const Term &a, const Term &b);
  int exponent;
  double coefficient;
  Term *next;
};

Term operator+(const Term &a, const Term &b);
::std::ostream &operator<<(::std::ostream &out, const Term &t);

// Numerically sort array1, and reorder array2 the same way.
template <long unsigned int N>
void SyncSortTwoArrays(::std::array<int, N> *arr1,
                       ::std::array<double, N> *arr2, int index);

} // namespace term

#endif
