#include "rational.h"

#include <cassert>

using namespace std;

namespace rational {

namespace {
int max(int i, int j) { return (i >= j ? i : j); }

int min(int i, int j) { return (i >= j ? j : i); }

} // namespace

int GCD(int i, int j) {
  if ((0 == i) || (0 == j)) {
    return 0;
  }
  if (i == j) {
    return i;
  }
  int divided = max(i, j), divisor = min(i, j);
  int ratio = divided / divisor;
  int remainder = divided - (ratio * divisor);
  if (0 == remainder) {
    return divisor;
  } else {
    return GCD(divided, remainder);
  }
}

Rational::Rational(int i, int j) {
  int gcd = GCD(i, j);

  // Don't see how this could happen.
  if (0 == gcd) {
    assert_perror(EINVAL);
  }
  a_ = i / gcd;
  q_ = j / gcd;
}

bool Rational::operator<(const Rational &r) const {
  assert(r.q_ != 0);
  assert(q_ != 0);
  return ((a_ / q_) < (r.a_ / r.q_));
}

bool Rational::operator>(const Rational &r) const {
  assert(r.q_ != 0);
  assert(q_ != 0);
  return ((a_ / q_) > (r.a_ / r.q_));
}

} // namespace rational
