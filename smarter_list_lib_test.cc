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

TEST_F(SmarterListTest, IndexOperator) { EXPECT_EQ("A", (*sl1_)[0].name); }

} // namespace testing
} // namespace smarter_list
