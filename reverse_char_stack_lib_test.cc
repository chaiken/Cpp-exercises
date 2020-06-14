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
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack));
}

TEST(ReverseCharStackTest, CopyConstructor) {
  CharStack stack1(kTestString);
  CharStack stack2(stack1);
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack2));
}

TEST(ReverseCharStackTest, Reset) {
  CharStack stack(kTestString);
  EXPECT_FALSE(stack.empty());
  stack.reset();
  EXPECT_TRUE(stack.empty());
}

TEST(ReverseCharStackTest, PushMultiple) {
  CharStack stack;
  stack.PushMultiple(kTestString.length(), kTestString.c_str());
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack));
}

TEST(ReverseCharStackDeathTest, PushFull) {
  int i = 0;
  CharStack stack;
  while (i++ <= kFull) {
    stack.Push('a');
  }
  EXPECT_EXIT(stack.Push('a'), testing::KilledBySignal(SIGABRT),
              "Unexpected error: Value too large for defined data type.");
}

TEST(ReverseCharStackDeathTest, PushTooFew) {
  CharStack stack;
  const char str[2] = "a";
  EXPECT_EXIT(stack.PushMultiple(100, str), testing::KilledBySignal(SIGABRT),
              "provided string 'a' is too short.");
}

TEST(ReverseCharStackDeathTest, PushTooMany) {
  CharStack stack;
  string too_long = kTestString + kTestString + kTestString;
  EXPECT_EXIT(stack.PushMultiple(100, too_long.c_str()),
              testing::KilledBySignal(SIGABRT),
              "Unexpected error: No message of desired type.");
}

TEST(ReverseCharStackDeathTest, PopEmpty) {
  CharStack stack;
  EXPECT_EXIT(stack.Pop(), testing::KilledBySignal(SIGABRT),
              "Unexpected error: No data available.");
}

TEST(ReverseCharStackDeathTest, PopTooMany) {
  CharStack stack(kTestString);
  char out[5];
  EXPECT_EXIT(stack.PopMultiple(100, out), testing::KilledBySignal(SIGABRT),
              "Unexpected error: No message of desired type.");
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
  st.Reverse();
  // error: lvalue required as left operand of assignment
  // cout.rdbuf() = oldCoutStreamBuf;
  cout.rdbuf(oldCoutStreamBuf);
  EXPECT_NE(string::npos,
            strCout.str().find("Empty stack need not be reversed."));
}

TEST(ReverseCharStackTest, BasicReversal) {
  CharStack stack(kTestString);
  stack.Reverse();
  EXPECT_EQ(kTestString, pop_all(stack));
}

TEST(ReverseCharStackTest, PopMultiple) {
  CharStack stack(kTestString);
  stack.Reverse();

  char str[50];
  stack.PopMultiple(6u, str);
  str[7] = '\0';

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
