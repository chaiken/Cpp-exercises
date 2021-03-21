#include <cstdlib>
#include <iostream>

#include "dyn_string.h"

using namespace std;

const ::std::string kTestString = "Strings are cheap.";

int main() {
  dyn_string::DynString a(kTestString.c_str());
  cout << a << endl;
  print_some(cout, a, 3);
  cout << endl;
  exit(EXIT_SUCCESS);
}
