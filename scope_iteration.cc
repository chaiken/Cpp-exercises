// Exercise 17, p. 64

#include <iostream>
#include <cstdlib>

using namespace std;

int test_func(void) {
  return (modulus(rand(), 2);
}

int main(void) {

  if ((bool d = test_func()))
    cout << "bool d is odd: " << d << endl;
  else
    cout << "bool d is even: " << d << endl;

  exit(EXIT_SUCCESS);
}
