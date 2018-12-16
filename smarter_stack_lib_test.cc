#include "smarter_stack.h"

#include <cstdio>
#include <cstring>

#include "gtest/gtest.h"

using namespace std;

namespace smarter_stack {
namespace testing {

class SmarterStackTest : public ::testing::Test {
public:
  void SetUp() override {
    vector<double> data1 = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    double data2[] = {6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
    st1 = new SmarterStack(data1);
    st2 = new SmarterStack(7, data2);
    st3 = new SmarterStack(1);
  }
  void TearDown() override {
    delete st1;
    delete st2;
    delete st3;
  }

protected:
  SmarterStack *st1, *st2, *st3;
};

TEST_F(SmarterStackTest, Ctors) {
  ostringstream ost, ost1;
  ost << *st1;
  // Taking the str() member forces the compiler to resolve the overload of
  // operator<<() to the STL definition.
  EXPECT_EQ("0123456", ost.str());
  EXPECT_TRUE(st1->full());
  ost1 << *st2;
  EXPECT_EQ("6543210", ost1.str());
  EXPECT_FALSE(st2->empty());
}

TEST_F(SmarterStackTest, IndexOperator) {
  EXPECT_EQ((*st1)[0], (*st2)[6]);
  EXPECT_EQ((*st1)[6], (*st2)[0]);
}

TEST_F(SmarterStackTest, PushAndPop) {
  EXPECT_EQ(6.0, st1->Pop());
  st1->Push(11.0);
  EXPECT_EQ(11.0, st1->Pop());
}

TEST_F(SmarterStackTest, PushIntoEmpty) {
  EXPECT_TRUE(st3->empty());
  st3->Push(6.0);
  EXPECT_FALSE(st3->empty());
  EXPECT_TRUE(st3->full());
  EXPECT_EQ(6.0, st3->Pop());
  EXPECT_TRUE(st3->empty());
}

using SmarterStackDeathTest = SmarterStackTest;

TEST_F(SmarterStackDeathTest, Empty) {
  EXPECT_DEATH(st3->Pop(), "Invalid argument.");
}

TEST_F(SmarterStackDeathTest, Full) {
  st3->Push(6.0);
  EXPECT_DEATH(st3->Push(6.0), "No space left on device.");
}

TEST_F(SmarterStackTest, Equality) {
  vector<double> data1 = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
  SmarterStack st4(data1);
  EXPECT_EQ(*st1, st4);
}

TEST_F(SmarterStackTest, CopyCtor) {
  SmarterStack st4(*st1);
  EXPECT_EQ(*st1, st4);
}

TEST_F(SmarterStackTest, Reverse) {
  EXPECT_FALSE(*st1 == *st2);
  (*st2).Reverse();
  EXPECT_EQ(*st1, *st2);
}

TEST_F(SmarterStackTest, DoubleReverseIsIdempotent) {
  SmarterStack st4(*st1);
  st4.Reverse();
  EXPECT_FALSE(*st1 == st4);
  ostringstream ost;
  ost << st4;
  EXPECT_EQ("6543210", ost.str());
  st4.Reverse();
  EXPECT_EQ(*st1, st4);
  ostringstream ost1;
  ost1 << st4;
  EXPECT_EQ("0123456", ost1.str());
}

TEST_F(SmarterStackDeathTest, EmptySingleEndedSubsequence) {
  EXPECT_DEATH(SmarterStack teststack = (*st1)(-1), "Invalid argument.");
}

TEST_F(SmarterStackDeathTest, OverlongSingleEndedSubsequence) {
  EXPECT_DEATH(SmarterStack teststack = (*st1)(100), "Invalid argument.");
}

TEST_F(SmarterStackTest, SingleEndedSubsequenceTest) {
  SmarterStack teststack = (*st1)(3);
  EXPECT_FALSE(teststack.empty());
  EXPECT_EQ(4, teststack.depth());
  ostringstream ost;
  ost << teststack;
  EXPECT_EQ("0123", ost.str());
  EXPECT_EQ(3.0, teststack.Pop());

  // The top element of st1 has index st1->depth()-1, so the 'to' parameter of
  // the subsequence points to the last element of st1.
  SmarterStack teststack2 = (*st1)(st1->depth() - 1);
  ostringstream ost1;
  ost1 << teststack2;
  EXPECT_EQ("0123456", ost1.str());
  EXPECT_EQ(st1->depth(), teststack2.depth());
  EXPECT_EQ(st1->Pop(), teststack2.Pop());
}

TEST_F(SmarterStackTest, EmptyDoubleEndedSubsequence) {
  EXPECT_DEATH(SmarterStack teststack = (*st1)(-1, 0), "Invalid argument.");
  // Produces a subsequence with element 0.
  //  EXPECT_DEATH(SmarterStack teststack = (*st1)(0, 0), "Invalid argument.");
  EXPECT_DEATH(SmarterStack teststack = (*st1)(5, 0), "Invalid argument.");
}

TEST_F(SmarterStackTest, OverlongDoubleEndedSubsequence) {
  EXPECT_DEATH(SmarterStack teststack = (*st1)(100), "Invalid argument.");
}

TEST_F(SmarterStackTest, DoubleEndedSubsequenceTest) {
  ostringstream ost;
  SmarterStack teststack = (*st1)(0, 3);
  EXPECT_FALSE(teststack.empty());
  ASSERT_EQ(4, teststack.depth());
  ost << teststack;
  EXPECT_EQ("0123", ost.str());
  EXPECT_EQ(3.0, teststack.Pop());

  // Reusing the teststack variable causes a use-after-free error.
  SmarterStack teststack2 = (*st1)(1, 4);
  ASSERT_EQ(4, teststack2.depth());
  ostringstream ost1;
  ost1 << teststack2;
  EXPECT_EQ("1234", ost1.str());
  EXPECT_EQ(4.0, teststack2.Pop());

  SmarterStack teststack3 = (*st1)(0, st1->depth() - 1);
  ostringstream ost2;
  ost2 << teststack3;
  EXPECT_EQ("0123456", ost2.str());
  ASSERT_EQ(st1->depth(), teststack3.depth());
  EXPECT_EQ(st1->Pop(), teststack3.Pop());
}

TEST_F(SmarterStackTest, IncreasingSubsequences) {
  vector<double> vec = {7, 9, 3, 2, 6, 8, 9, 2};
  SmarterStack st(vec);
  ostringstream ost;
  PrintIncreasingSubsequences(ost, st);
  EXPECT_EQ("(79),(3),(2689),(2),", ost.str());
}

} // namespace testing
} // namespace smarter_stack
