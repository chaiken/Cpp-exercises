#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;

int values[] = {40, 10, 100, 90, 20, 25};
constexpr int kElements = sizeof(values) / sizeof(int);

// Sign matters, not value.
int compare(const int *a, const int *b) { return (*a - *b); }
int revcompare(const int *a, const int *b) { return -(*a - *b); }

void swaptwo(int *a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}

void print_values(unsigned start) {
  unsigned i = start;
  cout << "{";
  while (i < kElements) {
    cout << " " << values[i] << " ";
    i++;
  }
  cout << "}" << endl;
}

void notqsort(int *base, size_t num, int func(const int *, const int *)) {
  assert(base != 0);
  assert(num > 0u);
  //  unsigned depth = kElements - num;
  //  cout << " depth: " << depth << endl;
  //  print_values(depth);
  //  cout << endl;

  // Sort the first pair.  This is the step that does work.
  if (func(base, base + 1) < 0) {
    swaptwo(base, base + 1);
  }
  // If unsorted material remains at the end, continue.
  // Simplification occurs by comparing fewer elements.
  if (num > 2) {
    notqsort(base + 1, num - 1, func);
  }
  // The values near the beginning of the list may need to be sorted a second
  // time.
  if (func(base, base + 1) < 0) {
    swaptwo(base, base + 1);
    notqsort(base + 1, num - 1, func);
  }
  return;
}

int main(void) {

  notqsort(values, kElements, compare);
  print_values(0u);
  cout << endl;

  notqsort(values, kElements, revcompare);
  print_values(0u);
  cout << endl;

  exit(EXIT_SUCCESS);
}
