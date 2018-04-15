#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace charstack {

namespace {
constexpr int kMaxLen = 64, kEmpty = -1, kFull = kMaxLen - 1;
}

class CharStack {
public:
  CharStack() : top_(kEmpty) { s_[0] = '\0'; }
  CharStack(const ::std::string &);
  void reset() { top_ = kEmpty; }
  void Push(char c);
  void PushMultiple(unsigned m, const char s1[]);
  void Reverse();
  void Print() const;
  char Pop();
  void PopMultiple(unsigned m, char s1[]);
  char top_of() const { return s_[top_]; }
  bool empty() const { return (top_ == kEmpty); }
  bool full() const { return (top_ == kFull); }

private:
  char s_[kMaxLen];
  int top_;
};

void reverse(const char s1[], char s2[]);
::std::string pop_all(CharStack st);
}
