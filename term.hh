#ifndef TERM_H
#define TERM_H

#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>

namespace term {

// A helper class for Polynomial.
struct Term {
  Term() : exponent(0), coefficient(0), next(nullptr){};
  Term(int e, double c, Term *n)
      : exponent(e), coefficient(c), next(std::unique_ptr<Term>(n)) {
    //    ::std::cout << "term members constructor" << ::std::endl;
  }
  // Implicitly called by operator+().
  Term(const int exp, const double coeff)
      : exponent(exp), coefficient(coeff), next(nullptr) {}
  // p. 126, _C++ Crash Course_: "Your preference should be to use noexcept move
  // constructors; often the compiler cannot use exception-throwing move
  // constructors."
  // What the generated default move ctr
  //       Term(Term &&t) noexcept = default;
  // does:
  //  [ RUN      ] TermTest.MoveTest
  // t2: 2.000000x^3
  // term_lib_test.cc:64: Failure
  // Expected equality of these values:
  //   0
  //  t.exponent
  //    Which is: 3
  // term_lib_test.cc:65: Failure
  // Expected equality of these values:
  //  0
  //  t.coefficient
  //    Which is: 2
  // [  FAILED  ] TermTest.MoveTest (0 ms)
  Term(Term &&t) noexcept : Term() {
    // It's odd that simply moving the parameter works when the default move
    // ctor does not.
    *this = std::move(t);
    t.exponent = 0;
    t.coefficient = 0;
    // Moved-from objects are not destroyed.
    t.next.reset();
  }
  Term(const Term &t) = delete;
  ~Term() {
    if (next.get()) {
#ifdef DEBUG
      std::cerr << "Deleting next pointer " << *this << std::endl;
#endif
      next.reset();
    }
  }
  bool empty() { return (0.0 == coefficient); }
  // Following
  // https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  // Why does default move-assignment operator work when default move-ctor does
  // not?
  Term &operator=(Term &&t) = default;
  Term &operator=(const Term &t) = delete;
  // Must be a friend, non-member function because operator<<() takes only one
  // parameter, so we cannot override with a function that takes two.
  friend ::std::ostream &operator<<(::std::ostream &out, const Term &t);
  friend Term operator+(const Term &a, const Term &b);
  // Without the following two operators, Polynomial::operator==() called
  // itself.
  friend bool operator==(const Term &a, const Term &b);
  friend bool operator!=(const Term &a, const Term &b);

  int exponent;
  double coefficient;
  std::unique_ptr<Term> next;
};

Term operator+(const Term &a, const Term &b);
bool operator==(const Term &a, const Term &b);
bool operator!=(const Term &a, const Term &b);
::std::ostream &operator<<(::std::ostream &out, const Term &t);

// Numerically sort array1, and reorder array2 the same way.
template <long unsigned int N>
void SyncSortTwoArrays(::std::array<int, N> *arr1,
                       ::std::array<double, N> *arr2, int index);

} // namespace term

#include "term_impl.hh"

#endif
