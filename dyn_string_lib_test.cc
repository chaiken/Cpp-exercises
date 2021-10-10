#include "gtest/gtest.h"

#include "dyn_string.h"

using namespace std;

namespace dyn_string {
namespace local_testing {

const ::std::string kTestString = "Strings are cheap.";

TEST(DynStringTest, MoveCtor) {
  DynString a(kTestString.c_str());
  DynString b(std::move(a));
  ASSERT_EQ(kTestString, b.contents());
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

TEST(DynStringTest, AssignWorks) {
  DynString a(kTestString.c_str());
  EXPECT_EQ(kTestString.length(), a.len());
  DynString b;
  b = a;
  ASSERT_EQ(0, a.compare(b));
}

TEST(DynStringTest, EmptyConcatHasNoEffect) {
  DynString a(kTestString.c_str());
  DynString b;
  DynString c;
  c.concat(a, b);
  ASSERT_EQ(0, a.compare(c));
  DynString d;
  d.concat(b, a);
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

TEST(DynStringTest, SwapWorks) {
  DynString a(kTestString.c_str());
  DynString b("a");
  DynString c(kTestString.c_str());
  DynString d("a");
  // Trivial case.
  ASSERT_EQ(0, b.compare(d));
  b.swap(d);
  ASSERT_EQ(0, b.compare(d));

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
  array<DynString, 6> dynstrarr1 = {d.contents(),      another.contents(),
                                    a.contents(),      somedots.contents(),
                                    longer.contents(), nummern.contents()};
  array<DynString, 6> dynstrarr2 = {somedots.contents(), nummern.contents(),
                                    longer.contents(),   a.contents(),
                                    d.contents(),        another.contents()};
  dyn_string_sort(&dynstrarr1[0], dynstrarr1.size(), dynstrarr1.size());

  auto it1 = dynstrarr1.begin(), it2 = dynstrarr2.begin();
  while (it1 != dynstrarr1.end()) {
    ASSERT_EQ(0, (*it1).compare(*it2));
    it1++, it2++;
  }
}

TEST(DynStringTest, EmptyReverseHasNoEffect) {
  DynString a;
  ASSERT_TRUE(a.empty());
  a.reverse();
  ASSERT_TRUE(a.empty());
}

TEST(DynStringTest, DoubleReverseIsIdempotent) {
  DynString a(kTestString.c_str());
  DynString b(kTestString.c_str());
  a.reverse();
  ASSERT_EQ(-1, a.compare(b));
  ASSERT_TRUE(0u < strlen(a.contents()));
  a.reverse();
  ASSERT_EQ(0, a.compare(b));
}

TEST(DynStringTest, ExtractionOperator) {
  DynString a(kTestString.c_str());
  ostringstream out;
  out << a;
  EXPECT_EQ(out.str(), kTestString);
}

TEST(DynStringTest, SubstringPrint) {
  DynString a(kTestString.c_str());
  ostringstream out;
  {
    /* compiler complains that print_some needs 3 parameters; not sure why
       default parameter does not work. */
    // print_some(out, a);
    print_some(out, a, 0u);
    EXPECT_TRUE(out.str().empty());
  }
  {
    ostringstream out;
    print_some(out, a, 1000u);
    EXPECT_FALSE(out.str().empty());
    EXPECT_EQ(out.str().length(), a.len());
  }
  {
    ostringstream out;
    print_some(out, a, 2u);
    EXPECT_FALSE(out.str().empty());
    EXPECT_EQ(2u, out.str().length());
  }
}

} // namespace local_testing
} // namespace dyn_string
