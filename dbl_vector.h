#ifndef DBL_VECTOR_H
#define DBL_VECTOR_H

// Implementation of a safe array type dbl_vect.
#include <iostream>

// This forward declaration is needed so that the forward declaration of
// Multiply compiles.
namespace dbl_vect {
class DoubleVector;
}

// Both these forward declarations are needed so that the friend declaration
// inside DoubleVector compiles.  Note that we do not need to include the
// matrix.h header here even though we need to include dbl_vector.h inside
// matrix.h.  That's because the function definition is part of matrix_lib.cc
// and needs the symbols of dbl_vector.cc.
namespace matrix {
class Matrix;
Matrix Add(const dbl_vect::DoubleVector &v, const Matrix &m);
dbl_vect::DoubleVector Multiply(const dbl_vect::DoubleVector &v,
                                const Matrix &m);
} // namespace matrix

namespace dbl_vect {

class DoubleVector {
public:
  explicit DoubleVector(int n = 10);
  DoubleVector(const DoubleVector &v);
  DoubleVector(const double *v, int sz);
  ~DoubleVector() {
    ::std::cout << "Destructor." << ::std::endl;
    delete[] p_;
  }
  double &element(int i);
  int ub() const { return (size_ - 1); }
  void Print() const;
  double SumElements() const;
  DoubleVector Add(const DoubleVector &);
  double DotProduct(const DoubleVector &v) const;
  double Amplitude() const;
  void Scale(const double scale);
  friend matrix::Matrix matrix::Add(const DoubleVector &v,
                                    const matrix::Matrix &m);
  friend DoubleVector matrix::Multiply(const DoubleVector &v,
                                       const matrix::Matrix &m);

private:
  double *p_;
  int size_;
};

DoubleVector SumVectors(const DoubleVector &, const DoubleVector &);
} // namespace dbl_vect

#endif
