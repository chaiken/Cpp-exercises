#ifndef SMARTER_LIST_H
#define SMARTER_LIST_H

#include <cassert>
#include <vector>

#include <iostream>

namespace smarter_list {

class ListNode {
public:
  ListNode() : name(""), next(nullptr) {}
  ListNode(::std::string namestr) : name(namestr), next(nullptr) {}
  ListNode(const ListNode *ln) : name(ln->name), next(nullptr) {}
  ListNode &operator=(const ListNode &ln) {
    name = ln.name;
    next = ln.next;
    return *this;
  }
  ::std::string name;
  ListNode *next;
};

class SmarterList {
public:
  SmarterList() : head_(nullptr) {}
  // String vector constructor.
  SmarterList(::std::vector<::std::string> strvec) {
    assert(!strvec.empty());
    head_ = nullptr;
    ListNode *previous = nullptr;
    for (auto it = strvec.begin(); it != strvec.end(); it++) {
      ListNode *node = new ListNode(*it);
      // mark as "unlikely()"?
      if (it == strvec.begin()) {
        head_ = node;
      } else {
        previous->next = node;
      }
      previous = node;
    }
    cursor_ = head_;
    assert(nullptr != head_);
    assert(nullptr != head_->next);
  }
  // Copy constructor.  The constructor could save the node number and provide
  // that for bounds checking of the index operator[], but that's not a common
  // list idiom.
  SmarterList(const SmarterList &sl) : head_(nullptr) {
    if (nullptr != sl.head_) {
      head_ = new ListNode(sl.head_);
      assert(nullptr != head_);
      head_->next = sl.head_->next;

      ListNode *current = head_;
      // Only changes cursor_, which is mutable.
      sl.reset();
      while (sl.cursor_->next) {
        ListNode *ln = new ListNode(*(sl.cursor_->next));
        assert(nullptr != ln);
        current->next = ln;
        current = current->next;
        // Only changes cursor_, which is mutable.
        ++sl;
      }
    }
    cursor_ = head_;
  }
  ~SmarterList() {
    assert(nullptr != head_);
    ListNode *node = head_, *next = head_;
    while (nullptr != node) {
      next = node->next;
      delete node;
      node = next;
    }
    head_ = nullptr;
  }
  // Reset the list cursor to the head.
  // Only changes cursor_, which is mutable.
  void reset() const { cursor_ = head_; }
  // Retrieve the list head.
  ListNode *begin() {
    reset();
    assert(nullptr != head_);
    return cursor_;
  }
  bool empty() { return (nullptr == head_); }
  SmarterList &operator+(const ListNode &ln);
  SmarterList &operator--();
  ListNode &operator[](int i);

  // Only changes cursor_, which is mutable.
  const ListNode *operator++() const;
  friend ::std::ostream &operator<<(::std::ostream &out, SmarterList &sl);

private:
  ListNode *head_;
  mutable ListNode *cursor_;
};

::std::ostream &operator<<(::std::ostream &out, const SmarterList &sl);

} // namespace smarter_list
#endif
