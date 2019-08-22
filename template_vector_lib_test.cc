#include "template_vector.h"

#include "complex.h"

#include <iostream>
#include <vector>

#include "gtest/gtest.h"

using namespace complex;
using namespace std;

namespace template_vect {
namespace testing {

TEST(TemplateVectorDeathTest, ZeroLengthVector) {
  EXPECT_DEATH(TemplateVector<int>(0), "Invalid argument");
}

TEST(TemplateVectorTest, DefaultCtor) {
  // These both hit Most Vexing Parse.
  // TemplateVector<int> tv();
  // TemplateVector<int> tv[2]();
  TemplateVector<int> tv(10);
  ASSERT_EQ(9, tv.ub());
}

TEST(TemplateVectorTest, ArrayCtor) {
  Complex carr[]{Complex(0.0, 0.0), Complex(1.0, 1.0), Complex(2.0, 2.0),
                 Complex(3.0, 3.0), Complex(4.0, 4.0)};
  TemplateVector<Complex> tv(carr, 5);
  int i = 0;
  for (Complex *tvit = tv.begin(); tvit != tv.end(); tvit++, i++) {
    EXPECT_EQ(carr[i], *tvit);
  }
}

TEST(TemplateVectorTest, VectorCtor) {
  vector<double> dv{{1.0, 2.0, 3.0, 4.0, 5.0}};
  TemplateVector<double> tv(dv);
  int i = 0;
  for (double x : dv) {
    EXPECT_EQ(x, tv[i]);
    i++;
  }
}

TEST(TemplateVectorTest, MoveCtor) {
  Complex carr[]{Complex(0.0, 0.0), Complex(1.0, 1.0), Complex(2.0, 2.0),
                 Complex(3.0, 3.0), Complex(4.0, 4.0)};
  TemplateVector<Complex> tv(carr, 5);
  TemplateVector<Complex> tv2(move(tv));
  int i = 0;
  for (Complex *tvit = tv2.begin(); tvit != tv2.end(); tvit++, i++) {
    EXPECT_EQ(carr[i], *tvit);
  }
}

TEST(TemplateVectorTest, MoveAssignment) {
  vector<double> dv{{1.0, 2.0, 3.0, 4.0, 5.0}};
  TemplateVector<double> tv1(dv), tv2(5);
  int i = 0;
  for (double x : dv) {
    ASSERT_NE(x, tv2[i]);
    i++;
  }
  tv2 = move(tv1);
  i = 0;
  for (double x : dv) {
    EXPECT_EQ(x, tv2[i]);
    i++;
  }
}

TEST(TemplateVectorTest, PrinterTest) {
  Complex carr[]{Complex(0.0, 0.0), Complex(1.0, 1.0), Complex(2.0, 2.0),
                 Complex(3.0, 3.0), Complex(4.0, 4.0)};
  TemplateVector<Complex> tv(carr, 5);
  ostringstream out, out2;
  out << tv;
  out2 << "0 + 0i" << ends << ", 1 + 1i" << ends << ", 2 + 2i" << ends
       << ", 3 + 3i" << ends << ", 4 + 4i" << ends << ", ";
  EXPECT_EQ(out2.str(), out.str());
}

} // namespace testing
} // namespace template_vect
