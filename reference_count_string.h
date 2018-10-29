#include <cassert>
#include <cstring>

#include <iostream>

namespace reference_counted_string {

class StringObject {
public:
  StringObject() : len_(0), ref_cnt_(1) {
    s_ = new char[1];
    assert(s_ != 0);
    s_[0] = 0;
  }

  StringObject(const char *p) : ref_cnt_(1) {
    len_ = strlen(p);
    s_ = new char[len_ + 1];
    assert(s_ != 0);
    strcpy(s_, p);
  }
  ~StringObject() {
    ::std::cout << "Deleting StringObject " << s_ << ::std::endl;
    delete[] s_;
  }

  int len_, ref_cnt_;
  char *s_;
};

class CountedString {
public:
  CountedString() {
    str_ = new StringObject;
    assert(str_ != 0);
  }
  CountedString(const char *p) {
    str_ = new StringObject(p);
    assert(str_ != 0);
    ::std::cout << "Char* constructor " << str_->s_ << ::std::endl;
  }
  CountedString(const ::std::string str) {
    str_ = new StringObject(str.c_str());
    assert(str_ != 0);
    ::std::cout << "String constructor " << str_->s_ << ::std::endl;
  }
  CountedString(const CountedString &str) {
    // Because ref_cnt_ is not mutex-protected and could be decremented in
    // another thread.
    assert(0 != str.str_->ref_cnt_);
    str_ = new StringObject(str.str_->s_);
    assert(str_ != 0);
    // The following line has the effect of changing str->ref_cnt_, but str is
    // const?
    str_->ref_cnt_++;
    ::std::cout << "Copy constructor " << str_->s_ << ::std::endl;
  }
  ~CountedString() {
    // Because ref_cnt_ is not mutex-protected and could be decremented in
    // another thread.
    assert(str_->ref_cnt_ >= 0);
    if (0 == --str_->ref_cnt_) {
      ::std::cout << "Deleting CountedString " << str_->s_ << ::std::endl;
      delete str_;
    }
  }
  friend CountedString operator+(const CountedString &str1,
                                 const CountedString &str2);
  void operator=(const CountedString &str);
  void Print() const { ::std::cout << str_->s_ << ::std::endl; }
  bool Compare(const CountedString &str) const {
    assert(str.str_->ref_cnt_ > 0);
    return (0 == strcmp(str_->s_, str.str_->s_));
  }

private:
  StringObject *str_;
};

CountedString operator+(const CountedString &str1, const CountedString &str2);

} // namespace reference_counted_string
