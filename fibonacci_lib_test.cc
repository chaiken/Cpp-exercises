#include "fibonacci.hh"

#include "gtest/gtest.h"

#include <climits>

namespace fibonacci {
namespace local_testing {

struct FibonacciTest : public ::testing::Test {
  void SetUp() {
    max_stack = get_max_stack_depth();
    ASSERT_EQ(0, max_stack.first);
  }
  rlim_t max_stack_depth() const { return max_stack.second; }
  std::pair<int, rlim_t> max_stack;
};

TEST_F(FibonacciTest, Zero) { EXPECT_EQ(0U, calc_sum(0U, max_stack_depth())); }

TEST_F(FibonacciTest, One) { EXPECT_EQ(1U, calc_sum(1U, max_stack_depth())); }

TEST_F(FibonacciTest, Larger) {
  EXPECT_EQ(6U, calc_sum(3U, max_stack_depth()));
}

TEST_F(FibonacciTest, StackOverflow) {
  EXPECT_THROW(calc_sum(UINT_MAX, max_stack_depth()), std::runtime_error);
}

} // namespace local_testing
} // namespace fibonacci
