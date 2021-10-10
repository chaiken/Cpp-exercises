#include <cstring>

#include "dyn_string.h"

using namespace std;

namespace dyn_string {

DynString::DynString(char *p) : len_(strlen(p)), s_(new char[strlen(p) + 1]) {
  /*
    The  strncpy()  function is similar, except that at most n bytes of src
     are copied.  Warning: If there is no null byte among the first n  bytes
     of src, the string placed in dest will not be null-terminated.
  */
  strncpy(s_.get(), p, len_ + 1u);
  assert(len_ == strlen(s_.get()));
}

DynString::DynString(const char *p)
    : len_(strlen(p)), s_(new char[strlen(p) + 1]) {
  strncpy(s_.get(), p, strlen(p) + 1u);
  assert(len_ == strlen(s_.get()));
}

DynString::DynString(DynString &&str) : len_(str.len_), s_(str.s_.release()) {}

DynString &DynString::operator=(const DynString &str) {
  if (&str != this) {
    this->s_ = make_unique<char[]>(str.len_ + 1u);
    strncpy(this->s_.get(), str.s_.get(), str.len_ + 1u);
    this->len_ = str.len_;
  }
  return *this;
}

void DynString::concat(const DynString &a, const DynString &b) {
  if (0u == b.len_) {
    *this = a;
    return;
  }
  if (0u == a.len_) {
    *this = b;
    return;
  }
  this->len_ = a.len_ + b.len_;
  this->s_ = make_unique<char[]>(this->len_ + 1u);
  // The strncpy() function is similar, except that at most n bytes of
  // src are copied.  Warning: If there is no null byte among the
  // first n bytes of src, the string placed in dest will not be
  // null-terminated.
  strncpy(this->s_.get(), a.contents(), a.len_ + 1u);
  // temp is null-terminated.
  assert(a.len_ == strlen(this->s_.get()));
  // The strcat() function appends the src string to the dest string,
  // overwriting the terminating null byte ('\0') at the end of dest,
  // and then adds a terminating null byte. If src contains n or more
  // bytes, strncat() writes n+1 bytes to dest (n from src plus the
  // terminating null byte).  Therefore, the size of dest must be at
  // least strlen(dest)+n+1.
  strncat(this->s_.get(), b.contents(), b.len_);
  assert(this->len_ == strlen(this->s_.get()));
}

int DynString::compare(const DynString &a) const {
  unsigned i = 0u;
  while ((a.s_.get()[i] == contents()[i]) && (a.s_.get()[i] != '\0')) {
    i++;
  }
  // Got to the end of both strings, and all chars matched.
  if ((len_ == i) && (a.len_ == len_)) {
    return 0;
  }
  // Evaluates to true if a.s_ is NULL.  Makes sense since NUL is ASCII 0x0.
  if (contents()[i] > a.s_.get()[i]) {
    return 1;
  }
  // (contents()[i] < a.s_[i])
  return -1;
}

void DynString::reverse() {
  if (0 == len_) {
    return;
  }
  assert(len_ == strlen(s_.get()));
  unsigned i = 0u;
  while (i < len_ / 2u) {
    char keep = s_.get()[i];
    // Subtract 1 in order to skip the NULL on reversal.
    s_.get()[i] = s_.get()[(len_ - 1u) - i];
    s_.get()[(len_ - 1u) - i] = keep;
    i++;
  }
  // Add it to the new end.
  s_.get()[len_] = '\0';
  assert(len_ == strlen(s_.get()));
}

void DynString::swap(DynString &str) {
  if (str == *this) {
    return;
  }
  DynString a(str.s_.get());
  str = *this;
  *this = a;
}

bool operator==(const DynString &a, const DynString &b) {
  return (0 == strcmp(a.s_.get(), b.s_.get()));
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
  out << a.s_.get();
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
