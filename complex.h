// -*-C++-*-

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
  friend ::std::ostream &operator<<(::std::ostream &out, Complex x);

private:
  double real_, imag_;
};

::std::ostream &operator<<(::std::ostream &out, Complex x);
} // namespace complex
