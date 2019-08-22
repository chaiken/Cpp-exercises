#include <cassert>

#include <iostream>

namespace template_vect {

template <typename T> T &TemplateVector<T>::operator[](int i) {
  assert((i <= ub()) && (i >= 0));
  return p_[i];
}

template <typename T> TemplateVector<T>::TemplateVector(int n) : size_(n) {
  ::std::cout << "Default ctor" << ::std::endl;
  if (n <= 0) {
    assert_perror(EINVAL);
  }
  p_ = new T[n];
  assert(nullptr != p_);
}

template <typename T>
TemplateVector<T>::TemplateVector(const T *v, int sz) : size_(sz) {
  ::std::cout << "Array ctor" << ::std::endl;
  assert(size_ > 0);
  p_ = new T[size_];
  assert(nullptr != p_);
  for (int i = 0; i < size_; i++) {
    p_[i] = v[i];
  }
}

template <typename T>
TemplateVector<T>::TemplateVector(const ::std::vector<T> vec)
    : size_(vec.size()) {
  ::std::cout << "::std::vector ctor" << ::std::endl;
  assert(size_ > 0);
  p_ = new T[size_];
  assert(nullptr != p_);
  // clang-format off
  // error: conversion from ‘__normal_iterator<const double*,[...]>’ to non-scalar type ‘__normal_iterator<double*,[...]>’ requested
  //  for (typename ::std::vector<T>::iterator it = vec.cbegin(); it != vec.cend(); it++, i++) {
  // clang-format on
  int i = 0;
  for (T x : vec) {
    p_[i] = x;
    i++;
  }
}

// Putting this code in the template_vector_lib.cc caused a linker failure.
template <typename T>
TemplateVector<T>::TemplateVector(TemplateVector &&vec) : size_(vec.size_) {
  ::std::cout << "Move ctor" << ::std::endl;
  assert(size_ > 0);
  // This move() is not needed, as vec is already an R-value reference.
  //  p_ = ::std::move(vec.p_);
  p_ = vec.p_;
  vec.p_ = nullptr;
}

template <typename T>
TemplateVector<T> &TemplateVector<T>::operator=(TemplateVector &&v) {
  ::std::cout << "move assignment operator" << ::std::endl;
  size_ = v.size_;
  ::std::swap(v.p_, p_);
  return *this;
}

} // namespace template_vect
