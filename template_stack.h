#ifndef TEMPLATE_STACK_H
#define TEMPLATE_STACK_H

#include <iostream>
#include <memory>
#include <string>

namespace template_stack {

namespace {
#ifdef CONSTSIZE
constexpr int32_t kDefaultSize = CONSTSIZE;
#else
constexpr int32_t kDefaultSize = 100;
#endif
} // namespace

template <typename T> class TemplateStack {
private:
  enum { EMPTY = -1 };
  int max_len_;
  int top_;
  std::unique_ptr<T[]> data_;

public:
  TemplateStack()
      : max_len_(kDefaultSize), top_(EMPTY),
        data_(std::unique_ptr<T[]>(new T[kDefaultSize])) {
    ::std::cout << "Default ctor" << ::std::endl;
  }
  // Move constructor.
  TemplateStack(TemplateStack &&ts);
  // R-value reference array ctr.
  TemplateStack(T(&&input)[], int len);
  TemplateStack(T input[], int len);
  // https://stackoverflow.com/questions/6077143/disable-copy-constructor
  TemplateStack(const TemplateStack &ts) = delete;
  // Constructs a TemplateStack from a good old C-style array. The array can be
  // const or non-const; it can capture both. Copied from Philipp Schrader's
  // span.h.  I don't think that it can be exercised from a class that defines a
  // destructor without resulting in "attempting free on address which was not
  // malloc()-ed".
  template <int N>
  constexpr TemplateStack(T (&arr)[N])
      : max_len_(N), top_(N - 1), data_(std::make_unique<T[]>(&arr[0])){};
  void reset() { top_ = EMPTY; }
  void push(T x) { data_[++top_] = x; }
  T pop() { return data_[top_--]; }
  T top_of() const { return data_[top_]; };
  bool empty() const { return (top_ == EMPTY); }
  bool full() const { return (top_ == (max_len_ - 1)); }
  int size() const { return max_len_; };
  T &operator[](int i) const { return data_[i]; }
  TemplateStack &operator=(TemplateStack &&input);
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
                                    const TemplateStack<T> &ts) {
    int i = ts.top_;
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

} // namespace template_stack

#include "template_stack_impl.h"

#endif
