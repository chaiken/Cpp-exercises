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

void DynString::assign(const DynString &str) {
  if (&str == this) {
    return;
  }
  delete[] s_;
  len_ = str.len_;
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strcpy(s_, str.s_);
}

void DynString::concat(const DynString &a, const DynString &b) {
  // DynString default constructor creates a string containing NULL, of strlen
  // 0u.
  if (0u == b.len_) {
    assign(a);
    return;
  }
  if (0u == a.len_) {
    assign(b);
    return;
  }
  char *temp = new char[a.len_ + b.len_ + 1];
  len_ = a.len_ + b.len_;
  // Intentionally omit trailing NULL on prefix.
  // The strncpy() function is similar, except that at most n bytes of src are
  // copied.  Warning:  If  there  is  no  null  byte among the first n bytes of
  // src, the string placed in dest will not be null-terminated.
  strncpy(temp, a.s_, a.len_);
  // As with strcat(), the resulting string in dest is always null-terminated.
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
  char *b = new char[len_ + 1], *c = new char[a.len_ + 1];
  unsigned i = 0u;
  strcpy(b, s_);
  strcpy(c, a.s_);
  while ((b[i] == c[i]) && (b[i] != '\0')) {
    i++;
  }
  // Got to the end of both strings, and all chars matched.
  if ((i == len_) && (i == a.len_)) {
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
  unsigned i = 0u;
  char temp[len_];
  strcpy(temp, s_);
  delete[] s_;
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  while (i < len_) {
    s_[i] = temp[len_ - (i + 1u)];
    i++;
  }
}

void DynString::print(size_t n) const {
  size_t ctr = 0u;
  while (ctr < n) {
    cout << *(s_ + ctr);
    ctr++;
  }
  //  cout << endl;
}

void DynString::swap(DynString &str) {
  char temp[len_ + 1];
  strcpy(temp, s_);

  assign(str);
  str.assign(temp);
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

} // namespace dyn_string
