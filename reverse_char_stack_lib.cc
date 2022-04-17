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

result CharStack::Push(char c) {
  if (full()) {
    cerr << "Stack overflow." << endl;
    return result::kFailure;
  }
  top_++;
  s_[top_] = c;
  return result::kSuccess;
}

result CharStack::PushMultiple(unsigned m, const char s1[]) {
  if (strlen(s1) < m) {
    cerr << "PushMultiple: provided string '" << s1 << "' is too short."
         << endl;
    return result::kFailure;
  }
  // No trailing NULL.
  if ((m + top_) > kFull) {
    cerr << "PushMultiple: provided string '" << s1 << "' is too long." << endl;
    return result::kFailure;
  }
  unsigned i = 0;
  result res = result::kSuccess;
  while ((i < m) && (result::kSuccess == res)) {
    res = Push(s1[i++]);
  }
  return res;
}

pair<char, result> CharStack::Pop() {
  if (empty()) {
    cerr << "Stack underflow." << endl;
    // make_pair() works with constant values, but not lvalues.   pairs can be
    // formed from lvalues only with the pair() constructor.
    return make_pair<char, result>('\0', result::kFailure);
  }
  return pair<char, result>(s_[top_--], result::kSuccess);
}

result CharStack::PopMultiple(const unsigned m, char s1[]) {
  // Go past any leading NUL characters.
  char c;
  while ('\0' == (c = Pop().first)) {
  }

  // Check if enough characters are left.
  if ((top_ - static_cast<int>(m) + 1) < kEmpty) {
    cerr << "PopMultiple: not enough characters are stacked." << endl;
    // Put last char back.
    Push(c);
    return result::kFailure;
  }

  // Return the next m.
  s1[0] = c;
  unsigned j;
  for (j = 1; j < m; j++) {
    pair<char, result> res = Pop();
    // The following lines are not reachable.
    //    if (result::kFailure == res.second) {
    //      return result::kFailure;
    //    }
    s1[j] = res.first;
  }
  return result::kSuccess;
}

result CharStack::Reverse() {
  char temp[kMaxLen];

  if (empty()) {
    cout << "Empty stack need not be reversed.";
    return result::kSuccess;
  }

  size_t original_len = top_;
  int i = 0;
  pair<char, result> res('\0', result::kSuccess);
  while (!empty() && (result::kSuccess == res.second)) {
    res = Pop();
    char c = res.first;
    if ('\0' == c) {
      continue;
    }
    temp[i++] = c;
  }
  top_ = original_len;
  strncpy(s_, temp, original_len);
  s_[original_len + 1] = '\0';
  return res.second;
}

std::ostream &operator<<(std::ostream &out, const CharStack &st) {
  int j = 0;
  while (j <= st.top_) {
    out << st.s_[j++];
  }
  out << endl;
  return out;
}

pair<string, result> pop_all(CharStack st) {
  string output;
  pair<char, result> res{'\0', result::kSuccess};
  while (!st.empty() && (result::kSuccess == res.second)) {
    res = st.Pop();
    char c = res.first;
    if ('\0' != c) {
      output.push_back(c);
    }
  }
  return pair<string, result>(output, res.second);
}

} // namespace charstack
