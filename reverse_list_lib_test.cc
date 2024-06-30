#include "reverse_list.hh"

#include "gtest/gtest.h"

#include <vector>

using namespace std;

namespace reverse {
namespace testing {

vector<int> testvecodd{{0, 1, 2, 3, 4, 5, 6}};
vector<int> testveceven{{0, 1, 2, 3, 4, 5}};
list<int> testlistodd(testvecodd.begin(), testvecodd.end());
list<int> reversedodd(testvecodd.rbegin(), testvecodd.rend());
list<int> testlisteven(testveceven.begin(), testveceven.end());
list<int> reversedeven(testveceven.rbegin(), testveceven.rend());

TEST(ReverseListTest, EmptyList) {
  list<int> emptylist;
  PerformReverse(emptylist);
  ASSERT_TRUE(emptylist.empty());
}

TEST(ReverseListTest, OneElement) {
  int arr[] = {3};
  list<int> alist(arr, arr + sizeof(arr) / sizeof(int));
  ASSERT_EQ(1U, alist.size());
  PerformReverse(alist);
  ASSERT_EQ(1U, alist.size());
  ASSERT_EQ(3, *(alist.begin()));
}

TEST(ReverseListTest, ReverseOdd) {
  ASSERT_EQ(7U, testlistodd.size());
  PerformReverse(testlistodd);
  EXPECT_EQ(7U, testlistodd.size());
  list<int>::iterator rit = reversedodd.begin();
  list<int>::iterator it = testlistodd.begin();
  while (rit != reversedodd.end() && it != testlistodd.end()) {
    EXPECT_EQ(*it, *rit);
    rit++, it++;
  }
}

TEST(ReverseListTest, ReverseEven) {
  ASSERT_EQ(6U, testlisteven.size());
  PerformReverse(testlisteven);
  EXPECT_EQ(6U, testlisteven.size());
  list<int>::iterator rit = reversedeven.begin();
  list<int>::iterator it = testlisteven.begin();
  while (rit != reversedeven.end() && it != testlisteven.end()) {
    EXPECT_EQ(*it, *rit);
    rit++, it++;
  }
}

} // namespace testing
} // namespace reverse
