#include "complex.h"

#include <cassert>
#include <cmath>

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

double Dot(const Complex &x, const Complex &y) {
  return ((x.real_ * y.real_) + (x.imag_ * y.imag_));
}

// "If the function had been written to return void, it would not have allowed
// multiple assignment." Namely, a=b=c would not then work.
Complex &Complex::operator=(const Complex &x) {
  assert(nullptr != this);
  if (this != &x) {
    real_ = x.real_;
    imag_ = x.imag_;
  }
  return *this;
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
  return {(x.real_ + y.real_), (x.imag_ + y.imag_)};
}

Complex operator-(const Complex &x, const Complex &y) {
  return {(x.real_ - y.real_), (x.imag_ - y.imag_)};
}

bool operator==(const Complex &x, const Complex &y) {
  return ((x.real_ == y.real_) && (x.imag_ == y.imag_));
}

bool operator!=(const Complex &x, const Complex &y) {
  return ((x.real_ != y.real_) || (x.imag_ != y.imag_));
}

// constexpr functions, since they are inlined, must be defined in the header.
// Otherwise,
//  inline function ‘constexpr complex::Complex complex::operator*(const
//  complex::Complex&, const complex::Complex&)’ used but never defined
// constexpr Complex operator*(const Complex &x, const Complex &y) {
//  return {(x.real_ * y.real_), (x.imag_ * y.imag_)};
//}

Complex operator+(const Complex &x, const double m) {
  return {(x.real_ + m), (x.imag_ + m)};
}

Complex operator+(const double m, const Complex &x) { return (x + m); }

// https://math.stackexchange.com/questions/44406/how-do-i-get-the-square-root-of-a-complex-number
// $(r(\cos(\theta)+ i \sin(\theta)))^{1/2} = ±\sqrt{r}(\cos(\theta/2) + i
// \sin(\theta/2))$
Complex sqrt(const Complex &z) {
  double coeffs[]{0.0, 0.0};
  // NaN.
  if ((0.0 == z.imag_) && (z.real_ < 0)) {
    return coeffs;
  }
  // z = r*cos(theta) + i* r*sin(theta)
  double r = ::std::sqrt((z.real_ * z.real_) + (z.imag_ * z.imag_));
  if (r != 0.0) {
    double theta = asin(z.imag_ / r);
    coeffs[0] = ::std::sqrt(r) * cos(theta / 2.0);
    coeffs[1] = ::std::sqrt(r) * sin(theta / 2.0);
  }
  return coeffs;
}

// error: ‘double complex::operator=(double, const complex::Complex&)’ must be a
// nonstatic member function
// double operator=(double d, const Complex &x) {
//  return x.real_;
// }

} // namespace complex
