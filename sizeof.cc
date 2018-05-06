#include <iostream>

using namespace std;

enum bounds { lb = -1, ub = 511 };
enum suit { clubs, diamonds, hearts, spades };

int main(void) {
  cout << "bool: " << sizeof(bool) << " char: " << sizeof(char) << endl;
  cout << "short: " << sizeof(short) << " int: " << sizeof(int) << endl;
  cout << "long: " << sizeof(long) << " float: " << sizeof(float) << endl;
  cout << "double: " << sizeof(double) << " long double: " << sizeof(long double) << endl;
  cout << "bounds: " << sizeof(bounds) << " suit: " << sizeof(suit) << endl;

  exit(0);
}
