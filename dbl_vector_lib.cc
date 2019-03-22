#include "dbl_vector.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

namespace dbl_vect {

double &DoubleVectorIterator::operator++() const { return (*dv_)[++position_]; }

double &Max(DoubleVector &v) {
  DoubleVectorIterator iter(v);
  int max = 0, i = 0;
  double a = *(v.begin());
  // The begin() and end() are those of DoubleVector and are accessed via
  // operator->().  The -> here does not indicate a pointer-based lookup.
  for (double *b = iter->begin(); b != iter->end(); b++) {
    if (*b > a) {
      a = *b;
      max = i;
    }
    i++;
  }
  // iter->ub() uses DoubleVector's member-access overload operator->().
  // v.ub() would use the DoubleVector object's call directly.
  assert(max >= 0 && max <= iter->ub());
  return v[max];
}

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
  while (i <= v.ub()) {
    p_[i] = v.p_[i];
    i++;
  }
}

DoubleVector::DoubleVector(const vector<double> v) {
  assert(v.size() > 0);
  size_ = v.size();
  p_ = new double[size_];
  for (int i = 0; i < size_; i++) {
    p_[i] = v.at(i);
  }
}

DoubleVector::DoubleVector(const double *v, int sz) : size_(sz) {
  p_ = new double[sz];
  assert(p_ != 0);
  size_ = sz;
  for (int i = 0; i < sz; i++) {
    p_[i] = *(v + i);
  }
}

double &DoubleVector::Element(int i) {
  assert(i >= 0 && i < size_);
  return p_[i];
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

double &DoubleVector::operator[](int i) {
  assert((i >= 0) && (i <= ub()));
  return p_[i];
}

DoubleVector &DoubleVector::operator=(const DoubleVector &v) {
  assert(ub() == v.ub());
  if (this != &v) {
    for (int i = 0; i <= ub(); i++) {
      p_[i] = v.p_[i];
    }
  }
  // To return a reference, we can't return 'this', as it is a pointer to an
  // object, not the object itself.
  return *this;
}

bool DoubleVector::operator==(DoubleVector &v) {
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

// Below is the text's function signature, but it must be wrong.
// As stated, the function takes consts as arguments and returns nothing, so
// what good is it?
// void SumVectors(const DoubleVector &a, const DoubleVector &b) {
DoubleVector SumVectors(const DoubleVector &a, const DoubleVector &b) {
  assert(a.ub() == b.ub());
  DoubleVector c(a);
  return c.Add(b);
}

DoubleVector operator+(DoubleVector &a, DoubleVector &b) {
  assert(a.ub() == b.ub());
  vector<double> temp;
  for (int i = 0; i <= a.ub(); i++) {
    temp.push_back(a[i] + b[i]);
  }
  return temp;
}

} // namespace dbl_vect
