#include "templated_stack.h"

#include <iostream>

#include "gtest/gtest.h"

using namespace std;

namespace templated_stack {
namespace testing {

constexpr char alphalist[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                              'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                              's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

class CharStackTest : public ::testing::Test {
public:
  CharStackTest() { charstack1 = new TemplatedStack<char, 20>(); }
  ~CharStackTest() { delete charstack1; }

  TemplatedStack<char, 20> *charstack1;
};

TEST_F(CharStackTest, DefaultCtorTest) {
  EXPECT_TRUE(charstack1->empty());
  int i = 0;
  while (!charstack1->full()) {
    charstack1->push(alphalist[i]);
    EXPECT_EQ(alphalist[i], charstack1->top_of());
    i++;
  }
  cout << *charstack1 << endl;
  EXPECT_EQ(alphalist[19], charstack1->pop());
  EXPECT_EQ(alphalist[18], charstack1->top_of());
  EXPECT_FALSE(charstack1->full());
  EXPECT_FALSE(charstack1->empty());
}

TEST_F(CharStackTest, MoveCtorTest) {
  TemplatedStack<char, 5> charstack2({'a', 'b', 'c', 'd', 'e'}, 5);
  ostringstream out;
  out << charstack2;
  EXPECT_EQ("e, d, c, b, a", out.str());
  cout << charstack2 << endl;
  EXPECT_FALSE(charstack2.empty());
  EXPECT_TRUE(charstack2.full());
  EXPECT_EQ(alphalist[4], charstack2.top_of());
  EXPECT_EQ(alphalist[4], charstack2.pop());
  EXPECT_EQ(alphalist[3], charstack2.top_of());
  int i = 3;
  while (!charstack2.empty()) {
    EXPECT_EQ(alphalist[i], charstack2.pop());
    i--;
  }
}

} // namespace testing
} // namespace templated_stack
