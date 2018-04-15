// https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
#include <iostream>

#include "reverse_char_stack_lib.h"
#include "gtest/gtest.h"

using namespace std;

const ::std::string kTestString("Not unreasonably long.");

namespace testing {

TEST(ReverseCharStackTest, StringConstructor) {
  charstack::CharStack stack(kTestString);
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack));
}

TEST(ReverseCharStackTest, PushMultiple) {
  charstack::CharStack stack;
  stack.PushMultiple(kTestString.length(), kTestString.c_str());
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack));
}

TEST(ReverseCharStackTest, BasicReversal) {
  charstack::CharStack stack(kTestString);
  stack.Reverse();
  EXPECT_EQ(kTestString, pop_all(stack));
}

TEST(ReverseCharStackTest, PopMultiple) {
  charstack::CharStack stack(kTestString);
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
}
