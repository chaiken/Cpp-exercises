#include <cassert>
#include <cstring>
#include <iostream>

namespace dyn_string {

class DynString {
public:
  DynString() : len_(0u) {
    s_ = new char[1];
    assert(s_ != 0);
    s_[0] = 0;
  }
  DynString(const char *p);
  DynString(const DynString &str);
  ~DynString() { delete[] s_; }

  void assign(const DynString &str);
  void print() const { ::std::cout << s_ << ::std::endl; }
  void concat(const DynString &a, const DynString &b);
  bool equals(const DynString &a);

private:
  char *s_;
  size_t len_;
};
}
