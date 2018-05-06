using namespace std;

#include "notqsort_lib.h"

namespace not_qsort {

namespace {
void swaptwo(int *a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}
}

// Sign matters, not value.
int compare(const int *a, const int *b) { return (*a - *b); }
int revcompare(const int *a, const int *b) { return -(*a - *b); }

void print_values(int *base, unsigned start, unsigned num) {
  unsigned i = start;
  cout << "{";
  while (i < num) {
    cout << " " << *base << " ";
    base++;
    i++;
  }
  cout << "}" << endl;
}

void notqsort(int *base, size_t num, size_t len,
              int func(const int *, const int *)) {
  assert(base != 0);
  assert(num > 0u);
  // unsigned depth = len - num;
  // cout << " depth: " << len - num << endl;
  // print_values(base, depth, len);
  // cout << endl;

  // Sort the first pair.  This is the step that does work.
  if (func(base, base + 1) < 0) {
    swaptwo(base, base + 1);
  }
  // If unsorted material remains at the end, continue.
  // Simplification occurs by comparing fewer elements.
  if (num > 2) {
    notqsort(base + 1, num - 1, len, func);
  }
  // The values near the beginning of the list may need to be sorted a second
  // time.
  if (func(base, base + 1) < 0) {
    swaptwo(base, base + 1);
    notqsort(base + 1, num - 1, len, func);
  }
  return;
}

} // namespace not_qsort
