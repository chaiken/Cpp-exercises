// -*-C++-*-

#ifndef COMPLEX_VECTOR_H
#define COMPLEX_VECTOR_H

#include "complex.hh"

#include <memory>
#include <vector>

namespace complex_vec {
class ComplexVector {
public:
  ComplexVector() : size_(0), c_(std::unique_ptr<complex::Complex[]>()) {}
  ComplexVector(const complex::Complex &x);
  ComplexVector(const ComplexVector &x) = delete;
  ComplexVector(ComplexVector &&x) = default;
  ComplexVector(const ::std::vector<complex::Complex> &x);
  ComplexVector(const complex::Complex x[], int count);
  /* The following results in a double-free if c_ is a unique_ptr to a Complex
     object rather than to an array.
  ~ComplexVector() {
    for (int i=0; i < size_; i++) {
      delete[] c_.get();
    }
    } */
  int ub() const { return size_ - 1; }
  complex::Complex &at(int i) const { return c_.get()[i]; }
  complex::Complex &operator[](int i);
  bool operator==(ComplexVector &c);
  bool operator!=(ComplexVector &c);
  ComplexVector &operator=(const ComplexVector &c);
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const ComplexVector &v);
  friend ComplexVector operator+(const ComplexVector &v,
                                 const ComplexVector &w);
  friend ::std::vector<double> operator*(ComplexVector &v, ComplexVector &w);

private:
  int size_;
  // If c_ is instead   std::unique_ptr<complex::Complex> c_, then the generated
  // ComplexVector destructor does not match the type of the variable to be
  // deleted.
  // ==ERROR: AddressSanitizer: alloc-dealloc-mismatch (operator new [] vs
  // operator delete) on 0x6020000002f0
  // Why manage the object as pointer in the first place?  Since an array whose
  // size is known only at runtime must be allocated on the heap.
  std::unique_ptr<complex::Complex[]> c_;
};

::std::ostream &operator<<(::std::ostream &out, const ComplexVector &v);
ComplexVector operator+(const ComplexVector &v, const ComplexVector &w);
::std::vector<double> operator*(ComplexVector &v, ComplexVector &w);
} // namespace complex_vec

#endif
