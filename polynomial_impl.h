// https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
// A 'specialization' of a template here means a particular value of the
// template parameter.

#ifndef POLYNOMIAL_IMPL_H
#define POLYNOMIAL_IMPL_H

#include "polynomial.h"
#include "term.h"
#include "term_impl.h"
#include "term_vector.h"

#include <cassert>

namespace polynomial {

// degree_ is never used for anything, so ditch it.
template <long unsigned int N>
Polynomial::Polynomial(::std::array<double, N> coef,
                       ::std::array<int, N> expon) {
  log("Polynomial arrays constructor");
  if (coef.empty() || expon.empty()) {
    std::cerr
        << "Cannot create a polynomial from empty coefficients or exponent.";
    assert_perror(EINVAL);
  }
  term::SyncSortTwoArrays(&expon, &coef, 0);
  // Head of list.
  term::Term *temp = new term::Term(expon[0], coef[0]);
  assert(temp != 0);
  h_ = 0;
  Prepend(temp);

  // Other nodes.
  for (int i = 1; i < static_cast<int>(N); i++) {
    assert(expon[i - 1] < expon[i]);
    term::Term *temp = new term::Term(expon[i], coef[i]);
    assert(temp != 0);
    Prepend(temp);
  }
  // degree_ = h_->exponent;
}
} // namespace polynomial

// I tried placing the following code in a term_vector_impl.h and then referring
// to it in a term_vector_lib_test.cc, but could not get rid of either linker
// failures or compilation failures (could not find class TermVector) depending
// on the order of header files.
namespace termvector {

template <long unsigned int N>
TermVector::TermVector(::std::array<double, N> coeff,
                       ::std::array<int, N> expon) {
#ifdef DEBUG
  ::std::cout << "TermVector arrays constructor" << ::std::endl;
#endif
  term::SyncSortTwoArrays(&expon, &coeff, 0);
  size_ = N;
  termvec_ = new term::Term[size_];
  termvec_[0] = term::Term(expon[0], coeff[0]);

  for (unsigned i = 1; i < static_cast<unsigned>(size_); i++) {
    assert(expon[i - 1] < expon[i]);
    termvec_[i] = term::Term(expon[i], coeff[i]);
  }
}
} // namespace termvector

#endif
