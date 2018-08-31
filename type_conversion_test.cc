#include <cstdlib>
#include <iostream>

using namespace std;

int main(void) {
  int i = 3, *p = &i;
  char c = 'b';
  float x = 2.14, *q = &x;

  cout << "i+c " << i+c << endl;
  cout << "x+i " << x+i << endl;
  cout << "p+i " << p+i << endl;
  cout << "p == &i " << (p == &i) << endl;
  cout << "*p - *q " << (*p - *q) << endl;
  cout << "static_cast<int>(x+i) " << static_cast<int>(x+i) << endl;

  exit(EXIT_SUCCESS);
}
