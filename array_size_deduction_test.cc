#include "array_size_deduction_impl.h"

#include "gtest/gtest.h"

#include <array>

constexpr char alphalist[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                              'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                              's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

TEST(SizeDeductionTest, CharArray) { ASSERT_EQ(26u, arraySize(alphalist)); }

TEST(SizeDeductionTest, StdArray) {
  const std::array<char, arraySize(alphalist)> alphabet{'a'};
  ASSERT_EQ(alphabet.size(), arraySize(alphalist));
}
