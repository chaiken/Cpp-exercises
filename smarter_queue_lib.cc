#include "smarter_queue.h"

using namespace std;

namespace smarter_queue {

ostream &operator<<(ostream &out, const SmarterQueue &sq) {
  for (int i = 0; i < sq.size_; i++) {
    out << sq.data_[i] << " ";
  }
  return out;
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

} // namespace smarter_queue
