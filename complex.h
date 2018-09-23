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
  Complex(const Complex &c) {
    real_ = c.real_;
    imag_ = c.imag_;
  }
  friend class complex_vec::ComplexVector;
  void assign(double r, double i) {
    real_ = r;
    imag_ = i;
  }
  void print() const {
    ::std::cout << real_ << " + " << imag_ << "i" << ::std::endl;
  }
  double DotProduct(const Complex &x) const;
  friend double Dot(const Complex &x, const Complex &y);
  double InnerAngle(Complex &x);
  // The name of the operator must be double(), as "modulus" is not an operator
  // that can be overridden, and new operators cannot be created.
  operator double() { return (sqrt(real_ * real_ + imag_ * imag_)); }
  // Reverse the vector.
  void operator-();
  Complex &operator=(const Complex &c);
  friend ::std::ostream &operator<<(::std::ostream &out, Complex x);
  // Make +, -, * and == friend functions, as a symmetric invocation for binary
  // operators is most natural.
  friend Complex operator+(const Complex &x, const Complex &y);
  friend Complex operator+(const Complex &x, const double m);
  friend Complex operator+(const double m, const Complex &x);
  friend Complex operator-(const Complex &x, const Complex &y);
  friend Complex operator*(const Complex &x, const Complex &y);
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
Complex operator+(const Complex &x, const Complex &y);
// Both versions are needed, as addition is commutative, but the compiler won't
// match invocations with reversed parameters.
Complex operator+(const Complex &x, const double m);
Complex operator+(const double m, const Complex &x);
Complex operator-(const Complex &x, const Complex &y);
Complex operator*(const Complex &x, const Complex &y);
// error: ‘double complex::operator=(double, const complex::Complex&)’ must be
// a nonstatic member function
// double operator=(double d, const Complex &x);
bool operator==(const Complex &x, const Complex &y);
bool operator!=(const Complex &x, const Complex &y);
} // namespace complex

#endif
