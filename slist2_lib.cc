
#include "slist2.h"

#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

namespace slist {

SingleLinkList::SingleLinkList(const char *s) {
  h_ = 0;
  const size_t len = strlen(s);
  assert(len > 0u);
  int i = static_cast<int>(len);
  while (i > 0) {
    Prepend(s[i - 1]);
    i--;
  }
}

size_t SingleLinkList::Length() const {
  size_t ans = 0u;
  SingleLinkListElem *elem = h_;
  while (0 != elem) {
    // Friend class accesses private data.
    elem = elem->next_;
    ans++;
  }
  return ans;
}

void SingleLinkList::Prepend(char c) {
  SingleLinkListElem *temp = new SingleLinkListElem(c, this);
  assert(0 != temp);
  h_ = temp;
}

void SingleLinkList::Delete() {
  if (0 == h_) {
    return;
  }
  SingleLinkListElem *temp = h_;
  h_ = h_->next_;
  delete temp;
}

void SingleLinkList::Print() const {
  SingleLinkListElem *temp = h_;
  while (temp != 0) {
    cout << temp->data_ << " -> ";
    temp = temp->next_;
  }
  cout << "0\n###" << endl;
}

void SingleLinkList::Release() {
  while (h_ != 0) {
    Delete();
  }
}

unsigned SingleLinkList::Count(char c) const {
  unsigned count = 0u;
  SingleLinkListElem *elem = h_;
  while (0 != elem) {
    if (c == elem->data_) {
      count++;
    }
    elem = elem->next_;
  }
  return count;
}

SingleLinkListElem *SingleLinkList::Tail() const {
  // Without this check, we dereference NULL in the loop's test.
  if (0 == h_) {
    return h_;
  }
  SingleLinkListElem *elem = h_;
  while (0 != elem->next_) {
    elem = elem->next_;
  }
  return elem;
}

void SingleLinkList::Append(SingleLinkList &sll) {
  SingleLinkListElem *elem = Tail();
  while (0 != sll.first()) {
    SingleLinkListElem *newelem = new SingleLinkListElem(sll.first()->data_);
    assert(0 != newelem);
    if (0 != elem) {
      elem->next_ = newelem;
    } else {
      h_ = newelem;
    }
    elem = newelem;
    elem->next_ = 0;
    sll.Delete();
  }
}

char SingleLinkList::Pop() {
  char c = first()->data_;
  Delete();
  return c;
}

} // namespace slist
