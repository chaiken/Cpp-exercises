#include "templated_stack.h"

#include <iostream>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

namespace templated_stack {
namespace testing {

constexpr char alphalist[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                              'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                              's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

constexpr char alphalist2[] = {'z', 'y', 'x', 'w', 'v', 'u', 't', 's', 'r',
                               'q', 'p', 'o', 'n', 'm', 'l', 'k', 'j', 'i',
                               'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};

class CharStackTest : public ::testing::Test {
public:
  CharStackTest() { charstack1 = new TemplatedStack<char>(); }
  ~CharStackTest() { delete charstack1; }

  TemplatedStack<char> *charstack1;
};

TEST_F(CharStackTest, DefaultCtorTest) {
  EXPECT_EQ(kDefaultSize, charstack1->size());
  EXPECT_EQ(CONSTSIZE, kDefaultSize);
  EXPECT_TRUE(charstack1->empty());
  for (char c : alphalist) {
    if (!charstack1->full()) {
      charstack1->push(c);
      EXPECT_EQ(c, charstack1->top_of());
    }
  }
  EXPECT_TRUE(charstack1->full());
  EXPECT_FALSE(charstack1->empty());
  cout << *charstack1 << endl;
  int i = CONSTSIZE;
  while (i > 0) {
    EXPECT_EQ(alphalist[i - 1], charstack1->pop());
    i--;
  }
  EXPECT_TRUE(charstack1->empty());
}

} // namespace testing
} // namespace templated_stack
