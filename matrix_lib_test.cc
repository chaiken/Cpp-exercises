#include "matrix.h"

#include "gtest/gtest.h"

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

} // testing
} // matrix
