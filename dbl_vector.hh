#ifndef DBL_VECTOR_H
#define DBL_VECTOR_H

// Implementation of a safe array type dbl_vect.
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

// This forward declaration is needed so that the forward declaration of
// Multiply compiles.
namespace dbl_vect {
class DoubleVector;
}

// Both these forward declarations are needed so that the friend declaration
// inside DoubleVector compiles.  Note that we do not need to include the
// matrix.hh header here even though we need to include dbl_vector.h inside
// matrix.hh.  That's because the function definition is part of matrix_lib.cc
// and needs the symbols of dbl_vector.cc.
namespace matrix {
class Matrix;
Matrix Add(const dbl_vect::DoubleVector &v, const Matrix &m);
dbl_vect::DoubleVector Multiply(const dbl_vect::DoubleVector &v,
                                const Matrix &m);
} // namespace matrix

namespace dbl_vect {

double Max(DoubleVector &);

// arrays, smart pointers and iterators:
// https://www.reddit.com/r/cpp/comments/qfce8b/c_smart_pointers_and_arrays/
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
  double &operator++() const;

private:
  DoubleVector *dv_;
  mutable int position_;
};

class DoubleVector {
public:
  // https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean
  // 'explicit' means that a cast is required if an int is provided when the
  // matching function definition specifies a DoubleVector.
  explicit DoubleVector()
      : size_(10), p_(std::unique_ptr<double[]>(new double[10])) {}
  DoubleVector(int n)
      : size_(n), p_(std::unique_ptr<double[]>(new double[n])) {}
  DoubleVector(const DoubleVector &v) = delete;
  DoubleVector(DoubleVector &&v) = default;
  DoubleVector(double v[], int sz)
      : size_(sz), p_(std::unique_ptr<double[]>(new double[size_])) {
    for (int i = 0; i < size_; i++) {
      p_[i] = v[i];
    }
  }
  DoubleVector(const ::std::vector<double> &v);
  double &Element(int i) {
    assert(i >= 0 && i < size_);
    return p_[i];
  }
  double &Iterate();
  int ub() const { return (size_ - 1); }
  double SumElements() const;
  double DotProduct(const DoubleVector &v) const;
  double Amplitude() const;
  DoubleVector operator*(const double scale);
  double &operator[](int i);
  double *begin() const { return &p_[0]; }
  // http://www.cplusplus.com/reference/iterator/end/
  // Returns an iterator pointing to the past-the-end element in the sequence:
  // Array: The function returns arr+N.
  double *end() const { return &p_[ub() + 1]; }
  DoubleVector &operator=(const DoubleVector &v) = delete;
  DoubleVector &operator=(DoubleVector &&v) = default;
  bool operator==(const DoubleVector &v) const;
  bool operator!=(const DoubleVector &v) const;
  // Not an overload but a friend, since the function signature doesn't match.
  friend ::std::ostream &operator<<(::std::ostream &out, DoubleVector &dv);
  friend DoubleVector operator+(DoubleVector &a, DoubleVector &b);
  friend double &Iterate();
  friend matrix::Matrix matrix::Add(const DoubleVector &v,
                                    const matrix::Matrix &m);
  friend DoubleVector matrix::Multiply(const DoubleVector &v,
                                       const matrix::Matrix &m);

private:
  int size_;
  std::unique_ptr<double[]> p_;
};

::std::ostream &operator<<(::std::ostream &out, DoubleVector &dv);
// Since it's not possible to provide a const overload of operator[] in addition
// to the non-const one, the arguments must be non-const
DoubleVector SumVectors(DoubleVector &&, DoubleVector &&);
DoubleVector operator+(DoubleVector &a, DoubleVector &b);
} // namespace dbl_vect

#endif
