// -*-C++-*-

#ifndef COMPLEX_H
#define COMPLEX_H

#include <cmath>

#include <iostream>

namespace complex_vec {
class ComplexVector;
}

namespace complex {

class Complex;

double Dot(const Complex &x, const Complex &y);

class Complex {
public:
  Complex() : real_(0.0), imag_(0.0) {
#ifdef DEBUG
    ::std::cout << "Default ctor" << ::std::endl;
#endif
  }
  Complex(double r) : real_(r), imag_(0.0) {}
  //  A literal type can be a class type that has all of the following
  //  properties: has a trivial destructor, is either an aggregate type, a type
  //  with at least one constexpr (possibly template) constructor that is not a
  //  copy or move constructor,
  constexpr Complex(double r, double i) : real_(r), imag_(i) {}
  // Need by binary operators.
  Complex(double ri[2]) : real_(ri[0]), imag_(ri[1]) {
#ifdef DEBUG
    ::std::cout << "Array ctor" << ::std::endl;
#endif
  }
  Complex(const Complex &c) : real_(c.real_), imag_(c.imag_) {
#ifdef DEBUG
    ::std::cout << "Copy ctor" << ::std::endl;
#endif
  }
  friend class complex_vec::ComplexVector;
  void assign(double r, double i) {
    real_ = r;
    imag_ = i;
  }

  double DotProduct(const Complex &x) const;
  friend double Dot(const Complex &x, const Complex &y);
  double InnerAngle(Complex &x);
  friend Complex sqrt(const Complex &z);
  // The name of the operator must be double(), as "modulus" is not an operator
  // that can be overridden, and new operators cannot be created.
  operator double() { return (::std::sqrt(real_ * real_ + imag_ * imag_)); }
  // Reverse the vector.
  void operator-();
  Complex &operator=(const Complex &c);
  friend ::std::ostream &operator<<(::std::ostream &out, Complex x);
  bool operator()(Complex &a) {
    ::std::cout << "Complex function object" << ::std::endl;
    return (*this < a);
  }
  // Make +, -, * and == friend functions, as a symmetric invocation for binary
  // operators is most natural.
  friend bool operator<(Complex &a, Complex &b);
  friend Complex operator+(const Complex &x, const Complex &y);
  friend Complex operator+(const Complex &x, const double m);
  friend Complex operator+(const double m, const Complex &x);
  friend Complex operator-(const Complex &x, const Complex &y);
  friend constexpr Complex operator*(const Complex &x, const Complex &y);
  // error: ‘double complex::operator=(double, const complex::Complex&)’ must be
  // a nonstatic member function
  // Overloading operator= requires two operands of the same type.
  // friend double operator=(double d, const Complex &x);
  friend bool operator==(const Complex &x, const Complex &y);
  friend bool operator!=(const Complex &x, const Complex &y);

private:
  double real_, imag_;
};

::std::ostream &operator<<(::std::ostream &out, Complex x);
Complex sqrt(const Complex &z);
bool operator<(Complex &a, Complex &b);
Complex operator+(const Complex &x, const Complex &y);
// Both versions are needed, as addition is commutative, but the compiler won't
// match invocations with reversed parameters.
Complex operator+(const Complex &x, const double m);
Complex operator+(const double m, const Complex &x);
Complex operator-(const Complex &x, const Complex &y);
constexpr Complex operator*(const Complex &x, const Complex &y) {
  return {(x.real_ * y.real_), (x.imag_ * y.imag_)};
};
// error: ‘double complex::operator=(double, const complex::Complex&)’ must be
// a nonstatic member function
// double operator=(double d, const Complex &x);
bool operator==(const Complex &x, const Complex &y);
bool operator!=(const Complex &x, const Complex &y);
} // namespace complex

#endif
