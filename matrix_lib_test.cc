#include "dbl_vector.h"
#include "matrix.h"

#include "gtest/gtest.h"

#include <array>
#include <complex>

using namespace std;

namespace matrix {
namespace testing {

bool is_even(int i) { return (2 * (i / 2) == i); }

TEST(MatrixLibTest, DefaultConstructor) {
  Matrix tensor1(3, 4);
  ASSERT_EQ(2, tensor1.ub1());
  ASSERT_EQ(3, tensor1.ub2());
}

TEST(MatrixLibTest, VectorConstructor) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(4, 5, testvec);
  PrintMatrix(tensor2);
  ASSERT_EQ(3, tensor2.ub1());
  ASSERT_EQ(4, tensor2.ub2());
  ASSERT_EQ(0.0, tensor2.Element(0, 0));
  ASSERT_EQ(1.0, tensor2.Element(0, 1));
  ASSERT_EQ(19.0, tensor2.Element(tensor2.ub1(), tensor2.ub2()));
  ASSERT_EQ(18.0, tensor2.Element(tensor2.ub1(), tensor2.ub2() - 1));
}

TEST(MatrixLibTest, TransformConstructorCopyTest) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(4, 5, testvec);
  Matrix tensor3(tensor2, copy);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub2(), tensor3.ub2());
  ASSERT_EQ(19.0, tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(18.0, tensor3.Element(tensor3.ub1(), tensor3.ub2() - 1));
}

TEST(MatrixLibTest, TransformConstructorTranposeTest) {
  vector<double> testvec;
  for (int i = 0; i < 25; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(5, 5, testvec);
  PrintMatrix(tensor2);
  Matrix tensor3(tensor2, transpose);
  PrintMatrix(tensor3);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub2(), tensor3.ub2());
  ASSERT_EQ(tensor2.Element(0, 1), tensor3.Element(1, 0));
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), 0),
            tensor3.Element(0, tensor3.ub1()));
}

TEST(MatrixLibTest, TransformConstructorNegativeTest) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(4, 5, testvec);
  PrintMatrix(tensor2);
  Matrix tensor3(tensor2, negative);
  PrintMatrix(tensor3);
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), tensor2.ub2()),
            -1 * tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), -1 * tensor3.Element(0, 0));
}

TEST(MatrixLibTest, TransformConstructorUpperTest) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(4, 5, testvec);
  PrintMatrix(tensor2);
  Matrix tensor3(tensor2, upper);
  PrintMatrix(tensor3);
  ASSERT_EQ(19.0, tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), tensor3.Element(0, 0));
}

TEST(MatrixLibTest, SubmatrixConstructionCopiesWithEmptyVectors) {
  vector<double> testvec;
  vector<int> rows, cols;
  for (int i = 0; i < 25; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(5, 5, testvec);
  PrintMatrix(tensor2);
  Matrix tensor3(tensor2, rows, cols);
  PrintMatrix(tensor3);
  ASSERT_EQ(tensor3.ub1(), tensor2.ub1());
  ASSERT_EQ(tensor3.ub2(), tensor2.ub2());
  ASSERT_EQ(tensor3.Element(0, 0), tensor2.Element(0, 0));
  ASSERT_EQ(tensor3.Element(2, 1), tensor2.Element(2, 1));
}

TEST(MatrixLibTest, SubmatrixConstructorSkipColumns) {
  vector<double> testvec;
  vector<int> rows, cols;
  for (int i = 0; i < 25; i++) {
    testvec.push_back(i);
  }
  for (int i = 0; i < 5; i++) {
    if (is_even(i)) {
      cols.push_back(1);
    } else {
      cols.push_back(0);
    }
  }
  Matrix tensor2(5, 5, testvec);
  PrintMatrix(tensor2);
  Matrix tensor3(tensor2, rows, cols);
  PrintMatrix(tensor3);
  ASSERT_EQ(tensor3.ub1(), tensor2.ub1());
  ASSERT_EQ(tensor3.ub2(), 2);
  ASSERT_EQ(tensor3.Element(0, 0), tensor2.Element(0, 0));
  ASSERT_EQ(tensor3.Element(0, 1), tensor2.Element(0, 2));
  ASSERT_EQ(tensor3.Element(0, 2), tensor2.Element(0, 4));
}

TEST(MatrixLibTest, SubmatrixConstructorSkipRows) {
  vector<double> testvec;
  vector<int> rows, cols;
  for (int i = 0; i < 25; i++) {
    testvec.push_back(i);
  }
  for (int i = 0; i < 5; i++) {
    if (is_even(i)) {
      rows.push_back(1);
    } else {
      rows.push_back(0);
    }
  }
  Matrix tensor2(5, 5, testvec);
  PrintMatrix(tensor2);
  Matrix tensor3(tensor2, rows, cols);
  PrintMatrix(tensor3);
  ASSERT_EQ(tensor3.ub2(), tensor2.ub2());
  ASSERT_EQ(tensor3.ub1(), 2);
  ASSERT_EQ(tensor3.Element(0, 0), tensor2.Element(0, 0));
  ASSERT_EQ(tensor3.Element(1, 0), tensor2.Element(2, 0));
  ASSERT_EQ(tensor3.Element(2, 0), tensor2.Element(4, 0));
}

TEST(MatrixLibTest, TraceTest) {
  vector<double> testvec;
  for (int i = 0; i < 25; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(5, 5, testvec);
  // {0, 6, 12, 18, 24}
  ASSERT_EQ(60, Trace(tensor2));
}

TEST(MatrixLibTest, TrivialDeterminantTest) {
  vector<double> testvec;
  for (int i = 0; i < 4; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(2, 2, testvec);
  PrintMatrix(tensor2);
  ASSERT_EQ(-2, Determinant(tensor2, 0.0));
}

TEST(MatrixLibTest, DeterminantTest) {
  vector<double> testvec;
  for (int i = 0; i < 9; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(3, 3, testvec);
  PrintMatrix(tensor2);
  ASSERT_EQ(0, Determinant(tensor2, 0.0));
}

TEST(MatrixLibTest, NonZeroDeterminantTest) {
  vector<double> testvec;
  for (int i = 0; i < 9; i++) {
    if (is_even(i)) {
      testvec.push_back(i);
    } else {
      testvec.push_back(2 * i);
    }
  }
  Matrix tensor2(3, 3, testvec);
  PrintMatrix(tensor2);
  ASSERT_EQ(144, Determinant(tensor2, 0.0));
  Matrix tensor3(tensor2, transpose);
  ASSERT_EQ(144, Determinant(tensor3, 0.0));
}

TEST(MatrixLibTest, TrivialCharacteristicPolynomialTest) {
  // https://en.wikipedia.org/wiki/Eigenvalues_and_eigenvectors#Two-dimensional_matrix_example
  vector<double> testvec = {2, 1, 1, 2};
  Matrix tensor2(2, 2, testvec);
  PrintMatrix(tensor2);
  vector<double> coeffs = GetCharacteristicPolynomialCoefficients(tensor2);
  ASSERT_EQ(1, coeffs.at(0));
  ASSERT_EQ(-4, coeffs.at(1));
  ASSERT_EQ(3, coeffs.at(2));

  array<complex<double>, 2> roots = GetQuadraticRoots(coeffs);
  ASSERT_EQ(3, roots[0].real());
  ASSERT_EQ(1, roots[1].real());
}

TEST(MatrixLibTest, SquareMultiplyTest) {
  double twofer_in[] = {1.0, 2.0};
  dbl_vect::DoubleVector vec(twofer_in, 2);
  vector<double> testvec = {1.0, 2.0, 3.0, 4.0};
  Matrix tensor(2, 2, testvec);
  PrintMatrix(tensor);
  dbl_vect::DoubleVector ans = matrix::Multiply(vec, tensor);
  EXPECT_EQ(vec.ub(), ans.ub());
  EXPECT_EQ(5, ans.element(0));
  EXPECT_EQ(11, ans.element(1));
  EXPECT_EQ(16, ans.SumElements());
}

TEST(MatrixLibTest, RectangularMultiplyTest) {
  double threefer_in[] = {1.0, 2.0, 3.0};
  dbl_vect::DoubleVector vec(threefer_in, 3);
  vector<double> testvec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  Matrix tensor(2, 3, testvec);
  PrintMatrix(tensor);
  dbl_vect::DoubleVector ans = matrix::Multiply(vec, tensor);
  EXPECT_EQ(vec.ub(), ans.ub());
  EXPECT_EQ(14, ans.element(0));
  EXPECT_EQ(32, ans.element(1));
  EXPECT_EQ(46.0, ans.SumElements());
}

} // namespace testing
} // namespace matrix
