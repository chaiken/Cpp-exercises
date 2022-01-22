#include "smarter_stack.h"

#include <cstdio>
#include <cstring>

#include "gtest/gtest.h"

using namespace std;

namespace smarter_stack {
namespace testing {

class SmarterStackTest : public ::testing::Test {
public:
  SmarterStackTest()
      : st1(SmarterStack(data1)), st2(SmarterStack(7, data2)),
        st3(SmarterStack(1)) {}
  vector<double> data1{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  // Without "7u",
  // error: flexible array member not at end of class
  double data2[7u]{6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
  SmarterStack st1, st2, st3;
};

TEST_F(SmarterStackTest, Ctors) {
  ostringstream ost, ost1;
  ost << st1;
  // Taking the str() member forces the compiler to resolve the overload of
  // operator<<() to the STL definition.
  EXPECT_EQ("0123456", ost.str());
  EXPECT_TRUE(st1.full());
  ost1 << st2;
  EXPECT_EQ("6543210", ost1.str());
  EXPECT_FALSE(st2.empty());
}

TEST_F(SmarterStackTest, IndexOperator) {
  EXPECT_EQ(st1[0], st2[6]);
  EXPECT_EQ(st1[6], st2[0]);
}

TEST_F(SmarterStackTest, PushAndPop) {
  EXPECT_EQ(6.0, st1.Pop());
  st1.Push(11.0);
  EXPECT_EQ(11.0, st1.Pop());
}

TEST_F(SmarterStackTest, PushIntoEmpty) {
  EXPECT_TRUE(st3.empty());
  st3.Push(6.0);
  EXPECT_FALSE(st3.empty());
  EXPECT_TRUE(st3.full());
  EXPECT_EQ(6.0, st3.Pop());
  EXPECT_TRUE(st3.empty());
}

TEST_F(SmarterStackTest, PushIntoFull) {
  EXPECT_TRUE(st1.full());
  try {
    st1.Push(3);
  } catch (SmarterStackException &sse) {
    string errstr(sse.what());
    ASSERT_TRUE(string::npos != errstr.find("SmarterStack is full."));
  }
}

TEST_F(SmarterStackTest, PopFromEmpty) {
  EXPECT_TRUE(st3.empty());
  try {
    st3.Pop();
  } catch (SmarterStackException &sse) {
    string errstr(sse.what());
    ASSERT_TRUE(string::npos != errstr.find("SmarterStack is empty."));
  }
}

TEST_F(SmarterStackTest, Equality) {
  SmarterStack st4(data1);
  EXPECT_EQ(st1, st4);
  EXPECT_FALSE(st1 == st3);
  EXPECT_FALSE(st1 == st2);
  SmarterStack st5(move(st1));
  SmarterStack st6(move(st2));
  SmarterStack st7(move(st3));
  EXPECT_FALSE(st5 == st6);
  EXPECT_FALSE(st5 == st7);
  st6.Reverse();
  SmarterStack st8(move(st6));
  EXPECT_TRUE(st5 == st8);
  const SmarterStack st9(1);
  const SmarterStack st10(move(st5));
  EXPECT_FALSE(st9 == st10);
  const vector<double> avec{-1, 1};
  const vector<double> bvec{1, 2};
  const SmarterStack st11(avec);
  const SmarterStack st12(bvec);
  EXPECT_FALSE(st11 == st12);
}

TEST_F(SmarterStackTest, MoveCtor) {
  SmarterStack st4(move(st1));
  EXPECT_EQ(SmarterStack(data1), st4);
}

TEST_F(SmarterStackTest, Reverse) {
  EXPECT_FALSE(st1 == st2);
  st2.Reverse();
  EXPECT_EQ(st1, st2);
}

TEST_F(SmarterStackTest, DoubleReverseIsIdempotent) {
  SmarterStack st4(move(st1));
  st4.Reverse();
  SmarterStack st5(data1);
  EXPECT_FALSE(st5 == st4);
  ostringstream ost;
  ost << st4;
  EXPECT_EQ("6543210", ost.str());
  st4.Reverse();
  EXPECT_EQ(st5, st4);
  ostringstream ost1;
  ost1 << st4;
  EXPECT_EQ("0123456", ost1.str());
}

TEST_F(SmarterStackTest, SingleEndedSubsequenceTest) {
  SmarterStack teststack = st1(3);
  EXPECT_FALSE(teststack.empty());
  EXPECT_EQ(4, teststack.depth());
  ostringstream ost;
  ost << teststack;
  EXPECT_EQ("0123", ost.str());
  EXPECT_EQ(3.0, teststack.Pop());

  // The top element of st1 has index st1.depth()-1, so the 'to' parameter of
  // the subsequence points to the last element of st1.
  SmarterStack teststack2 = st1(st1.depth() - 1);
  ostringstream ost1;
  ost1 << teststack2;
  EXPECT_EQ("0123456", ost1.str());
  EXPECT_EQ(st1.depth(), teststack2.depth());
  EXPECT_EQ(st1.Pop(), teststack2.Pop());
}

TEST_F(SmarterStackTest, EmptyDoubleEndedSubsequence) {
  try {
    SmarterStack teststack = st1(-1, 0);
  } catch (const SmarterStackException &sse) {
    string str = sse.what();
    cerr << str << endl;
    ASSERT_NE(string::npos, str.find("Out of range."));
    ASSERT_NE(string::npos, str.find("range_error"));
  }
  // Produces a subsequence with element 0.
  //  EXPECT_DEATH(SmarterStack teststack = st1(0, 0), "Invalid argument.");
  try {
    SmarterStack teststack = st1(5, 0);
  } catch (const SmarterStackException &sse) {
    string str(sse.what());
    cerr << str << endl;
    ASSERT_NE(string::npos, str.find("Out of range."));
    ASSERT_NE(string::npos, str.find("range_error"));
  }
}

TEST_F(SmarterStackTest, OverlongDoubleEndedSubsequence) {
  try {
    SmarterStack teststack = (st1)(100);
  } catch (const SmarterStackException &sse) {
    string str(sse.what());
    cerr << str << endl;
    ASSERT_NE(string::npos, str.find("Out of range."));
    ASSERT_NE(string::npos, str.find("range_error"));
  }
}

TEST_F(SmarterStackTest, DoubleEndedSubsequenceTest) {
  ostringstream ost;
  SmarterStack teststack = st1(0, 3);
  EXPECT_FALSE(teststack.empty());
  ASSERT_EQ(4, teststack.depth());
  ost << teststack;
  EXPECT_EQ("0123", ost.str());
  EXPECT_EQ(3.0, teststack.Pop());

  // Reusing the teststack variable causes a use-after-free error.
  SmarterStack teststack2 = st1(1, 4);
  ASSERT_EQ(4, teststack2.depth());
  ostringstream ost1;
  ost1 << teststack2;
  EXPECT_EQ("1234", ost1.str());
  EXPECT_EQ(4.0, teststack2.Pop());

  SmarterStack teststack3 = st1(0, st1.depth() - 1);
  ostringstream ost2;
  ost2 << teststack3;
  EXPECT_EQ("0123456", ost2.str());
  ASSERT_EQ(st1.depth(), teststack3.depth());
  EXPECT_EQ(st1.Pop(), teststack3.Pop());
}

TEST_F(SmarterStackTest, IncreasingSubsequences) {
  vector<double> vec = {7, 9, 3, 2, 6, 8, 9, 2};
  SmarterStack st(vec);
  ostringstream ost;
  PrintIncreasingSubsequences(ost, st);
  EXPECT_EQ("(79),(3),(2689),(2),", ost.str());
}

TEST_F(SmarterStackTest, BadIndex) {
  try {
    [[gnu::unused]] double d = st1[100];
  } catch (const SmarterStackException &sse) {
    string str = sse.what();
    cerr << str << endl;
    ASSERT_NE(string::npos, str.find("Out of range."));
    ASSERT_NE(string::npos, str.find("range_error"));
  }

  const SmarterStack ts(std::move(st1));
  try {
    [[gnu::unused]] double d = ts[100];
  } catch (const SmarterStackException &sse) {
    string str = sse.what();
    cerr << str << endl;
    ASSERT_NE(string::npos, str.find("Out of range."));
    ASSERT_NE(string::npos, str.find("range_error"));
  }
}

TEST(SmarterStackExceptionTest, EmptyCtor) {
  try {
    SmarterStack st(-1);
  } catch (const SmarterStackException &sse) {
    string str = sse.what();
    cerr << str << endl;
    ASSERT_NE(string::npos,
              str.find("SmarterStack depth must be greater than zero."));
    ASSERT_NE(string::npos, str.find("length_error"));
  }
}

/* Changing from raw new to unique_ptrs alleviates problems with bad
allocations.
// Trivial class that exists only to throw bad_alloc().
struct SmarterStackBadAlloc : public SmarterStack {
  SmarterStackBadAlloc(int size) : SmarterStack(size) {}
  SmarterStackBadAlloc(int size, const double *data)
      : SmarterStack(size, data) {}
  SmarterStackBadAlloc(const vector<double> data) : SmarterStack(data) {}
  SmarterStackBadAlloc(const SmarterStack &st) : SmarterStack(st) {}

  //
https://en.cppreference.com/w/cpp/memory/new/operator_new#Class-specific_overloads
  // clang-format off
  // error: ‘static void* smarter_stack::testing::SmarterStackBadAlloc::operator
new(size_t)’ marked ‘override’, but does not override
  //static void *operator new(size_t sz) override {
  // clang-format on
  static void *operator new(size_t sz) {
    cerr << "Throwing bad_alloc()" << endl;
    throw bad_alloc();
    return ::operator new(sz);
  }
  // This version doesn't cause a bad alloc, as SmarterStack itself does not
  // call SmarterStackBadAlloc's operator:new. static void *operator new(size_t
sz) throw() { cerr << "Throwing bad_alloc() instead of using " << sz << endl;
        throw bad_alloc();
        return ::operator new(sz);
     return nullptr;
    }
};

TEST(SmarterStackNoMemDeathTest, CtorSizeAlloc) {
  ASSERT_THROW(new SmarterStackBadAlloc(100), bad_alloc);
}

TEST(SmarterStackNoMemDeathTest, CtorArrayAlloc) {
  const double data{0.0};
  ASSERT_THROW(new SmarterStackBadAlloc(1, &data), bad_alloc);
}

TEST(SmarterStackNoMemDeathTest, CtorVectorAlloc) {
  vector<double> vec(5.0);
  ASSERT_THROW(new SmarterStackBadAlloc(vec), bad_alloc);
}

TEST(SmarterStackNoMemDeathTest, CtorCopyAlloc) {
  vector<double> vec(5.0);
  SmarterStack st(vec);
  ASSERT_THROW(new SmarterStackBadAlloc(st), bad_alloc);
}
*/

} // namespace testing
} // namespace smarter_stack
