#include "reverse_list.h"

#include <cassert>

#include <iostream>
#include <list>

namespace reverse {

namespace {

// The internal function that is convenient for recursion.
// I tried putting the four function parameters into a templated struct, but
// template argument deduction failed.  When I replaced the template parameters
// with decltype(declval) constructs, the resulting template parameters were
// R-value references which the struct iterator members could not accept as
// values.
template <typename ForwardIterator, typename ReverseIterator>
void ReverseInternal(ForwardIterator fit, ReverseIterator rit,
                     unsigned total_swaps, unsigned swaps) {
  // Done.
  if (swaps >= total_swaps) {
    ::std::cout << "Done." << ::std::endl;
    return;
  }
  ::std::swap(*fit, *rit);
  ::std::cout << "Recursing." << ::std::endl;
  ReverseInternal(++fit, ++rit, total_swaps, ++swaps);
}

} // namespace

// The simple public API.
template <typename T> void PerformReverse(::std::list<T> &alist) {
  // No work to do.
  if ((alist.empty()) || (1U == alist.size())) {
    ::std::cout << "No work to do." << ::std::endl;
    return;
  }
  ReverseInternal(alist.begin(), alist.rbegin(), alist.size() / 2, 0U);
}

} // namespace reverse
