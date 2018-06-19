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
      p_[i][j] = inputs.back();
      // pop_back() removes the element without returning it.
      inputs.pop_back();
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
}
