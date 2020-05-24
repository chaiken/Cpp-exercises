#ifndef TEMPLATED_STACK_H
#define TEMPLATED_STACK_H

#include <cassert>
#include <iostream>
#include <string>

namespace templated_stack {

namespace {
#ifdef CONSTSIZE
constexpr int32_t kDefaultSize = CONSTSIZE;
#else
constexpr int32_t kDefaultSize = 100;
#endif
} // namespace

template <typename T> class TemplatedStack {
private:
  enum { EMPTY = -1 };
  int max_len_;
  int top_;
  T *data_;

public:
  TemplatedStack()
      : max_len_(kDefaultSize), top_(EMPTY), data_(new T[kDefaultSize]) {
    ::std::cout << "Default ctor" << ::std::endl;
  }
  // Move constructor.
  TemplatedStack(TemplatedStack &&ts);
  // R-value reference array ctr.
  TemplatedStack(T(&&input)[], int len);
  TemplatedStack(T input[], int len);
  // https://stackoverflow.com/questions/6077143/disable-copy-constructor
  TemplatedStack(const TemplatedStack &ts) = delete;
  // Constructs a TemplatedStack from a good old C-style array. The array can be
  // const or non-const; it can capture both. Copied from Philipp Schrader's
  // span.h.  I don't think that it can be exercised from a class that defines a
  // destructor without resulting in "attempting free on address which was not
  // malloc()-ed".
  template <int N>
  constexpr TemplatedStack(T (&arr)[N])
      : max_len_(N), top_(N - 1), data_(&arr[0]){};
  ~TemplatedStack() { delete[] data_; }
  void reset() { top_ = EMPTY; }
  void push(T x) { data_[++top_] = x; }
  T pop() { return data_[top_--]; }
  T top_of() const { return data_[top_]; };
  bool empty() const { return (top_ == EMPTY); }
  bool full() const { return (top_ == (max_len_ - 1)); }
  int size() const { return max_len_; };
  T &operator[](int i) const { return data_[i]; }
  TemplatedStack &operator=(TemplatedStack &&input);
  void reverse(T *arr[], int n);
  void reverse(T arr[], int n);
  // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Making_New_Friends
  // the inserter is not itself a template but it still uses a template argument
  // (T). This is a problem since it’s not a member function. Such a friend
  // function is not a template, but the template acts as a factory for "making"
  // new friends. A new non-template function is created for each
  // specialization.
  //
  // https://www.justsoftwaresolutions.co.uk/cplusplus/hidden-friends.html
  // A friend defined (not just declared) inside a class is a hidden friend.
  // Hidden friends are visible to other code only if the friend takes the
  // object whose class in which it apppears as a parameter, in which case
  // Argument Dependent Lookup can find it.
  //
  // See also "Profiting from the Folly of Others":
  // https://accu.org/index.php/journals/2776
  // "another way of allowing the compiler to find hidden friends, and that is
  // to put a declaration of the function in the enclosing namespace"
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const TemplatedStack<T> &ts) {
    int i = ts.top_;
    assert(0 != ts.data_);
    while (i >= 0) {
      out << ts.data_[i];
      if (0 != i) {
        out << ", ";
      }
      i--;
    }
    return out;
  }
};

// void reverse(char *strarr[], int n);

} // namespace templated_stack

#include "templated_stack_impl.h"

#endif
