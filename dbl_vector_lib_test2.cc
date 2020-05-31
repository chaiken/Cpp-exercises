// Tests which were difficult to make work with the test fixture of
// dbl_vector_lib_test.cc.   TEST and TEST_F cannot coexist in one file:
//
// All tests in the same test case must use the same test fixture class, so
// mixing TEST_F and TEST in the same test case is illegal.  In test case
// DoubleVectorTest, test SumElementsWorks is defined using TEST_F but test
// CopyConstructorWorks is defined using TEST.  You probably want to change the
// TEST to TEST_F or move it to another test case.

#include "dbl_vector.h"
#include "gtest/gtest.h"

using namespace std;
using namespace dbl_vect;

TEST(DoubleVectorTest, CopyConstructorWorks) {
  DoubleVector a(5);
  int i = 0;
  while (i <= a.ub()) {
    a.Element(i) = 2.0 * i;
    i++;
  }
  DoubleVector b(a);
  ASSERT_EQ(a.SumElements(), b.SumElements());
}

TEST(DoubleVectorTest, CArrayConstructorWorks) {
  double a[5];
  int i = 0;
  while (i < 5) {
    a[i] = i * 10.0;
    i++;
  }
  DoubleVector b(static_cast<const double *>(a), 5);

  i = 0;
  double sum1 = 0;
  while (i < 5) {
    sum1 += a[i];
    i++;
  }
  ASSERT_EQ(sum1, b.SumElements());
}

TEST(DoubleVectorTest, AddWorks) {
  DoubleVector a(5), b(5);
  int i = 0;
  while (i <= a.ub()) {
    a.Element(i) = i;
    b.Element(i) = -i;
    i++;
  }
  ASSERT_EQ(10, a.SumElements());
  ASSERT_EQ(-10, b.SumElements());
  b.Add(a);
  ASSERT_EQ(0, b.SumElements());
}

// Initializing c and then assigning its value to the return value of SumVectors
// results in garbage:
// dbl_vector_lib_test2.cc:43: Failure
// Expected equality of these values:
//  0
//  c.SumElements()
//    Which is: 4.64689e-310
// Apparently '=' performs a 'deep copy' if c is created, but not if it already
// exists.
TEST(DoubleVectorTest, SumVectorsWorks) {
  DoubleVector a(5), b(5);
  int i = 0;
  while (i <= a.ub()) {
    a.Element(i) = i;
    b.Element(i) = -i;
    i++;
  }
  ASSERT_LT(0, a.SumElements());
  ASSERT_GT(0, b.SumElements());
  DoubleVector c = SumVectors(static_cast<const DoubleVector>(a),
                              static_cast<const DoubleVector>(b));
  ASSERT_EQ(0, c.SumElements());
}
