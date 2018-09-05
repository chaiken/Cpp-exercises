#include "complex.h"

#include <cassert>

#include <iostream>

using namespace std;

namespace complex {

void Complex::operator-() {
  real_ *= -1;
  imag_ *= -1;
  return;
}

ostream &operator<<(ostream &out, Complex x) {
  out << x.real_ << " + " << x.imag_ << "i" << ends;
  return out;
}

double Complex::DotProduct(const Complex &x) const {
  return ((x.real_ * real_) + (x.imag_ * imag_));
}

// Can't be a const function:
// error: invalid cast from type ‘const complex::Complex’ to type ‘double’
//   double dp = DotProduct(x), mod = double(*this);
double Complex::InnerAngle(Complex &x) {
  const double dp = DotProduct(x), mod = double(*this);
  const double xmod = double(x);
  assert(0 != (mod * xmod));
  // If x is outside the range  [-1, 1],  a  domain  error  occurs,  and  a  NaN
  // is returned.
  const double cosine = dp / (mod * xmod);
  double fractional = cosine;
  if (cosine > 1) {
    fractional = cosine - (static_cast<int>(cosine) % 1);
  }
  if (cosine < -1) {
    fractional = cosine + (static_cast<int>(cosine) % 1);
  }
  assert((fractional >= -1) && (fractional <= 1));
  const double ans = acos(fractional);
  return ans;
}

// These next two binary operator rely on the Complex(double ri[2]) constructor
// to convert their return values to a Complex object.
Complex operator+(const Complex &x, const Complex &y) {
  double coeffs[2];
  coeffs[0] = x.real_ + y.real_;
  coeffs[1] = x.imag_ + y.imag_;
  return coeffs;
}

Complex operator-(const Complex &x, const Complex &y) {
  double coeffs[2];
  coeffs[0] = x.real_ - y.real_;
  coeffs[1] = x.imag_ - y.imag_;
  return coeffs;
}

bool operator==(const Complex &x, const Complex &y) {
  return ((x.real_ == y.real_) && (x.imag_ == y.imag_));
}

Complex operator*(const Complex &x, const Complex &y) {
  Complex product(x.real_ * y.real_, x.imag_ * y.imag_);
  return product;
}

Complex operator+(const Complex &x, const double m) {
  Complex sum(x.real_ + m, x.imag_ + m);
  return sum;
}

Complex operator+(const double m, const Complex &x) { return (x + m); }

} // namespace complex
