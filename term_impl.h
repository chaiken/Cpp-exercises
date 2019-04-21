#ifndef TERM_IMPL_H
#define TERM_IMPL_H

#include <array>

namespace term {

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

} // namespace term

#endif
