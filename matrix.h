#include <array>
#include <complex>
#include <vector>

namespace matrix {

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

private:
  double **p_;
  int size1_, size2_;
};
// The characteristic polynomial is the sum of terms consisting of the diagonal
// elements of the matrix minus lambda times the subdeterminant that multiples
// that element.

double Trace(const Matrix &a);
::std::array<::std::complex<double>, 2>
GetQuadraticRoots(const ::std::vector<double> coeffs);
::std::vector<double> GetCharacteristicPolynomialCoefficients(const Matrix &a);
double Determinant(const Matrix &a, double sum);
void PrintMatrix(const Matrix &a);
}
