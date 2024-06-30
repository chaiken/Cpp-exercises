
#include "slist2.hh"

#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

namespace slist {

SingleLinkList::SingleLinkList(const char *s) {
  size_t ctr = strlen(s);
  while (ctr > 0) {
    Prepend(s[ctr - 1]);
    ctr--;
  }
}

size_t SingleLinkList::Length() const {
  size_t ans = 0u;
  SingleLinkListElem *elem = First();
  while (0 != elem) {
    elem = elem->next.get();
    ans++;
  }
  return ans;
}

void SingleLinkList::Prepend(char c) {
  std::unique_ptr<SingleLinkListElem> temp(new SingleLinkListElem(c));
  temp->next = move(h_);
  h_ = move(temp);
}

ostringstream &operator<<(ostringstream &out, const SingleLinkList &sll) {
  SingleLinkListElem *temp = sll.First();
  while (temp != 0) {
    out << temp->data << " -> ";
    temp = temp->next.get();
  }
  out << string("0\n###");
  return out;
}

void SingleLinkList::Delete() {
  if (0 == h_) {
    return;
  }
  unique_ptr<SingleLinkListElem> temp = move(h_);
  h_ = move(temp->next);
  temp.reset();
}

unsigned SingleLinkList::Count(char c) const {
  unsigned count = 0u;
  SingleLinkListElem *elem = First();
  while (0 != elem) {
    if (c == elem->data) {
      count++;
    }
    elem = elem->next.get();
  }
  return count;
}

SingleLinkListElem *SingleLinkList::Last() const {
  // Without this check, we dereference NULL in the loop's test.
  if (0 == h_) {
    return nullptr;
  }
  SingleLinkListElem *elem = h_.get();
  while (0 != elem->next) {
    elem = elem->next.get();
  }
  return elem;
}

void SingleLinkList::Append(SingleLinkList &sll) {
  SingleLinkListElem *elem = Last();
  // If the list to be appended is not empty.
  if (0 != sll.First()) {
    unique_ptr<SingleLinkListElem> newelem =
        make_unique<SingleLinkListElem>(sll.First()->data);
    newelem->next = move(sll.First()->next);
    // If the appended-to list is not empty.
    if (0 != elem) {
      elem->next = move(newelem);
    } else {
      h_ = std::move(newelem);
    }
  }
}

char SingleLinkList::Pop() {
  char c = First()->data;
  Delete();
  return c;
}

} // namespace slist
