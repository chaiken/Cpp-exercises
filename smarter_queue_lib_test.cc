#include "smarter_queue.hh"

#include <cerrno>
#include <cstdio>
#include <cstring>

#include "gtest/gtest.h"

using namespace std;

namespace smarter_queue {
namespace testing {

struct SmarterQueueTest : public ::testing::Test {
  SmarterQueueTest()
      : vec(vector<double>{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0}),
        st1(SmarterQueue(vec)) {}
  vector<double> vec;
  SmarterQueue st1;
};

TEST_F(SmarterQueueTest, VectorConstructor) {
  ASSERT_TRUE(st1.is_full());
  ASSERT_FALSE(st1.is_empty());
  cout << st1 << endl;
  for (double x : vec) {
    pair<int, double> result = st1.Pop();
    EXPECT_EQ(0, result.first);
    EXPECT_EQ(x, result.second);
  }
  ASSERT_FALSE(st1.is_full());
  ASSERT_TRUE(st1.is_empty());
}

TEST_F(SmarterQueueTest, OutputOperator) {
  ostringstream out;
  out << st1;
  EXPECT_EQ("0 1 2 3 4 5 6 ", out.str());
}

TEST_F(SmarterQueueTest, PushandPop) {
  SmarterQueue st2(7);
  ASSERT_TRUE(st2.is_empty());
  for (double x : vec) {
    st2.Push(x);
  }
  ASSERT_TRUE(st2.is_full());

  // Capture stderr.
  std::streambuf *oldCerrStreamBuf(cerr.rdbuf());
  std::ostringstream strCerr;
  cerr.rdbuf(strCerr.rdbuf());
  st2.Push(1);
  EXPECT_TRUE(std::string::npos !=
              strCerr.str().find("Queue is already full."));
  cerr.rdbuf(oldCerrStreamBuf);

  for (double x : vec) {
    pair<int, double> result = st2.Pop();
    EXPECT_EQ(0, result.first);
    EXPECT_EQ(x, result.second);
  }
  ASSERT_TRUE(st2.is_empty());
}

TEST_F(SmarterQueueTest, Peek) {
  int offset = 0;
  for (auto x : vec) {
    EXPECT_EQ(0, st1.Peek(offset).first);
    EXPECT_EQ(x, st1.Peek(offset).second);
    offset++;
  }
}

TEST_F(SmarterQueueTest, Rebalance) {
  pair<int, double> result = st1.Pop();
  EXPECT_EQ(0, result.first);
  EXPECT_EQ(0.0, result.second);
  result = st1.Pop();
  EXPECT_EQ(0, result.first);
  EXPECT_EQ(1.0, result.second);
  ASSERT_FALSE(st1.is_full());
  st1.Push(-11.0);
  ASSERT_TRUE(st1.is_full());
}

TEST_F(SmarterQueueTest, Equality) {
  SmarterQueue st2(vec);
  EXPECT_TRUE(st1 == st2);
  // Different sizes.
  SmarterQueue st3({1.0, 2.0});
  EXPECT_FALSE(st1 == st3);
  SmarterQueue st4(3), st5(3);
  EXPECT_TRUE(st4 == st5);
  // Same size, different elements.
  SmarterQueue st6({2.0, 1.0});
  EXPECT_FALSE(st6 == st3);
}

TEST_F(SmarterQueueTest, Subsequence) {
  SmarterQueue st2({1.0, 2.0});
  EXPECT_EQ(0, st1(1, 2).first);
  EXPECT_EQ(st2, st1(1, 2).second);
}

TEST_F(SmarterQueueTest, MoveCtor) {
  SmarterQueue st3(vec);
  SmarterQueue st2(move(st1));
  EXPECT_EQ(st2, st3);
}

TEST_F(SmarterQueueTest, IsIncreasing) {
  EXPECT_TRUE(SequenceIsIncreasing(vec));
  EXPECT_FALSE(SequenceIsIncreasing({6.0, 5.0, 4.0}));
}

TEST_F(SmarterQueueTest, IncreasingSubsequences) {
  ostringstream out;
  EXPECT_EQ(0, FindIncreasingSubsequences(out, SmarterQueue()));
  SmarterQueue sq({7.0, 9.0, 3.0, 2.0, 6.0, 8.0, 9.0, 2.0});
  ASSERT_EQ(0, FindIncreasingSubsequences(out, sq));
  EXPECT_EQ("(7 9 ), (3 ), (2 6 8 9 ), (2 )", out.str());
}

TEST_F(SmarterQueueTest, EmptyPop) {
  SmarterQueue st2;
  pair<int, double> result(st2.Pop());
  EXPECT_EQ(ENODATA, result.first);
  EXPECT_EQ(0.0, result.second);
}

TEST_F(SmarterQueueTest, BadBounds) { EXPECT_EQ(ERANGE, st1(-1, -3).first); }

TEST_F(SmarterQueueTest, OutOfRangePeek) {
  EXPECT_EQ(ERANGE, st1.Peek(1000).first);
  EXPECT_EQ(ERANGE, st1.Peek(st1.size()).first);
  EXPECT_EQ(ERANGE, st1.Peek(-1).first);
  SmarterQueue st2(3);
  EXPECT_EQ(ENODATA, st2.Peek(1).first);
}

} // namespace testing
} // namespace smarter_queue
