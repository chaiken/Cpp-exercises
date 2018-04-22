#include "gtest/gtest.h"

#include "dyn_string.h"

using namespace std;

const ::std::string kTestString = "Strings are cheap.";

TEST(DynStringTest, AssignWorks) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b;
  b.assign(a);
  ASSERT_TRUE(a.equals(b));
}

TEST(DynStringTest, EmptyConcatHasNoEffect) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b;
  dyn_string::DynString c;
  c.concat(a, b);
  ASSERT_TRUE(a.equals(c));
}

TEST(DynStringTest, ConcatWorks) {
  dyn_string::DynString a("a");
  dyn_string::DynString b("b");
  dyn_string::DynString c;
  dyn_string::DynString d("ab");
  c.concat(a, b);
  ASSERT_TRUE(c.equals(d));
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
