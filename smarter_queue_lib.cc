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

pair<int, SmarterQueue> SmarterQueue::operator()(int begin, int end) {
  pair<int, SmarterQueue> result;
  if ((begin < 0) || (end > (size_ - 1)) || (begin > end)) {
    cerr << "Limits are out of bounds." << endl;
    return make_pair<int, SmarterQueue>(ERANGE, SmarterQueue());
  }
  vector<double> vec;
  for (int i = begin; i <= end; i++) {
    vec.push_back(data_[i]);
  }
  return make_pair<int, SmarterQueue>(0, SmarterQueue(vec));
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

pair<int, double> SmarterQueue::Pop() {
  if (is_empty()) {
    return pair<int, double>(ENODATA, 0.0);
  }
  return pair<int, double>(0, data_[reader_cursor_++]);
}

pair<int, double> SmarterQueue::Peek(const int offset) const {
  if (is_empty()) {
    return pair<int, double>(ENODATA, 0.0);
  }
  const int index = reader_cursor_ + offset;
  if ((size_ <= index) || (offset < 0)) {
    return pair<int, double>(ERANGE, 0.0);
  }
  return pair<int, double>(0, data_[index]);
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

int FindIncreasingSubsequences(ostream &out, const SmarterQueue &sq) {
  if (sq.is_empty()) {
    return 0;
  }
  int offset = 0;
  out << "(";
  vector<double> vec;
  while (offset < sq.size()) {
    // This code is unreachable.
    //    int retval = 0;
    //    if ((retval = sq.Peek(offset).first) != 0) {
    //      return retval;
    //    }
    double save;
    // Save the increasing subsequences in a vector.
    vec.push_back(sq.Peek(offset).second);
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
  return 0;
}

} // namespace smarter_queue
