#ifndef TEMPLATE_MATRIX_MIN_H
#define TEMPLATE_MATRIX_MIN_H

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
#include <utility>
#include <variant>

namespace template_matrix_min {

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

// Value is in-range for the matrix.
template <int SZ> constexpr bool index_ok(const uint16_t index) {
  if (index > ((SZ * SZ) - 1)) {
    std::cerr << "Index " << index << " out of bounds." << std::endl;
    return false;
  }
  return true;
}

template <int SZ> constexpr int sum(const Mask<SZ> &selected) {
  int sum = 0U;
  for (const uint16_t idx : selected.mask_vals) {
    assert(index_ok<SZ>(idx));
    sum += Mask<SZ>::coeffs[idx];
  }
  return sum;
}

template <int SZ> bool mask_compare(const Mask<SZ> &a, const Mask<SZ> &b);

template <int SZ> void print_result(bool succeeded, const Mask<SZ> &m);

template <int SZ>
bool (*comparator)(const Mask<SZ> &, const Mask<SZ> &) = mask_compare;

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

template <int SZ> MaskSet<SZ> generate_permutations(const Mask<SZ> &input);

} // namespace template_matrix_min

#include "template_matrix-min_impl.h"

#endif
