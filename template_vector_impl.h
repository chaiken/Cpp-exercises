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
#ifdef DEBUG
  ::std::cout << "Array ctor" << ::std::endl;
#endif
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
// clang-format off
// ‘bool template_vect::operator=(const template_vect::TemplateVector<U>&, const
// template_vect::TemplateVector<V>&)’ must be a nonstatic member function
// clang-format on
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

// "Newton-Raphson" interval-bisecting in-place sort function.
template <typename T> void tvsort(TemplateVector<T> &tv) {
  static int index = 0;
#ifdef DEBUG
  static int depth = 0;
  ::std::cout << "Depth: " << depth << " index: " << index << ::std::endl;
#endif
  // Done when this loop hits ub().  <ub() makes ub()+1 safe.
  while (index < tv.ub()) {
    // Arrange in increasing order.
    if (tv[index + 1] < tv[index]) {
      // Try swapping the 0th element with the middle one, the 1st element with
      // the element 3/4 of the way through, the 2nd element with that 1/4 way
      // through . . .
      int candidate = (tv.ub() - index * (-1 ^ index)) / 2.0;
      candidate -= (candidate / tv.ub()) * tv.ub();
      // Here is the forward sorting.   The reverse is separate, below.
      // First try to place in the middle of the half of the TemplaceVector that
      // remains unsorted.
      if ((candidate > index) && (tv[candidate] < tv[index])) {
        ::std::swap(tv[candidate], tv[index]);
#ifdef DEBUG
        ::std::cout << "Reordering index = " << index << " with " << candidate
                    << ::std::endl;
        ::std::cout << tv << ::std::endl;
#endif
      } else {
        ::std::swap(tv[index], tv[index + 1]);
#ifdef DEBUG
        ::std::cout << "Reordering index = " << index << " with neighbor."
                    << ::std::endl;
        ::std::cout << tv << ::std::endl;
#endif
      }
      // Now recheck previous order.  index=ub()-1 and index=0 are fully checked
      // by forwards iteration.
      if ((index < (tv.ub() - 1)) && (index > 0)) {
#ifdef DEBUG
        depth++;
        ::std::cout << "Reversing; depth " << depth << ::std::endl;
#endif
        index--;
        tvsort(tv);
      }
#ifdef DEBUG
      depth--;
#endif
      index++;
    } else {
      // already ordered, so keep going.
      index++;
    }
  }
}

} // namespace template_vect
