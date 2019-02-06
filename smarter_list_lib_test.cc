#include "smarter_list.h"

#include "gtest/gtest.h"

#include <cassert>

using namespace std;

namespace smarter_list {
namespace testing {

class SmarterListTest : public ::testing::Test {
public:
  void SetUp() {
    vec_ = new vector<string>{"A", "B", "C", "D", "E", "F"};
    assert(nullptr != vec_);
    sl1_ = new SmarterList(*vec_);
    assert(nullptr != sl1_);
  }
  void TearDown() {
    delete vec_;
    delete sl1_;
  }

  vector<string> *vec_;
  SmarterList *sl1_;
};

TEST_F(SmarterListTest, ArrayConstructor) {
  // The assertion is useless with ASAN.
  ASSERT_FALSE(nullptr == sl1_);
  ostringstream output;
  output << *sl1_;
  EXPECT_EQ("A, B, C, D, E, F, \n", output.str());
}

TEST_F(SmarterListTest, IndexOperator) {
  int i = 0;
  for (auto it = vec_->begin(); it != vec_->end(); it++, i++) {
    EXPECT_EQ((*it), (*sl1_)[i].name);
  }
}

TEST_F(SmarterListTest, CopyConstructor) {
  SmarterList sl2(*sl1_);
  ASSERT_FALSE(sl2.empty());
  ostringstream output;
  output << sl2;
  EXPECT_EQ("A, B, C, D, E, F, \n", output.str());
}

TEST_F(SmarterListTest, DeleteNode) {
  --(*sl1_);
  ostringstream output;
  output << (*sl1_);
  EXPECT_EQ("A, B, C, D, E, \n", output.str());
}

TEST_F(SmarterListTest, AppendNode) {
  ListNode ln("G");
  (*sl1_) + ln;
  ostringstream output;
  output << (*sl1_);
  EXPECT_EQ("A, B, C, D, E, F, G, \n", output.str());
}

TEST_F(SmarterListTest, EmptyList) {
  SmarterList sl;
  ASSERT_TRUE(sl.empty());
  --sl;
  ASSERT_TRUE(sl.empty());
  ListNode ln("G");
  sl + ln;
  ASSERT_FALSE(sl.empty());
  ostringstream output;
  output << sl;
  EXPECT_EQ("G, \n", output.str());
}

TEST(SmarterListDeathTest, IndexEmptyList) {
  SmarterList sl;
  EXPECT_DEATH(sl[0], "No data available.");
}

} // namespace testing
} // namespace smarter_list
