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

} // namespace testing
} // namespace smarter_queue
