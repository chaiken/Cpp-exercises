#include "template_largest.h"

#include "gtest/gtest.h"

#include <list>

using namespace std;

namespace largest {
namespace testing {

TEST(SecondLargestTest, ListTest) {
  list<int> alist(100);
  for (int i = 0; i < 100; i++) {
    alist.push_back(i);
  }
  //  using contype = ::std::list<int>::value_type;
  int ans = FindSecondLargest<::std::list<int>>(alist);
  cout << "Second largest int is " << ans << endl;
  EXPECT_EQ(98, ans);

  list<int> blist(100);
  for (int i = 99; i >= 0; i--) {
    blist.push_back(i);
  }
  ans = FindSecondLargest<::std::list<int>>(blist);
  cout << "Second largest int is " << ans << endl;
  EXPECT_EQ(98, ans);
}

TEST(SecondLargestTest, VectorTest) {
  vector<int> avector(100);
  for (int i = 0; i < 100; i++) {
    avector.push_back(i);
  }
  //  using contype = ::std::vector<int>::value_type;
  int ans = FindSecondLargest<::std::vector<int>>(avector);
  cout << "Second largest int is " << ans << endl;
  EXPECT_EQ(98, ans);

  vector<int> bvector(100);
  for (int i = 99; i >= 0; i--) {
    bvector.push_back(i);
  }
  ans = FindSecondLargest<::std::vector<int>>(bvector);
  cout << "Second largest int is " << ans << endl;
  EXPECT_EQ(98, ans);
}

TEST(SecondLargestTest, SetTest) {
  set<int> aset({1, 2, 3, 4, 5, 6});
  //  using contype = ::std::set<int>::value_type;
  int ans = FindSecondLargest<::std::set<int>>(aset);
  cout << "Second largest int is " << ans << endl;
  EXPECT_EQ(5, ans);

  set<int> bset({6, 5, 4, 3, 2, 1});
  ans = FindSecondLargest<::std::set<int>>(bset);
  cout << "Second largest int is " << ans << endl;
  EXPECT_EQ(5, ans);

  set<int> cset({2, 4, 1, 5, 6, 3});
  ans = FindSecondLargest<::std::set<int>>(cset);
  cout << "Second largest int is " << ans << endl;
  EXPECT_EQ(5, ans);
}

} // namespace testing
} // namespace largest
