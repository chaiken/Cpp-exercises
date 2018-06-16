#include <iostream>

using namespace std;

void foo(int n) {
  static int ctr = 0;
  ctr++;
  cout << "n: " << n << endl;

  if (n <= 1) {
    cout << "Count is " << ctr << endl;
    return;
  } else {
    foo(n / 3);
  }
}

void foo2(int n, int ctr) {
  ctr++;
  if (n <= 1) {
    cout << "Count is " << ctr << endl;
    return;
  } else {
    foo2(n / 3, ctr);
  }
}

int main(void) {
  foo(21);
  foo(27);
  foo(243);
  foo2(21, 0);
  foo2(27, 0);
  foo2(243, 0);
  exit(0);
}
