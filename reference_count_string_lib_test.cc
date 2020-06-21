#include "reference_count_string.h"

#include "gtest/gtest.h"

using namespace std;

namespace reference_counted_string {
namespace testing {

TEST(ReferenceCountStringTest, ConstructorAndAssignmentTest) {
  CountedString ts1("abcde");
  CountedString ts2("12345");
  EXPECT_FALSE(ts1 == ts2);
  ts1 = ts2;
  EXPECT_TRUE(ts1 == ts2);
  ts2 = ts2;
  EXPECT_TRUE(ts1 == ts2);
}

TEST(ReferenceCountStringTest, CharStarEqualityTest) {
  CountedString ts1("abcde");
  EXPECT_TRUE(ts1 == "abcde");
}

TEST(ReferenceCountedStringTest, Concatenation) {
  CountedString ts1("abcde");
  CountedString ts2("12345");
  CountedString ts3("abcde12345");
  CountedString ts4 = ts1 + ts2;
  EXPECT_TRUE((ts4 == ts3));
  EXPECT_TRUE((ts3 == ts4));
  CountedString empty;
  ts4 = ts1 + empty;
  EXPECT_TRUE((ts4 == ts1));
}

TEST(ReferenceCountedStringTest, CopyCtorAndEquals) {
  CountedString ts1("abcde"), ts2(ts1);
  EXPECT_EQ(ts1.length(), ts2.length());
  EXPECT_TRUE(ts2 == ts1);
}

TEST(ReferenceCountedStringTest, Assignment) {
  CountedString ts1("abcde");
  CountedString ts2("abcde12345");
  EXPECT_NE(ts1.length(), ts2.length());
  EXPECT_FALSE(ts2 == ts1);
  ts2 = ts1;
  EXPECT_EQ(ts1.length(), ts2.length());
  EXPECT_TRUE(ts2 == ts1);
}

TEST(ReferenceCountedStringTest, Subscript) {
  const CountedString ts1("abcde");
  const CountedString ts2("edcba");
  char c;
  int i = 0, j;
  while ((c = ts1[i]) != -1) {
    j = ts2.length() - (i + 1);
    EXPECT_EQ(c, ts2[j]);
    i++;
  }
  const int k = 1;
  EXPECT_EQ('d', ts2[k]);
}

TEST(ReferenceCountedStringTest, ExtractionOperator) {
  CountedString ts1("abcde");
  ostringstream ostr;
  // Note no trailing newline from endl.
  ostr << ts1;
  EXPECT_EQ("abcde\n", ostr.str());
}

TEST(ReferenceCountedStringTest, Swap) {
  CountedString ts1("try me");
  CountedString ts2("do not try me");
  size_t len1 = ts1.length();
  Swap(ts1, ts1);
  EXPECT_EQ(len1, ts1.length());
  size_t len2 = ts2.length();
  Swap(ts1, ts2);
  EXPECT_EQ(len2, ts1.length());
  EXPECT_EQ(len1, ts2.length());
}

TEST(ReferenceCountedStringTest, NumberCtor) {
  CountedString ts(5);
  EXPECT_EQ(5u, ts.length());
}

TEST(ReferenceCountedStringTest, OverloadedFunctionCallBadInterval) {
  CountedString ts1("try me");
  CountedString ts2 = ts1(3, 3);
  EXPECT_EQ(0u, ts2.length());
  CountedString ts3 = ts1(-4, -3);
  EXPECT_EQ(0u, ts3.length());
}

TEST(ReferenceCountedStringTest, OverloadedFunctionCallWholeString) {
  CountedString ts1("try me");
  CountedString ts2("do not try me");
  CountedString str1 = ts1(0, ts1.length());
  EXPECT_EQ(ts1.length(), str1.length());
  EXPECT_TRUE(ts1 == str1);
}

TEST(ReferenceCountedStringTest, TooLongSubString) {
  CountedString ts1("try me");
  CountedString ts2 = ts1(0, 50);
  EXPECT_TRUE(ts2 == ts1);
}

TEST(ReferenceCountedStringTest, OverloadedFunctionCallSubstringAtStart) {
  CountedString ts2("do not try me");
  CountedString ts3("do not");
  CountedString str2 = ts2(0, ts3.length());
  EXPECT_EQ(ts3.length(), str2.length());
  EXPECT_TRUE(ts3 == str2);
}

TEST(ReferenceCountedStringTest, OverloadedFunctionCallSubstringAtEnd) {
  CountedString ts1("try me");
  CountedString ts2("do not try me");
  CountedString str3 = ts2(ts2.length() - ts1.length(), ts2.length());
  EXPECT_EQ(ts1.length(), str3.length());
  EXPECT_TRUE(ts1 == str3);
}

TEST(ReferenceCountedStringTest, EmptyStringAlwaysFound) {
  CountedString ts1("try me");
  // Trying Search("") results in error about inability to convert string to
  // char*.
  char empty[] = "";
  EXPECT_TRUE(ts1.Search(empty));
}

TEST(ReferenceCountedStringTest, DuplicateIsFound) {
  CountedString ts1("try me");
  char ts2[] = "try me";
  EXPECT_TRUE(ts1.Search(ts2));
}

TEST(ReferenceCountedStringTest, TooLongSubstring) {
  CountedString ts1("try me");
  CountedString ts2("do not try me");
  EXPECT_FALSE(ts1.Search(ts2));
}

TEST(ReferenceCountedStringTest, SubstringFindingCorrect) {
  CountedString ts1("try me");
  CountedString ts2("do not try me");
  EXPECT_TRUE(ts2.Search(ts1));
  EXPECT_FALSE(ts2.Search("try do not me"));
}

} // namespace testing
} // namespace reference_counted_string
