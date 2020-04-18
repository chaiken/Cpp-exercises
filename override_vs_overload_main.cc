#include "override_vs_overload.h"

#include <cstdlib>

using namespace override_overload;
using namespace std;

int main() {
  B b1, b2(10), *pb;
  D d1, d2(10), *pd = &d2;

  b1.print();          // result: "i = 0"
  b2.print();          // result: "i = 10"
  d1.print();          // result: "i = 0", as D's i member masks B's.
  d2.print();          // result: "i = 10"
  b1.print("b1.i = "); // result: "b1.i = 0"
  b2.print("b2.i = "); // result: "b2.i = 10"
  d1.print("d1.i = "); // result: "d1.i = 0"
  d2.print("d2.i = "); // result: "d2.i = 10"

  // static_cast() makes no difference, as expected.
  // pb = static_cast<B*>(pd);
  pb = pd;
  pb->print(); // result: "i = 10". We get D's print(). An override.
  // Here the base class printer runs, as the base class' print(string) is not
  // marked virtual.  Thus there is no dynamic lookup of the type of the object
  // to which the pointer refers and a B* ptr invokes the behavior of the base
  // class.
  pb->print("d2.i = "); // result: "d2.i = 5"
  pd->print();          // result: "i = 10"
  pd->print("d2.i = "); // result: "d2.i = 10", unchanged

  // There is no way to force the derived class behavior with the return value.
  // error: void value not ignored as it ought to be
  // 30 |   int j = pb->print("d2.i = ");
  // int j = pb->print("d2.i = ");
  int j = pd->print("d2.i = ");
  cout << "j is " << j << endl;

  exit(EXIT_SUCCESS);
}
