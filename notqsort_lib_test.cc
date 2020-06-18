#include "notqsort.h"

#include "gtest/gtest.h"

using namespace std;

namespace not_qsort {
namespace local_testing {

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

TEST(NotQsortLibTest, PrinterWorks) {
  // https://stackoverflow.com/questions/4810516/c-redirecting-stdout
  // clang-format off
  // error: invalid initialization of non-const reference of type ‘std::streambuf&’ {aka ‘std::basic_streambuf<char>&’} from an rvalue of type ‘std::basic_streambuf<char>*
  // clang-format on
  // streambuf &save_std_out_buf = cout.rdbuf();
  streambuf *save_std_out_buf = cout.rdbuf();
  ostringstream str_cout;
  cout.rdbuf(str_cout.rdbuf());
  notqsort(values, kElements, kElements, compare);
  print_values(values, 0, 1);
  EXPECT_EQ(0, str_cout.str().compare("{ 4955 }\n"));

  // Does not work.
  // str_cout.str().clear();

  ostringstream str_cout2;
  cout.rdbuf(str_cout2.rdbuf());
  notqsort(values, kElements, kElements, revcompare);
  print_values(values, 0, 1);
  EXPECT_EQ(0, str_cout2.str().compare("{ -1 }\n"));
  cout.rdbuf(save_std_out_buf);
}

} // namespace local_testing
} // namespace not_qsort
