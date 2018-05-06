#include <cstdlib>
#include <iostream>

#include "dyn_string.h"

using namespace std;

const ::std::string kTestString = "Strings are cheap.";

int main() {
  dyn_string::DynString a(kTestString.c_str());
  a.print();
  a.print(3);
  exit(EXIT_SUCCESS);
}
