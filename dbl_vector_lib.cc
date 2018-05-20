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
}

DoubleVector::DoubleVector(const DoubleVector &v) {
  p_ = new double[v.size_];
  size_ = v.size_;
  assert(p_ != 0);
  int i = 0;
  while (i < v.ub()) {
    p_[i] = v.p_[i];
    i++;
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

void DoubleVector::Add(const DoubleVector &a) {
  assert(size_ == a.size_);
  for (int i = 0; i < size_; i++) {
    p_[i] += a.p_[i];
  }
}

DoubleVector SumVectors(const DoubleVector &a, const DoubleVector &b) {
  assert(a.ub() == b.ub());
  DoubleVector out(a);
  out.Add(b);
  return out;
}

double DoubleVector::Amplitude() const {
  int i = 0;
  double sum = 0;
  while (i <= ub()) {
    sum += p_[i] * p_[i];
    i++;
  }
  return sum;
}

void DoubleVector::Scale(const double scale) {
  int i = 0;
  while (i <= ub()) {
    p_[i] *= scale;
    i++;
  }
  return;
}
}
