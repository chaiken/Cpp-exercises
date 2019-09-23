#ifndef VECTOR_IT_H
#define VECTOR_IT_H

#include <cassert>
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

// https://stackoverflow.com/questions/4660123/overloading-friend-operator-for-template-class/4661372#4661372
// The function is static, as it doesn't depend on type T and is not a class
// member.  In fact a member function with declaration
//   template <typename U> void tvassign(TemplateVector<U> &uvec)
// won't compile, with T used as the second typename within.
// Cannot be operator=():
// clang-format off
// error: ‘void template_vect::operator=(template_vect::TemplateVector<U>&,template_vect::TemplateVector<V>&)’ must be a nonstatic member function
// void operator=(TemplateVector<U> &uvec, TemplateVector<V> &vvec) template <typename U, typename V>
// clang-format on
template <typename U, typename V>
static void tvassign(TemplateVector<U> &uvec, TemplateVector<V> &vvec) {
  if (uvec == vvec) {
    return;
  }
  ::std::cout << "assignment with conversion function" << ::std::endl;
  constexpr bool ans1 = ::std::is_same<U, V>::value;
  if (!ans1) {
    ::std::cout << "Types don't match." << ::std::endl;
  }
  // Order matters: from is first.
  constexpr bool ans2 = ::std::is_convertible<U, V>::value;
  if (!ans2) {
    // Apparently not reached, as compilation fails.
    ::std::cerr << "Types are not convertible." << ::std::endl;
    assert(true == ans2);
  } else {
    ::std::cout << "Types are convertible." << ::std::endl;
  }
  if (uvec.size_ != vvec.size_) {
    ::std::cerr << "Cannot assign a TemplateVector to one of another size."
                << ::std::endl;
    assert(uvec.size_ == vvec.size_);
  }
  // template argument deduction/substitution failed:
  // In file included from template_vector_lib_test.cc:1:
  // template_vector.h:48:16: note:   deduced conflicting types for parameter
  // ‘_Tp’ (‘char*’ and ‘int* const’)
  // ::std::swap(static_cast<V *>(uvec.p_), static_cast<U* >(vvec.p_));
  for (int i = 0; i <= vvec.ub(); i++) {
    uvec[i] = static_cast<U>(vvec[i]);
  }
}

} // namespace template_vect

#include "template_vector_impl.h"

#endif
