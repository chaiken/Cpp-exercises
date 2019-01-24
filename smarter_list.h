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
  // Copy constructor.
  SmarterList(const SmarterList &sl) : head_(nullptr) {
    if (nullptr != sl.head_) {
      head_ = new ListNode(sl.head_);
      assert(nullptr != head_);

      ListNode *current = head_;
      while (current->next) {
        ListNode *ln = new ListNode(current->next);
        current->next = ln;
        current = current->next;
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
  void reset() { cursor_ = head_; }
  // Retrieve the list head.
  ListNode *begin() {
    reset();
    assert(nullptr != head_);
    return cursor_;
  }
  ListNode &operator[](int i);

  const ListNode *operator++();
  friend ::std::ostream &operator<<(::std::ostream &out, SmarterList &sl);

private:
  ListNode *head_;
  ListNode *cursor_;
};

::std::ostream &operator<<(::std::ostream &out, const SmarterList &sl);

} // namespace smarter_list
#endif
