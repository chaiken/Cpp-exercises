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
