#include "gtest/gtest.h"

#include "dyn_string.h"

using namespace std;

const ::std::string kTestString = "Strings are cheap.";

TEST(DynStringTest, AssignWorks) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b;
  b.assign(a);
  ASSERT_EQ(0, a.compare(b));
}

TEST(DynStringTest, EmptyConcatHasNoEffect) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b;
  dyn_string::DynString c;
  c.concat(a, b);
  ASSERT_EQ(0, a.compare(c));
}

TEST(DynStringTest, ConcatWorks) {
  dyn_string::DynString a("a");
  dyn_string::DynString b("b");
  dyn_string::DynString c;
  dyn_string::DynString d("ab");
  c.concat(a, b);
  ASSERT_EQ(0, c.compare(d));
}

TEST(DynStringTest, UnequalStringsCompareProperly) {
  dyn_string::DynString a("a");
  dyn_string::DynString b("b");
  ASSERT_EQ(-1, a.compare(b));
}

TEST(DynStringTest, ShorterStringIsLess) {
  dyn_string::DynString a("bc");
  dyn_string::DynString b("a");
  ASSERT_EQ(1, a.compare(b));
}

TEST(DynStringTest, EqualStringsCompareProperly) {
  dyn_string::DynString a("a");
  dyn_string::DynString b("a");
  ASSERT_EQ(0, a.compare(b));
}

TEST(DynStringTest, EmptyStringIsLeast) {
  dyn_string::DynString a("a");
  dyn_string::DynString b("");
  ASSERT_EQ(1, a.compare(b));
}

TEST(DynStringTest, DoubleReverseIsIdempotent) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b(kTestString.c_str());
  a.reverse();
  ASSERT_EQ(-1, a.compare(b));
  a.reverse();
  ASSERT_EQ(0, a.compare(b));
}

TEST(DynStringTest, SwapWorks) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b("a");
  dyn_string::DynString c(kTestString.c_str());
  dyn_string::DynString d("a");
  ASSERT_EQ(-1, a.compare(d));
  ASSERT_EQ(1, b.compare(c));
  a.swap(b);
  ASSERT_EQ(0, a.compare(d));
  ASSERT_EQ(0, b.compare(c));
}
