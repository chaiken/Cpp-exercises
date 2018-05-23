#include "dbl_vector.h"
#include "gtest/gtest.h"

// From Setup(), I can only call the default constructor from
// not the parameterized ones.
// https://stackoverflow.com/questions/38207346/specify-constructor-arguments-for-a-google-test-fixture
//  you cannot achieve what you want by instantiating a fixture using a
//  non-default constructor. However, there are other ways. Simply overload
//  the SetUp function and call that version explicitly in the tests.

using namespace std;
using namespace dbl_vect;

class DoubleVectorTest : public ::testing::Test {
public:
  void SetUp(int n) {
    a = new DoubleVector(n);
    int i = 0;
    while (i <= a->ub()) {
      a->element(i) = 2.0 * i;
      i++;
    }
  }
  virtual void TearDown() { delete a; }
  DoubleVector *a;
};

TEST_F(DoubleVectorTest, SumElementsWorks) {
  SetUp(5);
  ASSERT_EQ(20.0, a->SumElements());
}
