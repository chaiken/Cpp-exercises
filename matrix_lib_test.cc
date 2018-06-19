#include "matrix.h"

#include "gtest/gtest.h"

using namespace std;

namespace matrix {
namespace testing {

TEST(MatrixLibTest, DefaultConstructor) {
  Matrix tensor1(3, 4);
  ASSERT_EQ(2, tensor1.ub1());
  ASSERT_EQ(3, tensor1.ub2());
}

TEST(MatrixLibTest, VectorConstructor) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i * 0.3);
  }
  Matrix tensor2(4, 5, testvec);
  ASSERT_EQ(3, tensor2.ub1());
  ASSERT_EQ(4, tensor2.ub2());
  ASSERT_EQ((19 * 0.3), tensor2.Element(0, 0));
  ASSERT_EQ((18 * 0.3), tensor2.Element(0, 1));
  ASSERT_EQ(0, tensor2.Element(tensor2.ub1(), tensor2.ub2()));
  ASSERT_EQ(0.3, tensor2.Element(tensor2.ub1(), tensor2.ub2() - 1));
}

TEST(MatrixLibTest, TransformConstructorCopyTest) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i * 0.3);
  }
  Matrix tensor2(4, 5, testvec);
  Matrix tensor3(tensor2, copy);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub2(), tensor3.ub2());
  ASSERT_EQ(0, tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(0.3, tensor3.Element(tensor3.ub1(), tensor3.ub2() - 1));
}

TEST(MatrixLibTest, TransformConstructorTranposeTest) {
  vector<double> testvec;
  for (int i = 0; i < 25; i++) {
    testvec.push_back(i * 2);
  }
  Matrix tensor2(5, 5, testvec);
  Matrix tensor3(tensor2, transpose);
  ASSERT_EQ(tensor2.ub1(), tensor3.ub1());
  ASSERT_EQ(tensor2.ub2(), tensor3.ub2());
  ASSERT_EQ(tensor2.Element(0, 1), tensor3.Element(1, 0));
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), 0),
            tensor3.Element(0, tensor3.ub1()));
}

TEST(MatrixLibTest, TransformConstructorNegativeTest) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i * 0.3);
  }
  Matrix tensor2(4, 5, testvec);
  Matrix tensor3(tensor2, negative);
  ASSERT_EQ(tensor2.Element(tensor2.ub1(), tensor2.ub2()),
            -1 * tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), -1 * tensor3.Element(0, 0));
}

TEST(MatrixLibTest, TransformConstructorUpperTest) {
  vector<double> testvec;
  for (int i = 0; i < 20; i++) {
    testvec.push_back(i * 0.3);
  }
  Matrix tensor2(4, 5, testvec);
  Matrix tensor3(tensor2, upper);
  ASSERT_EQ(0.0, tensor3.Element(tensor3.ub1(), tensor3.ub2()));
  ASSERT_EQ(tensor2.Element(0, 0), tensor3.Element(0, 0));
}

} // testing
} // matrix
