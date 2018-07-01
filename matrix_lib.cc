#include "matrix.h"

#include <cassert>
#include <cstdlib>

#include <array>
#include <iostream>
#include <vector>

using namespace std;

namespace matrix {
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
}
