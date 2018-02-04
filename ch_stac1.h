#include <iostream>

namespace {
constexpr int kMaxLen = 30, kEmpty = -1, kFull = kMaxLen - 1;
}

class CharStack {
public:
  void reset() { top_ = kEmpty; }
  void Push(char c);
  void PushMultiple(int m, const char s1[]);
  void Reverse();
  void Print();
  char Pop();
  void PopMultiple(int m, char s1[]);
  char top_of() const { return s_[top_]; }
  bool empty() const { return (top_ == kEmpty); }
  bool full() const { return (top_ == kFull); }

private:
  char s_[kMaxLen];
  int top_;
};

char CharStack::Pop() {
  if (empty()) {
    ::std::cerr << "Stack underflow." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  return s_[top_--];
}

void CharStack::Push(char c) {
  if (full()) {
    ::std::cerr << "Stack overflow." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  top_++;
  s_[top_] = c;
}

void CharStack::PushMultiple(int m, const char s1[]) {
  if (strlen(s1) < m) {
    ::std::cerr << "PushMultiple: provided string is too short." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  // No trailing NULL.
  if ((m + top_) > kFull) {
    ::std::cerr << "PushMultiple: provided string is too long." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  int i = 0;
  while (i < m) {
    Push(s1[i++]);
  }
}

void CharStack::PopMultiple(int m, char s1[]) {
  // strlen(s1) returns 0, as it is empty.
  if (m > (kMaxLen - 1)) {
    ::std::cerr << "PopMultiple: provided string is too short." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  if ((top_ - m) <= kEmpty) {
    ::std::cerr << "PopMultiple: provided string is too long." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  int i = 0;
  while (i < m) {
    s1[i++] = Pop();
  }
  s1[m] = '\0';
}

void CharStack::Reverse() {
  char temp[kMaxLen];
  int i = 0;
  size_t original_len = strlen(s_);
  while (!empty()) {
    temp[i++] = Pop();
  }
  strncpy(s_, temp, original_len);
}

void CharStack::Print() {
  int j = 0;
  while (j <= top_) {
    ::std::cout << s_[j++];
  }
  ::std::cout << ::std::endl;
}
