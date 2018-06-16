#include <iostream>

using namespace std;

unsigned gcd(unsigned a, unsigned b) {
  unsigned r;
  unsigned lesser = (a < b) ? a : b;
  unsigned greater = (a > b) ? a : b;

  while (lesser > 0) {
    r = greater % lesser;
    if (r)
      lesser = r;
    else
      break;
  }
  return lesser;
}

int main() {
  unsigned count, a, b;
  cout << "How many GCDs?" << endl;
  cin >> count;
  while (count-- > 0) {
    cout << "First?" << endl;
    cin >> a;
    cout << "Second?" << endl;
    cin >> b;
    if ((a * b) == 0)
      throw new exception();
    else
      cout << "gcd(a,b) is " << gcd(a, b) << endl;
  }
}
