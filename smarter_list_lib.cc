#include "smarter_list.h"

#include <cassert>
#include <errno.h>

using namespace std;

namespace smarter_list {

// The explicit definition is needed because the compiler always pick this ctor
// rather than the default when no arguments are provided.  Is this due somehow
// to Most Vexing Parse?
ListNode::ListNode(const ListNode *ln) {
  if (ln) {
    name.assign(ln->name);
  }
}

bool operator==(const ListNode &a, const ListNode &b) {
  return (a.name == b.name);
}

bool operator!=(ListNode &a, ListNode &b) { return !(a == b); }

void SmarterList::Prepend(unique_ptr<ListNode> ln) {
  // The default ListNode ctor initializes the name string to empty.  Therefore
  // it's necessary to define a ListNode with an empty name and a nullptr next
  // pointer as empty and then write over it.
  if (!head_ || head_->empty()) {
    ln->next = unique_ptr<ListNode>();
  } else {
    ln->next = move(head_);
  }
  head_ = move(ln);
}

void SmarterList::Reverse() {
  if (empty() || !head_->next) {
    return;
  }
  unique_ptr<ListNode> cursor = move(head_);
  unique_ptr<ListNode> cursor_next;
  while (cursor) {
    cursor_next = move(cursor->next);
    Prepend(move(cursor));
    cursor = move(cursor_next);
  }
}

SmarterList::SmarterList(::std::vector<::std::string> strvec) {
  // Explicitly initializing with a nullptr
  //     head_ = std::make_unique<ListNode>(nullptr);
  // results in a runtime error.
  head_ = std::make_unique<ListNode>();
  for (auto it = strvec.begin(); it != strvec.end(); it++) {
    Prepend(std::make_unique<ListNode>(*it));
  }
  Reverse();
  cursor_ = head_.get();
}

// Callers of the function must check that the return value is not null and
// refrain from calling the function again if it is.
ListNode *SmarterList::operator++() const {
  if (!empty() && cursor_) {
    cursor_ = cursor_->next.get();
  } else {
    cursor_ = nullptr;
  }
  return cursor_;
}

ListNode *SmarterList::operator--() const {
  if (!cursor_) {
    return nullptr;
  }
  if (empty() || (*cursor_ == *(head_.get()))) {
    cursor_ = nullptr;
    return nullptr;
  }
  ListNode *previous = head_.get();
  while (previous && previous->next.get() != cursor_) {
    previous = previous->next.get();
  }
  cursor_ = previous;
  return cursor_;
}

bool operator==(const SmarterList &a, const SmarterList &b) {
  if ((!a.empty() && b.empty()) || (!b.empty() && a.empty())) {
    return false;
  }
  a.reset();
  b.reset();
  while (nullptr != a.current()) {
    if (nullptr == b.current()) {
      return false;
    }
    if (*a.current() != *b.current()) {
      return false;
    }
    ++a;
    ++b;
  }
  // b has one more ListNode.
  if (nullptr != b.current()) {
    return false;
  }
  return true;
}

bool operator!=(const SmarterList &a, const SmarterList &b) {
  return !(a == b);
}

ostream &operator<<(ostream &out, const SmarterList &sl) {
  sl.reset();
  for (const ListNode *ln = sl.begin(); ln != nullptr; ln = ++sl) {
    out << ln->name << ", ";
  }
  out << endl;
  return out;
}

// Note that the ListNodes returned in the pairs must be moved even though they
// are anonymous and not available outside the pairs. Failed with error about
// deleted ListNode copy ctor when the default move ctor was used.
pair<int, ListNode> SmarterList::operator[](const int i) {
  if (empty() || (i < 0)) {
    // While moving the empyt ListNode makes the function compile, trying to
    // access it causes a runtime error. The problem is that the compiler
    // chooses ListNode(smarter_list::ListNode const*) over the default ctor.
    return make_pair(ENODATA, move(ListNode()));
  }
  int j = 0;
  // Without resetting the cursor_ to head_, the behavior is unpredictable. This
  // is puzzling, as the ctors initialized the value of cursor_.
  reset();
  while (j != i) {
    this->operator++();
    j++;
    if (nullptr == current()) {
      return make_pair(ENODATA, move(ListNode()));
    }
  }
  return make_pair(0, move(ListNode(current()->name)));
}

// It's also possible to make the input non-const and move() it, but the next
// data member will get overwritten anyway.
// Would be better called Postpend().
void SmarterList::operator+(const ListNode &ln) {
  if (nullptr == head_) {
    Prepend(std::make_unique<ListNode>(ListNode(ln.name)));
    return;
  }
  // No need to reset(), as we want the last node anyway.
  while (nullptr != cursor_->next) {
    cursor_ = cursor_->next.get();
  }
  cursor_->next = std::make_unique<ListNode>(ListNode(ln.name));
}

} // namespace smarter_list
