#include "smarter_queue.h"

#include <cstdio>
#include <cstring>

#include "gtest/gtest.h"

using namespace std;

namespace smarter_queue {
namespace testing {

class SmarterQueueTest : public ::testing::Test {
public:
  void SetUp() {
    vec_ = new vector<double>{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    assert(nullptr != vec_);
    st1_ = new SmarterQueue(*vec_);
    assert(nullptr != st1_);
  }

  void TearDown() {
    delete vec_;
    delete st1_;
  }

  vector<double> *vec_;
  SmarterQueue *st1_;
};

TEST_F(SmarterQueueTest, VectorConstructor) {
  ASSERT_NE(nullptr, st1_);
  ASSERT_NE(nullptr, vec_);
  ASSERT_TRUE(st1_->is_full());
  ASSERT_FALSE(st1_->is_empty());
  cout << *st1_ << endl;
  for (double x : *vec_) {
    EXPECT_EQ(x, st1_->Pop());
  }
  ASSERT_FALSE(st1_->is_full());
  ASSERT_TRUE(st1_->is_empty());
}

TEST_F(SmarterQueueTest, OutputOperator) {
  ostringstream out;
  out << *st1_;
  EXPECT_EQ("0 1 2 3 4 5 6 ", out.str());
}

TEST_F(SmarterQueueTest, PushandPop) {
  SmarterQueue st2(7);
  ASSERT_TRUE(st2.is_empty());
  for (double x : *vec_) {
    st2.Push(x);
  }
  ASSERT_TRUE(st2.is_full());
  for (double x : *vec_) {
    EXPECT_EQ(x, st2.Pop());
  }
  ASSERT_TRUE(st2.is_empty());
}

TEST_F(SmarterQueueTest, Rebalance) {
  EXPECT_EQ(0.0, st1_->Pop());
  EXPECT_EQ(1.0, st1_->Pop());
  ASSERT_FALSE(st1_->is_full());
  st1_->Push(-11.0);
  ASSERT_TRUE(st1_->is_full());
}

TEST_F(SmarterQueueTest, Equality) {
  SmarterQueue st2(*vec_);
  EXPECT_TRUE(*st1_ == st2);
  SmarterQueue st3({1.0, 2.0});
  EXPECT_FALSE(*st1_ == st3);
  SmarterQueue st4(3), st5(3);
  EXPECT_TRUE(st4 == st5);
}

TEST_F(SmarterQueueTest, Subsequence) {
  SmarterQueue st2({1.0, 2.0});
  EXPECT_TRUE(st2 == (*st1_)(1, 2));
}

TEST_F(SmarterQueueTest, CopyCtor) {
  SmarterQueue st2(*st1_);
  EXPECT_TRUE(st2 == *st1_);
}

TEST_F(SmarterQueueTest, IsIncreasing) {
  EXPECT_TRUE(SequenceIsIncreasing(*vec_));
  EXPECT_FALSE(SequenceIsIncreasing({6.0, 5.0, 4.0}));
}

TEST_F(SmarterQueueTest, IncreasingSubsequences) {
  ostringstream out;
  SmarterQueue sq({7.0, 9.0, 3.0, 2.0, 6.0, 8.0, 9.0, 2.0});
  FindIncreasingSubsequences(out, sq);
  EXPECT_EQ("(7 9 ), (3 ), (2 6 8 9 ), (2 )", out.str());
}

using SmarterQueueDeathTest = SmarterQueueTest;

TEST_F(SmarterQueueDeathTest, EmptyPop) {
  SmarterQueue st2;
  EXPECT_DEATH(st2.Pop(), "No data available");
}

TEST_F(SmarterQueueDeathTest, BadBounds) {
  EXPECT_DEATH((*st1_)(-1, -3), "Invalid argument");
}

} // namespace testing
} // namespace smarter_queue
