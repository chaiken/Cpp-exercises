#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace charstack {

namespace {
constexpr int kMaxLen = 64, kEmpty = -1, kFull = kMaxLen - 1;
}

enum struct result { kSuccess, kFailure };

class CharStack {
public:
  CharStack() : top_(kEmpty) { s_[0] = '\0'; }
  CharStack(const ::std::string &);
  CharStack(const CharStack &);
  void reset() { top_ = kEmpty; }
  result Push(char c);
  result PushMultiple(unsigned m, const char s1[]);
  result Reverse();
  friend std::ostream &operator<<(std::ostream &out, const CharStack &st);
  std::pair<char, result> Pop();
  result PopMultiple(unsigned m, char s1[]);
  char top_of() const { return s_[top_]; }
  bool empty() const { return (top_ == kEmpty); }
  bool full() const { return (top_ == kFull); }

private:
  char s_[kMaxLen];
  int top_;
};

std::ostream &operator<<(std::ostream &out, const CharStack &st);
std::pair<std::string, result> pop_all(CharStack st);
} // namespace charstack
