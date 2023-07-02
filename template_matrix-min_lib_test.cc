#include "template_matrix-min.h"

#include "gtest/gtest.h"

namespace template_matrix_min {
namespace local_testing {

// clang-format off
// Problem statement from the ACCU journal.
const std::array<int, 4U *4U> amatrix = {{
    1,      2,   3,   4,
    3,      4,   7,   8,
    4,      7,   9,  10,
    17,  13, 14, 26
  }};

const std::array<int, 9> bmatrix = {{
    1, 2,  3,
    3, 4,  7,
    4, 7, 14
  }};
// clang-format on

// The static variable is in the class's namespace, so it must explicitly be set
// to the value in the current namespace. Note that the parameter is a template
// specialization, as the size variable can be inferred from the result of the
// statement.  The two definitions correspond to different specializations.
template <>
std::array<int, 16U> Mask<4U>::coeffs =
    template_matrix_min::local_testing::amatrix;

template <>
std::array<int, 9U> Mask<3U>::coeffs =
    template_matrix_min::local_testing::bmatrix;

TEST(MatrixMinTest, Colnum) {
  for (uint16_t idx = 0u; idx < 4U; idx++) {
    EXPECT_EQ(colnum<4U>(idx), colnum<4U>(idx + 4U));
    // Exceeds matrix bounds, but doesn't matter for test.
    EXPECT_NE(colnum<4U>(idx), colnum<4U>((idx + 4U) - 1U));
  }
}

TEST(MatrixMinTest, Rownum) {
  for (uint16_t idx = 0u; idx < 4U; idx++) {
    EXPECT_EQ(rownum<4U>(idx), rownum<4U>(idx - colnum<4U>(idx)));
    EXPECT_NE(rownum<4U>(idx), rownum<4U>(idx + 4U));
  }
}

TEST(MatrixMinTest, Equality) {
  // Array ctor.
  Mask<4U> amask(std::array<uint16_t, 4U>{{1, 2, 3, 4}});
  Mask<4U> bmask(std::array<uint16_t, 4U>{{4, 2, 3, 1}});
  EXPECT_EQ(amask, amask);
  EXPECT_NE(amask, bmask);

  // Copy ctor.
  Mask<4U> cmask(amask);
  EXPECT_EQ(cmask, amask);
  EXPECT_NE(bmask, cmask);

  // Copy assignment.
  Mask<4U> dmask = cmask;
  EXPECT_EQ(dmask, cmask);
  bmask = cmask;
  EXPECT_EQ(bmask, cmask);
}

TEST(MatrixMinTest, Sum) {
  EXPECT_EQ(40, sum<4U>({{0, 5, 10, 15}}));
  EXPECT_EQ(40, sum<4U>({{1, 4, 10, 15}}));
  Mask<4U> bad(std::array<uint16_t, 4U>{{0, 0, 0, 512}});
  EXPECT_DEATH(sum<4U>(bad), "Index 512 out of bounds.");
}

TEST(MatrixMinTest, Compare) {
  const Mask<3U> amask(std::array<uint16_t, 3U>{{0, 4U, 8U}});
  const Mask<3U> bmask(std::array<uint16_t, 3U>{{0, 5U, 7U}});
  EXPECT_TRUE(mask_compare(bmask, amask));
  // https://stackoverflow.com/questions/5424990/c-stdset-insert-causing-segmentation-fault
  // When the set is initialized without the comparator, the *2nd* insertion
  // causes a SEGV, as the comparator is a nullptr.
  MaskSet<3U> outputs(comparator<3U>);
  ASSERT_EQ(0U, outputs.size());
  std::pair<MaskSetIterator<3U>, bool> result = outputs.insert(amask);
  EXPECT_EQ(true, result.second);
  EXPECT_EQ(outputs.begin(), result.first);
  ASSERT_EQ(1U, outputs.size());

  // bmask has lower sum (15) compared to amask (sum = 19);
  EXPECT_LT(sum(bmask), sum(amask));
  result = outputs.insert(bmask);
  EXPECT_EQ(true, result.second);
  EXPECT_EQ(outputs.begin(), result.first);
  ASSERT_EQ(2U, outputs.size());

  const Mask<3U> cmask(amask);
  result = outputs.insert(cmask);
  EXPECT_EQ(false, result.second);
  ASSERT_EQ(2U, outputs.size());

  const Mask<3U> dmask(std::array<uint16_t, 3U>{{1U, 3U, 8U}});
  EXPECT_EQ(sum(dmask), sum(amask));
  result = outputs.insert(dmask);
  //  dmask has the same sum (19) as amask, so insertion fails since
  //  comparator() shows them to be the same even though  operator==() shows
  //  them to be different.
  EXPECT_EQ(false, result.second);
  ASSERT_EQ(2U, outputs.size());
}

TEST(MatrixMinTest, SwapCols) {
  // Initialize with diagonal elements.
  Mask<4U> initial(std::array<uint16_t, 4U>{{0, 5, 10, 15}});
  const Mask<4U> save(initial);
  EXPECT_FALSE(initial.swap_cols(0, 0));
  EXPECT_EQ(save, initial);

  EXPECT_TRUE(initial.swap_cols(0, 2));
  const Mask<4U> expected(std::array<uint16_t, 4U>{{2, 5, 8, 15}});
  EXPECT_NE(save, initial);
  EXPECT_EQ(initial, expected);
}

TEST(MatrixMinTest, GeneratePermutations) {
  const Mask<4U> initial(std::array<uint16_t, 4U>{{0, 5, 10, 15}});
  MaskSet<4U> outputs(comparator<4U>);
  outputs = generate_permutations<4U>(initial);
  for (const Mask<4U> &m : outputs) {
    std::cerr << sum(m) << ", " << m << std::endl;
  }

  std::cout << std::endl;
  const Mask<3U> initial2(std::array<uint16_t, 3U>{{0, 4, 8}});
  MaskSet<3U> outputs2(comparator<3U>);
  outputs2 = generate_permutations<3U>(initial2);
  for (const Mask<3U> &m : outputs2) {
    std::cerr << sum(m) << ", " << m << std::endl;
  }
}

} // namespace local_testing
} // namespace template_matrix_min
