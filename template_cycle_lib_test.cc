#include "polynomial.h"
#include "template_cycle.h"

#include "gtest/gtest.h"

#include <array>

using namespace std;

namespace tcycle {
namespace testing {

// Recall double and float are illegal template parameters.
TEST(TemplateCycleTest, IntType) {
  int a = 1, b = 2, c = 3;
  // The fact that the function invocation is in the same namespace is
  // apparently not enough for the linker.
  // /usr/bin/ld: /tmp/ccFtmrEW.o: in function
  // `tcycle::testing::TemplateCycleTest_IntType_Test::TestBody()':
  // /home/alison/gitsrc/Cpp-Exercises/template_cycle_lib_test.cc:15: undefined
  // reference to `void tcycle::cycle<int>(int&, int&, int&)'
  //  cycle(a, b, c);
  tcycle::cycle(a, b, c);
  ASSERT_EQ(2, a);
  ASSERT_EQ(3, b);
  ASSERT_EQ(1, c);
}

TEST(TemplateCycleTest, PolynomialTest) {
  ::std::array<int, 3> expon = {{1, 2, 3}};
  ::std::array<double, 3> coeffs = {{1.0, 2.0, 3.0}};
  polynomial::Polynomial testpoly1(coeffs, expon);
  polynomial::Polynomial testpoly2 = testpoly1 + testpoly1;
  polynomial::Polynomial testpoly3 = testpoly2 + testpoly1;
  tcycle::move_cycle(testpoly1, testpoly2, testpoly3);
}

} // namespace testing
} // namespace tcycle
