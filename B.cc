#include <iostream>

using namespace std;

int goo(int i);

int foo(int i) { return (i * 5); }

int main(void) {
  cout << "foo(5): " << foo(5) << endl;
  cout << "goo(5): " << goo(5) << endl;
}
