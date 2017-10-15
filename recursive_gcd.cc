#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

unsigned long calc_gcd(unsigned long input1, unsigned long input2, int cycles) {
  cout << "input1 " << input1 << " input2 " << input2;
  unsigned long remainder = input1 % input2;
  if (!remainder) {
    return input2;
  } else {
    cout << " cycles: " << ++cycles << endl;
    return calc_gcd(input2, remainder, cycles);
  }
}

int main(int argc, char **argv) {
  string line, input1, input2;
  unsigned long operand1, operand2, ans;
  while (1) {
    cin >> input1;
    if (input1 == ".")
      exit(EXIT_SUCCESS);
    cin >> input2;
    operand1 = strtoul(input1.c_str(), NULL, 10);
    assert(operand1 > 0ul);
    operand2 = strtoul(input2.c_str(), NULL, 10);
    assert(operand2 > 0ul);
    ans = calc_gcd(operand1, operand2, 0);
    cout << endl
         << "GCD of " << operand1 << " and " << operand2 << " = " << ans
         << endl << endl;
  }
  exit(EXIT_SUCCESS);
}
