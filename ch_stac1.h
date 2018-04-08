#include <iostream>

namespace {
constexpr int kMaxLen = 64, kEmpty = -1, kFull = kMaxLen - 1;
}

class CharStack {
public:
  CharStack() : top_(kEmpty) { s_[0] = '\0'; }
  CharStack(char *str) : top_(strlen(str)) { strcpy(s_, str); }
  void reset() { top_ = kEmpty; }
  void Push(char c);
  void PushMultiple(unsigned m, const char s1[]);
  void Reverse();
  void Print();
  char Pop();
  void PopMultiple(unsigned m, char s1[]);
  char top_of() const { return s_[top_]; }
  bool empty() const { return (top_ == kEmpty); }
  bool full() const { return (top_ == kFull); }

private:
  char s_[kMaxLen];
  int top_;
};

void CharStack::Push(char c) {
  if (full()) {
    ::std::cerr << "Stack overflow." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  top_++;
  s_[top_] = c;
}

void CharStack::PushMultiple(unsigned m, const char s1[]) {
  if (strlen(s1) < m) {
    ::std::cerr << "PushMultiple: provided string " << s1 << " is too short."
                << ::std::endl;
    exit(EXIT_FAILURE);
  }
  // No trailing NULL.
  if ((m + top_) > kFull) {
    ::std::cerr << "PushMultiple: provided string " << s1 << " is too long."
                << ::std::endl;
    exit(EXIT_FAILURE);
  }
  unsigned i = 0;
  while (i < m) {
    Push(s1[i++]);
  }
}

char CharStack::Pop() {
  if (empty()) {
    ::std::cerr << "Stack underflow." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  return s_[top_--];
}

void CharStack::PopMultiple(unsigned m, char s1[]) {
  strcpy(s1, "");
  if ((top_ - static_cast<int>(m)) < kEmpty) {
    ::std::cerr << "PopMultiple: provided string is too long." << ::std::endl;
    exit(EXIT_FAILURE);
  }
  unsigned i = 0u;
  while (i < m) {
    char c = Pop();
    if (c != '\0' && (0 == i)) {
      i++;
      continue;
    }
    s1[i++] = c;
  }
  s1[m] = '\0';
}

void CharStack::Reverse() {
  char temp[kMaxLen];
  int i = 0;
  size_t original_len = top_;
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
