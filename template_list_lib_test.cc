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
  TemplateListTest() {
    newlist = new list<long int>();
    FillList(newlist);
    trivlist = new list<int>();
    FillTrivialList(trivlist);
  }
  ~TemplateListTest() {
    delete newlist;
    delete trivlist;
  }

  list<long int> *newlist;
  list<int> *trivlist;
};

TEST_F(TemplateListTest, SizeCheck) {
  ASSERT_EQ(static_cast<long unsigned>(ELEMNUM), newlist->size());
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
  out << *newlist;
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
  FillComplexList(complexlist);
  ASSERT_FALSE(complexlist.empty());
  ASSERT_EQ(static_cast<long unsigned>(SMALLNUM), complexlist.size());

  ostringstream out;
  out << complexlist;

  map<Complex, int> randcounter;
  struct object_stats<Complex> complexstats;
  CalculateListStatisticsObject(&complexlist, randcounter, complexstats);
  cout << "Complex mode: " << complexstats.mode.first << " with count "
       << complexstats.mode.second << endl;

  // out.str().find(complexstats.mode.first) is string::npos.
  // to_string(complexstats.mode.first) is completely broken.
  ostringstream out2;
  out2 << complexstats.mode.first;
  EXPECT_NE(::std::string::npos, out.str().find(out2.str()));
}

TEST_F(TemplateListTest, InsertionTest) {
  // The assignment below involves an implicit type conversion which allows the
  // comparisons below to execute without an explicit cast.
  long unsigned listlen = ELEMNUM;
  ASSERT_EQ(listlen, trivlist->size());
  // Find the arbitrary insertion point.
  ::std::list<int>::iterator it = trivlist->begin();
  while (*it != 65) {
    it++;
  }
  cout << "Before insertion, *prev(it): " << *prev(it) << endl;
  ::std::list<int> templist;
  templist.push_back(ELEMNUM * 2);
  ::std::copy(templist.begin(), templist.end(), ::std::inserter(*trivlist, it));
  EXPECT_EQ(listlen + 1, trivlist->size());
  cout << "After insertion, *prev(it): " << *prev(it) << endl;
  EXPECT_EQ(ELEMNUM * 2, *(prev(it)));
  // it itself is unchanged.
  EXPECT_EQ(65, *it);
}

} // namespace testing
} // namespace template_list
