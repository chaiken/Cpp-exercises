#ifndef NOTQSORT_LIB_H_
#define NOTQSORT_LIB_H_

#include <cassert>
#include <cstdlib>
#include <iostream>

namespace not_qsort {

// base is an advancing pointer into the array to be sorted.
// num is the number of elements in the remaining unsorted portion.
// len is the total length. Initially num == len.
void notqsort(int *base, size_t num, size_t len,
              int func(const int *, const int *));
// start is the ordinal number of the first value in the unsorted portion.
void print_values(int *base, unsigned start, unsigned len);
int compare(const int *a, const int *b);
int revcompare(const int *a, const int *b);
}

#endif
