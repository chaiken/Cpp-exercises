// https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
// A 'specialization' of a template here means a particular value of the
// template parameter.

#ifndef POLYNOMIAL_IMPL_H
#define POLYNOMIAL_IMPL_H

#include "term.h"
#include "term_vector.h"

#include <cassert>

namespace polynomial {

template <long unsigned int N>
Polynomial::Polynomial(std::array<double, N> coef, std::array<int, N> expon) {
#ifdef DEBUG
  log("Polynomial arrays constructor");
#endif
  // Following is covered by a DEATH test that gcov does not recognize.
  if (coef.empty() || expon.empty()) {
    std::cerr
        << "Cannot create a polynomial from empty coefficients or exponent.";
    assert_perror(EINVAL);
  }
  term::SyncSortTwoArrays(&expon, &coef, 0);
  // Does not work, as  it overwrites the class member of the same name!
  //     std::unique_ptr<term::Term> h_(new term::Term(expon[0], coef[0]));
  h_ = std::unique_ptr<term::Term>(new term::Term(expon[0], coef[0]));

  // Other nodes.
  for (int i = 1; i < static_cast<int>(N); i++) {
    Prepend(std::unique_ptr<term::Term>(new term::Term(expon[i], coef[i])));
  }
}

} // namespace polynomial

// I tried placing the following code in a term_vector_impl.h and then referring
// to it in a term_vector_lib_test.cc, but could not get rid of either linker
// failures or compilation failures (could not find class TermVector) depending
// on the order of header files.
namespace termvector {

template <long unsigned int N>
TermVector::TermVector(std::array<double, N> coeff, std::array<int, N> expon) {
#ifdef DEBUG
  std::cerr << "TermVector arrays constructor" << std::endl;
#endif
  term::SyncSortTwoArrays(&expon, &coeff, 0);
  size_ = N;
  // Performs allocation.
  //  termvec_[0] = std::make_unique<term::Term>(Term(expon[0], coeff[0]));
  assert(coeff.size() == static_cast<unsigned>(size_));
  assert(expon.size() == static_cast<unsigned>(size_));
  termvec_ = std::make_unique<term::Term[]>(3u);

  for (unsigned i = 0; i < static_cast<unsigned>(size_); i++) {
    termvec_[i].exponent = expon[i];
    termvec_[i].coefficient = coeff[i];
  }
}
} // namespace termvector

#endif
