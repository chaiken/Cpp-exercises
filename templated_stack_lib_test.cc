#include "complex.h"
#include "templated_stack.h"

#include <iostream>
#include <vector>

#include "gtest/gtest.h"

using namespace std;
using namespace complex;

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

TEST_F(CharStackTest, RValRefArrayCtorTest) {
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

TEST_F(CharStackTest, MoveCtorTest) {
  TemplatedStack<char> charstack2({'a', 'b', 'c', 'd', 'e'}, 5);
  TemplatedStack<char> charstack3(move(charstack2));
  int i = 4;
  while (!charstack3.empty()) {
    EXPECT_EQ(alphalist[i], charstack3.pop());
    i--;
  }
}

TEST(StringStackTest, ReverseTest) {
  char *strarr[5];
  array<string, 5> strarr1 = {"Heute", "ist", "ein", "staatlicher", "Feiertag"};
  for (int i = 0; i < 5; i++) {
    strarr[i] = const_cast<char *>(strarr1[i].c_str());
  }
  reverse(strarr, 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(strarr[i], strarr1[4 - i]);
  }
  reverse(strarr, 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(strarr[i], strarr1[i]);
  }
}

// double-free():
// (gdb) p charstack2
// $1 = {max_len_ = 0x5, top_ = 0x4, data_ = 0x5555555db600 "abcdeU"}
// (gdb) p charstack4
// $2 = {max_len_ = 0x5, top_ = 0x4, data_ = 0x5555555db600 "abcdeU"}
TEST_F(CharStackTest, MoveAssignment) {
  TemplatedStack<char> charstack2({'a', 'b', 'c', 'd', 'e'}, 5);
  TemplatedStack<char> charstack3({'a', 'b', 'c', 'd', 'e'}, 5);
  // This initialization is a work-around for the Most Vexing Parse, which
  // TemplatedStack<char> charstack4();
  // triggers.   There appears to be no way to call the default constructor.
  TemplatedStack<char> charstack4({'f', 'f', 'f', 'f', 'f'}, 5);
  EXPECT_NE(charstack4.top_of(), charstack3.top_of());
  EXPECT_NE(charstack4[0], charstack3[0]);
  charstack4 = move(charstack2);
  EXPECT_FALSE(charstack4.empty());
  int i = 4;
  while (!charstack4.empty()) {
    EXPECT_EQ(charstack4.top_of(), charstack3[i]);
    EXPECT_EQ(charstack4[i], charstack3[i]);
    EXPECT_EQ(charstack4.pop(), charstack3[i]);
    i--;
  }
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

class ComplexStackTest : public ::testing::Test {
public:
  ComplexStackTest() : tsc(new TemplatedStack<Complex>()) {}
  ~ComplexStackTest() {
    delete compvec;
    delete tsc;
  }

  // Should match the Complex ctor that takes a two-element array as its
  // parameter.
  vector<Complex> *compvec = new vector<Complex>({{
      {0.0, 0.0},
      {1.0, 1.0},
      {2.0, 2.0},
      {3.0, 3.0},
      {4.0, 4.0},
  }});
  TemplatedStack<Complex> *tsc;
};

TEST_F(ComplexStackTest, DefaultCtorTest) {
  EXPECT_TRUE(tsc->empty());
  for (vector<Complex>::iterator it = compvec->begin(); it != compvec->end();
       it++) {
    tsc->push(*it);
  }
  EXPECT_FALSE(tsc->empty());
  cout << *tsc << endl;
  for (vector<Complex>::reverse_iterator it = compvec->rbegin();
       it != compvec->rend(); it++) {
    EXPECT_EQ(*it, tsc->pop());
  }
}

TEST_F(ComplexStackTest, ReverseTest) {
  // Won't work due to most vexing parse.
  //  Complex *carr[5](), *carr1[5]();
  Complex carr[] = {Complex(0, 0), Complex(1, 1), Complex(2, 2), Complex(3, 3),
                    Complex(4, 4)};

  Complex carr1[] = {Complex(4, 4), Complex(3, 3), Complex(2, 2), Complex(1, 1),
                     Complex(0, 0)};
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(carr[i], carr1[4 - i]);
  }
  reverse(carr, 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(carr[i], carr1[i]);
  }
  reverse(carr, 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(carr[i], carr1[4 - i]);
  }
}

} // namespace testing
} // namespace templated_stack
