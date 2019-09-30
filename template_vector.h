#ifndef VECTOR_IT_H
#define VECTOR_IT_H

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

namespace template_vect {

template <typename T> class TemplateVector {
public:
  explicit TemplateVector(int n = 100);
  TemplateVector(const T *v, int sz);
  TemplateVector(const ::std::vector<T> v);
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
  // See comment in templated_stack.h.
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
  int size_;
  int cursor_ = 0;
};

} // namespace template_vect

#include "template_vector_impl.h"

#endif
