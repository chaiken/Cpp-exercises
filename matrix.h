#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <complex>
#include <vector>

namespace dbl_vect {
class DoubleVector;
}

namespace matrix {

class MatrixIterator;

enum transform { copy, transpose, negative, upper };

class Matrix {
public:
  Matrix(int d1, int d2);
  Matrix(int d1, int d2, ::std::vector<double> inputs);
  Matrix(const Matrix &a, transform t);
  // square sub-matrix constructor
  Matrix(const Matrix &a, ::std::vector<int> rows, ::std::vector<int> cols);
  ~Matrix();
  int ub1() const { return (size1_ - 1); }
  int ub2() const { return (size2_ - 1); }
  double &Element(int i, int j) const;
  friend class MatrixIterator;
  friend Matrix Add(const dbl_vect::DoubleVector &v, const Matrix &m);
  friend dbl_vect::DoubleVector Multiply(const dbl_vect::DoubleVector &v,
                                         const Matrix &m);

private:
  double **p_;
  int size1_, size2_;
};

class MatrixIterator {
public:
  MatrixIterator(Matrix &m)
      : elem_(m.p_), position_(0), rownum_(m.size1_), colnum_(m.size2_) {}
  double &Iterate();
  double &successor();
  double &predecessor();
  void reset();
  double &item();
  friend ::std::ostream &operator<<(::std::ostream &out, const Matrix &a);

private:
  int RowIndex(int offset = 0);
  int ColIndex(int offset = 0);
  double **elem_;
  int position_, rownum_, colnum_;
};

double Trace(const Matrix &a);
// The characteristic polynomial is the sum of terms consisting of the diagonal
// elements of the matrix minus lambda times the subdeterminant that multiples
// that element.
::std::array<::std::complex<double>, 2>
GetQuadraticRoots(const ::std::vector<double> coeffs);
::std::vector<double> GetCharacteristicPolynomialCoefficients(const Matrix &a);
double Determinant(const Matrix &a, double sum);
::std::ostream &operator<<(::std::ostream &out, const Matrix &a);
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
