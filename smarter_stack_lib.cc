#include "smarter_stack.h"

#include <cassert>
#include <cerrno>

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

} // namespace smarter_stack
