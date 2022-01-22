#include "smarter_stack.h"

#include <cerrno>

#include <limits>

using namespace std;

namespace smarter_stack {

namespace {

// One-element stack is defined as increasing.
bool is_increasing(const SmarterStack &stack) {
  // depth() returns number of elements in array indexed from 0 to top_.
  for (int i = 0, j = 1; j < stack.depth(); i++, j++) {
    if (stack[i] > stack[j]) {
      return false;
    }
  }
  return true;
}

} // namespace

SmarterStack::SmarterStack(int size) : size_(size), top_(-1) {
  if (size <= 0) {
    const std::length_error le("SmarterStack depth must be greater than zero.");
    throw SmarterStackException(le);
  }
  data_ = std::unique_ptr<double[]>(new double[size]);
}

SmarterStack::SmarterStack(int size, const double *data)
    : size_(size), top_(size - 1) {
  data_ = std::unique_ptr<double[]>(new double[size]);
  int i = 0;
  while (i < size) {
    data_[i] = data[i];
    i++;
  }
}

SmarterStack::SmarterStack(const ::std::vector<double> data)
    : size_(data.size()), top_(data.size() - 1) {
  data_ = std::unique_ptr<double[]>(new double[data.size()]);
  // Could use a vector iterator, but still have to iterate i.
  for (int i = 0; i < static_cast<int>(data.size()); i++) {
    data_[i] = data.at(i);
  }
}

ostream &operator<<(ostream &out, const SmarterStack &stack) {
  for (int i = 0; i <= stack.top_; i++) {
    out << (stack.data_[i]);
  }
  return out;
}

double SmarterStack::operator[](int i) {
  if ((i > top_) || (i < 0)) {
    range_error re("Out of range.");
    throw SmarterStackException(re);
  }
  return data_[i];
}

double SmarterStack::operator[](int i) const {
  if ((i > top_) || (i < 0)) {
    range_error re("Out of range.");
    throw SmarterStackException(re);
  }
  return data_[i];
}

bool operator==(SmarterStack &a, SmarterStack &b) {
  if (a.size_ != b.size_) {
    return false;
  }
  int i = 0;
  while (i < a.size_) {
    if (a[i] != b[i]) {
      return false;
    }
    i++;
  }
  return true;
}

bool operator==(const SmarterStack &a, const SmarterStack &b) {
  if (a.size_ != b.size_) {
    return false;
  }
  int i = 0;
  while (i < a.size_) {
    if (a[i] != b[i]) {
      return false;
    }
    i++;
  }
  return true;
}

// Overload the function operator to provide subsequences of the stack.
// Returns to+1 elements from 0 to to.
SmarterStack SmarterStack::operator()(int to) const {
  // Last valid index in data_[] is top_.
  if ((to < 0) || (to > top_)) {
    range_error re("Out of range.");
    throw SmarterStackException(re);
  }
  vector<double> temp;
  for (int j = 0; j <= to; j++) {
    temp.push_back(data_[j]);
  }
  // Implicitly convert the vector to a SmarterStack.
  return temp;
}

SmarterStack SmarterStack::operator()(int from, int to) const {
  // Last valid index in data_[] is top_.
  if ((to > top_) || (to < from) || (from < 0)) {
    range_error re("Out of range.");
    throw SmarterStackException(re);
  }
  vector<double> temp;
  for (int j = from; j <= to; j++) {
    temp.push_back(data_[j]);
  }
  // Implicitly convert the vector to a SmarterStack.
  return temp;
}

void SmarterStack::Push(double datum) {
  if (full()) {
    overflow_error oe("SmarterStack is full.");
    throw SmarterStackException(oe);
  }
  // top_ is equal to the index of the the last element in the array.
  data_[++top_] = datum;
}

double SmarterStack::Pop() {
  if (empty()) {
    underflow_error ue("SmarterStack is empty.");
    throw SmarterStackException(ue);
  }
  // The last element that can be popped is at index 0.
  return (data_[top_--]);
}

void SmarterStack::Reverse() {
  SmarterStack temp(size_);
  int i = 0;
  while (!empty()) {
    temp.Push(data_[i]);
    // Pushing Pop()'ed data on temp directly results in two reversals.
    Pop();
    i++;
  }
  while (!temp.empty()) {
    Push(temp.Pop());
  }
}

void PrintIncreasingSubsequences(ostream &out, const SmarterStack &st) {
  int from = 0, to = 0;
  // depth() returns number of elements, which equals highest populated index+1.
  while ((to < st.depth())) {
    // A single-element stack is defined increasing.
    if (!is_increasing(st(from, to))) {
      out << "(";
      out << st(from, to - 1);
      out << "),";
      // Start a new subsequence.
      from = to;
    }
    to++;
  }
  // If the final element is less than its predecessor, it has not gotten
  // printed.
  if (from == st.depth() - 1) {
    out << "(";
    out << st(from, from);
    out << "),";
  }
}

const char *SmarterStackException::what() const noexcept {
  // This code caused a use-after-free when it ran.  I moved the operation to
  // the constructor instead.   At the time, the two variables were private and
  // were in the constructor's initialization list. std::string holder =
  // std::string(exception_name_) + std::string(more_); return holder.c_str();
  return extended_error_.c_str();
  // return "a";
}

} // namespace smarter_stack
