#include <ostream>

namespace generic_stack {

class GenericStack {
public:
  GenericStack(void *arr, int size) : size_(size), arr_(arr), top_(0){};
  GenericStack(const GenericStack &astack)
      : size_(astack.size_), arr_(astack.arr_), top_(astack.top_){};
  // These functions could be implemented as operator+() and operator-(), but
  // that seems unnatural.
  // Generic versions won't compile.
  // void Push(void *datum);
  // void *Pop();
  // With no templates, each stack type needs its own operators.
  void DoublePush(double datum);
  double DoublePop();
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const GenericStack &astack);

  bool full() const { return top_ == (size_ - 1); }
  bool empty() const { return 0 == top_; }

private:
  int size_;
  void *arr_;
  int top_;
};

::std::ostream &operator<<(::std::ostream &out, const GenericStack &astack);
void Reverse(double data[], int size);
} // namespace generic_stack
