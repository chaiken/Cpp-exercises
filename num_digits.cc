#include <cmath>

int NumDigits(int num) {
  if (0 == num)
    return 1;
  double doubnum = static_cast<double>(num);
  if (num < 0) {
    // Add a digit for the minus sign.
    return (1 + ceil(log10(-1.0 * doubnum)));
  }
  return (ceil(log10(doubnum)));
}
