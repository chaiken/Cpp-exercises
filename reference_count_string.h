#include <cassert>
#include <cstring>

#include <iostream>

namespace reference_counted_string {

// The ctor increments ref_cnt_, but the dtor does not decrement it.
class StringObject {
public:
  StringObject() : len_(0), ref_cnt_(1) {
    s_ = new char[1];
    assert(s_ != 0);
    s_[0] = 0;
  }
  StringObject(int n) : len_(n), ref_cnt_(1) {
    s_ = new char[len_ + 1];
    for (int i = 0; i <= n; i++) {
      s_[i] = '\0';
    }
    assert(s_ != 0);
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

  size_t len_;
  int ref_cnt_;
  char *s_;
};

// The CountedString constructor never increments ref_cnt_, as the StringObject
// constructor does that.  It decrements ref_cnt_ upon manual StringObject
// deletion.
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
    ::std::cout << "Copy constructor " << str_->s_ << ::std::endl;
  }
  CountedString(int n) {
    assert(n >= 0);
    str_ = new StringObject(n);
    assert(str_ != 0);
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
  size_t length() { return str_->len_; }
  friend void Swap(CountedString &str1, CountedString &str2);
  friend CountedString operator+(const CountedString &str1,
                                 const CountedString &str2);
  bool operator==(const CountedString &str1);
  void operator=(const CountedString &str);
  char operator[](const int i);
  CountedString operator()(int from, int to);
  operator char *();
  void Print() const { ::std::cout << str_->s_ << ::std::endl; }

private:
  StringObject *str_;
};

CountedString operator+(const CountedString &str1, const CountedString &str2);
void Swap(CountedString &str1, CountedString &str2);
} // namespace reference_counted_string
