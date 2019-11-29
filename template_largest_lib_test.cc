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

} // namespace testing
} // namespace largest
