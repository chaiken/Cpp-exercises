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

TEST(ReferenceCountedStringTest, CopyCtorAndEquals) {
  CountedString test_string1("abcde"), test_string2(test_string1);
  EXPECT_EQ(test_string1.length(), test_string2.length());
  EXPECT_TRUE(test_string2 == test_string1);
}

TEST(ReferenceCountedStringTest, Assignment) {
  CountedString test_string1("abcde");
  CountedString test_string2("abcde12345");
  EXPECT_NE(test_string1.length(), test_string2.length());
  EXPECT_FALSE(test_string2 == test_string1);
  test_string2 = test_string1;
  EXPECT_EQ(test_string1.length(), test_string2.length());
  EXPECT_TRUE(test_string2 == test_string1);
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

TEST(ReferenceCountedStringTest, CharPtrConversionOperator) {
  CountedString test_string1("abcde");
  ostringstream ostr;
  // Note no trailing newline from endl.
  ostr << test_string1;
  EXPECT_EQ("abcde", ostr.str());
}

TEST(ReferenceCountedStringTest, Swap) {
  CountedString test_string1("try me");
  CountedString test_string2("do not try me");
  size_t len1 = test_string1.length();
  Swap(test_string1, test_string1);
  EXPECT_EQ(len1, test_string1.length());
  size_t len2 = test_string2.length();
  Swap(test_string1, test_string2);
  EXPECT_EQ(len2, test_string1.length());
  EXPECT_EQ(len1, test_string2.length());
}

} // namespace testing
} // namespace reference_counted_string
