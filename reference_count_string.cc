#include "reference_count_string.h"

using namespace std;

namespace reference_counted_string {

void CountedString::Assign(const CountedString &str) {
  assert(str.str_->ref_cnt_ > 0);
  if (str.str_ == str_) {
    return;
  }
  if (0 == --str_->ref_cnt_) {
    delete str_;
  }
  str_ = str.str_;
  str_->ref_cnt_++;
}

} // namespace reference_counted_string
