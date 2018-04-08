#include <iostream>

#include "reverse_char_stack_lib.h"
#include "gtest/gtest.h"

using namespace std;

TEST(ReverseCharStackTest, PopWholeStack) {
  CharStack stack("Not unreasonably long.");
  EXPECT_EQ(".gnol ylbanosaernu toN", pop_all(stack));
}

TEST(ReverseCharStackTest, BasicReversal) {
  CharStack stack("Not unreasonably long.");
  stack.Reverse();
  EXPECT_EQ("Not unreasonably long.", pop_all(stack));
}
