#include "one_index_vector.h"

#include "complex.h"

#include "gtest/gtest.h"

using namespace std;

namespace one_index_vector {
namespace local_testing {

struct OneIndexVectorTest : public testing::Test {
  OneIndexVectorTest()
      : vec0(vector<int>()), vec1(vector<int>({{10, 20, 30, 40, 50, 60}})),
        vec2(vector<char>({{'a', 'b', 'c', 'd', 'e', 'f', 'g'}})) {}
  vector<int> vec0, vec1;
  vector<char> vec2;
};

TEST_F(OneIndexVectorTest, BaseCtorTest) {
  OneIndexVector<int> v0;
  EXPECT_TRUE(v0.empty());
  EXPECT_EQ(0u, v0.size());

  OneIndexVector<int> ov1(10u, 8);
  EXPECT_FALSE(ov1.empty());
  EXPECT_EQ(10u, ov1.size());
  EXPECT_EQ(8, *ov1.begin());

  OneIndexVector<complex::Complex> v2(1u, complex::Complex(-4, 0.1));
  EXPECT_FALSE(v2.empty());
  EXPECT_EQ(1u, v2.size());
}

TEST_F(OneIndexVectorTest, MoveCtorAndAssign) {
  // Move empty vector.
  OneIndexVector<int> ov0(move(vec0));
  EXPECT_EQ(vec0.size(), ov0.size());
  EXPECT_TRUE(ov0.empty());

  size_t save = vec1.size();
  OneIndexVector<int> ov1(move(vec1));
  EXPECT_EQ(save, ov1.size());
  EXPECT_FALSE(ov1.empty());
  EXPECT_TRUE(vec1.empty());

  OneIndexVector<char> ov2;
  save = vec2.size();
  ov2 = move(vec2);
  EXPECT_TRUE(vec2.empty());
  EXPECT_FALSE(ov2.empty());
  EXPECT_EQ(save, ov2.size());
}

TEST_F(OneIndexVectorTest, RangeCtorTest) {
  vector<int>::iterator first = vec1.begin();
  vector<int>::iterator last = first;
  last++;
  OneIndexVector<int> ov1(first, last);
  ASSERT_EQ(1u, ov1.size());
}

TEST_F(OneIndexVectorTest, ElementAccessTest) {
  vector<int> vec3(vec1);
  OneIndexVector<int> ov1(move(vec1));
  for (size_t i = 0u; i < vec3.size(); i++) {
    EXPECT_EQ(ov1[i + 1u].first, true);
    EXPECT_EQ(ov1[i + 1u].second, vec3[i]);
    EXPECT_EQ(ov1.at(i + 1u).first, true);
    EXPECT_EQ(ov1.at(i + 1u).second, vec3.at(i));
  }
}

TEST_F(OneIndexVectorTest, IllegalElementAccessOperator) {
  OneIndexVector<char> ov1(move(vec2));
  EXPECT_EQ(ov1[0].first, false);
  // Triggers heap-buffer overflow.
  //  EXPECT_EQ(ov1[0].second, char{'\0'});
}

TEST_F(OneIndexVectorTest, IllegalElementAccessAt) {
  OneIndexVector<char> ov1(move(vec2));
  EXPECT_EQ(ov1.at(0).first, false);
}

TEST_F(OneIndexVectorTest, IllegalElementHighOperator) {
  vector<char> vec3(vec2);
  OneIndexVector<char> ov1(move(vec2));
  EXPECT_EQ(ov1[vec3.size() + 1].first, false);
}

TEST_F(OneIndexVectorTest, IllegalElementHighAt) {
  vector<char> vec3(vec2);
  OneIndexVector<char> ov1(move(vec2));
  EXPECT_EQ(ov1.at(vec3.size() + 1).first, false);
}

} // namespace local_testing
} // namespace one_index_vector
