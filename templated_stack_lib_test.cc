#include "templated_stack.h"

#include <iostream>

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
  EXPECT_TRUE(charstack1->empty());
  for (char c : alphalist) {
    if (!charstack1->full()) {
      charstack1->push(c);
      EXPECT_EQ(c, charstack1->top_of());
    }
  }
  EXPECT_FALSE(charstack1->full());
  EXPECT_FALSE(charstack1->empty());
  cout << *charstack1 << endl;
  //  int i = 0;
  for (char c : alphalist2) {
    if (!charstack1->empty()) {
      // operator[] overload not found: why?
      // error: no match for ‘operator==’ (operand types are ‘const char’ and
      // ‘const templated_stack::TemplatedStack<char>’)
      // EXPECT_EQ(c, charstack1[i++]);
      EXPECT_EQ(c, charstack1->pop());
    }
  }
  EXPECT_TRUE(charstack1->empty());
}

TEST_F(CharStackTest, MoveCtorTest) {
  TemplatedStack<char> charstack2({'a', 'b', 'c', 'd', 'e'}, 5);
  ostringstream out;
  out << charstack2;
  EXPECT_EQ("e, d, c, b, a", out.str());
  cout << charstack2 << endl;
  EXPECT_FALSE(charstack2.empty());
  EXPECT_TRUE(charstack2.full());
  EXPECT_EQ(alphalist[4], charstack2.top_of());
  int i = 4;
  while (!charstack2.empty()) {
    EXPECT_EQ(alphalist[i], charstack2[i]);
    EXPECT_EQ(alphalist[i], charstack2.pop());
    i--;
  }
  EXPECT_TRUE(charstack2.empty());
  TemplatedStack<char> charstack3({'a', 'b', 'c', 'd', 'e'}, 5);
  charstack3.reset();
  EXPECT_TRUE(charstack3.empty());
}

/* clang-format off
==14045==ERROR: AddressSanitizer: attempting free on address which was not
 malloc()-ed
Otherwise the test passes.
TEST_F(CharStackTest, ArrayCtor) {
  char newarray[5];
  for (int i = 0;
    i < 5; i++) { newarray[i] = alphalist[i];
  }
  TemplatedStack<const char> newdata = newarray;
  EXPECT_EQ(5, newdata.size());
  EXPECT_EQ(alphalist[4], newdata.top_of());
  cout << "newdata: " << endl;
  cout << newdata;
  cout << endl << "***" << endl;
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(newarray[i], newdata[i]);
  }
}
clang-format on
*/

} // namespace testing
} // namespace templated_stack
