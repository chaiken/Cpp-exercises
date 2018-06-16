#include "slist2.h"

#include "gtest/gtest.h"

using namespace std;
using namespace slist;

TEST(SingleLinkListTest, PopWorks) {
  SingleLinkList sll("abc");
  ASSERT_EQ(3u, sll.Length());
  ASSERT_EQ('a', sll.Pop());
  ASSERT_EQ(2u, sll.Length());
}

TEST(SingleLinkListTest, StringConstructorWorks) {
  SingleLinkList sll("abc");
  sll.Print();
  ASSERT_EQ('a', sll.Pop());
}

TEST(SingleLinkListTest, PrependWorks) {
  SingleLinkList s1;
  s1.Prepend('a');
  s1.Prepend('b');
  ASSERT_EQ('b', s1.Pop());
  ASSERT_EQ('a', s1.Pop());
}

TEST(SingleLinkListTest, LengthWorks) {
  SingleLinkList sll("abc");
  ASSERT_EQ(3u, sll.Length());
}

TEST(SingleLinkListTest, EmptyListHasZeroLength) {
  SingleLinkList sll;
  ASSERT_EQ(0u, sll.Length());
}

TEST(SingleLinkListTest, DeleteWorks) {
  SingleLinkList sll("abc");
  sll.Delete();
  ASSERT_EQ(2u, sll.Length());
  ASSERT_EQ('b', sll.Pop());
}

TEST(SingleLinkListTest, EmptyStringHasZeroLength) {
  SingleLinkList sll;
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
  SingleLinkList sll;
  ASSERT_EQ(0u, sll.Count('g'));
}

TEST(SingleLinkListTest, AppendWorks) {
  SingleLinkList sll1("abc");
  SingleLinkList sll2("def");
  unsigned sum = sll1.Length() + sll2.Length();
  sll1.Append(sll2);
  ASSERT_EQ(sum, sll1.Length());
  ASSERT_EQ('a', sll1.Pop());
}

TEST(SingleLinkListTest, EmptyAppendHasNoEffect) {
  SingleLinkList sll1("abc");
  SingleLinkList sll2;
  unsigned len = sll1.Length();
  sll1.Append(sll2);
  ASSERT_EQ(len, sll1.Length());
}

TEST(SingleLinkListTest, AppendToEmptyListIsCopyConstructor) {
  SingleLinkList sll1("abc");
  SingleLinkList sll2;
  sll2.Append(sll1);
  ASSERT_EQ(3u, sll2.Length());
  ASSERT_EQ('a', sll2.Pop());
}

// Cannot think of any way to test without accessing private data.
// Therefore, change Tail() to be a private function.
// TEST(SingleLinkListTest, TailWorks) {
//  SingleLinkList sll("abc");
//  ASSERT_EQ('c', sll.Tail().Pop());
//}
