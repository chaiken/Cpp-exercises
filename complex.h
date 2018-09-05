// -*-C++-*-

#ifndef COMPLEX_H
#define COMPLEX_H

#include <cmath>

#include <iostream>

namespace complex {

class Complex {
public:
  Complex() {
    real_ = 0;
    imag_ = 0;
  }
  Complex(double r) {
    real_ = r;
    imag_ = 0;
  }
  Complex(double r, double i) {
    real_ = r;
    imag_ = i;
  }
  // Need by binary operators.
  Complex(double ri[2]) {
    real_ = ri[0];
    imag_ = ri[1];
  }
  void assign(double r, double i) {
    real_ = r;
    imag_ = i;
  }
  void print() const {
    ::std::cout << real_ << " + " << imag_ << "i" << ::std::endl;
  }
  double DotProduct(const Complex &x) const;
  double InnerAngle(Complex &x);
  // The name of the operator must be double(), as "modulus" is not an operator
  // that can be overridden, and new operators cannot be created.
  operator double() { return (sqrt(real_ * real_ + imag_ * imag_)); }
  // Reverse the vector.
  void operator-();
  // Not needed: compiler-generated assignment operator works.
  // Complex& operator=(const Complex& c);
  friend ::std::ostream &operator<<(::std::ostream &out, Complex x);
  // Make +, -, * and == friend functions, as a symmetric invocation for binary
  // operators is most natural.
  friend Complex operator+(const Complex &x, const Complex &y);
  friend Complex operator-(const Complex &x, const Complex &y);
  friend Complex operator*(const Complex &x, const Complex &y);
  friend bool operator==(const Complex &x, const Complex &y);

private:
  double real_, imag_;
};

::std::ostream &operator<<(::std::ostream &out, Complex x);
Complex operator+(const Complex &x, const Complex &y);
Complex operator-(const Complex &x, const Complex &y);
Complex operator*(const Complex &x, const Complex &y);
bool operator==(const Complex &x, const Complex &y);
} // namespace complex

#endif
