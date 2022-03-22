#include "dbl_vector.h"
#include "gtest/gtest.h"
#include <cmath>

using namespace std;
namespace dbl_vect {
namespace local_testing {

class DoubleVectorTest : public ::testing::Test {
public:
  DoubleVectorTest() : a(DoubleVector(5)), b(DoubleVector(5)) {
    assert(a.ub() == b.ub());
    int i = 0;
    while (i <= a.ub()) {
      a.Element(i) = 2.0 * i;
      b.Element(i) = 0.5 * i;
      i++;
    }
  }
  DoubleVector a, b;
};

TEST_F(DoubleVectorTest, SumElementsWorks) { ASSERT_EQ(20.0, a.SumElements()); }

TEST_F(DoubleVectorTest, DotProductWorks) { ASSERT_EQ(30.0, a.DotProduct(b)); }

TEST_F(DoubleVectorTest, AmplitudeWorks) {
  double val = a.DotProduct(a);
  ASSERT_LE(0, val);
  ASSERT_EQ(sqrt(val), a.Amplitude());
}

TEST_F(DoubleVectorTest, MultiplicationWorks) {
  DoubleVector c{b * 4.0};
  ASSERT_EQ(a.SumElements(), c.SumElements());
}

TEST_F(DoubleVectorTest, SubscriptWorks) {
  for (int i = 0; i <= a.ub(); i++) {
    EXPECT_TRUE(a[i] == a.Element(i));
  }
}

TEST_F(DoubleVectorTest, AssignmentWorks) {
  EXPECT_FALSE(a == b);
  vector<double> save;
  for (int i = 0; i <= b.ub(); i++) {
    save.push_back(b[i]);
  }
  DoubleVector c(save);
  a = std::move(b);
  EXPECT_TRUE(a == c);
}

TEST_F(DoubleVectorTest, PrintingWorks) {
  ostringstream out;
  out << a;
  EXPECT_EQ("0,0 1,2 2,4 3,6 4,8 \n", out.str());
}

TEST_F(DoubleVectorTest, PlusWorks) {
  DoubleVector c = a + b;
  ASSERT_EQ(c.ub(), a.ub());
  ASSERT_EQ(a.SumElements() + b.SumElements(), c.SumElements());
  // Prints the leading string with ::std::cout, then calls the operator<<() for
  // the DoubleVector.
  cout << "a: " << a;
  cout << "b: " << b;
  cout << "Sum: " << c;
}

TEST_F(DoubleVectorTest, DoubleVectorIteratorBasic) {
  DoubleVectorIterator iter(a), iter2(b);
  double val = ++iter;
  ASSERT_EQ(val, (a)[1]);
  double val2 = ++iter2;
  ASSERT_EQ(val2, (b)[1]);
  val = ++iter;
  ASSERT_EQ(val, (a)[2]);
}

TEST_F(DoubleVectorTest, MaxWorks) {
  // The array is monotonically increasing.
  double c = (a)[a.ub()];
  ASSERT_EQ(c, Max(a));
}

TEST_F(DoubleVectorTest, MemberOperatorTest) {
  DoubleVectorIterator c(a);
  // Call DoubleVector's ub() method via the member-access operator overload of
  // DoubleVectorIterator.
  EXPECT_EQ(4, c->ub());
}

TEST(DoubleVectorSimpleTest, CArrayConstructorWorks) {
  double vec[] = {0, 10, 20, 30, 40};
  DoubleVector b(vec, 5);
  ASSERT_EQ(100, b.SumElements());
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
TEST(DoubleVectorSimpleTest, SumVectorsWorks) {
  DoubleVector a(5), b(5);
  int i = 0;
  while (i <= a.ub()) {
    a.Element(i) = i;
    b.Element(i) = -i;
    i++;
  }
  ASSERT_LT(0, a.SumElements());
  ASSERT_GT(0, b.SumElements());
  DoubleVector c = SumVectors(std::move(a), std::move(b));
  ASSERT_EQ(0, c.SumElements());
}

TEST(DoubleVectorSimpleTest, EqualityOperatorWorks) {
  DoubleVector a(5), b(4);
  ASSERT_FALSE(a == b);
  ASSERT_TRUE(a != b);
}

} // namespace local_testing
} // namespace dbl_vect

// /home/alison/src/exercises/Pohl/dbl_vector_lib_test.cc:57: multiple
// definition of `main'
// /home/alison/gitsrc/googletest/googletest/make/gtest_main.a(gtest_main.o):/home/alison/gitsrc/googletest/googletest/make/../src/gtest_main.cc:34:
// first defined here
// int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}

// From Setup(), I can only call the default constructor from
// not the parameterized ones.
// https://stackoverflow.com/questions/38207346/specify-constructor-arguments-for-a-google-test-fixture
//  you cannot achieve what you want by instantiating a fixture using a
//  non-default constructor. However, there are other ways. Simply overload
//  the SetUp function and call that version explicitly in the tests.
// error: invalid conversion from ‘dbl_vect::DoubleVector*’ to ‘int’
// [-fpermissive]
//   DoubleVector c(a);
// TEST_F(DoubleVectorTest, CopyConstructorWorks) {
//  DoubleVector c(a);
//  ASSERT_EQ(a.SumElements(), c.SumElements());
//}
