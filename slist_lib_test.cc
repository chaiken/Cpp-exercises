#include "slist.hh"

#include "gtest/gtest.h"

using namespace std;
namespace slist {
namespace local_testing {

TEST(SingleLinkListTest, StringConstructorWorks) {
  SingleLinkList sll("abc");
  ASSERT_EQ('a', sll.first()->data);
}

TEST(SingleLinkListTest, EmptyString) {
  SingleLinkList sll("");
  ASSERT_TRUE(sll.empty());
}

// Make sure that deleting an empty list has no effect rather than triggering an
// error.
TEST(SingleLinkListTest, DeleteEmptyList) {
  SingleLinkList sll("a");
  ASSERT_FALSE(sll.empty());
  sll.Pop();
  ASSERT_TRUE(sll.empty());
  sll.Delete();
}

TEST(SingleLinkListTest, PrependWorks) {
  SingleLinkList s1{""};
  s1.Prepend('a');
  s1.Prepend('b');
  ASSERT_EQ('b', s1.first()->data);
  ASSERT_EQ('a', s1.first()->next->data);
}

TEST(SingleLinkListTest, LengthWorks) {
  SingleLinkList sll("abc");
  ASSERT_EQ(3u, sll.Length());
}

TEST(SingleLinkListTest, DeleteWorks) {
  SingleLinkList sll("abc");
  sll.Delete();
  ASSERT_EQ(2u, sll.Length());
  ASSERT_EQ('b', sll.first()->data);
}

TEST(SingleLinkListTest, EmptyStringHasZeroLength) {
  SingleLinkList sll{""};
  ASSERT_EQ(0u, sll.Length());
}

TEST(SingleLinkListTest, CountWorks) {
  SingleLinkList sll(
      "/usr/bin/g++ -ggdb -Wall -Wextra -g -O0 -fno-inline -fsanitize=address "
      "-I/home/alison/gitsrc/googletest/googletest/include -ggdb -g "
      "-fsanitize=undefined -L/home/alison/gitsrc/googletest/googletest/make "
      "-lpthread /home/alison/gitsrc/googletest/googletest/make/libgtest.a "
      "/home/alison/gitsrc/googletest/googletest/make/gtest_main.a "
      "slist_lib.cc slist_lib_test.cc -o slist_lib_test");
  ASSERT_EQ(29u, sll.Count('g'));
}

TEST(SingleLinkListTest, EmpyStringHasCountZero) {
  SingleLinkList sll{""};
  ASSERT_EQ(0u, sll.Count('g'));
}

TEST(SingleLinkListTest, TailWorks) {
  SingleLinkList sll("abc");
  ASSERT_EQ('c', sll.Tail()->data);
  ASSERT_EQ(nullptr, sll.Tail()->next);
}

TEST(SingleLinkListTest, AppendWorks) {
  SingleLinkList sll1("abc");
  SingleLinkList sll2("def");
  size_t sum = sll1.Length() + sll2.Length();
  sll1.Append(move(sll2));
  ASSERT_EQ(sum, sll1.Length());
  ASSERT_EQ('f', sll1.Tail()->data);
  ASSERT_EQ('a', sll1.first()->data);
}

TEST(SingleLinkListTest, AppendEmpty) {
  SingleLinkList sll("def");
  SingleLinkList sll1("def");
  while (!sll.empty()) {
    sll.Delete();
  }
  sll1.Append(move(sll));
  EXPECT_EQ(3u, sll1.Length());
}

TEST(SingleLinkListTest, PopWorks) {
  SingleLinkList sll("abc");
  ASSERT_EQ(3u, sll.Length());
  ASSERT_EQ('a', sll.Pop());
  ASSERT_EQ(2u, sll.Length());
}

TEST(SingleLinkListTest, ExtractionOperator) {
  SingleLinkList sll("abc");
  ostringstream out;
  out << sll;
  EXPECT_EQ("a -> b -> c -> 0\n", out.str());
}

TEST(SingleLinkListTest, RecursivePrint) {
  ostringstream strCout;
  // Save stdout's default buffer pointer.
  streambuf *sbuf = cout.rdbuf();
  cout.rdbuf(strCout.rdbuf());
  SingleLinkList sll("abc");
  sll.Print(sll.first());
  // Restore stdout's default buffer.
  cout.rdbuf(sbuf);
  EXPECT_EQ("a -> b -> c -> 0\n", strCout.str());
}

} // namespace local_testing
} // namespace slist
