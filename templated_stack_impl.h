#ifndef TEMPLATED_STACK_IMPL_H
#define TEMPLATED_STACK_IMPL_H

#include <cassert>
#include <cstdio>

// Any functions that refer explicitly to the template parameter must live here.
namespace templated_stack {

// Move constructor.
template <typename T, int N>
TemplatedStack<T, N>::TemplatedStack(T(&&input)[], int val)
    : max_len_(val), top_(0), data_(new T[val]) {
  assert(0 != data_);
  assert(val > 0);
  int i = 0;
  while (i < val) {
    data_[i] = input[i];
    i++;
  }
  top_ = val - 1;
}

} // namespace templated_stack
#endif
