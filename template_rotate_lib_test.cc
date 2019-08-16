#include "template_rotate.h"

#include "complex.h"

#include "gtest/gtest.h"

#include <array>

using namespace complex;
using namespace std;

namespace rotate_array {
namespace testing {

TEST(TemplateRotateTest, IntArray) {
  array<double, 5UL> anarr{1.0, 2.0, 3.0, 4.0, 5.0};
  rotate_array::rotate_array(&anarr);
  EXPECT_EQ(5.0, anarr[0]);
  EXPECT_EQ(1.0, anarr[1]);
  EXPECT_EQ(2.0, anarr[2]);
  EXPECT_EQ(3.0, anarr[3]);
  EXPECT_EQ(4.0, anarr[4]);
}

TEST(TemplateRotateTest, ComplexArray) {
  array<Complex, 5> anarr({{
      {1.0, 1.0},
      {2.0, 2.0},
      {3.0, 3.0},
      {4.0, 4.0},
      {5.0, 5.0},
  }});
  rotate_array::rotate_array(&anarr);
  EXPECT_EQ(Complex(5.0, 5.0), anarr[0]);
  EXPECT_EQ(Complex(1.0, 1.0), anarr[1]);
  EXPECT_EQ(Complex(2.0, 2.0), anarr[2]);
  EXPECT_EQ(Complex(3.0, 3.0), anarr[3]);
  EXPECT_EQ(Complex(4.0, 4.0), anarr[4]);
}

} // namespace testing
} // namespace rotate_array
