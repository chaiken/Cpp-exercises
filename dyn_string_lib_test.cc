#include "gtest/gtest.h"

#include "dyn_string.h"

using namespace std;
using namespace dyn_string;

const ::std::string kTestString = "Strings are cheap.";

TEST(DynStringTest, AssignWorks) {
  DynString a(kTestString.c_str());
  DynString b;
  b.assign(a);
  ASSERT_EQ(0, a.compare(b));
}

TEST(DynStringTest, EmptyConcatHasNoEffect) {
  DynString a(kTestString.c_str());
  DynString b;
  DynString c;
  c.concat(a, b);
  ASSERT_EQ(0, a.compare(c));
}

TEST(DynStringTest, ConcatWorks) {
  DynString a("a");
  DynString b("b");
  DynString c;
  DynString d("ab");
  c.concat(a, b);
  ASSERT_EQ(0, c.compare(d));
}

TEST(DynStringTest, UnequalStringsCompareProperly) {
  DynString a("a");
  DynString b("b");
  ASSERT_EQ(-1, a.compare(b));
}

TEST(DynStringTest, ShorterStringIsLess) {
  DynString a("bc");
  DynString b("a");
  ASSERT_EQ(1, a.compare(b));
}

TEST(DynStringTest, EqualStringsCompareProperly) {
  DynString a("a");
  DynString b("a");
  ASSERT_EQ(0, a.compare(b));
}

TEST(DynStringTest, EmptyStringIsLeast) {
  DynString a("a");
  DynString b("");
  ASSERT_EQ(1, a.compare(b));
}

TEST(DynStringTest, SwapWorks) {
  DynString a(kTestString.c_str());
  DynString b("a");
  DynString c(kTestString.c_str());
  DynString d("a");
  ASSERT_EQ(-1, a.compare(d));
  ASSERT_EQ(1, b.compare(c));
  a.swap(b);
  ASSERT_EQ(0, a.compare(d));
  ASSERT_EQ(0, b.compare(c));
}

TEST(DynStringTest, SortWorks) {
  DynString another("u7");
  DynString d("d");
  DynString a("a");
  DynString somedots("..");
  DynString longer("Hello world.");
  DynString nummern("1234");
  array<DynString, 6> dynstrarr1 = {d, another, a, somedots, longer, nummern};
  array<DynString, 6> dynstrarr2 = {somedots, nummern, longer, a, d, another};
  dyn_string_sort(&dynstrarr1[0], dynstrarr1.size(), dynstrarr1.size());

  auto it1 = dynstrarr1.begin(), it2 = dynstrarr2.begin();
  while (it1 != dynstrarr1.end()) {
    ASSERT_EQ(0, (*it1).compare(*it2));
    // it1->print();
    // it2->print();
    it1++, it2++;
  }
}

TEST(DynStringTest, DoubleReverseIsIdempotent) {
  DynString a(kTestString.c_str());
  DynString b(kTestString.c_str());
  a.reverse();
  ASSERT_EQ(-1, a.compare(b));
  a.reverse();
  ASSERT_EQ(0, a.compare(b));
}
