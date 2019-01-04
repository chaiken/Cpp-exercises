#ifndef SMARTER_QUEUE_H
#define SMARTER_QUEUE_H

#include <cassert>
#include <cerrno>

#include <iostream>
#include <vector>

namespace smarter_queue {

class SmarterQueue {
public:
  SmarterQueue()
      : size_(0), reader_cursor_(0), writer_cursor_(0), data_(nullptr) {}
  SmarterQueue(const ::std::vector<double> &vec)
      : size_(vec.size()), reader_cursor_(0), writer_cursor_(vec.size()) {
    data_ = new double[vec.size()];
    assert(data_ != nullptr);
    int i = 0;
    for (auto x : vec) {
      data_[i] = x;
      i++;
    }
  }
  SmarterQueue(int length)
      : size_(length), reader_cursor_(0), writer_cursor_(0) {
    data_ = new double[size_];
    assert(nullptr != data_);
  }
  ~SmarterQueue() { delete[] data_; }
  bool is_empty() const { return reader_cursor_ == writer_cursor_; }
  bool is_full() const {
    return ((writer_cursor_ == size_) && (0 == reader_cursor_));
  }
  // Must be a friend rather than class-member operator<< can take only one
  // argument. and needs access to private data.
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const SmarterQueue &sq);
  void Push(double dat);
  double Pop();

private:
  int size_;
  int reader_cursor_;
  int writer_cursor_;
  double *data_;
};

::std::ostream &operator<<(::std::ostream &out, const SmarterQueue &sq);
} // namespace smarter_queue

#endif // SMARTER_QUEUE_H
