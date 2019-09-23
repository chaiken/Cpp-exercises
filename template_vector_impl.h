#include <cassert>

#include <iostream>
#include <type_traits>

namespace template_vect {

template <typename T> T &TemplateVector<T>::operator[](int i) {
  assert((i <= ub()) && (i >= 0));
  return p_[i];
}

template <typename T> const T &TemplateVector<T>::operator[](int i) const {
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

// I wanted to make an operator==() instead, but couldn't figure out how to use
// two template parameters within the class.
// clang-format on
// ‘bool template_vect::operator=(const template_vect::TemplateVector<U>&, const
// template_vect::TemplateVector<V>&)’ must be a nonstatic member function
// clang-format off
template <typename U, typename V>
bool operator==(const TemplateVector<U> &tv1, const TemplateVector<V> &tv2) {
  // error: expected primary-expression before ‘(’ token
  // bool ans = ::std::is_same<U,V>(U, V)::value;
  constexpr bool ans = ::std::is_same<U, V>::value;
  if (!ans) {
    ::std::cerr << "Types don't match." << ::std::endl;
    return false;
  }
  if (tv1.ub() != tv2.ub()) {
    ::std::cerr << "Sizes don't match." << ::std::endl;
    return false;
  }
  for (int i = 0; i <= tv1.ub(); i++) {
    if (tv1[i] != tv2[i]) {
      ::std::cerr << "Element " << i << " doesn't match." << ::std::endl;
      return false;
    }
  }
  return true;
}

// If the function arguments are passed by value as simple TemplateVector
// objects, then the compiler complains that the copy constructor was deleted.
// Apparently passing objects by value involves copying them, so that any object
// whose copy constructor has been deleted cannot be passed by value, only by
// pointer or reference.
template <typename U, typename V>
void tvswap(TemplateVector<U> &uvec, TemplateVector<V> &vvec) {
  if (uvec == vvec) {
    return;
  }
  // Create a TemplateVector to hold the values of U.
  ::std::vector<U> temp;
  int i = 0;
  for (U *tvit = uvec.begin(); tvit != uvec.end(); tvit++, i++) {
    temp.push_back(*tvit);
  }
  TemplateVector<U> tempvec(temp);

  tvassign(uvec, vvec);
  tvassign(vvec, tempvec);
}

} // namespace template_vect
