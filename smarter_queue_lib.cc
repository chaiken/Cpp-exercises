#include "smarter_queue.h"

#include <cassert>
#include <cerrno>

using namespace std;

namespace smarter_queue {

ostream &operator<<(ostream &out, const SmarterQueue &sq) {
  for (int i = 0; i < sq.size_; i++) {
    out << sq.data_[i] << " ";
  }
  return out;
}

SmarterQueue SmarterQueue::operator()(int begin, int end) {
  if ((begin < 0) || (end > (size_ - 1)) || (begin > end)) {
    cerr << "Limits are out of bounds." << endl;
    assert_perror(EINVAL);
  }
  vector<double> vec;
  for (int i = begin; i <= end; i++) {
    vec.push_back(data_[i]);
  }
  return vec;
}

bool operator==(const SmarterQueue &sq1, const SmarterQueue &sq2) {
  if (sq1.size_ != sq2.size_) {
    return false;
  }
  for (int i = 0; i < sq1.size_; i++) {
    if (sq1.data_[i] != sq2.data_[i]) {
      return false;
    }
  }
  return true;
}

void SmarterQueue::Push(double dat) {
  if (is_full()) {
    cerr << "Queue is already full." << endl;
    return;
  }
  if (writer_cursor_ == size_) {
    cout << "Shifting queue." << endl;
    for (int i = reader_cursor_; i < size_; i++) {
      data_[i - reader_cursor_] = data_[i];
    }
    reader_cursor_ = 0;
    return;
  }
  data_[writer_cursor_++] = dat;
}

double SmarterQueue::Pop() {
  if (is_empty()) {
    assert_perror(ENODATA);
  }
  return data_[reader_cursor_++];
}

double SmarterQueue::Peek(const int offset) const {
  if (is_empty() || (offset < 0)) {
    assert_perror(ENODATA);
  }
  const int index = reader_cursor_ + offset;
  if (size_ <= index) {
    assert_perror(ERANGE);
  }
  return data_[index];
}

// Pass the argument by value to avoid const-conversion with the iterator.
bool SequenceIsIncreasing(vector<double> vec) {
  if (vec.empty() || (1 == vec.size())) {
    return true;
  }
  double last = vec.front();
  for (vector<double>::iterator it = vec.begin(); it < vec.end(); it++) {
    if ((*it) < last) {
      return false;
    }
    last = *it;
  }
  return true;
}

void FindIncreasingSubsequences(ostream &out, const SmarterQueue &sq) {
  if (sq.is_empty()) {
    return;
  }
  int offset = 0;
  out << "(";
  vector<double> vec;
  while (offset < sq.size()) {
    double save;
    // Save the increasing subsequences in a vector.
    vec.push_back(sq.Peek(offset));
    if (!SequenceIsIncreasing(vec)) {
      // Putting decreasing element back on FIFO Queue doesn't work, since
      // reader and writer are at different ends by design.
      // sq1.Push(vec.pop_back());
      // Remove and save the decreasing element.
      save = vec.back();
      vec.pop_back();
      out << vec;
      out << "), (";

      // Start next iteration with decreasing element.
      vec.clear();
      vec.push_back(save);
    }
    offset++;
  }
  // Make sure last element gets printed.
  if (!vec.empty()) {
    out << vec;
    out << ")";
  }
}

} // namespace smarter_queue
