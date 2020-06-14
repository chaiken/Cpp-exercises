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

  size_t len() const { return len_; }
  bool empty() const { return (0u == len_); }
  DynString &operator=(const DynString &str);
  friend bool operator==(const DynString &a, const DynString &b);
  friend std::ostream &operator<<(std::ostream &out, const DynString &a);
  // Apparently defining two friends with the same name doesn't work.
  friend std::ostream &print_some(std::ostream &out, const DynString &a,
                                  size_t n);
  void concat(const DynString &a, const DynString &b);
  int compare(const DynString &a) const;
  void reverse();
  void swap(DynString &a);

private:
  char *s_;
  size_t len_;
};

bool operator==(const DynString &a, const DynString &b);
std::ostream &operator<<(std::ostream &out, const DynString &a);
std::ostream &print_some(std::ostream &out, const DynString &a, size_t n);
void dyn_string_sort(DynString *dynstr, size_t num, size_t len);
} // namespace dyn_string
