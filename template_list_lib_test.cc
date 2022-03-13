#include "template_list.h"

#include "complex.h"

#include <cstdlib>

#include <list>

#include "gtest/gtest.h"

using namespace std;
using namespace complex;

namespace template_list {
namespace testing {

class TemplateListTest : public ::testing::Test {
public:
  TemplateListTest() : newlist(list<long int>()), trivlist(list<int>()) {
    FillList(newlist);
    FillTrivialList(trivlist);
  }
  list<long int> newlist;
  list<int> trivlist;
};

TEST_F(TemplateListTest, SizeCheck) {
  ASSERT_EQ(static_cast<long unsigned>(ELEMNUM), newlist.size());
}

TEST_F(TemplateListTest, BasicStatistics) {
  map<long, int> randcounter;
  struct list_stats<long> mathstats;
  CalculateListStatistics(newlist, randcounter, mathstats);
  cout << "min: " << mathstats.min << endl;
  cout << "mean: " << mathstats.mean << endl;
  cout << "max: " << mathstats.max << endl;
  cout << "mode: " << mathstats.mode.first << " with count "
       << mathstats.mode.second << endl;
  cout << "Number of unique results from " << ELEMNUM
       << " random numbers: " << randcounter.size() << endl;
}

TEST_F(TemplateListTest, TrivStatistics) {
  map<int, int> trivcounter;
  struct list_stats<int> mathstats;
  CalculateListStatistics(trivlist, trivcounter, mathstats);
  ASSERT_EQ(0, mathstats.min);
  ASSERT_EQ(49999.5, mathstats.mean);
  ASSERT_EQ(99999, mathstats.max);
  ASSERT_EQ(1, mathstats.mode.second);
  cout << "Number of unique results from " << ELEMNUM
       << " random numbers: " << trivcounter.size() << endl;
}

TEST_F(TemplateListTest, PrintingTest) {
  ostringstream out;
  out << newlist;
  EXPECT_GT(out.str().size(), 0u);
  map<long, int> randcounter;
  struct list_stats<long> mathstats;
  CalculateListStatistics(newlist, randcounter, mathstats);
  EXPECT_NE(string::npos, out.str().find(to_string(mathstats.min)));
  EXPECT_NE(string::npos, out.str().find(to_string(mathstats.mode.first)));
  EXPECT_NE(string::npos, out.str().find(to_string(mathstats.max)));
}

TEST_F(TemplateListTest, ObjectStatisticsTest) {
  list<Complex> complexlist;
  FillComplexListTrivial(complexlist);
  ASSERT_FALSE(complexlist.empty());
  ASSERT_EQ(static_cast<long unsigned>(SMALLNUM), complexlist.size());

  ostringstream out;
  out << complexlist;
  cout << out.str() << endl;

  map<Complex, int> counters;
  struct object_stats<Complex> complexstats;
  CalculateListStatisticsObject(complexlist, counters, complexstats);
  cout << "Complex mode: " << complexstats.mode.first << " with count "
       << complexstats.mode.second << endl;
  EXPECT_NE(counters.find(complexstats.mode.first), counters.end());

  // Check that the value reported as the mode appears in the container.
  // out.str().find(complexstats.mode.first) is string::npos.
  // to_string(complexstats.mode.first) is completely broken.
  ostringstream out2;
  out2 << complexstats.mode.first;
  EXPECT_NE(string::npos, out.str().find(out2.str()));
}

TEST_F(TemplateListTest, SimpleInsertionTest) {
  // The assignment below involves an implicit type conversion which allows the
  // comparisons below to execute without an explicit cast.
  long unsigned listlen = ELEMNUM;
  ASSERT_EQ(listlen, trivlist.size());
  // Find the arbitrary insertion point.
  list<int>::iterator it = trivlist.begin();
  while (*it != 65) {
    it++;
  }
  cout << "Before insertion, *prev(it): " << *prev(it) << endl;
  list<int> templist;
  templist.push_back(ELEMNUM * 2);
  copy(templist.begin(), templist.end(), inserter(trivlist, it));
  EXPECT_EQ(listlen + 1, trivlist.size());
  cout << "After insertion, *prev(it): " << *prev(it) << endl;
  EXPECT_EQ(ELEMNUM * 2, *(prev(it)));
  // it itself is unchanged.
  EXPECT_EQ(65, *it);
}

TEST_F(TemplateListTest, SimpleEraseTest) {
  // The assignment below involves an implicit type conversion which allows the
  // comparisons below to execute without an explicit cast.
  long unsigned listlen = ELEMNUM;
  // Find the arbitrary insertion point.
  list<int>::iterator it = trivlist.begin();
  while (*it != 65) {
    it++;
  }
  EXPECT_EQ(65, *it);
  // Following http://www.cplusplus.com/reference/list/list/erase/
  it = trivlist.erase(it);
  EXPECT_EQ(66, *it);
  EXPECT_EQ(listlen - 1, trivlist.size());
}

TEST(TemplateListHelperTest, BetterInsertionTest) {
  long unsigned listlen = ELEMNUM;
  shared_ptr<list<int>> sharedlist(new list<int>());
  FillTrivialList(*sharedlist);
  // Find the arbitrary insertion point.
  list<int>::iterator it = sharedlist->begin();
  int ctr = 0;
  while (*it != 65) {
    it++;
    ctr++;
  }
  ListHelper<int> lh(sharedlist);
  cout << "List use count before do_insert(): " << sharedlist.use_count() << endl;
  lh.do_insert(it, ELEMNUM * 2);
  // it itself is unchanged.
  EXPECT_EQ(64, *prev(it));
  EXPECT_EQ(ELEMNUM * 2, *it);
  EXPECT_EQ(65, *next(it));
  it = sharedlist->begin();
  int ctr2 = 0;
  while ((*it != ELEMNUM * 2) && (it != sharedlist->end())) {
    it++;
    ctr2++;
  }
  EXPECT_EQ(ctr2, ctr);
  EXPECT_EQ(listlen + 1, sharedlist->size());
}

TEST(TemplateListHelperTest, BetterErasureTest) {
  long unsigned listlen = ELEMNUM;
  shared_ptr<list<int>> sharedlist(new list<int>());
  FillTrivialList(*sharedlist);
  // Find the arbitrary erasure point.
  list<int>::iterator it = sharedlist->begin();
  cout << "List use count after getting iterator: " << sharedlist.use_count() << endl;
  while (*it != 65) {
    it++;
  }
  EXPECT_EQ(65, *it);
  ListHelper<int> lh(sharedlist);
  cout << "List use count before do_erase(): " << sharedlist.use_count() << endl;
  lh.do_erase(it);
  EXPECT_EQ(66, *it);
  EXPECT_EQ(64, *prev(it));
  EXPECT_EQ(listlen - 1, sharedlist->size());
}

} // namespace testing
} // namespace template_list
