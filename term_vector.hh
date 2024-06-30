#ifndef TERM_VECTOR_H
#define TERM_VECTOR_H

#include "term.hh"

#include <cassert>

#include <array>
#include <memory>
#include <vector>

namespace termvector {

class TermVector {
public:
  //  TermVector() : size_(1u)  {
  TermVector() : size_(1u), termvec_(new term::Term(0, 0.0)) {
    //    termvec_ = std::make_unique<term::Term>(term::Term(0, 0.0));
  }
  TermVector(::std::vector<double> coeff, ::std::vector<int> expon);

  template <long unsigned int N>
  TermVector(::std::array<double, N> coeff, ::std::array<int, N> expon);

  term::Term *begin() const { return (&termvec_[0]); }
  term::Term *end() const { return (&termvec_[size_]); }
  bool empty() const { return (0 == size_); }
  int size() const { return size_; }
  term::Term &at(int n) const;

private:
  int size_;
  std::unique_ptr<term::Term[]> termvec_;
};

} // namespace termvector

#endif
