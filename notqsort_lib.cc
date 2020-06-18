using namespace std;

#include "notqsort.h"

namespace not_qsort {

namespace {
void swaptwo(int *a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}
} // namespace

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

// We can make base a void* and pass size_t size into it,
// but base += size is illegal without first casting base
// to a type.  Not only compare() but print_values() would
// need to be passed in as a different function pointer for
// each type.  The right way to solve this problem in C++ is
// with a template.
void notqsort(int *base, size_t num, size_t len,
              int func(const int *, const int *)) {
  assert(base != 0);
  if (num < 2u) {
    return;
  }
  // unsigned depth = len - num;
  // cout << " depth: " << len - num << endl;
  // print_values(base, depth, len);
  // cout << endl;

  // Simplification occurs by comparing fewer elements.
  // Process last two elements, then last 3 . . .
  if (num > 2u) {
    notqsort(base + 1, num - 1, len, func);
  }
  if (func(base, base + 1) < 0) {
    // Sort the pair at the cursor.  This is the step that does all the work.
    swaptwo(base, base + 1);
    // May have to resort the end of the array based on changes at the
    // beginning.
    notqsort(base + 1, num - 1, len, func);
  }
  return;
}

} // namespace not_qsort
