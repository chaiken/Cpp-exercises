// The part of the reverse_char_stack that is tested.
#include "reverse_char_stack_lib.h"

using namespace std;

// The following cannot be placed in the header file without triggering a
// 'multiple definition' error.
CharStack::CharStack(const string &str) {
  top_ = str.length();
  strncpy(s_, str.c_str(), str.length());
  s_[++top_] = '\0';
}

void CharStack::Push(char c) {
  if (full()) {
    cerr << "Stack overflow." << endl;
    exit(EXIT_FAILURE);
  }
  top_++;
  s_[top_] = c;
}

void CharStack::PushMultiple(unsigned m, const char s1[]) {
  if (strlen(s1) < m) {
    cerr << "PushMultiple: provided string " << s1 << " is too short." << endl;
    exit(EXIT_FAILURE);
  }
  // No trailing NULL.
  if ((m + top_) > kFull) {
    cerr << "PushMultiple: provided string " << s1 << " is too long." << endl;
    exit(EXIT_FAILURE);
  }
  unsigned i = 0;
  while (i < m) {
    Push(s1[i++]);
  }
}

char CharStack::Pop() {
  if (empty()) {
    cerr << "Stack underflow." << endl;
    exit(EXIT_FAILURE);
  }
  return s_[top_--];
}

void CharStack::PopMultiple(const unsigned m, char s1[]) {
  strcpy(s1, "");
  if ((top_ - static_cast<int>(m)) < kEmpty) {
    cerr << "PopMultiple: provided string is too long." << endl;
    exit(EXIT_FAILURE);
  }
  unsigned i = 0u;
  while (i < m) {
    char c = Pop();
    if (('\0' == c) && (0 == i)) {
      i++;
      continue;
    }
    s1[i++] = c;
  }
  s1[m] = '\0';
}

void CharStack::Reverse() {
  char temp[kMaxLen];

  if (empty()) {
    cout << "Empty stack need not be reversed.";
    return;
  }

  size_t original_len = top_;
  int i = 0;
  while (!empty()) {
    char c = Pop();
    if ('\0' == c) {
      continue;
    }
    temp[i++] = c;
  }
  top_ = original_len;
  strncpy(s_, temp, original_len);
  s_[original_len + 1] = '\0';
}

void CharStack::Print() const {
  int j = 0;
  while (j <= top_) {
    cout << s_[j++];
  }
  cout << endl;
}

string pop_all(CharStack st) {
  string output;
  while (!st.empty()) {
    char c = st.Pop();
    if ('\0' != c) {
      output.push_back(c);
    }
  }
  return output;
  output.push_back('\0');
}

void reverse(const char s1[], char s2[]) {
  CharStack ch_stack;
  ch_stack.reset();

  int i = 0, j = 0;
  while (s1[i] != '\0') {
    ch_stack.Push(s1[i]);
    i++;
  }
  while (i--) {
    s2[j++] = ch_stack.Pop();
  }
  // Don't forget terminal NULL for second string.
  s2[j] = '\0';
  // Check that second string's termination is correct.
  assert(strlen(s1) == strlen(s2));
}
