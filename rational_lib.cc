#include "rational.h"

using namespace std;

namespace rational {

namespace {
int max_absv(int i, int j) {
  int a = abs(i);
  int b = abs(j);
  return (a >= b ? a : b);
}

int min_absv(int i, int j) {
  int a = abs(i);
  int b = abs(j);
  return (a >= b ? b : a);
}

} // namespace

int GCD(int i, int j) {
  if ((0 == i) || (0 == j)) {
    return 1;
  }
  if (i == j) {
    return i;
  }
  int divided = max_absv(i, j), divisor = min_absv(i, j);
  int ratio = divided / divisor;
  int remainder = divided - (ratio * divisor);
  if (0 == remainder) {
    return divisor;
  } else {
    return GCD(divided, remainder);
  }
}

Rational::Rational(int i, int j) {
  if (0 == j) {
    throw invalid_argument("0 is an illegal denominator value.");
  }
  int gcd = GCD(i, j);
  numerator_ = i / gcd;
  denominator_ = j / gcd;
}

bool Rational::operator<(const Rational &r) const {
  return ((numerator_ / denominator_) < (r.numerator_ / r.denominator_));
}

bool Rational::operator>(const Rational &r) const {
  return ((numerator_ / denominator_) > (r.numerator_ / r.denominator_));
}

} // namespace rational
