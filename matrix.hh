#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <complex>
#include <memory>
#include <vector>

#include "dbl_vector.hh"

/* namespace dbl_vect {
class DoubleVector;
} */

namespace matrix {

class MatrixIterator;

enum transform { copy, transpose, negative, upper };

// A Matrix is an array of DoubleVectors.
class Matrix {
public:
  Matrix(int d1, int d2, int offset = 0);
  Matrix(int d1, int d2, std::vector<double> &inputs, int offset = 0);
  Matrix(const Matrix &a, transform t);
  // square sub-matrix constructor
  Matrix(const Matrix &a, std::vector<int> rows, std::vector<int> cols);
  Matrix(const Matrix &m) = delete;
  Matrix(Matrix &&m) = default;
  int numrows() const { return size1_; }
  int numcols() const { return size2_; }
  // Public accessors handle the offset. Private functions continue with
  // zero-based arrays.
  int ub1() const { return (start_ + (numrows() - 1)); }
  int ub2() const { return (start_ + (numcols() - 1)); }
  int lb() const { return start_; }
  double Element(int i, int j) const;
  // operator[] is needed in order to allow MatrixIterator to access elements
  // from a Matrix*.   Otherwise, the compiler tries DoubleVector's operator[],
  // which doesn't work.
  dbl_vect::DoubleVector &operator[](int i);
  friend class MatrixIterator;
  friend Matrix Add(const dbl_vect::DoubleVector &v, const Matrix &m);
  friend dbl_vect::DoubleVector Multiply(const dbl_vect::DoubleVector &v,
                                         const Matrix &m);

private:
  double InternalElement(int i, int j) const;
  std::unique_ptr<dbl_vect::DoubleVector[]> p_;
  // start_ is the publicly visible index of the first element, presumably 0
  // or 1.
  int size1_, size2_, start_ = 0;
};

// smart pointers, arrays and iterators:
// https://www.reddit.com/r/cpp/comments/qfce8b/c_smart_pointers_and_arrays/
class MatrixIterator {
public:
  MatrixIterator(Matrix &m)
      : position_(0), rownum_(m.numrows()), colnum_(m.numcols()), mp_(&m) {}
  // TODO: change Iterate() to operator++().
  double &Iterate();
  double &successor();
  double &predecessor();
  void reset();
  double &item();
  friend std::ostream &operator<<(std::ostream &out, const Matrix &a);

private:
  int RowIndex(int offset = 0);
  int ColIndex(int offset = 0);
  // position_ is the index of the element enumerated as if the matrix were a 1D
  // array.
  int position_, rownum_, colnum_;
  // I considered DoubleVector** instead, but the usual practice is that
  // FooIterator manipulates a pointer to Foo.
  Matrix *mp_;
};

double Trace(const Matrix &a);
// The characteristic polynomial is the sum of terms consisting of the diagonal
// elements of the matrix minus lambda times the subdeterminant that multiples
// that element.
std::array<std::complex<double>, 2>
GetQuadraticRoots(const std::vector<double> coeffs);
std::pair<std::vector<double>, bool>
GetCharacteristicPolynomialCoefficients(const Matrix &a);
std::pair<double, bool> Determinant(const Matrix &a, double sum);
std::ostream &operator<<(std::ostream &out, const Matrix &a);
double Max(Matrix &m);

// https://stackoverflow.com/questions/16718166/friend-function-declaration-definition-inside-a-namespace
// Q.: Why do I have to declare a friend function twice?
// Because the friend declaration does not provide a usable declaration of the
// function in the namespace. It declares that, if that function is declared in
// that namespace, it will be a friend.
Matrix Add(const dbl_vect::DoubleVector &v, const Matrix &m);
dbl_vect::DoubleVector Multiply(const dbl_vect::DoubleVector &v,
                                const Matrix &m);
} // namespace matrix

#endif
