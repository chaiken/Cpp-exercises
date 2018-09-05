#include "complex.h"

#include <iostream>

using namespace std;

namespace complex {

double Complex::DotProduct(const Complex &x) {
  return ((x.real_ * real_) + (x.imag_ * imag_));
}

void Complex::operator-() {
  real_ *= -1;
  imag_ *= -1;
  return;
}

ostream &operator<<(ostream &out, Complex x) {
  out << x.real_ << " + " << x.imag_ << "i" << ends;
  return out;
}

} // namespace complex
