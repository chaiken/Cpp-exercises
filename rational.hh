/* -*-c++-*- */
#include <iostream>

namespace rational {

constexpr int kMult = 100;

int GCD(int i, int j);

class Rational {
public:
  Rational(int n = 0) : numerator_(n), denominator_(1) {}
  Rational(int i, int j);
  Rational(double r) : numerator_(r * kMult), denominator_(kMult) {}
  void Print() const {
    ::std::cout << numerator_ << "/" << denominator_ << ::std::endl;
  }
  // double() rather than Double(), as otherwise there is no overload.
  // clang-format off
  /* An error without const:
     error: no match for ‘operator==’ (operand types are ‘const rational::Rational’ and ‘const rational::Rational’)
     if (lhs == rhs) {
       ~~~~^~~~~~
     /home/alison/gitsrc/googletest/googletest/include/gtest/gtest.hh:1421:11: note: candidate: ‘operator==(double, double)’ <built-in>
     /home/alison/gitsrc/googletest/googletest/include/gtest/gtest.hh:1421:11: note: conversion of argument 2 would be ill-formed:
     /home/alison/gitsrc/googletest/googletest/include/gtest/gtest.hh:1421:11: error: passing ‘const rational::Rational’ as ‘this’ argument discards qualifiers [-fpermissive]
     In file included from rational_lib_test.cc:1:
     rational.hh:15:3: note:   in call to ‘rational::Rational::operator double()’
     operator double() {
  */
  // clang-format on
  // static_cast<double>() shown in text is not needed.
  operator double() const {
    ::std::cout << "Calling double() with " << numerator_ << "," << denominator_
                << ::std::endl;
    return numerator_ / denominator_;
  }
  // Since double() is a conversion, it doesn't need an explicit return type. It
  // seems odd that == does.
  bool operator==(const Rational &r) const {
    return ((r.numerator_ == numerator_) && (r.denominator_ == denominator_));
  }
  bool operator>(const Rational &r) const;
  bool operator<(const Rational &r) const;

private:
  long numerator_, denominator_;
};

inline int Greater(int i, int j) { return (i > j ? i : j); }
inline double Greater(double x, double y) { return (x > y ? x : y); }
/* Without a definition for double():
   rational.hh: In function ‘rational::Rational
   rational::Greater(rational::Rational, rational::Rational)’: rational.hh:24:61:
   error: no match for ‘operator>’ (operand types are ‘rational::Rational’ and
   ‘rational::Rational’) inline Rational Greater(Rational w, Rational z) {
   return (w > z ? w : z); }
*/
inline Rational Greater(Rational w, Rational z) { return (w > z ? w : z); }

} // namespace rational
