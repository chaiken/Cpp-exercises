#include "reference_count_string.h"

#include "gtest/gtest.h"

using namespace std;

namespace reference_counted_string {
namespace testing {

TEST(ReferenceCountStringTest, ConstructorAndAssignmentTest) {
  CountedString test_string1("abcde");
  test_string1.Print();
  CountedString test_string2("12345");
  test_string2.Print();
  EXPECT_EQ(true, test_string1.Compare(test_string1));
  EXPECT_EQ(false, test_string1.Compare(test_string2));
  test_string1 = test_string2;
  test_string1.Print();
  EXPECT_EQ(true, test_string1.Compare(test_string2));
}

TEST(ReferenceCountedStringTest, Concatenation) {
  CountedString test_string1("abcde");
  CountedString test_string2("12345");
  CountedString test_string3("abcde12345");
  CountedString test_string4 = test_string1 + test_string2;
  EXPECT_EQ(true, test_string4.Compare(test_string3));
  EXPECT_EQ(true, test_string3.Compare(test_string4));
}

} // namespace testing
} // namespace reference_counted_string
