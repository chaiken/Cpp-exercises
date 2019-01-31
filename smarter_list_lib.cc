#include "smarter_list.h"

#include <cassert>

#include <iostream>

using namespace std;

namespace smarter_list {

// Callers of the function must check that the return value is not null and
// refrain from calling the function again if it is.
const ListNode *SmarterList::operator++() const {
  assert(nullptr != head_);
  static ListNode *current = head_;
  current = cursor_->next;
  cursor_ = current;
  return (cursor_);
}

ostream &operator<<(ostream &out, SmarterList &sl) {
  sl.reset();
  for (const ListNode *ln = sl.begin(); ln != nullptr; ln = ++sl) {
    out << ln->name << ", ";
  }
  out << endl;
  return out;
}

ListNode &SmarterList::operator[](int i) {
  assert(i >= 0);
  if (nullptr == head_) {
    assert_perror(ENODATA);
  }
  ListNode *ln = head_;
  int j = 0;
  while ((nullptr != ln) && (j < i)) {
    ln = ln->next;
    j++;
  }
  return *ln;
}

} // namespace smarter_list
