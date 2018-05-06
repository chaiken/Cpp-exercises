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
  ~DynString() {
    //    ::std::cout << "Destructor" << ::std::endl;
    //    print();
    //    ::std::cout << "freeing s_: " << s_ << " of this: " << this <<
    //    ::std::endl;
    delete[] s_;
  }

  void assign(const DynString &str);
  void print() const { ::std::cout << s_ << ::std::endl; }
  void print(size_t n) const;
  void concat(const DynString &a, const DynString &b);
  int compare(const DynString &a) const;
  void reverse();
  void swap(DynString &a);

private:
  char *s_;
  size_t len_;
};
}
