// https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
#include <iostream>

#include "reverse_char_stack.h"
#include "gtest/gtest.h"

using namespace std;

const ::std::string kTestString("Not unreasonably long.");

namespace charstack {
namespace local_testing {

TEST(ReverseCharStackTest, StringConstructor) {
  CharStack stack(kTestString);
  pair<string, result> res{pop_all(stack)};
  EXPECT_EQ(result::kSuccess, res.second);
  EXPECT_EQ(".gnol ylbanosaernu toN", res.first);
}

TEST(ReverseCharStackTest, CopyConstructor) {
  CharStack stack1(kTestString);
  CharStack stack2(stack1);
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack2).first);
}

TEST(ReverseCharStackTest, Reset) {
  CharStack stack(kTestString);
  EXPECT_FALSE(stack.empty());
  stack.reset();
  EXPECT_TRUE(stack.empty());
}

TEST(ReverseCharStackTest, PushMultiple) {
  CharStack stack;
  result res = stack.PushMultiple(kTestString.length(), kTestString.c_str());
  EXPECT_EQ(result::kSuccess, res);
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack).first);
}

TEST(ReverseCharStackTest, PushFull) {
  int i = 0;
  CharStack stack;
  while (i++ <= kFull) {
    stack.Push('a');
  }
  EXPECT_EQ(result::kFailure, stack.Push('a'));
  EXPECT_EQ(result::kFailure, stack.PushMultiple(3, "abc"));
}

TEST(ReverseCharStackTest, PushTooFew) {
  CharStack stack;
  const char str[2] = "a";
  EXPECT_EQ(result::kFailure, stack.PushMultiple(100, str));
}

TEST(ReverseCharStackTest, PushTooMany) {
  CharStack stack;
  string too_long = kTestString + kTestString + kTestString;
  EXPECT_EQ(result::kFailure, stack.PushMultiple(100, too_long.c_str()));
}

TEST(ReverseCharStackTest, PopEmpty) {
  CharStack stack;
  EXPECT_EQ(result::kFailure, stack.Pop().second);
}

TEST(ReverseCharStackTest, PopTooMany) {
  CharStack stack(kTestString);
  char out[5];
  EXPECT_EQ(result::kFailure, stack.PopMultiple(100, out));
}

TEST(ReverseCharStackTest, ExtractionOperator) {
  ostringstream out;
  CharStack stack(kTestString);
  out << stack;
  EXPECT_NE(string::npos, out.str().find(kTestString));
}

// https://stackoverflow.com/questions/4810516/c-redirecting-stdout
// Following approach in async_logger_lib_test_improved.cc and associated
// library.
TEST(ReverseCharStackTest, EmptyStack) {
  streambuf *oldCoutStreamBuf = cout.rdbuf();
  ostringstream strCout;
  // error: lvalue required as left operand of assignment
  // cout.rdbuf() = strCout.rdbuf();
  // Apparently a rdbuf() call operator makes this version work?
  cout.rdbuf(strCout.rdbuf());
  CharStack st;
  EXPECT_EQ(result::kSuccess, st.Reverse());
  // error: lvalue required as left operand of assignment
  // cout.rdbuf() = oldCoutStreamBuf;
  cout.rdbuf(oldCoutStreamBuf);
  EXPECT_NE(string::npos,
            strCout.str().find("Empty stack need not be reversed."));
}

TEST(ReverseCharStackTest, BasicReversal) {
  CharStack stack(kTestString);
  EXPECT_EQ(result::kSuccess, stack.Reverse());
  pair<string, result> res = pop_all(stack);
  EXPECT_EQ(result::kSuccess, res.second);
  EXPECT_EQ(kTestString, res.first);
}

TEST(ReverseCharStackTest, PopMultiple) {
  CharStack stack(kTestString);
  EXPECT_EQ(result::kSuccess, stack.Reverse());

  char str[8] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
  EXPECT_EQ(result::kSuccess, stack.PopMultiple(6u, str));
  EXPECT_EQ(6u, strlen(str));

  const string &str2 = kTestString.substr(0, 6);
  EXPECT_EQ(0, strcmp(str2.c_str(), str));
  // ASSERT_EQ() does pointer equality on pointers. If used on two C strings, it
  // tests if they are in the same memory location, not if they have the same
  // value. Therefore, if you want to compare C strings (e.g. const char*) by
  // value, use ASSERT_STREQ()
  ASSERT_STREQ(str2.c_str(), str);
}

} // namespace local_testing
} // namespace charstack
