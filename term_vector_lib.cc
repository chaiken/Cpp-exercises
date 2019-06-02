#include "term_vector.h"

#include <iostream>

using namespace std;

namespace termvector {

// ToDo: sort these terms by exponent, as with the array constructor.
TermVector::TermVector(vector<double> coeff, vector<int> expon) {
  cout << "TermVector vector constructor" << endl;
  assert(coeff.size() == expon.size());
  size_ = coeff.size();
  termvec_ = new term::Term[size_];
  int i = 0;
  for (vector<int>::iterator it = expon.begin(); it != expon.end(); it++) {
    termvec_[i] = term::Term(expon.at(i), coeff.at(i));
    i++;
  }
}

TermVector::~TermVector() {
  delete[] termvec_;
  // Following
  // https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/
  termvec_ = 0;
}

const term::Term &TermVector::at(int n) const {
  assert((n >= 0) && (n < size_));
  return termvec_[n];
}

} // namespace termvector
