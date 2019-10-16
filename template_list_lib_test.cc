#include "template_list.h"

#include <cstdlib>

#include <list>

#include "gtest/gtest.h"

using namespace std;

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
} // namespace testing
} // namespace template_list
