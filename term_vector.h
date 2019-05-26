#ifndef TERM_VECTOR_H
#define TERM_VECTOR_H

#include "term.h"

#include <cassert>

#include <array>
#include <vector>

namespace termvector {

class TermVector {
public:
  TermVector() : termvec_(0), size_(0) {}
  TermVector(::std::vector<double> coeff, ::std::vector<int> expon);
  template <long unsigned int N>
  TermVector(::std::array<double, N> coeff, ::std::array<int, N> expon);
  ~TermVector() { delete[] termvec_; }
  term::Term *begin() { return &termvec_[0]; }
  term::Term *end() { return &termvec_[size_]; }
  bool empty() const { return (0 == size_); }
  int size() const { return size_; }
  const term::Term &at(int n) const;

private:
  term::Term *termvec_;
  int size_;
};

} // namespace termvector

#endif
