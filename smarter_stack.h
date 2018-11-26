#include <cassert>

#include <ostream>
#include <vector>

namespace smarter_stack {

class SmarterStack {
public:
  SmarterStack() : size_(0), top_(0){};
  // An empty stack of a given size.
  SmarterStack(int size) : size_(size), top_(0) {
    data_ = new double[size];
    assert(nullptr != data_);
  }
  // Ctor with array of doubles.
  SmarterStack(int size, const double *data) : size_(size), top_(size) {
    data_ = new double[size];
    assert(nullptr != data_);
    int i = 0;
    while (i < size) {
      data_[i] = data[i];
      i++;
    }
  }

  // Ctor with vector of doubles.
  SmarterStack(const ::std::vector<double> data)
      : size_(data.size()), top_(data.size()) {
    data_ = new double[data.size()];
    assert(nullptr != data_);
    // Could use a vector iterator, but still have to iterate i.
    for (int i = 0; i < static_cast<int>(data.size()); i++) {
      data_[i] = data.at(i);
    }
  }
  ~SmarterStack() { delete[] data_; }

  void Push(double datum);
  double Pop();

  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const SmarterStack &st);
  friend bool operator==(const SmarterStack &a, const SmarterStack &b);

  bool full() const { return (top_ == size_); }
  bool empty() const { return (top_ == 0); }
  double operator[](int i);

private:
  int size_, top_;
  double *data_;
};

::std::ostream &operator<<(::std::ostream &out, const SmarterStack &st);
bool operator==(const SmarterStack &a, const SmarterStack &b);
} // namespace smarter_stack
