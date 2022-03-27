#include <iostream>

#include "gcd.h"

using namespace std;

int main() {
  cout << "gcd(100, 8) is " << gcd(100, 8) << endl;
  cout << "gcd(35, 8) is " << gcd(35, 8) << endl;
  cout << "gcd(1, 117) is " << gcd(1, 117) << endl;

  cout << "gcd2(100, 8) is " << gcd2(100, 8) << endl;
  cout << "gcd2(35, 8) is " << gcd2(35, 8) << endl;
  cout << "gcd2(1, 117) is " << gcd2(1, 117) << endl;
}
