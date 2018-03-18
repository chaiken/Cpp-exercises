#include <cmath>
#include <iostream>

using namespace std;

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

int main(void) {
  int digitnum = NumDigits(150);
  cout << "150 has " << digitnum << " digits." << endl;
  digitnum = NumDigits(-150);
  cout << "-150 has " << digitnum << " digits." << endl;
  exit(EXIT_SUCCESS);
}
