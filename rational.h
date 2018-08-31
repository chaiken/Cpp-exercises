#include <iostream>

namespace rational {

constexpr int kMult = 100;

class Rational {
public:
  Rational(int n = 0) : a_(n), q_(1) {}
  Rational(int i, int j) : a_(i), q_(j) {}
  Rational(double r) : a_(r * kMult), q_(kMult) {}
  void Print() const { ::std::cout << a_ << "/" << q_ << ::std::endl; }
  // double() rather than Double(), as otherwise there is no overload.
  // clang-format off
  /* An error without const:
     error: no match for ‘operator==’ (operand types are ‘const rational::Rational’ and ‘const rational::Rational’)
     if (lhs == rhs) {
       ~~~~^~~~~~
     /home/alison/gitsrc/googletest/googletest/include/gtest/gtest.h:1421:11: note: candidate: ‘operator==(double, double)’ <built-in>
     /home/alison/gitsrc/googletest/googletest/include/gtest/gtest.h:1421:11: note: conversion of argument 2 would be ill-formed:
     /home/alison/gitsrc/googletest/googletest/include/gtest/gtest.h:1421:11: error: passing ‘const rational::Rational’ as ‘this’ argument discards qualifiers [-fpermissive]
     In file included from rational_lib_test.cc:1:
     rational.h:15:3: note:   in call to ‘rational::Rational::operator double()’
     operator double() {
  */
  // clang-format on
  // static_cast<double>() shown in text is not needed.
  operator double() const {
    ::std::cout << "Calling double() with " << a_ << "," << q_ << ::std::endl;
    return a_ / q_;
  }

private:
  long a_, q_;
};

inline int Greater(int i, int j) { return (i > j ? i : j); }
inline double Greater(double x, double y) { return (x > y ? x : y); }
/* Without a definition for double():
   rational.h: In function ‘rational::Rational rational::Greater(rational::Rational, rational::Rational)’:
   rational.h:24:61: error: no match for ‘operator>’
   (operand types are ‘rational::Rational’ and ‘rational::Rational’)
   inline Rational Greater(Rational w, Rational z) { return (w > z ? w : z); }
*/
inline Rational Greater(Rational w, Rational z) { return (w > z ? w : z); }

} // namespace rational
