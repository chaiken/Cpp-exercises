#include "smarter_list.h"

#include <cassert>

#include <iostream>
// #include <memory>

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

SmarterList &SmarterList::operator+(const ListNode &ln) {
  // Does this violate RAII?   If so, what is an RAII way to accomplish the same
  // thing?  The two lines below result in SEGV.
  // unique_ptr<ListNode> newln = make_unique<ListNode>(ln);
  // cursor_->next = newln.get();
  ListNode *newln = new ListNode(ln);
  // No need to reset(), as we want the last node anyway.
  if (nullptr != head_) {
    while (nullptr != cursor_->next) {
      cursor_ = cursor_->next;
    }
    cursor_->next = newln;
  } else {
    head_ = newln;
  }
  // Should be processed by copy constructor.  That seems inefficient, but how
  // else to do it?
  return *this;
}

SmarterList &SmarterList::operator--() {
  if (!empty()) {
    // In case the list contains only one element, we need to initialize save.
    ListNode *save = head_;
    save->next = head_->next;
    // No need to reset(), as it's okay if cursor_ is at list end.
    while (nullptr != cursor_->next) {
      save = cursor_;
      cursor_ = cursor_->next;
    }
    // Does this violate RAII?   If so, what is an RAII way to accomplish the
    // same thing?
    delete cursor_;
    save->next = nullptr;
    // So cursor_ is not left NULL for next caller.
    reset();
  }
  return *this;
}

} // namespace smarter_list
