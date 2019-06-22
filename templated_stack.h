#ifndef TEMPLATED_STACK_H
#define TEMPLATED_STACK_H

#include <cassert>
#include <iostream>

namespace templated_stack {

template <typename T, int N> class TemplatedStack;

template <typename T, int N> class TemplatedStack {
private:
  enum { EMPTY = -1 };
  int max_len_;
  int top_;
  T *data_;

public:
  explicit TemplatedStack() : max_len_(N), top_(EMPTY), data_(new T[N]) {}
  TemplatedStack(T(&&input)[], int val);
  ~TemplatedStack() { delete[] data_; }
  void reset() { top_ = EMPTY; }
  void push(T x) { data_[++top_] = x; }
  T pop() { return data_[top_--]; }
  T top_of() const { return data_[top_]; };
  bool empty() const { return (top_ == EMPTY); }
  bool full() const { return (top_ == (max_len_ - 1)); }
  // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Making_New_Friends
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const TemplatedStack<T, N> &ts) {
    int i = ts.top_;
    assert(0 != ts.data_);
    while (i >= 0) {
      out << ts.data_[i];
      if (0 != i) {
        out << ", ";
      }
      i--;
    }
    return out;
  }
};

} // namespace templated_stack

#include "templated_stack_impl.h"

#endif
