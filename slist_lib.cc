
#include "slist.hh"

#include <cstring>
#include <iostream>

using namespace std;

namespace slist {

SingleLinkList::SingleLinkList(const char *s) {
  size_t len = strlen(s);
  if (0u == len) {
    cerr << "Provide at least one character to create a list." << endl;
  } else {
    int i = static_cast<int>(len);
    while (i > 0) {
      unique_ptr<slistelem> elem{(new slistelem(s[i - 1]))};
      elem->next = move(h_);
      h_ = move(elem);
      i--;
    }
  }
}

size_t SingleLinkList::Count(char c) const {
  slistelem *cursor = first();
  size_t result = 0u;
  while (cursor) {
    if (c == cursor->data) {
      result++;
    }
    cursor = cursor->next.get();
  }
  return result;
}

size_t SingleLinkList::Length() const {
  size_t len = 0u;
  slistelem *cursor = first();
  while (cursor) {
    len++;
    cursor = cursor->next.get();
  }
  return len;
}

void SingleLinkList::Prepend(char c) {
  unique_ptr<slistelem> temp{new slistelem(c)};
  temp->next = move(h_);
  h_ = move(temp);
}

slistelem *SingleLinkList::Tail() const {
  slistelem *save = nullptr, *tail = h_.get();
  while (tail) {
    save = tail;
    tail = tail->next.get();
  }
  return save;
}

void SingleLinkList::Append(SingleLinkList &&sll) {
  if (sll.empty()) {
    return;
  }
  slistelem *tail = Tail();
  while (sll.first()) {
    unique_ptr<slistelem> elem{new slistelem(sll.first()->data)};
    sll.Delete();
    tail->next = move(elem);
    tail = tail->next.get();
  }
}

void SingleLinkList::Delete() {
  if (!h_) {
    return;
  }
  unique_ptr<slistelem> temp = move(h_);
  h_ = move(temp->next);
  temp.reset();
}

ostream &operator<<(ostream &out, const SingleLinkList &sll) {
  slistelem *cursor = sll.first();
  while (cursor) {
    out << cursor->data << " -> ";
    cursor = cursor->next.get();
  }
  out << "0" << endl;
  return out;
}

// Tail-recursive version not obviously simpler than the while-loop version.
// Not possible to make this function an extraction operator since it does not
// need an object parameter and cannot return an ostream reference.
void SingleLinkList::Print(slistelem *cursor) const {
  if (!cursor) {
    cout << "0" << endl;
    return;
  }
  cout << cursor->data << " -> ";
  Print(cursor->next.get());
}

// Stack interface
char SingleLinkList::Pop() {
  char ret = first()->data;
  Delete();
  return ret;
}

} // namespace slist
