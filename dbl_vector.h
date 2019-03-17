#ifndef DBL_VECTOR_H
#define DBL_VECTOR_H

// Implementation of a safe array type dbl_vect.
#include <iostream>
#include <vector>

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

double &Max(DoubleVector &);

class DoubleVectorIterator {
public:
  // Can't be const argument, as then dv_ would have to be const, and the
  // operator[] would fail to match.
  DoubleVectorIterator(DoubleVector &v) : dv_(&v), position_(0) {}
  // https://stackoverflow.com/questions/4928557/how-do-i-create-and-use-a-class-arrow-operator
  // The member-access operator provides access to the methods of the
  // DoubleVector class to users of DoubleVectorIterator that don't have
  // information about DoubleVector itself.
  DoubleVector *operator->() { return dv_; }
  double &Iterate() const;

private:
  DoubleVector *dv_;
  mutable int position_;
};

class DoubleVector {
public:
  // https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
  // 'explicit' means that a cast is required if an int is provided when the
  // matching function definition specifies a DoubleVector.
  explicit DoubleVector(int n = 10);
  DoubleVector(const DoubleVector &v);
  DoubleVector(const double *v, int sz);
  DoubleVector(const ::std::vector<double> v);
  ~DoubleVector() {
    ::std::cout << "Destructor." << ::std::endl;
    delete[] p_;
  }
  double &Element(int i);
  double &Iterate();
  int ub() const { return (size_ - 1); }
  void Print() const;
  double SumElements() const;
  DoubleVector Add(const DoubleVector &);
  double DotProduct(const DoubleVector &v) const;
  double Amplitude() const;
  void Scale(const double scale);
  double &operator[](int i);
  DoubleVector &operator=(const DoubleVector &v);
  bool operator==(DoubleVector &v);
  friend DoubleVector operator+(DoubleVector &a, DoubleVector &b);
  friend double &Iterate();
  friend matrix::Matrix matrix::Add(const DoubleVector &v,
                                    const matrix::Matrix &m);
  friend DoubleVector matrix::Multiply(const DoubleVector &v,
                                       const matrix::Matrix &m);

private:
  double *p_;
  int size_;
};

void PrintDblVector(DoubleVector &v);
DoubleVector operator+(DoubleVector &a, DoubleVector &b);
DoubleVector SumVectors(const DoubleVector &, const DoubleVector &);
} // namespace dbl_vect

#endif
