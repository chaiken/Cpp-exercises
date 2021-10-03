#ifndef VECTOR_IT_H
#define VECTOR_IT_H

#include <functional>
#include <iostream>
#include <vector>

namespace template_vect {

template <typename T> class TemplateVector {
public:
  explicit TemplateVector(int n = 100);
  // First size_t is size of array paramter arr; second is size of the copy,
  // which can be smaller.  The first could be a template parameter, but then
  // the class definition and all ctors would require it.  Two parameters also
  //  would complicate assignment operators.
  TemplateVector(const T (&arr)[], size_t sz1, size_t sz2);
  TemplateVector(const ::std::vector<T> &v);
  TemplateVector(TemplateVector &&v);
  ~TemplateVector() { delete[] p_; }
  int ub() const { return (size_ - 1); }
  T &operator[](int i);
  const T &operator[](int i) const;
  TemplateVector &operator=(TemplateVector &&v);
  typedef T *iterator;
  iterator begin() const { return &p_[0]; }
  iterator end() const { return &p_[size_]; }
  iterator operator++() { return &p_[cursor_++]; };
  iterator operator--() { return &p_[cursor_--]; };
  // The function is not found by the linker if not placed in the header.
  // See comment in template_stack.h.
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const TemplateVector<T> &tv) {
    for (T *tvit = tv.begin(); tvit != tv.end(); tvit++) {
      out << *tvit << ", ";
    }
    return out;
  }
  // clang-format off
  // https://stackoverflow.com/questions/9787593/implicit-type-conversion-with-template
  // No 'static' here:
  // template_vector.h:35:79: error: storage class specifiers invalid in friend
  // function declarations friend static void tvassign(TemplateVector<U> &uvec,
  // TemplateVector<V> &vvec);
  // clang-format on
  template <typename U, typename V>
  friend void tvassign(TemplateVector<U> &uvec, TemplateVector<V> &vvec);
  template <typename U, typename V>
  friend bool operator==(const TemplateVector<U> &tv1,
                         const TemplateVector<V> &tv2);

private:
  T *p_;
  size_t size_;
  int cursor_ = 0;
};

// http://www.cplusplus.com/reference/stdexcept/invalid_argument/
class assignment_error : public std::logic_error {
public:
  explicit assignment_error(const std::string &what_arg)
      : std::logic_error(what_arg) {}
  explicit assignment_error(const char *what_arg)
      : std::logic_error(what_arg) {}
};

} // namespace template_vect

#include "template_vector_impl.h"

#endif
