#include "gtest/gtest.h"

#include "dyn_string.h"

using namespace std;

const ::std::string kTestString = "Strings are cheap.";

TEST(DynStringTest, EmptyConcatHasNoEffect) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b;
  dyn_string::DynString c;
  c.concat(a, b);
  ASSERT_TRUE(a.equals(c));
}

TEST(DynStringTest, AssignWorks) {
  dyn_string::DynString a(kTestString.c_str());
  dyn_string::DynString b;
  b.assign(a);
  ASSERT_TRUE(a.equals(b));
}
