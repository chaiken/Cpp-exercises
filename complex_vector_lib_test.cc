#include "complex_vector.h"

#include "gtest/gtest.h"

#include <vector>

using namespace std;
using namespace complex;

namespace complex_vec {
namespace testing {

TEST(ComplexVectorLibTest, CopyAndSubscriptConstructor) {
  Complex first(3.0, 4.0);
  ComplexVector cv1(first);
  ComplexVector cv2(first);
  EXPECT_EQ(cv1[0], cv2[0]);
}

TEST(ComplexVectorLibTest, VectorConstructorTest) {
  vector<Complex> vec;
  Complex first(1.0, 2.0);
  vec.push_back(first);
  Complex second(3.0, 4.0);
  vec.push_back(second);
  ComplexVector cv(vec);
  cout << cv[0] << endl;
  cout << cv[1] << endl;
  EXPECT_EQ(first, cv[0]);
  EXPECT_EQ(second, cv[1]);
  EXPECT_EQ(1, cv.ub());
  cout << cv << endl;
}

TEST(ComplexVectorLibTest, EqualityOperatorTest) {
  vector<Complex> vec;
  Complex first(1.0, 2.0);
  vec.push_back(first);
  Complex second(3.0, 4.0);
  vec.push_back(second);
  ComplexVector cv(vec);
  cout << cv[0] << endl;
  cout << cv[1] << endl;
  EXPECT_TRUE(first == cv[0]);
  EXPECT_TRUE(second == cv[1]);
}

TEST(ComplexVectorLibTest, ComplexConverterTest) {
  Complex second[] = {Complex(1.0, 2.0), Complex(3.0, 4.0)};
  ComplexVector cv(second, 2);
  EXPECT_TRUE(Complex(1.0, 2.0) == cv[0]);
  EXPECT_TRUE(Complex(3.0, 4.0) == cv[1]);
}

TEST(ComplexVectorLibTest, AdditionTest) {
  Complex first(3.0, 4.0);
  ComplexVector cv1(first);
  cout << cv1 << endl;
  Complex second(1.0, 2.0);
  ComplexVector cv2(second);
  cout << cv2 << endl;
  ComplexVector cv3(cv1 + cv2);
  cout << cv3 << endl;
  EXPECT_EQ(cv3[0], cv1[0] + cv2[0]);
}

TEST(ComplexVectorLibTest, DotProductTest) {
  Complex first[] = {Complex(3.0, 4.0), Complex(-1.0, -2.0)};
  ComplexVector cv1(first, 2);
  Complex second[] = {Complex(1.0, 2.0), Complex(3.0, 4.0)};
  ComplexVector cv2(second, 2);
  vector<double> v = cv1 * cv2;
  EXPECT_EQ(2, static_cast<int>(v.size()));
  EXPECT_EQ(11.0, v[0]);
  EXPECT_EQ(-11.0, v[1]);
}

TEST(ComplexVectorLibTest, EqualityTest) {
  Complex first[] = {Complex(3.0, 4.0), Complex(-1.0, -2.0)};
  ComplexVector cv1(first, 2);
  Complex second[] = {Complex(1.0, 2.0), Complex(3.0, 4.0)};
  ComplexVector cv2(second, 2);
  EXPECT_FALSE(cv1 == cv2);
  ComplexVector cv3(second, 2);
  EXPECT_TRUE(cv2 == cv3);
}

} // namespace testing
} // namespace complex_vec
