#include "gtest/gtest.h"

#include "notqsort_lib.h"

using namespace std;
using namespace not_qsort;

int values[] = {9, 4, 23, 43, 99, 4955, -1, 234};
constexpr size_t kElements = sizeof(values) / sizeof(int);
int sorted_values[] = {-1, 4, 9, 23, 43, 99, 234, 4955};
int rev_sorted_values[] = {4955, 234, 99, 43, 23, 9, 4, -1};

TEST(NotQsortLibTest, CompareWorks) {
  notqsort(values, kElements, kElements, compare);
  unsigned i = 0u;
  while (i < kElements) {
    ASSERT_EQ(values[i], rev_sorted_values[i]);
    i++;
  }
}

TEST(NotQsortLibTest, RevCompareWorks) {
  notqsort(values, kElements, kElements, revcompare);
  unsigned i = 0u;
  while (i < kElements) {
    ASSERT_EQ(values[i], sorted_values[i]);
    i++;
  }
}
