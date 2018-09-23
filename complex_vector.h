// -*-C++-*-

#ifndef COMPLEX_VECTOR_H
#define COMPLEX_VECTOR_H

#include "complex.h"

#include <vector>

namespace complex_vec {
class ComplexVector {
public:
  ComplexVector() : c_(0), size_(0) {
    ::std::cout << "Default constructor." << ::std::endl;
  }
  ComplexVector(const complex::Complex &x);
  ComplexVector(const ComplexVector &x) : c_(x.c_), size_(x.size_) {}
  ComplexVector(const ::std::vector<complex::Complex> &x);
  ComplexVector(const complex::Complex x[], int count);
  ~ComplexVector() { delete[] c_; }
  int ub() const { return size_ - 1; }
  const complex::Complex &element(int i) const { return c_[i]; }
  complex::Complex &operator[](int i);
  bool operator==(ComplexVector &c);
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const ComplexVector &v);
  friend ComplexVector operator+(const ComplexVector &v,
                                 const ComplexVector &w);
  friend ::std::vector<double> operator*(ComplexVector &v, ComplexVector &w);

private:
  complex::Complex *c_;
  int size_;
};

::std::ostream &operator<<(::std::ostream &out, const ComplexVector &v);
ComplexVector operator+(const ComplexVector &v, const ComplexVector &w);
::std::vector<double> operator*(ComplexVector &v, ComplexVector &w);
} // namespace complex_vec

#endif
