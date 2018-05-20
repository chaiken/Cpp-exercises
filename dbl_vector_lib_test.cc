#include "dbl_vector.h"
#include "gtest/gtest.h"

using namespace std;
using namespace dbl_vect;

TEST(DoubleVectorTest, SumElementsWorks) {
  DoubleVector a(5);
  int i = 0;
  while (i <= a.ub()) {
    a.element(i) = 2.0 * i;
    i++;
  }
  ASSERT_EQ(20.0, a.SumElements());
}
