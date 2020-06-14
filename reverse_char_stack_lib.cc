// The part of the reverse_char_stack that is tested.
#include "reverse_char_stack.h"

using namespace std;

namespace charstack {

// The following cannot be placed in the header file without triggering a
// 'multiple definition' error.
CharStack::CharStack(const string &str) {
  top_ = str.length() - 1;
  strncpy(s_, str.c_str(), str.length());
  s_[++top_] = '\0';
}

CharStack::CharStack(const CharStack &input) {
  strncpy(s_, input.s_, input.top_ + 1);
  top_ = input.top_;
}

void CharStack::Push(char c) {
  if (full()) {
    cerr << "Stack overflow." << endl;
    assert_perror(EOVERFLOW);
  }
  top_++;
  s_[top_] = c;
}

void CharStack::PushMultiple(unsigned m, const char s1[]) {
  if (strlen(s1) < m) {
    cerr << "PushMultiple: provided string '" << s1 << "' is too short."
         << endl;
    assert_perror(ENOMSG);
  }
  // No trailing NULL.
  if ((m + top_) > kFull) {
    cerr << "PushMultiple: provided string '" << s1 << "' is too long." << endl;
    assert_perror(EMSGSIZE);
  }
  unsigned i = 0;
  while (i < m) {
    Push(s1[i++]);
  }
}

char CharStack::Pop() {
  if (empty()) {
    cerr << "Stack underflow." << endl;
    assert_perror(ENODATA);
  }
  return s_[top_--];
}

void CharStack::PopMultiple(const unsigned m, char s1[]) {
  // Go past any leading NUL characters.
  char c;
  while ('\0' == (c = Pop())) {
  }

  // Check if enough characters are left.
  if ((top_ - static_cast<int>(m) + 1) < kEmpty) {
    cerr << "PopMultiple: not enough characters are stacked." << endl;
    // Put last char back.
    Push(c);
    assert_perror(ENOMSG);
  }

  // Return the next m.
  s1[0] = c;
  unsigned j;
  for (j = 1; j < m; j++) {
    s1[j] = Pop();
  }
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

std::ostream &operator<<(std::ostream &out, const CharStack &st) {
  int j = 0;
  while (j <= st.top_) {
    out << st.s_[j++];
  }
  out << endl;
  return out;
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

} // namespace charstack
