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
  str.str_->ref_cnt_++;
}

CountedString operator+(const CountedString &str1, const CountedString &str2) {
  assert(0 != str1.str_->ref_cnt_);
  assert(0 != str2.str_->ref_cnt_);
  ::std::string result(::std::string(str1.str_->s_) +
                       ::std::string(str2.str_->s_));
  // Invoke the implicit constructor.
  return (result);
}

void Swap(CountedString &str1, CountedString &str2) {
  if (str1 == str2) {
    return;
  }
  CountedString tmp(str1);
  str1 = str2;
  str2 = tmp;
}

bool CountedString::operator==(const CountedString &str1) {
  return (0 == strcmp(str_->s_, str1.str_->s_));
}

bool CountedString::operator==(const char *str) {
  return (0 == strcmp(str_->s_, str));
}

char CountedString::operator[](const int i) {
  if ((static_cast<size_t>(i) < str_->len_) && i >= 0) {
    return -1;
  }
  return (str_->s_[i]);
}

CountedString CountedString::operator()(int from, int to) {
  if ((to <= from) || (from >= static_cast<int>(length())) || (from < 0)) {
    return "";
  }
  to = (to > static_cast<int>(length())) ? static_cast<int>(length()) : to;
  // Leaks memory; temp is not destroyed on exit.
  // char *temp = new char[to - from + 1];
  char temp[to - from + 1];
  for (int i = from; i < to; i++) {
    temp[i - from] = str_->s_[i];
  }
  temp[to - from] = 0;
  // Calls implicit char* Ctor of CountedString.
  return temp;
}

// Makes << operator work with CountedString objects.
CountedString::operator char *() { return str_->s_; }

bool CountedString::Search(char *str) {
  int slen = static_cast<int>(strlen(str)), len = static_cast<int>(length());
  // Empty string always found.
  if (!slen) {
    return true;
  }
  if (len < slen) {
    return false;
  }
  CountedString ts(str);
  int i = 0;
  while (i <= (len - slen)) {
    // Violates const-correctness.
    //  if (ts == *this(i, i+slen)) {
    // Hits ASAN due to use-after-free when the StringObject Dtor runs for temp
    // in operator() above.
    // char *substr = (*this)(i, i+slen).str_->s_;
    CountedString temp = (*this)(i, i + slen);
    if (!strcmp(ts, temp.str_->s_)) {
      return true;
    }
    i++;
  }
  return false;
}

} // namespace reference_counted_string
