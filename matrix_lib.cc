#include "matrix.h"

#include "dbl_vector.h"

#include <cassert>
#include <cmath>
#include <cstdlib>

#include <array>
#include <complex>
#include <iostream>
#include <vector>

using namespace std;

// Needed so that the definition of the Multiply friend function compiles.   The
// header file above is not enough!
namespace dbl_vect {
class DoubleVector;
}

namespace matrix {

double &MatrixIterator::Iterate() {
  position_++;
  // The row number is the integer part of the sequential position within the
  // array.  The column number is the sequential position minus the number of
  // elements in lower-position full rows.
  int index1 = position_ / colnum_, index2 = position_ - (index1 * colnum_);
  assert(index1 <= rownum_);
  assert(index2 <= colnum_);
  return elem_[index1][index2];
}

double Max(Matrix &m) {
  MatrixIterator iter(m);
  double val, max = m.Element(0, 0);
  int end = (m.ub1() + 1) * (m.ub2() + 1);
  for (int i = 1; i < end; i++) {
    val = iter.Iterate();
    if (val > max) {
      max = val;
    }
  }
  return max;
}

Matrix::Matrix(int d1, int d2) : size1_(d1), size2_(d2) {
  assert(d1 > 0 && d2 > 0);
  p_ = new double *[size1_];
  assert(p_ != 0);
  for (int i = 0; i < size1_; i++) {
    p_[i] = new double[size2_];
    assert(p_[i] != 0);
  }
}
Matrix::Matrix(int d1, int d2, vector<double> inputs) : size1_(d1), size2_(d2) {
  assert(d1 > 0 && d2 > 0);
  assert(static_cast<size_t>(d1 * d2) <= inputs.size());
  p_ = new double *[size1_];
  assert(p_ != 0);
  for (int i = 0; i < size1_; i++) {
    p_[i] = new double[size2_];
    for (int j = 0; j < size2_; j++) {
      p_[i][j] = inputs.front();
      inputs.erase(inputs.begin());
    }
  }
}

// The 1 below is needed because ub1 and ub2 are decremented.
Matrix::Matrix(const Matrix &a, transform t)
    : size1_(a.ub1() + 1), size2_(a.ub2() + 1) {
  p_ = new double *[size1_];
  assert(p_ != 0);
  for (int i = 0; i < size1_; i++) {
    p_[i] = new double[size2_];
    assert(p_[i] != 0);
    for (int j = 0; j < size2_; j++) {
      switch (t) {
      case copy:
        p_[i][j] = a.Element(i, j);
        break;
      case transpose:
        p_[i][j] = a.Element(j, i);
        break;
      case negative:
        p_[i][j] = -a.Element(i, j);
        break;
      case upper:
        if (i <= j) {
          p_[i][j] = a.Element(i, j);
        } else {
          p_[i][j] = 0.0;
        }
        break;
      default:
        cout << "Illegal constructor parameter." << endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

// Submatrix constructor.  Some of the values in rows and cols vectors are zero,
// indicating that the corresponding rows and columns will be omitted.
Matrix::Matrix(const Matrix &a, vector<int> rows, vector<int> cols) {
  // Empty rows vector means use all rows.
  if (!rows.empty()) {
    assert(static_cast<int>(rows.size()) == (a.ub1() + 1));
    size1_ = 0;
    for (int i = 0; i < static_cast<int>(rows.size()); i++) {
      if (rows.at(i) != 0) {
        size1_++;
      }
    }
  } else {
    size1_ = a.ub1() + 1;
  }
  // Empty cols vector means use all cols.
  if (!cols.empty()) {
    assert(static_cast<int>(cols.size()) == (a.ub2() + 1));
    size2_ = 0;
    for (int i = 0; i < static_cast<int>(cols.size()); i++) {
      if (cols.at(i) != 0) {
        size2_++;
      }
    }
  } else {
    size2_ = a.ub2() + 1;
  }
  cout << endl
       << "Creating " << size1_ << " by " << size2_ << " submatrix from "
       << a.size1_ << " by " << a.size2_ << " matrix." << endl;
  p_ = new double *[size1_];
  assert(p_ != 0);
  int out_row = 0;
  for (int i = 0; i <= a.ub1(); i++, out_row++) {
    // skip this row
    if ((!rows.empty()) && (0 == rows.at(i))) {
      out_row--;
      continue;
    }
    // We need to allocate the row vectors at the index of the output array.
    p_[out_row] = new double[size2_];
    assert(p_[out_row] != 0);
    int out_col = 0;
    for (int j = 0; j <= a.ub2(); j++, out_col++) {
      // skip some columns
      if ((!cols.empty()) && (0 == cols.at(j))) {
        out_col--;
      } else {
        p_[out_row][out_col] = a.Element(i, j);
      }
    }
  }
}

Matrix::~Matrix() {
  for (int i = 0; i <= ub1(); i++) {
    delete[] p_[i];
  }
  delete[] p_;
}

double &Matrix::Element(int i, int j) const {
  assert(i >= 0 && i <= ub1() && j >= 0 && j <= ub2());
  return p_[i][j];
}

double Trace(const Matrix &a) {
  assert(a.ub1() == a.ub2());
  double sum = 0.0;
  for (int i = 0; i <= a.ub1(); i++) {
    sum += a.Element(i, i);
  }
  return sum;
}

// Helper function for submatrix definition.
vector<int> ExcludeDesignatedElement(int elemnum, int to_exclude) {
  // Initialize vector to all ones.
  vector<int> vec;
  for (int k = 0; k <= elemnum; k++) {
    vec.push_back(1);
  }
  // We need to remove one element for each submatrix.
  vec.at(to_exclude) = 0;
  return (vec);
}

// x = -b/2a +- 1/2a * sqrt(b^2 - 4ac)
// a = 0, b = 1, c = 2.
array<complex<double>, 2> GetQuadraticRoots(const vector<double> coeffs) {
  assert(3 == coeffs.size());
  assert(coeffs.at(0) != 0.0);

  complex<double> root1, root2;
  array<complex<double>, 2> result;
  double diff =
      ((coeffs.at(1) * coeffs.at(1)) - (4 * coeffs.at(0) * coeffs.at(2)));
  if (diff < 0) {
    result[0] = {(-coeffs.at(1)) / (2 * coeffs.at(0)), sqrt(-diff)};
    result[1] = {(-coeffs.at(1)) / (2 * coeffs.at(0)), -sqrt(-diff)};
  } else {
    double realroot0 = (-coeffs.at(1) + sqrt(diff)) / (2 * coeffs.at(0));
    double realroot1 = (-coeffs.at(1) - sqrt(diff)) / (2 * coeffs.at(0));
    result[0] = {realroot0, 0.0};
    result[1] = {realroot1, 0.0};
  }
  // Notably, complex values are printed fine this way.
  cout << "Eigenvalues are " << result[0] << '\t' << result[1] << endl;
  return result;
}

vector<double> GetCharacteristicPolynomialCoefficients(const Matrix &a) {
  assert(a.ub1() == a.ub2());
  vector<double> coeffs;
  // Coefficient of leading term is always (-)1.
  coeffs.push_back(pow(-1.0, a.ub1() + 1));
  // A 2x2 matrix with elements (a,b) in first row and (c,d) in second has
  // characteristic polynomial (x^2 - (a+d)x + (ab-cd)).
  if (1 == a.ub1()) {
    coeffs.push_back(-1.0 * Trace(a));
    coeffs.push_back(Determinant(a, 0.0));
    cout << "Coefficients are " << coeffs.at(0) << '\t' << coeffs.at(1) << '\t'
         << coeffs.at(2) << endl;
    return coeffs;
  }
  for (int i = 0; i <= a.ub1(); i++) {
    vector<int> excluded = ExcludeDesignatedElement(a.ub1(), i);
    // Create the submatrix from which the coefficient will be calculated.
    Matrix sub(a, excluded, excluded);
    coeffs.push_back(Determinant(sub, 0.0));
  }
  return coeffs;
}

double Determinant(const Matrix &a, double sum) {
  if (a.ub1() != a.ub2()) {
    cout << "Only square matrices have determinants." << endl;
    exit(EXIT_FAILURE);
  }
  if (a.ub1() < 1) {
    return 0;
  }
  if (1 == (a.ub1())) {
    cout << "Taking determinant of 2x2." << endl;
    int val = ((a.Element(0, 0) * a.Element(1, 1)) -
               (a.Element(0, 1) * a.Element(1, 0)));
    cout << "Determinant of 2x2 is " << val << endl;
    // For a 2x2 original matrix, the value below is the final result.
    return val;
  } else {
    // A matrix of rank n has n-1 submatrices whose determinants must each be
    // computed.
    for (int i = 0; i <= a.ub1(); i++) {
      // Initialize row-selection vector to all ones, then remove one row for
      // each submatrix.
      vector<int> rows = ExcludeDesignatedElement(a.ub1(), i);

      for (int j = 0; j <= a.ub2(); j++) {
        // Initialize row-selection vector to all ones, then remove one row for
        // each submatrix.
        vector<int> cols = ExcludeDesignatedElement(a.ub2(), j);
        Matrix submatrix(a, rows, cols);
        cout << "Taking subdeterminant for " << i << "," << j << endl;
        sum += pow(-1, j) * a.Element(i, j) * Determinant(submatrix, sum);
        cout << "Sum is " << sum << " for i " << i << ",j " << j << endl;
      }
      cout << "Done with determinants for " << i << endl;
    }
  }
  return sum;
}

void PrintMatrix(const Matrix &a) {
  cout << endl
       << "Matrix of size " << (a.ub1() + 1) << "x" << (a.ub2() + 1) << endl;
  for (int i = 0; i <= a.ub1(); i++) {
    for (int j = 0; j <= a.ub2(); j++) {
      cout << a.Element(i, j) << "\t";
    }
    cout << endl;
  }
}

// Add the elements of v to each row of m.
Matrix Add(const dbl_vect::DoubleVector &v, const Matrix &m) {
  assert(v.size_ == m.size2_);
  Matrix ans(m, copy);

  int i, j;
  for (i = 0; i <= m.ub1(); i++) {
    for (j = 0; j <= m.ub2(); j++) {
      ans.p_[i][j] = m.p_[i][j] + v.p_[j];
    }
  }
  return ans;
}

// The friend function of both DoubleVector and Matrix, which is a member of
// neither.
dbl_vect::DoubleVector Multiply(const dbl_vect::DoubleVector &v,
                                const Matrix &m) {
  // v is a column vector.  We multiply it by the size1_ row vectors of m, each
  // with size2_ elements.
  assert(v.size_ == m.size2_);
  dbl_vect::DoubleVector ans(m.size2_);
  int i, j;

  // Iterate over the row vectors in m.
  for (i = 0; i <= m.ub1(); i++) {
    ans.p_[i] = 0;
    // Iterate over the columns of the given row vector.
    for (j = 0; j <= m.ub2(); j++) {
      ans.p_[i] += v.p_[j] * m.p_[i][j];
    }
  }
  return ans;
}

} // namespace matrix
