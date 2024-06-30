#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>

namespace dyn_string {

class DynString {
public:
  // unique_ptrs must be initialized through member-initializer list.
  // Otherwise s_(char *) will attempt to match a non-existent unique_ptr call
  // operator().
  DynString() : len_(0u), s_(std::make_unique<char[]>(1u)) {}
  DynString(char *p);
  DynString(const char *p);
  DynString(DynString &&str) = default;

  const char *contents() const { return s_.get(); }
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
  size_t len_;
  std::unique_ptr<char[]> s_;
};

bool operator==(const DynString &a, const DynString &b);
std::ostream &operator<<(std::ostream &out, const DynString &a);
std::ostream &print_some(std::ostream &out, const DynString &a, size_t n);
void dyn_string_sort(DynString *dynstr, size_t num, size_t len);
} // namespace dyn_string
