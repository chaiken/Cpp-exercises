#include "complex_vector.h"

#include <cassert>

#include <vector>

using namespace std;
using namespace complex;

namespace complex_vec {

Complex &ComplexVector::operator[](int i) {
  assert((i >= 0) && (i < size_));
  return c_[i];
}

// Copy constructor.
ComplexVector::ComplexVector(const Complex &x) {
  size_ = 1;
  c_ = new Complex[1];
  // c_ = x; << doesn't work.
  c_[0] = x;
}

ComplexVector::ComplexVector(const Complex x[], int count) {
  size_ = count;
  int i = 0;
  c_ = new Complex[size_];
  while (i < count) {
    c_[i] = x[i];
    i++;
  }
}

ComplexVector::ComplexVector(const vector<Complex> &x) {
  size_ = x.size();
  int i = 0;
  c_ = new Complex[size_];
  while (i < size_) {
    c_[i] = x.at(i);
    i++;
  }
}

// Use element() to access Complex objects from ComplexVector object, and then
// call Complex's overloaded << operator.  When the ComplexVector parameter in
// the function definition below was not a const reference, the function exit
// destroyed the ComplexVector object!
ostream &operator<<(ostream &out, const ComplexVector &v) {
  int i = 0;
  while (i <= v.ub()) {
    out << v.element(i++) << ends;
    out << "; ";
  }
  out << endl;
  return out;
}

ComplexVector operator+(const ComplexVector &v, const ComplexVector &w) {
  assert(v.ub() == w.ub());
  int i = 0;
  vector<Complex> sumvec;
  while (i <= v.ub()) {
    Complex c(v.c_[i] + w.c_[i]);
    sumvec.push_back(c);
    i++;
  }
  return sumvec;
}

// The parameters passed to the operator cannot be constified, as the subscript
// operator of Complex does not itself return a const.
vector<double> operator*(ComplexVector &v, ComplexVector &w) {
  assert(v.ub() == w.ub());
  vector<double> sumvec;
  int i = 0;
  while (i <= v.ub()) {
    sumvec.push_back(complex::Dot(v[i], w[i]));
    i++;
  }
  return sumvec;
}

} // namespace complex_vec
