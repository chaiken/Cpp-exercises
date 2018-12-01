#include "smarter_stack.h"

#include <cassert>
#include <cerrno>

#include <iostream>
#include <ostream>

using namespace std;

namespace smarter_stack {

ostream &operator<<(ostream &out, const SmarterStack &stack) {
  for (int i = 0; i < stack.top_; i++) {
    out << stack.data_[i];
  }
  out << endl;
  return out;
}

double SmarterStack::operator[](int i) {
  assert((i < top_) && (i >= 0));
  return data_[i];
}

double SmarterStack::operator[](int i) const {
  assert((i < top_) && (i >= 0));
  return data_[i];
}

bool operator==(SmarterStack &a, SmarterStack &b) {
  if (a.size_ != b.size_) {
    return false;
  }
  int i = 0;
  while (i < a.size_) {
    if (a[i] != b[i]) {
      return false;
    }
    i++;
  }
  return true;
}

bool operator==(const SmarterStack &a, const SmarterStack &b) {
  if (a.size_ != b.size_) {
    return false;
  }
  int i = 0;
  while (i < a.size_) {
    if (a[i] != b[i]) {
      return false;
    }
    i++;
  }
  return true;
}

void SmarterStack::Push(double datum) {
  if (full()) {
    assert_perror(ENOSPC);
  }
  // top_ is greater than the index of the the last element.
  data_[top_++] = datum;
}

double SmarterStack::Pop() {
  if (empty()) {
    assert_perror(EINVAL);
  }
  // The last element that can be popped is at index 0.
  return (data_[--top_]);
}

void SmarterStack::Reverse() {
  SmarterStack temp(size_);
  int i = 0;
  while (!empty()) {
    temp.Push(data_[i]);
    Pop();
    i++;
  }
  assert(temp.full());
  assert(empty());
  while (!temp.empty()) {
    Push(temp.Pop());
  }
}

} // namespace smarter_stack
