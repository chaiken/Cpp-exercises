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

} // namespace complex
