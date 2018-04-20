#include <cstring>

#include "dyn_string.h"

namespace dyn_string {

DynString::DynString(const char *p) {
  len_ = strlen(p);
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strncpy(s_, p, len_);
}

DynString::DynString(const DynString &str) : len_(str.len_) {
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strncpy(s_, str.s_, len_);
}

void DynString::assign(const DynString &str) {
  if (&str == this) {
    return;
  }
  delete[] s_;
  len_ = str.len_;
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strncpy(s_, str.s_, len_);
}

void DynString::concat(const DynString &a, const DynString &b) {
  char *temp = new char[a.len_ + b.len_ + 1];
  len_ = a.len_ + b.len_;
  strncpy(temp, a.s_, a.len_);
  strncat(temp, b.s_, b.len_);
  delete[] s_;
  s_ = new char[len_ + 1];
  assert(s_ != 0);
  strncpy(s_, temp, a.len_ + b.len_ + 1);
}

bool DynString::equals(const DynString &a) {
  if (!strcmp(a.s_, s_)) {
    return true;
  }
  return false;
}
}
