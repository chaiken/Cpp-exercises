#include <cerrno>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include <vector>

namespace smarter_stack {

// Provide an error message for the indicated failure.
class SmarterStackException : public std::exception {
public:
  // Setting extended_error_ in the initialization list caused a use-after-free
  // error in the what() method.
  SmarterStackException(const std::exception &e) : std::exception(e) {
    extended_error_ =
        std::string(typeid(e).name()) + ": " + std::string(e.what());
  }
  const char *what() const noexcept;
  ~SmarterStackException() {}

private:
  std::string extended_error_;
};

class SmarterStack {
public:
  SmarterStack() : size_(0), top_(-1){};

  // https://en.cppreference.com/w/cpp/memory/new/operator_new#Class-specific_overloads
  // The example code shows the replacement new called only from
  // non-class-member main() functions.  I tried putting
  // clang-format off
  // SmarterStack *st = new(true) SmarterStack(int);
  // clang-format on
  // but doing so does not call the SmarterStack constructor and therefore
  // does not improve test coverage.
  /*  static void *operator new(size_t sz, bool b = false) throw() {
    if (b) {
      std::cerr << "Returning nullptr instead of " << sz << " allocation."
                << std::endl;
      return nullptr;
    } else {
      // The following line compiles with C++17 only.
      return ::operator new(sz);
      // return std::malloc(sz);
    }
    }

  SmarterStack(int size) : size_(size), top_(-1) {
  //
  https://en.cppreference.com/w/cpp/memory/new/operator_new#Class-specific_overloads
  //  SmarterStack(int size, bool fail = false) : size_(size), top_(-1) {
    std::cerr << "Size ctor" << std::endl;
    if (size <= 0) {
      const std::length_error le(
          "SmarterStack depth must be greater than zero.");
      throw SmarterStackException(le);
    }
    // even with C++17:
    // clang-format off
    // error: no matching function for call to ‘operator new [](sizetype,
  bool&)’
    // data_ = new(fail) double[size];
    // data_ = new(double[size], fail);
    // error: expected type-specifier before ‘)’ token
    //    data_ = static_cast<double *>(new (size * sizeof(double), fail));
    // clang-format on
    if (nullptr == data_) {
      throw std::bad_alloc();
    }
    } */

  // An empty stack of a given size.
  SmarterStack(int size) : size_(size), top_(-1) {
    if (size <= 0) {
      const std::length_error le(
          "SmarterStack depth must be greater than zero.");
      throw SmarterStackException(le);
    }
    data_ = new double[size];
    if (nullptr == data_) {
      throw std::bad_alloc();
    }
  }

  // Ctor with array of doubles.  Allow for stack to be partially full by
  // passing size as a separate parameter.
  SmarterStack(int size, const double *data) : size_(size), top_(size - 1) {
    data_ = new double[size];
    if (nullptr == data_) {
      throw std::bad_alloc();
    }
    int i = 0;
    while (i < size) {
      data_[i] = data[i];
      i++;
    }
  }

  // Ctor with vector of doubles.
  SmarterStack(const ::std::vector<double> data)
      : size_(data.size()), top_(data.size() - 1) {
    data_ = new double[data.size()];
    if (nullptr == data_) {
      throw std::bad_alloc();
    }
    // Could use a vector iterator, but still have to iterate i.
    for (int i = 0; i < static_cast<int>(data.size()); i++) {
      data_[i] = data.at(i);
    }
  }
  // Copy ctor.
  SmarterStack(const SmarterStack &st) : size_(st.size_), top_(st.size_ - 1) {
    data_ = new double[size_];
    if (nullptr == data_) {
      throw std::bad_alloc();
    }
    for (int i = 0; i < size_; i++) {
      data_[i] = st.data_[i];
    }
  }

  ~SmarterStack() { delete[] data_; }

  void Push(double datum);
  double Pop();
  void Reverse();

  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const SmarterStack &st);
  friend bool operator==(SmarterStack &a, SmarterStack &b);
  friend bool operator==(const SmarterStack &a, const SmarterStack &b);

  bool full() const { return (top_ + 1 == size_); }
  bool empty() const { return (-1 == top_); }
  void clear() { top_ = -1; }
  // depth() returns the number of populated elements, which may be < size_.
  int depth() const { return top_ + 1; }
  double operator[](int i);
  double operator[](int i) const;
  // Return a subsequence of the stack starting with from of length
  // ((top_-from)+1).
  SmarterStack operator()(int from) const;
  // Return a subsequence of the stack starting with from and ending with to, of
  // length ((to - from) + 1).
  SmarterStack operator()(int from, int to) const;

private:
  // Number of elements in data_.
  int size_;
  // Index of topmost element in data_ == size_-1.
  int top_;
  double *data_;
};

::std::ostream &operator<<(::std::ostream &out, const SmarterStack &st);
bool operator==(SmarterStack &a, SmarterStack &b);
bool operator==(const SmarterStack &a, const SmarterStack &b);
// Print into an output stream all the monotonically increasing subsequences
// found in the stack.
void PrintIncreasingSubsequences(::std::ostream &out, const SmarterStack &st);

} // namespace smarter_stack
