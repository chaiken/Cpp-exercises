// https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
// A 'specialization' of a template here means a particular value of the
// template parameter.

#include <cassert>

namespace polynomial {

// Numerically sort array1, and reorder array2 the same way.
template <long unsigned int N>
void SyncSortTwoArrays(::std::array<int, N> *array1,
                       ::std::array<double, N> *array2, int index) {
  if ((N - 1) == static_cast<long unsigned int>(index)) {
    return;
  }
  double dparam;
  int iparam;
  if ((*array1)[index] > (*array1)[index + 1]) {
    iparam = (*array1)[index + 1];
    (*array1)[index + 1] = (*array1)[index];
    (*array1)[index] = iparam;
    dparam = (*array2)[index + 1];
    (*array2)[index + 1] = (*array2)[index];
    (*array2)[index] = dparam;
  }
  // index++ below resulted in stack overflow.
  SyncSortTwoArrays(array1, array2, ++index);
  // Elements at index and index+1 are now sorted, but elements at index and
  // index-1 may be out of order.
  if ((index >= 1) && ((*array1)[index - 1] > (*array1)[index])) {
    SyncSortTwoArrays(array1, array2, --index);
  }
}

// degree_ is never used for anything, so ditch it.
template <long unsigned int N>
Polynomial::Polynomial(::std::array<double, N> coef,
                       ::std::array<int, N> expon) {
  SyncSortTwoArrays(&expon, &coef, 0);
  // Head of list.
  term *temp = new term(expon[0], coef[0]);
  assert(temp != 0);
  h_ = 0;
  Prepend(temp);

  // Other nodes.
  for (unsigned i = 1; i < static_cast<unsigned>(N); i++) {
    assert(expon[i - 1] < expon[i]);
    term *temp = new term(expon[i], coef[i]);
    assert(temp != 0);
    Prepend(temp);
  }
  // degree_ = h_->exponent;
}
} // namespace polynomial
