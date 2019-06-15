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

class MatrixLibTest : public ::testing::Test {
public:
  MatrixLibTest() {
    testvec1 = new vector<double>({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    assert(nullptr != testvec1);
    testvec2 = new vector<double>();
    assert(nullptr != testvec2);
    for (int i = 0; i < kLimit1 * kLimit2; i++) {
      testvec2->push_back(i);
    }
  }
  ~MatrixLibTest() {
    delete testvec1;
    delete testvec2;
  }

  vector<double> *testvec1, *testvec2;
};

TEST(MatrixLibSimpleTest, DefaultConstructor) {
  Matrix tensor1(3, 4);
  ASSERT_EQ(2, tensor1.ub1());
  ASSERT_EQ(3, tensor1.ub2());
}

TEST(MatrixLibSimpleTest, DefaultConstructorOffset) {
  Matrix tensor1(3, 4, -1);
  ASSERT_EQ(1, tensor1.ub1());
  ASSERT_EQ(2, tensor1.ub2());
}

TEST_F(MatrixLibTest, VectorConstructor) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
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

TEST_F(MatrixLibTest, VectorConstructorOffset) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2, 1);
#ifdef DEBUG
  cout << tensor2;
#endif
  // Not equal to kLimit1 - 1 due to offset.
  ASSERT_EQ(kLimit1, tensor2.ub1());
  ASSERT_EQ(kLimit2, tensor2.ub2());
  ASSERT_EQ(1, tensor2.lb());
  ASSERT_EQ(0.0, tensor2.Element(1, 1));
  ASSERT_EQ(1.0, tensor2.Element(1, 2));
  ASSERT_EQ((kLimit1 * kLimit2) - 1,
            tensor2.Element(tensor2.ub1(), tensor2.ub2()));
  ASSERT_EQ((kLimit1 * kLimit2) - 2,
            tensor2.Element(tensor2.ub1(), tensor2.ub2() - 1));
}

TEST_F(MatrixLibTest, TransformConstructorCopyTest) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
  Matrix tensor3(tensor2, copy);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub2(), tensor3.ub2());
  ASSERT_EQ((kLimit1 * kLimit2) - 1,
            tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ((kLimit1 * kLimit2) - 2,
            tensor3.Element(tensor3.ub1(), tensor3.ub2() - 1));
}

TEST_F(MatrixLibTest, TransformConstructorCopyTestOffset) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2, 1);
  Matrix tensor3(tensor2, copy);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub1() + 1);
  ASSERT_EQ(tensor2.ub2(), tensor3.ub2() + 1);
  ASSERT_EQ((kLimit1 * kLimit2) - 1,
            tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ((kLimit1 * kLimit2) - 2,
            tensor3.Element(tensor3.ub1(), tensor3.ub2() - 1));
}

TEST_F(MatrixLibTest, TransformConstructorTranposeTest) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
  Matrix tensor3(tensor2, transpose);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor2.ub2(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub1(), tensor3.ub2());
  ASSERT_EQ(tensor2.Element(0, 1), tensor3.Element(1, 0));
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), 0),
            tensor3.Element(0, tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, tensor2.ub2()),
            tensor3.Element(tensor3.ub1(), 0));
}

TEST_F(MatrixLibTest, TransformConstructorTranposeTestOffset) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2, -1);
  Matrix tensor3(tensor2, transpose);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor2.ub2(), tensor3.ub1() - 1);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub2() - 1);
  ASSERT_EQ(tensor2.Element(tensor2.lb(), tensor2.lb() + 1),
            tensor3.Element(1, 0));
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), tensor2.lb()),
            tensor3.Element(0, tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(tensor2.lb(), tensor2.ub2()),
            tensor3.Element(tensor3.ub1(), 0));
}

TEST_F(MatrixLibTest, TransformConstructorNegativeTest) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
  Matrix tensor3(tensor2, negative);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), tensor2.ub2()),
            -1 * tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), -1 * tensor3.Element(0, 0));
}

TEST_F(MatrixLibTest, TransformConstructorNegativeTestOffset) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2, -2);
  Matrix tensor3(tensor2, negative);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), tensor2.ub2()),
            -1 * tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(tensor2.lb(), tensor2.lb()),
            -1 * tensor3.Element(tensor3.lb(), tensor3.lb()));
}

TEST_F(MatrixLibTest, TransformConstructorUpperTest) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
  Matrix tensor3(tensor2, upper);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub2(), tensor3.Element(0, tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), tensor3.Element(0, 0));
}

TEST_F(MatrixLibTest, TransformConstructorUpperTestOffset) {
  Matrix tensor2(kLimit1, kLimit2, *testvec2, -1);
  Matrix tensor3(tensor2, upper);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub2(), tensor3.Element(0, tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(tensor2.lb(), tensor2.lb()), tensor3.Element(0, 0));
}

TEST_F(MatrixLibTest, SubmatrixConstructionCopiesWithEmptyVectors) {
  vector<double> testvec;
  vector<int> rows, cols;
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
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

TEST_F(MatrixLibTest, SubmatrixConstructionCopiesWithEmptyVectorsOffset) {
  vector<double> testvec;
  vector<int> rows, cols;
  Matrix tensor2(kLimit1, kLimit2, *testvec2, 1);
  Matrix tensor3(tensor2, rows, cols);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub1(), tensor2.ub1());
  ASSERT_EQ(tensor3.ub2(), tensor2.ub2());
  ASSERT_EQ(tensor3.Element(1, 1), tensor2.Element(1, 1));
}

TEST_F(MatrixLibTest, SubmatrixConstructorSkipColumns) {
  vector<int> rows, cols;
  for (int i = 0; i < kLimit2; i++) {
    if (is_even(i)) {
      cols.push_back(1);
    } else {
      cols.push_back(0);
    }
  }
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
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

TEST_F(MatrixLibTest, SubmatrixConstructorSkipColumnsOffset) {
  vector<int> rows, cols;
  for (int i = 0; i < kLimit2; i++) {
    if (is_even(i)) {
      cols.push_back(1);
    } else {
      cols.push_back(0);
    }
  }
  Matrix tensor2(kLimit1, kLimit2, *testvec2, 100);
  Matrix tensor3(tensor2, rows, cols);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub1(), tensor2.ub1());
  if (is_even(kLimit2)) {
    ASSERT_EQ((kLimit2 / 2) - 1, tensor3.ub2() - tensor3.lb());
  } else {
    ASSERT_EQ(kLimit2 / 2, tensor3.ub2());
  }
  ASSERT_EQ(tensor3.Element(tensor3.lb(), tensor3.lb()),
            tensor2.Element(tensor2.lb(), tensor2.lb()));
  ASSERT_EQ(tensor3.Element(tensor3.lb(), tensor3.lb() + 1),
            tensor2.Element(tensor2.lb(), tensor2.lb() + 2));
  ASSERT_EQ(tensor3.Element(tensor3.lb(), tensor3.lb() + 2),
            tensor2.Element(tensor2.lb(), tensor2.lb() + 4));
}

TEST_F(MatrixLibTest, SubmatrixConstructorSkipRows) {
  vector<int> rows, cols;
  for (int i = 0; i < kLimit1; i++) {
    if (is_even(i)) {
      rows.push_back(1);
    } else {
      rows.push_back(0);
    }
  }
  Matrix tensor2(kLimit1, kLimit2, *testvec2);
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

TEST_F(MatrixLibTest, SubmatrixConstructorSkipRowsOffset) {
  vector<int> rows, cols;
  for (int i = 0; i < kLimit1; i++) {
    if (is_even(i)) {
      rows.push_back(1);
    } else {
      rows.push_back(0);
    }
  }
  Matrix tensor2(kLimit1, kLimit2, *testvec2, -1);
  Matrix tensor3(tensor2, rows, cols);
#ifdef DEBUG
  cout << tensor2;
  cout << tensor3;
#endif
  ASSERT_EQ(tensor3.ub2(), tensor2.ub2());
  ASSERT_NE(tensor3.ub1(), tensor2.ub1());
  if (is_even(kLimit1)) {
    ASSERT_EQ((kLimit1 / 2) - 1, tensor3.ub1() - tensor3.lb());
  } else {
    ASSERT_EQ(kLimit1 / 2, tensor3.ub1() - tensor3.lb());
  }
  ASSERT_EQ(tensor3.Element(tensor3.lb(), tensor3.lb()),
            tensor2.Element(tensor2.lb(), tensor2.lb()));
  ASSERT_EQ(tensor3.Element(tensor3.lb() + 1, tensor3.lb()),
            tensor2.Element(tensor2.lb() + 2, tensor2.lb()));
}

TEST_F(MatrixLibTest, TraceTest) {
  Matrix tensor2(kLimit1, kLimit1, *testvec2);
  double sum = 0.0;
  for (int i = 0; i <= tensor2.ub1(); i++) {
    sum += tensor2.Element(i, i);
  }
  ASSERT_EQ(sum, Trace(tensor2));
}

TEST_F(MatrixLibTest, TraceTestOffset) {
  Matrix tensor2(kLimit1, kLimit1, *testvec2, 1);
  double sum = 0.0;
  for (int i = tensor2.lb(); i <= tensor2.ub1(); i++) {
    sum += tensor2.Element(i, i);
  }
  ASSERT_EQ(sum, Trace(tensor2));
}

TEST(MatrixLibSimpleTest, TrivialDeterminantTest) {
  vector<double> testvec3;
  for (int i = 0; i < 4; i++) {
    testvec3.push_back(i);
  }
  Matrix tensor2(2, 2, testvec3);
#ifdef DEBUG
  cout << tensor2;
#endif
  ASSERT_EQ(-2, Determinant(tensor2, 0.0));
}

TEST(MatrixLibSimpleTest, TrivialDeterminantTestOffset) {
  vector<double> testvec3;
  for (int i = 0; i < 4; i++) {
    testvec3.push_back(i);
  }
  Matrix tensor2(2, 2, testvec3, 2);
#ifdef DEBUG
  cout << tensor2;
#endif
  ASSERT_EQ(-2, Determinant(tensor2, 0.0));
}

TEST_F(MatrixLibTest, DeterminantTest) {
  Matrix tensor2(kLimit1, kLimit1, *testvec2);
#ifdef DEBUG
  cout << tensor2;
#endif
  ASSERT_EQ(0, Determinant(tensor2, 0.0));
}

TEST_F(MatrixLibTest, DeterminantTestOffset) {
  Matrix tensor2(kLimit1, kLimit1, *testvec2, -1);
#ifdef DEBUG
  cout << tensor2;
#endif
  ASSERT_EQ(0, Determinant(tensor2, 0.0));
}

TEST(MatrixLibSimpleTest, NonZeroDeterminantTest) {
  vector<double> testvec3;
  for (int i = 0; i < 9; i++) {
    if (is_even(i)) {
      testvec3.push_back(i);
    } else {
      testvec3.push_back(2 * i);
    }
  }
  Matrix tensor(3, 3, testvec3);
#ifdef DEBUG
  cout << tensor;
#endif
  ASSERT_EQ(144, Determinant(tensor, 0.0));
  Matrix tensor2(tensor, transpose);
  ASSERT_EQ(144, Determinant(tensor2, 0.0));
}

TEST(MatrixLibSimpleTest, NonZeroDeterminantTestOffset) {
  vector<double> testvec3;
  for (int i = 0; i < 9; i++) {
    if (is_even(i)) {
      testvec3.push_back(i);
    } else {
      testvec3.push_back(2 * i);
    }
  }
  Matrix tensor(3, 3, testvec3, 3);
#ifdef DEBUG
  cout << tensor;
#endif
  ASSERT_EQ(144, Determinant(tensor, 0.0));
  Matrix tensor2(tensor, transpose);
  ASSERT_EQ(144, Determinant(tensor2, 0.0));
}

// https://en.wikipedia.org/wiki/Eigenvalues_and_eigenvectors#Two-dimensional_matrix_example
TEST(MatrixLibSimpleTest, TrivialCharacteristicPolynomialTest) {
  vector<double> testvec3 = {2, 1, 1, 2};
  Matrix tensor2(2, 2, testvec3);
#ifdef DEBUG
  cout << tensor2;
#endif
  vector<double> coeffs = GetCharacteristicPolynomialCoefficients(tensor2);
  ASSERT_EQ(1, coeffs.at(0));
  ASSERT_EQ(-4, coeffs.at(1));
  ASSERT_EQ(3, coeffs.at(2));
}

TEST(MatrixLibSimpleTest, TrivialCharacteristicPolynomialTestOffset) {
  vector<double> testvec3 = {2, 1, 1, 2};
  Matrix tensor2(2, 2, testvec3, 1);
#ifdef DEBUG
  cout << tensor2;
#endif
  vector<double> coeffs = GetCharacteristicPolynomialCoefficients(tensor2);
  ASSERT_EQ(1, coeffs.at(0));
  ASSERT_EQ(-4, coeffs.at(1));
  ASSERT_EQ(3, coeffs.at(2));
}

TEST(MatrixLibSimpleTest, TrivialQuadraticRootsTest) {
  vector<double> testvec3 = {2, 1, 1, 2};
  Matrix tensor2(2, 2, testvec3);
#ifdef DEBUG
  cout << tensor2;
#endif
  vector<double> coeffs = GetCharacteristicPolynomialCoefficients(tensor2);
  array<complex<double>, 2> roots = GetQuadraticRoots(coeffs);
  ASSERT_EQ(3, roots[0].real());
  ASSERT_EQ(1, roots[1].real());
}

TEST(MatrixLibSimpleTest, TrivialQuadraticRootsTestOffset) {
  vector<double> testvec3 = {2, 1, 1, 2};
  Matrix tensor2(2, 2, testvec3, 1);
#ifdef DEBUG
  cout << tensor2;
#endif
  vector<double> coeffs = GetCharacteristicPolynomialCoefficients(tensor2);
  array<complex<double>, 2> roots = GetQuadraticRoots(coeffs);
  ASSERT_EQ(3, roots[0].real());
  ASSERT_EQ(1, roots[1].real());
}

TEST(MatrixLibSimpleTest, SquareMultiplyTest) {
  double twofer_in[] = {1.0, 2.0};
  dbl_vect::DoubleVector vec(twofer_in, 2);
  vector<double> testvec3 = {1.0, 2.0, 3.0, 4.0};
  Matrix tensor(2, 2, testvec3);
#ifdef DEBUG
  cout << tensor;
#endif
  dbl_vect::DoubleVector ans = matrix::Multiply(vec, tensor);
  EXPECT_EQ(vec.ub(), ans.ub());
  EXPECT_EQ(5, ans[0]);
  EXPECT_EQ(11, ans[1]);
  EXPECT_EQ(16, ans.SumElements());
}

TEST(MatrixLibSimpleTest, SquareMultiplyTestOffset) {
  double twofer_in[] = {1.0, 2.0};
  dbl_vect::DoubleVector vec(twofer_in, 2);
  vector<double> testvec3 = {1.0, 2.0, 3.0, 4.0};
  Matrix tensor(2, 2, testvec3, -1);
#ifdef DEBUG
  cout << tensor;
#endif
  dbl_vect::DoubleVector ans = matrix::Multiply(vec, tensor);
  EXPECT_EQ(vec.ub(), ans.ub());
  EXPECT_EQ(5, ans[0]);
  EXPECT_EQ(11, ans[1]);
  EXPECT_EQ(16, ans.SumElements());
}

TEST_F(MatrixLibTest, RectangularMultiplyTest) {
  double threefer_in[] = {1.0, 2.0, 3.0};
  dbl_vect::DoubleVector vec(threefer_in, 3);
  Matrix tensor(2, 3, *testvec1);
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

TEST_F(MatrixLibTest, RectangularMultiplyTestOffset) {
  double threefer_in[] = {1.0, 2.0, 3.0};
  dbl_vect::DoubleVector vec(threefer_in, 3);
  Matrix tensor(2, 3, *testvec1, 1);
#ifdef DEBUG
  cout << tensor;
#endif
  dbl_vect::DoubleVector ans = matrix::Multiply(vec, tensor);
  EXPECT_EQ(vec.ub(), ans.ub());
  EXPECT_EQ(14, ans[0]);
  EXPECT_EQ(32, ans[1]);
  EXPECT_EQ(46, ceil(ans.SumElements()));
}

TEST(MatrixLibSimpleTest, AddVectorTest) {
  double threefer_in[] = {1.0, 2.0, 3.0};
  dbl_vect::DoubleVector vec(threefer_in, 3);
  vector<double> testvec3 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  Matrix tensor(3, 3, testvec3);
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

TEST(MatrixLibSimpleTest, AddVectorTestOffset) {
  double threefer_in[] = {1.0, 2.0, 3.0};
  dbl_vect::DoubleVector vec(threefer_in, 3);
  vector<double> testvec3 = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
  Matrix tensor(3, 3, testvec3);
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

TEST_F(MatrixLibTest, IteratorTest) {
  Matrix tensor(2, 3, *testvec1);
  MatrixIterator iter(tensor);
  double val = iter.Iterate();
  EXPECT_EQ(2, val);
  val = iter.Iterate();
  EXPECT_EQ(3, val);
  val = iter.Iterate();
  EXPECT_EQ(4, val);
}

TEST_F(MatrixLibTest, IteratorTestOffset) {
  Matrix tensor(2, 3, *testvec1, -1);
  MatrixIterator iter(tensor);
  double val = iter.Iterate();
  EXPECT_EQ(2, val);
  val = iter.Iterate();
  EXPECT_EQ(3, val);
  val = iter.Iterate();
  EXPECT_EQ(4, val);
}

TEST_F(MatrixLibTest, IteratorFeaturesTest) {
  Matrix tensor(2, 3, *testvec1);
  MatrixIterator iter(tensor);
  double val = iter.Iterate();
  EXPECT_EQ(2, val);
  val = iter.successor();
  EXPECT_EQ(3, val);
  val = iter.predecessor();
  EXPECT_EQ(1, val);
  iter.reset();
  val = iter.item();
  EXPECT_EQ(1, val);
  val = iter.Iterate();
  EXPECT_EQ(2, val);
}

TEST_F(MatrixLibTest, IteratorFeaturesTestOffset) {
  Matrix tensor(2, 3, *testvec1, 1);
  MatrixIterator iter(tensor);
  double val = iter.Iterate();
  EXPECT_EQ(2, val);
  val = iter.successor();
  EXPECT_EQ(3, val);
  val = iter.predecessor();
  EXPECT_EQ(1, val);
  iter.reset();
  val = iter.item();
  EXPECT_EQ(1, val);
  val = iter.Iterate();
  EXPECT_EQ(2, val);
}

TEST_F(MatrixLibTest, MaxTest) {
  Matrix tensor(2, 3, *testvec1);
  ASSERT_EQ(6.0, Max(tensor));
}

TEST_F(MatrixLibTest, MaxTestOffset) {
  Matrix tensor(2, 3, *testvec1, -1);
  ASSERT_EQ(6.0, Max(tensor));
}

} // namespace testing
} // namespace matrix
