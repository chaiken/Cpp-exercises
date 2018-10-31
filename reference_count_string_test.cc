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
  EXPECT_EQ(false, (test_string1 == test_string2));
  test_string1 = test_string2;
  test_string1.Print();
  EXPECT_EQ(true, (test_string1 == test_string2));
}

TEST(ReferenceCountedStringTest, Concatenation) {
  CountedString test_string1("abcde");
  CountedString test_string2("12345");
  CountedString test_string3("abcde12345");
  CountedString test_string4 = test_string1 + test_string2;
  EXPECT_EQ(true, (test_string4 == test_string3));
  EXPECT_EQ(true, (test_string3 == test_string4));
  CountedString empty;
  test_string4 = test_string1 + empty;
  EXPECT_EQ(true, (test_string4 == test_string1));
}

TEST(ReferenceCountedStringTest, Subscript) {
  CountedString test_string1("abcde");
  CountedString test_string2("edcba");
  char c;
  int i = 0, j;
  while ((c = test_string1[i]) != -1) {
    ::std::cout << "i: " << i << " c: " << c << ::std::endl;
    j = test_string2.length() - (i + 1);
    EXPECT_EQ(c, test_string2[j]);
    i++;
  }
}

} // namespace testing
} // namespace reference_counted_string
