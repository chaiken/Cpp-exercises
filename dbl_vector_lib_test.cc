#include "dbl_vector.h"
#include "gtest/gtest.h"
#include <cmath>

using namespace std;
namespace dbl_vect {
namespace testing {

class DoubleVectorTest : public ::testing::Test {
public:
  DoubleVectorTest(int n = 5) {
    a = new DoubleVector(n);
    b = new DoubleVector(n);
    assert(a->ub() == b->ub());
    int i = 0;
    while (i <= a->ub()) {
      a->Element(i) = 2.0 * i;
      b->Element(i) = 0.5 * i;
      i++;
    }
  }
  virtual ~DoubleVectorTest() {
    cout << "Test destructor." << endl;
    assert(a != b);
    delete a;
    delete b;
  }
  DoubleVector *a, *b;
};

TEST_F(DoubleVectorTest, SumElementsWorks) {
  ASSERT_EQ(20.0, a->SumElements());
}

TEST_F(DoubleVectorTest, DotProductWorks) {
  ASSERT_EQ(30.0, a->DotProduct(*b));
}

TEST_F(DoubleVectorTest, AmplitudeWorks) {
  double val = a->DotProduct(*a);
  ASSERT_LE(0, val);
  ASSERT_EQ(sqrt(val), a->Amplitude());
}

TEST_F(DoubleVectorTest, ScaleWorks) {
  b->Scale(4.0);
  ASSERT_EQ(a->SumElements(), b->SumElements());
}

TEST_F(DoubleVectorTest, AddWorks) {
  DoubleVector c(*b);
  DoubleVector d = c.Add(*a);
  ASSERT_EQ(a->SumElements() + b->SumElements(), d.SumElements());
}

TEST_F(DoubleVectorTest, SubscriptWorks) {
  for (int i = 0; i <= a->ub(); i++) {
    EXPECT_TRUE((*a)[i] == a->Element(i));
  }
}

TEST_F(DoubleVectorTest, AssignmentWorks) {
  EXPECT_FALSE((*a) == (*b));
  // a = b causes a failure when the destructor runs.
  *a = *b;
  EXPECT_TRUE((*a) == (*b));
}

TEST_F(DoubleVectorTest, PlusWorks) {
  DoubleVector c(*a);
  c = *a + *b;
  PrintDblVector(c);
  ASSERT_EQ(a->SumElements() + b->SumElements(), c.SumElements());
}

TEST_F(DoubleVectorTest, IteratorWorks) {
  for (int i = 0; i <= a->ub(); i++) {
    double b = a->Iterate();
    ASSERT_EQ(b, (*a)[i]);
  }
}

TEST_F(DoubleVectorTest, DoubleVectorIteratorBasic) {
  DoubleVectorIterator iter(*a), iter2(*b);
  double val = iter.Iterate();
  ASSERT_EQ(val, (*a)[1]);
  double val2 = iter2.Iterate();
  ASSERT_EQ(val2, (*b)[1]);

  val = iter.Iterate();
  ASSERT_EQ(val, (*a)[2]);
}

TEST_F(DoubleVectorTest, MaxWorks) {
  // The array is monotonically increasing.
  double c = (*a)[a->ub()];
  ASSERT_EQ(c, Max(*a));
}

TEST_F(DoubleVectorTest, MemberOperatorTest) {
  DoubleVectorIterator c(*a);
  // Call DoubleVector's ub() method via the member-access operator overload of
  // DoubleVectorIterator.
  EXPECT_EQ(4, c->ub());
}

} // namespace testing
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
//  ASSERT_EQ(a->SumElements(), c.SumElements());
//}
