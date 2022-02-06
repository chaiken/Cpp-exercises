#ifndef SMARTER_LIST_H
#define SMARTER_LIST_H

#include <iostream>
#include <memory>
#include <vector>

namespace smarter_list {

struct ListNode {
  // This declaration is needed.
  ListNode() = default;
  ListNode(::std::string namestr) : name(namestr) {}
  ListNode(const ListNode *ln);
  // Default did not move next pointer.
  ListNode(ListNode &&ln) : name(ln.name), next(std::move(ln.next)) {}
  ListNode &operator=(ListNode &&ln) = default;
  bool empty() const { return (name.empty() && !next); }

  std::string name;
  std::unique_ptr<ListNode> next;
};

bool operator==(const ListNode &a, const ListNode &b);
bool operator!=(const ListNode &a, const ListNode &b);

class SmarterList {
public:
  SmarterList() : head_(std::unique_ptr<ListNode>()), cursor_(nullptr) {}
  SmarterList(ListNode &ln)
      : head_(std::make_unique<ListNode>(std::move(ln))), cursor_(head_.get()) {
  }
  SmarterList(const SmarterList &sl) = delete;
  SmarterList(SmarterList &&sl) = default;
  SmarterList(::std::vector<::std::string> strvec);

  void Reverse();
  // Reset the list cursor to the head.
  // Only changes cursor_, which is mutable.
  void reset() const { cursor_ = head_.get(); }
  ListNode *current() const { return cursor_; }
  // Retrieve the list head.
  ListNode *begin() const {
    reset();
    return cursor_;
  }
  bool empty() const { return (nullptr == head_); }
  void operator+(const ListNode &ln);
  std::pair<int, ListNode> operator[](const int i);

  // These operators change only cursor_.
  ListNode *operator++() const;
  ListNode *operator--() const;
  friend ::std::ostream &operator<<(::std::ostream &out, const SmarterList &sl);

private:
  std::unique_ptr<ListNode> head_;
  // Mutable cursor_ provides a "view" of the list without changing the
  // contents.
  mutable ListNode *cursor_;
  void Prepend(std::unique_ptr<ListNode> ln);
};

bool operator==(const SmarterList &a, const SmarterList &b);
bool operator!=(const SmarterList &a, const SmarterList &b);
::std::ostream &operator<<(::std::ostream &out, const SmarterList &sl);

} // namespace smarter_list
#endif
