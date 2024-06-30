#include <climits>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <type_traits>

using namespace std;

namespace template_vect {

template <typename T> TemplateVector<T>::TemplateVector(int n) : size_(n) {
  cout << "Default ctor" << ::endl;
  if (n <= 0) {
    throw length_error{"TemplateVector default constructor: illegal size"};
  }
  p_ = unique_ptr<T[]>(new T[n]);
  // Prevent valgrind from complaining about uninitialized memory.
  for (int i = 0; i < n; i++) {
    p_[i] = INT_MAX;
  }
}

// First size_t is size of array paramter arr; second is size of the copy,
// The array parameter is preserved.
template <typename T>
TemplateVector<T>::TemplateVector(const T arr[], size_t sz1, size_t sz2)
    : size_(sz2) {
#ifdef DEBUG
  cout << "Array ctor" << ::endl;
  cout << "Input array size is: " << sz1 << endl;
#endif
  // clang-format off
  // https://en.cppreference.com/w/c/language/sizeof
  //
  // The following works only when the array is copied onto the stack.
  // "Number of elements in any array a including VLA (since C99) may be
  // determined with the expression sizeof a / sizeof a[0]. Note that if a has
  // pointer type (such as after array-to-pointer conversion of function
  // parameter type adjustment), this expression would simply divide the number
  // of bytes in a pointer type by the number of bytes in the pointed type."
  //
  // The compiler does not know the size of the array, so it cannot instantiate
  // the template:
  // template_vector_impl.hh:46:33: error: invalid application of ‘sizeof’ to incomplete type ‘const complex::Complex []’
  //  if ((size_ <= 0) || (size_ > (sizeof(v)/sizeof(v[0])))) {
  // clang-format on
  // sz1 can be anything, as the compiler has no way of checking it.
  if ((size_ <= 0) || (size_ > sz1) || (sz1 <= 0)) {
    throw length_error{"TemplateVector array constructor: illegal size"};
  }
  p_ = unique_ptr<T[]>(new T[size_]);
  for (size_t i = 0u; i < sz2; i++) {
    p_[i] = arr[i];
  }
}

// Consumes the passed-in array.
template <typename T>
TemplateVector<T>::TemplateVector(T arr[], size_t sz1, size_t sz2)
    : size_(sz2) {
  if ((size_ <= 0) || (size_ > sz1) || (sz1 <= 0)) {
    throw length_error{"TemplateVector array constructor: illegal size"};
  }
  p_ = unique_ptr<T[]>(new T[size_]);
  for (size_t i = 0u; i < sz2; i++) {
    if constexpr (is_copy_assignable<T>::value) {
      p_[i] = arr[i];
    } else {
      std::swap(p_[i], arr[i]);
    }
  }
}

template <typename T>
TemplateVector<T>::TemplateVector(const vector<T> &vec) : size_(vec.size()) {
  cout << "vector ctor" << ::endl;
  p_ = unique_ptr<T[]>(new T[size_]);
  // clang-format off
  // error: conversion from ‘__normal_iterator<const double*,[...]>’ to non-scalar type ‘__normal_iterator<double*,[...]>’ requested
  //  for (typename vector<T>::iterator it = vec.cbegin(); it != vec.cend(); it++, i++) {
  // clang-format on
  int i = 0;
  for (T x : vec) {
    p_[i] = x;
    i++;
  }
}

template <typename T>
TemplateVector<T>::TemplateVector(TemplateVector &&vec) : size_(vec.size_) {
  ::std::cout << "Move ctor" << ::std::endl;
  p_ = std::move(vec.p_);
  // With this setting, the public functions can detect that the
  // TemplateVector is empty.
  vec.size_ = 0u;
  vec.p_.reset();
}

template <typename T> T &TemplateVector<T>::operator[](int i) {
  if ((i > ub()) || (i < 0)) {
    throw out_of_range{"TemplateVector::operator[]"};
  }
  return p_[i];
}

template <typename T> const T &TemplateVector<T>::operator[](int i) const {
  if ((i > ub()) || (i < 0)) {
    throw out_of_range{"TemplateVector::operator[]"};
  }
  return p_[i];
}

// A class-member operator==() makes no sense, as it does not use the template
// parameter.   The function is therefore static, as it does not depend on a
// particular template instantation.
// clang-format off
// ‘bool template_vect::operator=(const template_vect::TemplateVector<U>&, const
// template_vect::TemplateVector<V>&)’ must be a nonstatic member function
// clang-format on
template <typename U, typename V>
static bool operator==(const TemplateVector<U> &tv1,
                       const TemplateVector<V> &tv2) {
  // error: expected primary-expression before ‘(’ token
  // bool ans = is_same<U,V>(U, V)::value;
  constexpr bool ans = is_same<U, V>::value;
  if (!ans) {
    cerr << "Types don't match." << ::endl;
    return false;
  }
  if (tv1.ub() != tv2.ub()) {
    cerr << "Sizes don't match." << ::endl;
    return false;
  }
  for (int i = 0; i <= tv1.ub(); i++) {
    if (tv1[i] != tv2[i]) {
      cerr << "Element " << i << " doesn't match." << ::endl;
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
  cout << "assignment with conversion function" << ::endl;
  constexpr bool ans1 = is_same<U, V>::value;
  if (!ans1) {
    cout << "Types don't match." << ::endl;
  }
  // Order matters: from-type is first.
  constexpr bool ans2 = is_convertible<U, V>::value;
  if (!ans2) {
    // Apparently not reached, as compilation fails.
    throw assignment_error{"Types are not convertible."};
  } else {
    cout << "Types are convertible." << ::endl;
  }
  if (uvec.size_ != vvec.size_) {
    throw assignment_error{
        "Cannot assign a TemplateVector to one of another size."};
  }
  // template argument deduction/substitution failed:
  // In file included from template_vector_lib_test.cc:1:
  // template_vector.hh:48:16: note:   deduced conflicting types for parameter
  // ‘_Tp’ (‘char*’ and ‘int* const’)
  // swap(static_cast<V *>(uvec.p_), static_cast<U* >(vvec.p_));
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
  vector<U> temp;
  int i = 0;
  for (U *tvit = uvec.begin(); tvit != uvec.end(); tvit++, i++) {
    temp.push_back(*tvit);
  }
  TemplateVector<U> tempvec(temp);

  tvassign(uvec, vvec);
  tvassign(vvec, tempvec);
}

// "Newton-Raphson" interval-bisecting in-place sort function.
// Arrange in increasing order.
template <typename T> void tvsort(TemplateVector<T> &tv) {
  static int index = 0;
#ifdef DEBUG
  static int depth = 0;
  cout << "Depth: " << depth << " index: " << index << ::endl;
#endif
  // Done when this loop hits ub().  <ub() makes ub()+1 safe.
  while (index < tv.ub()) {
    // If neighboring elements are out of order, check swap with remote element.
    if (tv[index + 1] < tv[index]) {
      // Try swapping the 0th element with the middle one, the 1st element with
      // the element 3/4 of the way through, the 2nd element with that 1/4 way
      // through . . .
      int candidate = (tv.ub() - index * (-1 ^ index)) / 2.0;
      // Not trivial due to integer math.
      candidate -= (candidate / tv.ub()) * tv.ub();
      // Here is the forward sorting.   The reverse is separate, below.
      // First try to place in the middle of the half of the TemplaceVector that
      // remains unsorted.
      if ((candidate > index) && (tv[candidate] < tv[index])) {
        swap(tv[candidate], tv[index]);
#ifdef DEBUG
        cout << "Reordering index = " << index << " with " << candidate
             << ::endl;
        cout << tv << ::endl;
#endif
      } else {
        // Remote element is already ordered, so just swap with neighbor.
        swap(tv[index], tv[index + 1]);
#ifdef DEBUG
        cout << "Reordering index = " << index << " with neighbor." << ::endl;
        cout << tv << ::endl;
#endif
      }
      // Now recheck previous order.  index=ub()-1 and index=0 are fully checked
      // by forwards iteration.
      if ((index < (tv.ub() - 1)) && (index > 0)) {
#ifdef DEBUG
        depth++;
        cout << "Reversing; depth " << depth << ::endl;
#endif
        index--;
        tvsort(tv);
      }
#ifdef DEBUG
      depth--;
#endif
      index++;
    } else { // Adjacent elements are already ordered, so proceed to next ones.
      index++;
    }
  } // while loop
}

} // namespace template_vect
