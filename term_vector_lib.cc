#include "term_vector.hh"

#include <iostream>

using namespace std;

namespace termvector {

// ToDo: sort these terms by exponent, as with the array constructor.
TermVector::TermVector(vector<double> coeff, vector<int> expon) {
  assert(coeff.size() == expon.size());
  size_ = coeff.size();
  // How is it that the following compiles?
  termvec_ = std::make_unique<term::Term[]>(size_);
  int i = 0;
  for (vector<int>::iterator it = expon.begin(); it != expon.end(); it++) {
    termvec_[i] = term::Term(*it, coeff.at(i));
    i++;
  }
}

term::Term &TermVector::at(int n) const {
  assert((n >= 0) && (n < size_));
  return termvec_[n];
}

} // namespace termvector
