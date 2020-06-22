
#include "slist.h"

#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

namespace slist {

SingleLinkList::SingleLinkList(const char *s) {
  h_ = 0;
  size_t len = strlen(s);
  if (0u == len) {
    cerr << "Provide at least one character to create a list." << endl;
    assert_perror(EINVAL);
  }
  int i = static_cast<int>(len);
  while (i > 0) {
    slistelem *elem = new slistelem;
    elem->data = s[i - 1];
    elem->next = h_;
    h_ = elem;
    i--;
  }
}

unsigned SingleLinkList::Count(char c) const {
  slistelem *cursor = first();
  unsigned result = 0u;
  while (cursor != 0) {
    if (c == cursor->data) {
      result++;
    }
    cursor = cursor->next;
  }
  return result;
}

size_t SingleLinkList::Length() const {
  size_t len = 0u;
  slistelem *cursor = first();
  while (cursor != 0) {
    len++;
    cursor = cursor->next;
  }
  return len;
}

void SingleLinkList::Prepend(char c) {
  slistelem *temp = new slistelem;
  assert(temp != 0);
  temp->next = h_;
  temp->data = c;
  h_ = temp;
}

slistelem *SingleLinkList::Tail() const {
  slistelem *save = 0, *tail = h_;
  while (0 != tail) {
    save = tail;
    tail = tail->next;
  }
  return save;
}

void SingleLinkList::Append(SingleLinkList &&sll) {
  if (0u == sll.Length()) {
    cerr << "Will not append zero-length string." << endl;
    assert_perror(EINVAL);
  }
  slistelem *tail = Tail();
  while (0 != sll.first()) {
    // Copy the appended list's data.
    slistelem *elem = new slistelem;
    assert(0 != elem);
    elem->data = sll.first()->data;
    // Done with this element.
    sll.Delete();
    // Place the new element at the end of this list.
    tail->next = elem;
    elem->next = 0;
    // The end of this list is now the new element.
    tail = elem;
  }
}

void SingleLinkList::Delete() {
  if (0 == h_) {
    return;
  }
  slistelem *temp = h_;
  h_ = h_->next;
  delete temp;
}

ostream &operator<<(ostream &out, const SingleLinkList &sll) {
  slistelem *temp = sll.first();
  while (temp != 0) {
    out << temp->data << " -> ";
    temp = temp->next;
  }
  out << "0" << endl;
  return out;
}

// Tail-recursive version not obviously simpler than the while-loop version.
// Not possible to make this function an extraction operator since it does not
// need an object parameter and cannot return an ostream reference.
void SingleLinkList::Print(slistelem *cursor) const {
  if (0 == cursor) {
    cout << "0" << endl;
    return;
  }
  cout << cursor->data << " -> ";
  Print(cursor->next);
}

void SingleLinkList::Release() {
  while (h_ != 0) {
    Delete();
  }
}

// Stack interface
char SingleLinkList::Pop() {
  char ret = first()->data;
  Delete();
  return ret;
}

} // namespace slist
