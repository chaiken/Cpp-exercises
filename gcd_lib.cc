// The part of gcd that will be tested.
#include <iostream>

using namespace std;

unsigned gcd(unsigned a, unsigned b) {
  unsigned divisor = (a < b) ? a : b;
  unsigned dividend = (a > b) ? a : b;
  unsigned remainder = 1;

  while (divisor > 1) {
    remainder = dividend % divisor;
    if (remainder) {
      dividend = divisor;
      divisor = remainder;
      gcd(divisor, dividend);
    } else {
      break;
    }
  }
  return divisor;
}

unsigned gcd2(unsigned a, unsigned b) {
  unsigned divisor = (a < b) ? a : b;
  unsigned dividend = (a > b) ? a : b;
  unsigned remainder = 1;

  while (divisor > 0) {
    remainder = dividend % divisor;
    if (remainder) {
      dividend = divisor;
      divisor = remainder;
    } else {
      break;
    }
  }
  return divisor;
}
