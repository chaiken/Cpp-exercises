#include "complex.hh"

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

bool operator<(const Complex &a, const Complex &b) {
#ifdef DEBUG
  cout << "Complex comparison override" << endl;
#endif
  // Avoid calling self!
  return ((a.ComplexModulus() - b.ComplexModulus()) < 0);
}

bool operator<(Complex &a, Complex &b) {
#ifdef DEBUG
  cout << "Complex comparison override" << endl;
#endif
  // Avoid calling self!
  return ((a.ComplexModulus() - b.ComplexModulus()) < 0);
}

ostream &operator<<(ostream &out, Complex x) {
  out << x.real_ << " + " << x.imag_ << "i";
  return out;
}

double DotProduct(const Complex &x, const Complex &y) {
  return ((x.real() * y.real()) + (x.imaginary() * y.imaginary()));
}

double Dot(const Complex &x, const Complex &y) {
  return ((x.real() * y.real()) + (x.imaginary() * y.imaginary()));
}

// "If the function had been written to return void, it would not have allowed
// multiple assignment." Namely, a=b=c would not then work.
Complex &Complex::operator=(const Complex &x) {
  if (this != &x) {
    real_ = x.real_;
    imag_ = x.imag_;
  }
  return *this;
}

double InnerAngle(const Complex &x, const Complex &y) {
  const double dp = DotProduct(x, y);
  const double xmod = x.ComplexModulus(), ymod = y.ComplexModulus();
  assert(0 != (xmod * ymod));
  // If x is outside the range  [-1, 1],  a  domain  error  occurs,  and  a  NaN
  // is returned.
  const double cosine = dp / (xmod * ymod);
  assert((cosine >= -1) && (cosine <= 1));
  return acos(cosine);
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
pair<Complex, bool> sqrt(const Complex &z) {
#ifdef DEBUG
  cout << "Complex sqrt override" << endl;
#endif
  double coeffs[]{0.0, 0.0};
  // NaN.
  if ((0.0 == z.imag_) && (z.real_ < 0)) {
    cerr << "Invalid argument " << z << endl;
    return pair<Complex, bool>{z, false};
  }
  // z = r*cos(theta) + i* r*sin(theta)
  // Without ::std:: prefix, compiler cannot decide whether the function is
  // recursive.
  double r = ::std::sqrt((z.real_ * z.real_) + (z.imag_ * z.imag_));
  if (r != 0.0) {
    double theta = asin(z.imag_ / r);
    coeffs[0] = ::std::sqrt(r) * cos(theta / 2.0);
    coeffs[1] = ::std::sqrt(r) * sin(theta / 2.0);
  }
  return pair<Complex, bool>{Complex(coeffs), true};
}

// error: ‘double complex::operator=(double, const complex::Complex&)’ must be a
// nonstatic member function
// double operator=(double d, const Complex &x) {
//  return x.real_;
// }

} // namespace complex
