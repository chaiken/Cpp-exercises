/*
The following problem was posed in the "Homework Challenge" feature of
the May 2023 issue of ACCU's _CVu_ journal.''

Choose one and only one element from each row and each column.  Minimize the sum
of the 4 selected elements.

        1	2	3	4
        3	4	7	8
        4	7	9	10
        17	13	14	26

*/

#ifndef TEMPLATE_MATRIX_MIN_H
#define TEMPLATE_MATRIX_MIN_H

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <set>
#include <utility>
#include <variant>

namespace template_matrix_min {

// The ith element of the mask indicates from which column the value in row i is
// selected.
template <int SZ> struct Mask {
  Mask() {
    static_assert(SZ != 0);
    for (uint16_t val : mask_vals) {
      val = 0U;
    }
  }
  Mask(const std::array<uint16_t, SZ> &arr);
  Mask(const Mask &msk) : mask_vals(std::array<uint16_t, SZ>(msk.mask_vals)) {}

  bool swap_cols(const uint16_t first_idx, const uint16_t second_idx);

  Mask<SZ> operator=(const Mask<SZ> &original);

  // https://stackoverflow.com/questions/4660123/overloading-friend-operator-for-template-class
  // In principle, the operator could attempt to print an array with a different
  // SZ than the class, but it's hard to conceive how to express such an
  // attempt.
  template <int I>
  friend std::ostream &operator<<(std::ostream &out, const Mask<I> &m);

  std::array<uint16_t, SZ> mask_vals;
  static std::array<int, SZ * SZ> coeffs;
};

template <int SZ> constexpr uint16_t colnum(const uint16_t index) {
  return (index % SZ);
}

template <int SZ> constexpr uint16_t rownum(const uint16_t index) {
  return abs(index - colnum<SZ>(index)) / SZ;
}

// The returned value indicates if a mask member is in-range for the matrix.
template <int SZ> constexpr bool index_ok(const uint16_t index) {
  if (index > ((SZ * SZ) - 1)) {
    std::cerr << "Index " << index << " out of bounds." << std::endl;
    return false;
  }
  return true;
}

// Given a Mask, calculate the figure-of-merit cost sum.
template <int SZ> constexpr int sum(const Mask<SZ> &selected) {
  return std::accumulate(selected.mask_vals.cbegin(), selected.mask_vals.cend(),
                         0, ([&](int sum = 0, uint16_t idx) {
                           assert(index_ok<SZ>(idx));
                           return sum + Mask<SZ>::coeffs[idx];
                         }));
}

// In essence, mask_compare() is operator<() for Masks.
template <int SZ> bool mask_compare(const Mask<SZ> &a, const Mask<SZ> &b);

template <int SZ>
bool (*comparator)(const Mask<SZ> &, const Mask<SZ> &) = mask_compare;

template <int SZ> void print_result(bool succeeded, const Mask<SZ> &m);

// Inspired by
//  template <class T> using Members = FOLLY_POLY_MEMBERS(&T::bar);
// in folly/folly/Poly.h
// or
// template <class T> using Members = folly::PolyMembers<&T::draw>;
// in folly/folly/docs/Poly.md.
template <int SZ>
using MaskSet =
    typename std::set<Mask<SZ>, bool (*)(const Mask<SZ> &, const Mask<SZ> &)>;

template <int SZ> using MaskSetIterator = typename MaskSet<SZ>::iterator;

// generate_permutations populates the MaskSet with members whose calculated
// cost sum is unique.
template <int SZ> MaskSet<SZ> generate_permutations(const Mask<SZ> &input);

} // namespace template_matrix_min

#include "template_matrix-min_impl.h"

#endif
