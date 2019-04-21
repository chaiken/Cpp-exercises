// https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
// A 'specialization' of a template here means a particular value of the
// template parameter.

#ifndef POLYNOMIAL_IMPL_H
#define POLYNOMIAL_IMPL_H

#include "term.h"
#include "term_impl.h"

#include <cassert>

namespace polynomial {

// degree_ is never used for anything, so ditch it.
template <long unsigned int N>
Polynomial::Polynomial(::std::array<double, N> coef,
                       ::std::array<int, N> expon) {
  log("Polynomial arrays constructor");
  term::SyncSortTwoArrays(&expon, &coef, 0);
  // Head of list.
  term::Term *temp = new term::Term(expon[0], coef[0]);
  assert(temp != 0);
  h_ = 0;
  Prepend(temp);

  // Other nodes.
  for (unsigned i = 1; i < static_cast<unsigned>(N); i++) {
    assert(expon[i - 1] < expon[i]);
    term::Term *temp = new term::Term(expon[i], coef[i]);
    assert(temp != 0);
    Prepend(temp);
  }
  // degree_ = h_->exponent;
}
} // namespace polynomial

#endif
