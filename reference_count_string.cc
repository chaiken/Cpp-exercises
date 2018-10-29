#include "reference_count_string.h"

using namespace std;

namespace reference_counted_string {

void CountedString::operator=(const CountedString &str) {
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

CountedString operator+(const CountedString &str1, const CountedString &str2) {
  assert(0 != str1.str_->ref_cnt_);
  assert(0 != str2.str_->ref_cnt_);
  ::std::string result(::std::string(str1.str_->s_) +
                       ::std::string(str2.str_->s_));
  // Invoke the implicit constructor.
  return (result);
}

} // namespace reference_counted_string
