#include <cstring>

#include "dyn_string.h"

using namespace std;

namespace dyn_string {

DynString::DynString(const char *p) {
  len_ = strlen(p);
  // In C:
  // man strlen: The strlen() function calculates the length of the string
  // pointed to by s, excluding the terminating null byte ('\0').
  // The following line therefore needs a '1'.
  // man strcpy: The strcpy() function copies the string pointed to by src,
  // including the terminating null byte ('\0'), to the buffer pointed to by
  // dest.
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strcpy(s_, p);
  //  cout << "Char* constructor this: " << this << " with s_ " << dec << s_
  //       << endl;
}

DynString::DynString(const DynString &str) : len_(str.len_) {
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strcpy(s_, str.s_);
  // cout << "Copy constructor this: " << this << " with s_ " << s_ << endl;
}

DynString &DynString::operator=(const DynString &str) {
  if (&str != this) {
    delete[] s_;
    len_ = str.len_;
    s_ = new char[len_ + 1];
    assert(s_ != 0);
    strcpy(s_, str.s_);
  }
  return *this;
}

void DynString::concat(const DynString &a, const DynString &b) {
  // DynString default constructor creates a string containing NULL, of strlen
  // 0u.
  if (0u == b.len_) {
    *this = a;
    return;
  }
  if (0u == a.len_) {
    *this = b;
    return;
  }
  // New string needs only one trailing NULL.
  char *temp = new char[a.len_ + b.len_ + 1];
  len_ = a.len_ + b.len_;
  // The  strcat()  function appends the src string to the dest string,
  // overwriting the terminating null byte ('\0') at the end of dest, and then
  // adds a terminating  null byte. Therefore strcpy() followed by strcat()
  // doesn't leave a NULL in the middle.
  strcpy(temp, a.s_);
  strcat(temp, b.s_);
  delete[] s_;
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strcpy(s_, temp);
  delete[] temp;
}

// Using the copy constructor to create new DynString objects and then
// incrementing the string pointers inside
// the DynStrings for the char-by-char comparison didn't work, as the strings
// were changed when the runtime tried to free them, resulting in "free():
// invalid pointer" error".   See double-free_dyn_string_lib.txt.  To make the
// copy-constructor approach work, it would be necessary to explicitly free a
// copy of the initial char* pointer, perhaps with an overloaded destructor.
int DynString::compare(const DynString &a) const {
  char b[len_ + 1], c[a.len_ + 1];
  assert(s_ != 0);
  assert(strlen(s_) == len_);
  strcpy(b, s_);
  strcpy(c, a.s_);

  unsigned i = 0u;
  while ((b[i] == c[i]) && (b[i] != '\0')) {
    i++;
  }
  // Got to the end of both strings, and all chars matched.
  if ((len_ == i) && (a.len_ == len_)) {
    return 0;
  }
  // Evaluates to true if c.s_ is NULL.  Makes sense since NUL is ASCII 0x0.
  if (b[i] > c[i]) {
    return 1;
  }
  // (b[i] < c[i])
  return -1;
}

void DynString::reverse() {
  if (0 == len_) {
    return;
  }
  assert(len_ == strlen(s_));

  // Leave room for NULL; a string of length n needs n+1 bytes.
  char temp[len_ + 1];
  strcpy(temp, s_);
  delete[] s_;
  s_ = new char[len_ + 1];
  assert(s_ != 0);

  unsigned i = 0u;
  while (i < len_) {
    // Skip the NULL on reversal.
    s_[i] = temp[(len_ - 1) - i];
    i++;
  }
  // Add it to the new end.
  s_[len_] = '\0';
  assert(len_ == strlen(s_));
}

/* void DynString::print(size_t n) const {
size_t ctr = 0u;
while (ctr < n) {
  cout << *(s_ + ctr);
  ctr++;
}
//  cout << endl;
} */

void DynString::swap(DynString &str) {
  if (str == *this) {
    return;
  }
  DynString a(str);
  str = *this;
  *this = a;
}

bool operator==(const DynString &a, const DynString &b) {
  return (0 == strcmp(a.s_, b.s_));
}

// Cannot be a member function, as it acts on a list of objects, and is
// therefore not associated with a particular one.
// We don't have to pass in a function pointer, as compare() is a member
// function of the class of objects to which is applied.
void dyn_string_sort(DynString *dynstr, size_t cursor, size_t len) {
  assert(dynstr != 0);
  if (cursor < 2u) {
    return;
  }

  // unsigned depth = len - cursor;
  // cout << " depth: " <<  depth << endl;
  // unsigned i = depth;
  // while (i < len) {
  //    (dynstr+i)->print();
  //    i++;
  //  }
  // cout << endl;

  // Note that all these functions add and subtract 1 rather than incrementing
  // or decrementing.
  if (cursor > 2) {
    // Work done on the way down to the end of the array.
    if (dynstr->compare(*(dynstr + 1)) > 0) {
      dynstr->swap(*(dynstr + 1));
    }
    // Descend to the bottom.
    dyn_string_sort(dynstr + 1, cursor - 1, len);
  }
  // If upon return from previous sort, the array is
  // disordered, descend again and fix until there's
  // no more work.
  // Work done on the way back up from the end of the array.
  if (dynstr->compare(*(dynstr + 1)) > 0) {
    dynstr->swap(*(dynstr + 1));
    dyn_string_sort(dynstr + 1, cursor - 1, len);
  }
  return;
}

std::ostream &operator<<(std::ostream &out, const DynString &a) {
  out << a.s_;
  return out;
}

std::ostream &print_some(std::ostream &out, const DynString &a, size_t n = 0) {
  if (!((0u == n) || a.empty())) {
    size_t bound = (n <= a.len()) ? n : a.len();
    size_t ctr = 0u;
    while (ctr < bound) {
      out << a.s_[ctr];
      ctr++;
    }
    // Apparently std::ostringstream automagically adds the trailing null.
    // out << '0';
  }
  return out;
}

} // namespace dyn_string
