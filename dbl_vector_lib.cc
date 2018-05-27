#include "dbl_vector.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

namespace dbl_vect {

DoubleVector::DoubleVector(int n) : size_(n) {
  assert(n > 0);
  p_ = new double[size_];
  assert(p_ != 0);
  int i = 0;
  while (i <= ub()) {
    p_[i] = 0.0;
    i++;
  }
}

DoubleVector::DoubleVector(const DoubleVector &v) {
  p_ = new double[v.size_];
  size_ = v.size_;
  assert(p_ != 0);
  int i = 0;
  while (i <= v.ub()) {
    p_[i] = v.p_[i];
    i++;
  }
}

DoubleVector::DoubleVector(const double *v, int sz) {
  p_ = new double[sz];
  assert(p_ != 0);
  size_ = sz;
  for (int i = 0; i < sz; i++) {
    p_[i] = *(v + i);
  }
}

double &DoubleVector::element(int i) {
  assert(i >= 0 && i < size_);
  return p_[i];
}

void DoubleVector::Print() const {
  cout << "vector of size " << size_ << endl;
  for (int i = 0; i < size_; i++) {
    cout << p_[i] << "\t";
  }
  cout << endl;
}

double DoubleVector::DotProduct(const DoubleVector &v) const {
  assert(size_ == v.size_);
  double sum = 0.0;
  for (int i = 0; i < size_; i++) {
    sum += p_[i] * v.p_[i];
  }
  return sum;
}

double DoubleVector::SumElements() const {
  double sum = 0;
  for (int i = 0; i < size_; i++) {
    sum += p_[i];
  }
  return sum;
}

double DoubleVector::Amplitude() const {
  double sum = DotProduct(*this);
  return sqrt(sum);
}

void DoubleVector::Scale(const double scale) {
  int i = 0;
  while (i <= ub()) {
    p_[i] *= scale;
    i++;
  }
  return;
}

DoubleVector DoubleVector::Add(const DoubleVector &a) {
  assert(ub() == a.ub());
  for (int i = 0; i <= ub(); i++) {
    p_[i] += a.p_[i];
  }
  return *this;
}

// Below is the text's function signature, but it must be wrong.
// As stated, the function takes consts as arguments and returns nothing, so
// what good is it?
// void SumVectors(const DoubleVector &a, const DoubleVector &b) {
DoubleVector SumVectors(const DoubleVector &a, const DoubleVector &b) {
  assert(a.ub() == b.ub());
  DoubleVector c(a);
  return c.Add(b);
}
}
