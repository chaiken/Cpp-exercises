#include "template_stack.h"

#include <iostream>
#include <vector>

#include "gtest/gtest.h"

using namespace std;

namespace template_stack {
namespace testing {

constexpr char alphalist[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                              'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                              's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

constexpr char alphalist2[] = {'z', 'y', 'x', 'w', 'v', 'u', 't', 's', 'r',
                               'q', 'p', 'o', 'n', 'm', 'l', 'k', 'j', 'i',
                               'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};

// CONSTSIZE is defined in the Makefile.  That's the only point of this
// otherwise duplicate code.
TEST(CharStackTest, DefaultCtorTest) {
  TemplateStack<char> charstack;
  EXPECT_EQ(kDefaultSize, charstack.size());
  EXPECT_EQ(CONSTSIZE, kDefaultSize);
  EXPECT_TRUE(charstack.empty());
  for (char c : alphalist) {
    if (!charstack.full()) {
      charstack.push(c);
      EXPECT_EQ(c, charstack.top_of());
    }
  }
  EXPECT_TRUE(charstack.full());
  EXPECT_FALSE(charstack.empty());
  cout << charstack << endl;
  int i = CONSTSIZE;
  while (i > 0) {
    EXPECT_EQ(alphalist[i - 1], charstack.pop());
    i--;
  }
  EXPECT_TRUE(charstack.empty());
}

} // namespace testing
} // namespace template_stack
