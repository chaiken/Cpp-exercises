// -*-C++-*-
// Input to https://godbolt.org/z/ft5wjR.   The output is in
// macro-vs-template_Compiler-Explorer.S.
//
// About QWORD:
// https://stackoverflow.com/questions/7865511/qword-storing-implementing-using-32-bit-regs
//
// Note in macro-vs-template_Compiler-Explorer.S that stupidmacro() calls sqrt()
// three times and operator*() twice, while constexprversion() calls sqrt() once
// and cubeme() once, and cubeme() then calls operator*() twice.  The reduction
// in calls to sqrt() with the constexpr implementation is a significant savings
// given that sqrt() calls the array constructor.

#include <cmath>
#include <iostream>

class Complex {
public:
  constexpr Complex(double r, double i) : real_(r), imag_(i) {}
  Complex(double ri[2]) : real_(ri[0]), imag_(ri[1]) {}
  friend Complex sqrt(const Complex &z);
  friend constexpr Complex operator*(const Complex &x, const Complex &y);

private:
  double real_, imag_;
};

constexpr Complex operator*(const Complex &x, const Complex &y) {
  return {(x.real_ * y.real_), (x.imag_ * y.imag_)};
};

#define CUBE(X) ((X) * (X) * (X))
constexpr Complex cubeme(const Complex &x) { return x * x * x; }

Complex sqrt(const Complex &z) {
  double coeffs[]{0.0, 0.0};
  if ((0.0 == z.imag_) && (z.real_ < 0)) {
    return coeffs;
  }
  double r = ::std::sqrt((z.real_ * z.real_) + (z.imag_ * z.imag_));
  if (r != 0.0) {
    double theta = asin(z.imag_ / r);
    coeffs[0] = ::std::sqrt(r) * cos(theta / 2.0);
    coeffs[1] = ::std::sqrt(r) * sin(theta / 2.0);
  }
  return coeffs;
}

void stupidmacro(void) {
  Complex cn(27.0, 13.5);
  CUBE(sqrt(cn));
}

void constexprversion(void) {
  Complex cn(27.0, 13.5);
  cubeme(sqrt(cn));
}
