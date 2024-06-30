namespace template_matrix_min {

template <int SZ> bool operator==(const Mask<SZ> &a, const Mask<SZ> &b) {
  return (a.mask_vals == b.mask_vals);
}
template <int SZ> bool operator!=(const Mask<SZ> &a, const Mask<SZ> &b) {
  return !(a == b);
}

template <int SZ> Mask<SZ>::Mask(const std::array<uint16_t, SZ> &arr) {
  static_assert(SZ != 0);
  mask_vals = arr;
}

template <int SZ> Mask<SZ> Mask<SZ>::operator=(const Mask<SZ> &original) {
  // The following works only with default-constructed lvals and will not modify
  // already initialized masks:
  //  mask_vals(original.mask_vals);
  this->mask_vals = original.mask_vals;
  return *this;
}

// Mask a < Mask b if a's sum is less than b's.   Hence a will compare before b
// in MaskSet.
template <int SZ> bool mask_compare(const Mask<SZ> &a, const Mask<SZ> &b) {
  return (sum<SZ>(a) < sum<SZ>(b));
}

// Safely swap the columns of mask elements.  0U < first_idx, second_idx < SZ
// since the function swaps columns in a Mask, not in the input array.
template <int SZ>
bool Mask<SZ>::swap_cols(const uint16_t first_idx, const uint16_t second_idx) {
  if (first_idx == second_idx) {
    return false;
  }
  // Check preconditions.
  assert((first_idx <= (SZ - 1U)) && (second_idx <= (SZ - 1U)));
  uint16_t first_row = rownum<SZ>(mask_vals[first_idx]);
  uint16_t second_row = rownum<SZ>(mask_vals[second_idx]);
  if (first_row == second_row) {
    return false;
  }

  uint16_t first_col = colnum<SZ>(mask_vals[first_idx]);
  uint16_t second_col = colnum<SZ>(mask_vals[second_idx]);
  if (first_col == second_col) {
    return false;
  }

  this->mask_vals[first_idx] += (second_col - first_col);
  this->mask_vals[second_idx] += (first_col - second_col);
  return true;
}

template <int SZ>
std::ostream &operator<<(std::ostream &out, const Mask<SZ> &m) {
  for (uint16_t val : m.mask_vals) {
    out << val << ' ';
  }
  return out;
}

template <int SZ> void print_result(bool succeeded, const Mask<SZ> &m) {
#ifndef DEBUG
  return;
#endif
  if (succeeded) {
    std::cerr << "Inserted: " << m << std::endl;
  } else {
    std::cerr << "Not inserted: " << m << std::endl;
  }
}

template <int SZ> MaskSet<SZ> generate_permutations(const Mask<SZ> &input) {
  MaskSet<SZ> outputs(comparator<SZ>);
  size_t ctr = 0U;
  for (int i = 0; i < SZ; i++) {
    Mask<SZ> candidate(input);
    for (int j = 0; j < SZ; j++) {
      if ((j != i) && (candidate.swap_cols(i, j))) {
        std::pair<MaskSetIterator<SZ>, bool> result =
            outputs.emplace(candidate);
        print_result(result.second, candidate);
        for (int k = 0; k < SZ; k++) {
          if ((k != j) && (candidate.swap_cols(j, k))) {
            result = outputs.emplace(candidate);
            print_result(result.second, candidate);
          }
          ctr++;
        }
      }
    }
  }
  std::cout << "Inserted " << outputs.size() << " out of " << ctr
            << " total possibilities." << std::endl;
  std::cout << "Minimum cost is " << sum(*outputs.cbegin())
            << " and maximum is " << sum(*outputs.crbegin()) << "."
            << std::endl;
  return outputs;
}

} // namespace template_matrix_min
