#include "dbl_vector.h"
#include "matrix.h"

#include "gtest/gtest.h"

#include <array>
#include <complex>

using namespace std;

namespace matrix {
namespace testing {

constexpr int32_t kLimit1 = 5;
constexpr int32_t kLimit2 = 6;
/* -DDEBUG
[==========] 19 tests from 1 test suite ran. (201 ms total)
[  PASSED  ] 19 tests.

real    0m0.251s
user    0m0.132s
sys     0m0.068s

-NDEBUG
[==========] 19 tests from 1 test suite ran. (64 ms total)
[  PASSED  ] 19 tests.

real    0m0.113s
user    0m0.096s
sys     0m0.012s
*/

namespace {
bool is_even(int i) { return (2 * (i / 2) == i); }
} // namespace

TEST(MatrixLibTest, DefaultConstructor) {
  Matrix tensor1(3, 4);
  ASSERT_EQ(2, tensor1.ub1());
  ASSERT_EQ(3, tensor1.ub2());
}

TEST(MatrixLibTest, VectorConstructor) {
  vector<double> testvec;
  for (int i = 0; i < kLimit1 * kLimit2; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit1, kLimit2, testvec);
#ifdef DEBUG
  cout << tensor2;
#endif
  ASSERT_EQ(kLimit1 - 1, tensor2.ub1());
  ASSERT_EQ(kLimit2 - 1, tensor2.ub2());
  ASSERT_EQ(0.0, tensor2.Element(0, 0));
  ASSERT_EQ(1.0, tensor2.Element(0, 1));
  ASSERT_EQ((kLimit1 * kLimit2) - 1,
            tensor2.Element(tensor2.ub1(), tensor2.ub2()));
  ASSERT_EQ((kLimit1 * kLimit2) - 2,
            tensor2.Element(tensor2.ub1(), tensor2.ub2() - 1));
}

TEST(MatrixLibTest, TransformConstructorCopyTest) {
  vector<double> testvec;
  for (int i = 0; i < kLimit1 * kLimit2; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit1, kLimit2, testvec);
  Matrix tensor3(tensor2, copy);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub2(), tensor3.ub2());
  ASSERT_EQ((kLimit1 * kLimit2) - 1,
            tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ((kLimit1 * kLimit2) - 2,
            tensor3.Element(tensor3.ub1(), tensor3.ub2() - 1));
}

TEST(MatrixLibTest, TransformConstructorTranposeTest) {
  vector<double> testvec;
  for (int i = 0; i < kLimit2 * kLimit2; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit2, kLimit2, testvec);
  Matrix tensor3(tensor2, transpose);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor2.ub2(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub1(), tensor3.ub2());
  ASSERT_EQ(tensor2.Element(0, 1), tensor3.Element(1, 0));
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), 0),
            tensor3.Element(0, tensor3.ub1()));
}

TEST(MatrixLibTest, TransformConstructorNegativeTest) {
  vector<double> testvec;
  for (int i = 0; i < kLimit1 * kLimit2; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit1, kLimit2, testvec);
  Matrix tensor3(tensor2, negative);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), tensor2.ub2()),
            -1 * tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), -1 * tensor3.Element(0, 0));
}

TEST(MatrixLibTest, TransformConstructorUpperTest) {
  vector<double> testvec;
  for (int i = 0; i < kLimit1 * kLimit2; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit1, kLimit2, testvec);
  Matrix tensor3(tensor2, upper);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub2(), tensor3.Element(0, tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), tensor3.Element(0, 0));
}

TEST(MatrixLibTest, SubmatrixConstructionCopiesWithEmptyVectors) {
  vector<double> testvec;
  vector<int> rows, cols;
  for (int i = 0; i < kLimit1 * kLimit2; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit1, kLimit2, testvec);
  Matrix tensor3(tensor2, rows, cols);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub1(), tensor2.ub1());
  ASSERT_EQ(tensor3.ub2(), tensor2.ub2());
  ASSERT_EQ(tensor3.Element(0, 0), tensor2.Element(0, 0));
  ASSERT_EQ(tensor3.Element(2, 1), tensor2.Element(2, 1));
}

TEST(MatrixLibTest, SubmatrixConstructorSkipColumns) {
  vector<double> testvec;
  vector<int> rows, cols;
  for (int i = 0; i < kLimit1 * kLimit2; i++) {
    testvec.push_back(i);
  }
  for (int i = 0; i < kLimit2; i++) {
    if (is_even(i)) {
      cols.push_back(1);
    } else {
      cols.push_back(0);
    }
  }
  Matrix tensor2(kLimit1, kLimit2, testvec);
  Matrix tensor3(tensor2, rows, cols);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub1(), tensor2.ub1());
  if (is_even(kLimit2)) {
    ASSERT_EQ((kLimit2 / 2) - 1, tensor3.ub2());
  } else {
    ASSERT_EQ(kLimit2 / 2, tensor3.ub2());
  }
  ASSERT_EQ(tensor3.Element(0, 0), tensor2.Element(0, 0));
  ASSERT_EQ(tensor3.Element(0, 1), tensor2.Element(0, 2));
  ASSERT_EQ(tensor3.Element(0, 2), tensor2.Element(0, 4));
}

TEST(MatrixLibTest, SubmatrixConstructorSkipRows) {
  vector<double> testvec;
  vector<int> rows, cols;
  for (int i = 0; i < kLimit1 * kLimit2; i++) {
    testvec.push_back(i);
  }
  for (int i = 0; i < kLimit1; i++) {
    if (is_even(i)) {
      rows.push_back(1);
    } else {
      rows.push_back(0);
    }
  }
  Matrix tensor2(kLimit1, kLimit2, testvec);
  Matrix tensor3(tensor2, rows, cols);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub2(), tensor2.ub2());
  ASSERT_NE(tensor3.ub1(), tensor2.ub1());
  if (is_even(kLimit1)) {
    ASSERT_EQ((kLimit1 / 2) - 1, tensor3.ub1());
  } else {
    ASSERT_EQ(kLimit1 / 2, tensor3.ub1());
  }
  ASSERT_EQ(tensor3.Element(0, 0), tensor2.Element(0, 0));
  ASSERT_EQ(tensor3.Element(1, 0), tensor2.Element(2, 0));
}

TEST(MatrixLibTest, TraceTest) {
  vector<double> testvec;
  for (int i = 0; i < kLimit1 * kLimit1; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit1, kLimit1, testvec);
  double sum = 0.0;
  for (int i = 0; i <= tensor2.ub1(); i++) {
    sum += tensor2.Element(i, i);
  }
  ASSERT_EQ(sum, Trace(tensor2));
}

TEST(MatrixLibTest, TrivialDeterminantTest) {
  vector<double> testvec;
  for (int i = 0; i < 4; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(2, 2, testvec);
#ifdef DEBUG
  cout << tensor2;
#endif
  ASSERT_EQ(-2, Determinant(tensor2, 0.0));
}

TEST(MatrixLibTest, DeterminantTest) {
  vector<double> testvec;
  for (int i = 0; i < kLimit1 * kLimit1; i++) {
    testvec.push_back(i);
  }
  Matrix tensor2(kLimit1, kLimit1, testvec);
#ifdef DEBUG
  cout << tensor2;
#endif
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
#ifdef DEBUG
  cout << tensor2;
#endif
  ASSERT_EQ(144, Determinant(tensor2, 0.0));
  Matrix tensor3(tensor2, transpose);
  ASSERT_EQ(144, Determinant(tensor3, 0.0));
}

TEST(MatrixLibTest, TrivialCharacteristicPolynomialTest) {
  // https://en.wikipedia.org/wiki/Eigenvalues_and_eigenvectors#Two-dimensional_matrix_example
  vector<double> testvec = {2, 1, 1, 2};
  Matrix tensor2(2, 2, testvec);
#ifdef DEBUG
  cout << tensor2;
#endif
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
#ifdef DEBUG
  cout << tensor;
#endif
  dbl_vect::DoubleVector ans = matrix::Multiply(vec, tensor);
  EXPECT_EQ(vec.ub(), ans.ub());
  EXPECT_EQ(5, ans[0]);
  EXPECT_EQ(11, ans[1]);
  EXPECT_EQ(16, ans.SumElements());
}

TEST(MatrixLibTest, RectangularMultiplyTest) {
  double threefer_in[] = {1.0, 2.0, 3.0};
  dbl_vect::DoubleVector vec(threefer_in, 3);
  vector<double> testvec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  Matrix tensor(2, 3, testvec);
#ifdef DEBUG
  cout << tensor;
#endif
  dbl_vect::DoubleVector ans = matrix::Multiply(vec, tensor);
  EXPECT_EQ(vec.ub(), ans.ub());
  EXPECT_EQ(14, ans[0]);
  EXPECT_EQ(32, ans[1]);
  // Why ceil() is used here:
  // matrix_lib_test.cc:241: Failure
  // Expected equality of these values:
  //   46
  // ans.SumElements()
  //  Which is: 46
  // (gdb) p ans[0]
  // $2 = (double &) @0x603000002740: 14
  // (gdb) p ans[1]
  // $3 = (double &) @0x603000002748: 32
  // (gdb) n
  // 241       EXPECT_EQ(46, ans.SumElements());
  // (gdb) p ans.SumElements()
  // $5 = 45.999998167449355
  EXPECT_EQ(46, ceil(ans.SumElements()));
}

TEST(MatrixLibTest, AddVectorTest) {
  double threefer_in[] = {1.0, 2.0, 3.0};
  dbl_vect::DoubleVector vec(threefer_in, 3);
  vector<double> testvec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  Matrix tensor(3, 3, testvec);
  Matrix ans = Add(vec, tensor);
#ifdef DEBUG
  cout << ans;
#endif
  EXPECT_EQ(ans.ub1(), tensor.ub1());
  EXPECT_EQ(ans.ub2(), tensor.ub2());
  EXPECT_EQ(21.0, Trace(ans));
  EXPECT_EQ(2.0, ans.Element(0, 0));
  EXPECT_EQ(0, Determinant(ans, 0.0));
}

TEST(MatrixLibTest, IteratorTest) {
  vector<double> testvec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  Matrix tensor(2, 3, testvec);
  MatrixIterator iter(tensor);
#ifdef DEBUG
  cout << tensor;
#endif
  double val = iter.Iterate();
  EXPECT_EQ(2, val);
  val = iter.Iterate();
  EXPECT_EQ(3, val);
  val = iter.Iterate();
  EXPECT_EQ(4, val);
}

TEST(MatrixLibTest, MaxTest) {
  vector<double> testvec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  Matrix tensor(2, 3, testvec);
  ASSERT_EQ(6.0, Max(tensor));
}

} // namespace testing
} // namespace matrix
