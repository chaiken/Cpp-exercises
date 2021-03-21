#include <cassert>
#include <cstdlib>
#include <iostream>

#include "notqsort.h"

using namespace std;
using namespace not_qsort;

int data[] = {9, 4, 23, 43, 99, 4955, -1, 234};
constexpr size_t kHowmany = sizeof(data) / sizeof(int);

int main(void) {

  notqsort(data, kHowmany, kHowmany, compare);
  print_values(data, 0u, kHowmany);
  cout << endl;

  notqsort(data, kHowmany, kHowmany, revcompare);
  print_values(data, 0u, kHowmany);
  cout << endl;

  exit(EXIT_SUCCESS);
}
