#include "complex_vector.h"

#include <cassert>

#include <vector>

using namespace std;
using namespace complex;

namespace complex_vec {

ComplexVector::ComplexVector(const Complex &x) : size_(1) {
  c_ = std::unique_ptr<Complex[]>(new Complex[1]);
  c_.get()[0] = x;
}

ComplexVector::ComplexVector(const Complex x[], int count) : size_(count) {
  c_ = std::unique_ptr<Complex[]>(new Complex[size_]);
  int i = 0;
  while (i < count) {
    c_.get()[i] = x[i];
    i++;
  }
}

ComplexVector::ComplexVector(const vector<Complex> &x) : size_(x.size()) {
  c_ = std::unique_ptr<Complex[]>(new Complex[size_]);
  int i = 0;
  while (i < size_) {
    c_.get()[i] = x.at(i);
    i++;
  }
}

// If the function calls only public functions of the class like at(), it
// could be a non-member, except that "Overloading assignment and subscription
// functions share several characteristics.   Both must be done as nonstatic
// member functions, and both usually involve a reference return type."
Complex &ComplexVector::operator[](int i) {
  assert((i >= 0) && (i < size_));
  return c_.get()[i];
}

bool ComplexVector::operator==(ComplexVector &c) {
  int i;
  if (ub() != c.ub()) {
    return false;
  }
  for (i = 0; i <= ub(); i++) {
    if ((*this)[i] != c[i]) {
      return false;
    }
  }
  return true;
}

bool ComplexVector::operator!=(ComplexVector &c) { return (!operator==(c)); }

ComplexVector &ComplexVector::operator=(const ComplexVector &cv) {
  // if (*this == c) results in
  // "error: no match for ‘operator==’ (operand types are
  // ‘complex_vec::ComplexVector’ and ‘const complex_vec::ComplexVector’)"
  if (this != &cv) {
    assert(ub() == cv.ub());
    for (int i = 0; i <= cv.ub(); i++) {
      c_.get()[i] = cv.c_.get()[i];
    }
  }
  return *this;
}

// Use at() to access Complex objects from ComplexVector object, and then
// call Complex's overloaded << operator.  When the ComplexVector parameter in
// the function definition below was not a const reference, the function exit
// destroyed the ComplexVector object!
ostream &operator<<(ostream &out, const ComplexVector &v) {
  int i = 0;
  while (i <= v.ub()) {
    out << v.at(i++) << ends;
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
    Complex c(v.c_.get()[i] + w.c_.get()[i]);
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
