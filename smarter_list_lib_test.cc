#include "smarter_list.hh"

#include "gtest/gtest.h"

using namespace std;

namespace smarter_list {
namespace testing {

struct SmarterListTest : public ::testing::Test {
  vector<string> vec{"A", "B", "C", "D", "E", "F"};
  // Substituting parentheses for braces results in compiler error "vec is not a
  // type" since the compiler apparently mistakes the intended object
  // instantation for a function call.
  SmarterList sl{vec};
};

TEST_F(SmarterListTest, PointerCtor) {
  ListNode ln("A");
  sl.reset();
  ListNode ln2(sl.current());
  EXPECT_EQ(ln2, ln);
}

TEST_F(SmarterListTest, Reverse) {
  SmarterList sl2(vec);
  sl.Reverse();
  EXPECT_NE(sl2, sl);
  sl.Reverse();
  EXPECT_EQ(sl2, sl);
  SmarterList sl3;
  sl3.Reverse();
  EXPECT_NE(sl3, sl);
}

TEST_F(SmarterListTest, VectorConstructor) {
  ostringstream output;
  output << sl;
  EXPECT_EQ("A, B, C, D, E, F, \n", output.str());
}

TEST_F(SmarterListTest, MoveConstructor) {
  SmarterList sl2(move(sl));
  ASSERT_FALSE(sl2.empty());
  ostringstream output;
  output << sl2;
  EXPECT_EQ("A, B, C, D, E, F, \n", output.str());
}

TEST_F(SmarterListTest, Equality) {
  SmarterList sl2{vec};
  EXPECT_EQ(*sl.current(), *sl2.current());
  EXPECT_EQ(sl, sl2);
  SmarterList sl3{};
  EXPECT_TRUE(sl3 != sl2);
  SmarterList sl4({"A", "A"});
  SmarterList sl5({"A"});
  EXPECT_NE(sl4, sl5);
  // For 100% test coverage.
  EXPECT_NE(sl5, sl4);
}

TEST_F(SmarterListTest, IndexOperator) {
  for (unsigned i = 0u; i < vec.size(); i++) {
    EXPECT_EQ(0, sl[i].first);
    EXPECT_EQ(vec.at(i), sl[i].second.name);
  }
  EXPECT_EQ(ENODATA, sl[500].first);
  EXPECT_EQ(nullptr, sl[500].second);
}

TEST_F(SmarterListTest, ForwardsAndBackwards) {
  sl.reset();
  EXPECT_EQ("A", sl.current()->name);
  ++sl;
  EXPECT_EQ("B", sl.current()->name);
  --sl;
  EXPECT_EQ("A", sl.current()->name);
  for (int i = 0; i < 10; i++) {
    ++sl;
  }
  EXPECT_EQ(nullptr, sl.current());
  sl.reset();
  ++sl;
  ++sl;
  ++sl;
  for (int i = 0; i < 10; i++) {
    --sl;
  }
  EXPECT_EQ(nullptr, sl.current());
}

TEST_F(SmarterListTest, AppendNode) {
  ListNode ln("G");
  sl + ln;
  ostringstream output;
  output << sl;
  EXPECT_EQ("A, B, C, D, E, F, G, \n", output.str());
}

TEST(SmarterSimpleTest, EmptyList) {
  SmarterList sl1;
  EXPECT_TRUE(sl1.empty());
  EXPECT_EQ(nullptr, sl1.current());
  --sl1;
  EXPECT_EQ(nullptr, sl1.current());
  ListNode ln("G");
  sl1 + ln;
  ASSERT_FALSE(sl1.empty());
  ostringstream output;
  output << sl1;
  EXPECT_EQ("G, \n", output.str());
}

TEST(SmarterSimpleTest, IndexEmptyList) {
  SmarterList sl;
  EXPECT_EQ(sl[0].first, ENODATA);
  EXPECT_EQ(sl[0].second, nullptr);
}

} // namespace testing
} // namespace smarter_list
