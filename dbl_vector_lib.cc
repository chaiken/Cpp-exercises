#include "dbl_vector.hh"
#ifdef DEBUG
#include <cassert>
#endif
#include <cmath>
#include <iostream>

using namespace std;

namespace dbl_vect {

double &DoubleVectorIterator::operator++() const { return (*dv_)[++position_]; }

double Max(DoubleVector &v) {
  DoubleVectorIterator iter(v);
  double max = *(v.begin());
  // The begin() and end() are those of DoubleVector and are accessed via
  // operator->().  The -> here does not indicate a pointer-based lookup.
  for (double *b = iter->begin(); b != iter->end(); b++) {
    if (*b > max) {
      max = *b;
    }
  }
  return max;
}

DoubleVector::DoubleVector(const vector<double> &v) {
  size_ = v.size();
  p_ = std::unique_ptr<double[]>(new double[size_]);
  for (int i = 0; i < size_; i++) {
    p_[i] = v.at(i);
  }
}

ostream &operator<<(ostream &out, DoubleVector &dv) {
  int i = 0;
  DoubleVectorIterator iter(dv);
  // -> is once again associated with operator->() override.
  for (double *a = iter->begin(); a != iter->end(); a++, i++) {
    out << i << "," << *a << " ";
  }
  out << endl;
  return out;
}

double DoubleVector::DotProduct(const DoubleVector &v) const {
#ifdef DEBUG
  assert(size_ == v.size_);
#endif
  double sum = 0.0;
  for (int i = 0; i < size_; i++) {
    sum += p_[i] * v.p_[i];
  }
  return sum;
}

double DoubleVector::SumElements() const {
  double sum = 0;
  int i = ub();
  while (i >= 0) {
    sum += p_[i];
    i--;
  }
  return sum;
}

double DoubleVector::Amplitude() const {
  double sum = DotProduct(*this);
  return sqrt(sum);
}

DoubleVector DoubleVector::operator*(const double scale) {
  int i = 0;
  vector<double> output;
  while (i <= ub()) {
    output.push_back(p_[i] * scale);
    i++;
  }
  return output;
}

double &DoubleVector::operator[](int i) {
#ifdef DEBUG
  assert((i >= 0) && (i <= ub()));
#endif
  return p_[i];
}

bool DoubleVector::operator==(const DoubleVector &v) const {
  if (ub() != v.ub()) {
    return false;
  }
  for (int i = 0; i < ub(); i++) {
    if (p_[i] != v.p_[i]) {
      return false;
    }
  }
  return true;
}

bool DoubleVector::operator!=(const DoubleVector &v) const {
  return (!operator==(v));
}

// Below is the textbook's function signature, but it must be wrong.
// As stated, the function takes consts as arguments and returns nothing, so
// what good is it?
// void SumVectors(const DoubleVector &a, const DoubleVector &b) {
DoubleVector SumVectors(DoubleVector &&a, DoubleVector &&b) {
#ifdef DEBUG
  assert(a.ub() == b.ub());
#endif
  DoubleVector c(a + b);
  return c;
}

DoubleVector operator+(DoubleVector &a, DoubleVector &b) {
#ifdef DEBUG
  assert(a.ub() == b.ub());
#endif
  vector<double> temp;
  for (int i = 0; i <= a.ub(); i++) {
    temp.push_back(a[i] + b[i]);
  }
  return temp;
}

} // namespace dbl_vect
