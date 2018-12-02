#include "smarter_stack.h"

#include <cstdio>

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
  cout << *st1;
  EXPECT_TRUE(st1->full());
  cout << *st2;
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
  st4.Reverse();
  EXPECT_EQ(*st1, st4);
}

TEST_F(SmarterStackTest, EmptySubsequence) {
  SmarterStack teststack = (*st1)(0);
  EXPECT_TRUE(teststack.empty());
}

TEST_F(SmarterStackTest, SubsequenceTest) {
  SmarterStack teststack = (*st1)(3);
  EXPECT_FALSE(teststack.empty());
  EXPECT_EQ(2.0, teststack.Pop());
}

TEST_F(SmarterStackTest, OverlongSubsequence) {
  SmarterStack teststack = (*st1)(100);
  EXPECT_FALSE(teststack.empty());
  EXPECT_EQ(6.0, teststack.Pop());
}

} // namespace testing
} // namespace smarter_stack
