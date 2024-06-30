#include <cassert>
#include <cstdlib>
#include <iostream>

#include "notqsort.hh"

using namespace std;
using namespace not_qsort;

int input[] = {9, 4, 23, 43, 99, 4955, -1, 234};
constexpr size_t kHowmany = sizeof(input) / sizeof(int);

int main(void) {

  notqsort(input, kHowmany, kHowmany, compare);
  print_values(input, 0u, kHowmany);
  cout << endl;

  notqsort(input, kHowmany, kHowmany, revcompare);
  print_values(input, 0u, kHowmany);
  cout << endl;

  exit(EXIT_SUCCESS);
}
