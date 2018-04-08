// The part of gcd that will be tested.
#include <iostream>

using namespace std;

unsigned gcd(unsigned a, unsigned b) {
  unsigned lesser = (a < b) ? a : b;
  unsigned greater = (a > b) ? a : b;
  unsigned r = 1;

  while (lesser > 0) {
    r = greater % lesser;
    if (r)
      lesser = r;
    else
      break;
  }
  return lesser;
}

unsigned gcd2(unsigned a, unsigned b) {
  unsigned lesser = (a < b) ? a : b;
  unsigned greater = (a > b) ? a : b;
  unsigned r = 1;

  while (lesser > 0) {
    r = greater % lesser;
    if (r)
      lesser = r;
    else
      break;
  }
  return lesser;
}
