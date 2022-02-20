#ifndef TEMPLATE_STACK_IMPL_H
#define TEMPLATE_STACK_IMPL_H

#include <cassert>
#include <cstdio>

#include <iostream>

// Any functions that refer explicitly to the template parameter must live here.
namespace template_stack {

// Array of R-value references constructor.
template <typename T>
TemplateStack<T>::TemplateStack(T(&&input)[], int val)
    : max_len_(val), top_(val - 1), data_(std::unique_ptr<T[]>(new T[val])) {
  ::std::cout << "R-value reference array ctor" << ::std::endl;
  int i = 0;
  // Still copying, not in the spirit of a move constructor.
  while (i < val) {
    data_[i] = input[i];
    i++;
  }
}

template <typename T>
TemplateStack<T>::TemplateStack(TemplateStack &&ts)
    : max_len_(ts.max_len_), top_(ts.top_) {
  ::std::cout << "Move ctor" << ::std::endl;
  data_ = ::std::move(ts.data_);
  ts.data_ = nullptr;
}

template <typename T>
TemplateStack<T>::TemplateStack(T input[], int len)
    : max_len_(len), top_(len - 1), data_(std::unique_ptr<T[]>(new T[len])) {
  ::std::cout << "Array-copy ctor" << ::std::endl;
  // AddressSanitizer: attempting free on address which was not malloc()-ed:
  // 0x7ffc2a53f3d0 in thread T0
  // ::std::swap(data_, input);
  // input = nullptr;
  for (int i = 0; i < len; i++) {
    data_[i] = input[i];
  }
}

// Should work if (this->max_len_ < input.top_) since we are stealing input's
// storage by swapping the pointers to it.
template <typename T>
TemplateStack<T> &TemplateStack<T>::operator=(TemplateStack &&input) {
  ::std::cout << "TemplateStack move assignment operator" << ::std::endl;
  max_len_ = input.max_len_;
  top_ = input.top_;
  ::std::swap(data_, input.data_);
  //  input.data_ = nullptr;
  return *this;
}

// For char* arrays.
template <typename T> void reverse(T *arr[], int n) {
  TemplateStack<T *> stk(arr, n);
  assert(!stk.empty());
  assert(stk.full());
  for (int i = 0; i < n; i++) {
    arr[i] = stk.pop();
  }
}

// For Complex arrays.  I couldn't figure out how to initialize an array of type
// Complex*.
template <typename T> void reverse(T arr[], int n) {
  TemplateStack<T> stk(arr, n);
  assert(!stk.empty());
  assert(stk.full());
  for (int i = 0; i < n; i++) {
    arr[i] = stk.pop();
  }
}

} // namespace template_stack
#endif
