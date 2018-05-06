#include <iostream>
#include <cstdlib>

using namespace std;

int main(void) {

  char c = 'A';
  // implicit cast to bool
  cout << c << " has integer value " << int(c)
       << " and !'A' is " << !c << endl << endl;

  int i = 3, m = 0;
  cout << "i = " << i << ", !i = " << !i << endl;
  cout << "!!i = " << !!i << ", !m = " << !m << endl << endl;

  int j = 1;
  bool p = false, q = true;
  // bools print as (1,0)
  cout << "p is " << p << ", q is " << q << endl;
  cout << "!p = " << !p << ", !q is " << !q << endl << endl;

  // illustrates lazy evaluation
  cout << "!(i+j) || m = " << (!(i+j) || m) << endl;
  cout << "q || (j/m) = " << (q || (j/m)) << endl;
  cout << "(j/m) || q = " << ((j/m) || q);

  exit(EXIT_SUCCESS);
}
