#ifndef VECTOR_IT_H
#define VECTOR_IT_H

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
  T *begin() const { return &p_[0]; }
  T *end() const { return &p_[size_]; }
  TemplateVector &operator=(TemplateVector &&v);
  T *operator++() { return &p_[cursor_++]; };
  // The function is not found by the linker if not placed in the header.
  // See comment in templated_stack.h.
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const TemplateVector<T> &tv) {
    for (T *tvit = tv.begin(); tvit != tv.end(); tvit++) {
      out << *tvit << ", ";
    }
    return out;
  }

private:
  T *p_;
  int size_;
  int cursor_ = 0;
};

} // namespace template_vect

#include "template_vector_impl.h"

#endif
