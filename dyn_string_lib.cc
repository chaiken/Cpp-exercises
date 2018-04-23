#include <cstring>

#include "dyn_string.h"

#define _RET_IP_ (unsigned long)__builtin_return_address(0)

using namespace std;

namespace dyn_string {

DynString::DynString(const char *p) {
  len_ = strlen(p);
  s_ = new char[len_];
  assert(s_ != 0);
  strcpy(s_, p);
  //  cout << "Char* constructor this: " << this << " with s_ " << dec << s_
  //       << endl;
}

DynString::DynString(const DynString &str) : len_(str.len_) {
  s_ = new char[len_];
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
  s_ = new char[len_];
  assert(s_ != 0);
  strcpy(s_, str.s_);
}

void DynString::concat(const DynString &a, const DynString &b) {
  char *temp = new char[a.len_ + b.len_];
  len_ = a.len_ + b.len_;
  strcpy(temp, a.s_);
  strcat(temp, b.s_);
  delete[] s_;
  s_ = new char[len_];
  assert(s_ != 0);
  strcpy(s_, temp);
}

// Using the copy constructor to create new DynString objects and then
// incrementing the string pointers inside
// the DynStrings for the char-by-char comparison didn't work, as the strings
// were changed when the runtime tried to free them, resulting in "free():
// invalid pointer" error".   See double-free_dyn_string_lib.txt.  To make the
// copy-constructor approach work, it would be necessary to explicitly free a
// copy of the initial char* pointer, perhaps with an overloaded destructor.
int DynString::compare(const DynString &a) const {
  char *b = new char[len_], *c = new char[a.len_];
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
}
