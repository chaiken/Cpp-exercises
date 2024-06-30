#ifndef SMARTER_QUEUE_H
#define SMARTER_QUEUE_H

#include <iostream>
#include <memory>
#include <vector>

namespace smarter_queue {

class SmarterQueue {
public:
  SmarterQueue()
      : size_(0), reader_cursor_(0), writer_cursor_(0), data_(nullptr) {}
  // Vector constructor.
  SmarterQueue(const ::std::vector<double> &vec)
      : size_(vec.size()), reader_cursor_(0), writer_cursor_(vec.size()),
        data_(std::unique_ptr<double[]>(new double[vec.size()])) {
    int i = 0;
    for (auto x : vec) {
      data_[i] = x;
      i++;
    }
  }
  // Empty queue of defined length.
  SmarterQueue(int length)
      : size_(length), reader_cursor_(0), writer_cursor_(0),
        data_(std::unique_ptr<double[]>(new double[size_])) {
    int i = 0;
    while (i < size_) {
      data_[i] = 0.0;
      i++;
    }
  }
  SmarterQueue(const SmarterQueue &sq) = delete;
  SmarterQueue(SmarterQueue &&sq) = default;
  bool is_empty() const { return reader_cursor_ == writer_cursor_; }
  bool is_full() const {
    return ((writer_cursor_ == size_) && (0 == reader_cursor_));
  }
  int size() const { return (writer_cursor_ - reader_cursor_); }
  // subsequence overload of the function operator.
  // Number of elements in the new SmarterQueue is (end - begin + 1) since both
  // bounds are included.
  // First returned value is an error code which is C-errno upon failure.
  std::pair<int, SmarterQueue> operator()(int begin, int end);
  friend bool operator==(const SmarterQueue &sq1, const SmarterQueue &sq2);
  // Must be a friend rather than class-member operator<< can take only one
  // argument. and needs access to private data.
  friend ::std::ostream &operator<<(::std::ostream &out,
                                    const SmarterQueue &sq);
  void Push(double dat);
  // A modifying reader which increments the offset.
  // First returned value is an error code which is C-errno upon failure.
  std::pair<int, double> Pop();
  // A non-modifying reader with a caller-managed offset.
  // First returned value is an error code which is C-errno upon failure.
  std::pair<int, double> Peek(const int offset) const;

private:
  int size_;
  int reader_cursor_;
  int writer_cursor_;
  std::unique_ptr<double[]> data_;
};

::std::ostream &operator<<(::std::ostream &out, const SmarterQueue &sq);
bool operator==(const SmarterQueue &sq1, const SmarterQueue &sq2);
bool SequenceIsIncreasing(::std::vector<double> vec);
// The returned value is an error code which is C-errno upon failure.
int FindIncreasingSubsequences(::std::ostream &out, const SmarterQueue &sq);

} // namespace smarter_queue

#endif // SMARTER_QUEUE_H
