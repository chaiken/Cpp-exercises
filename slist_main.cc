#include "slist.h"

#include <iostream>

using namespace std;
using namespace slist;

int main(void) {
  SingleLinkList *p;
  {
    SingleLinkList w;
    w.Prepend('A');
    w.Prepend('B');
    w.Print(w.first());
    w.Delete();
    w.Print(w.first());
    p = &w;
    p->Print(w.first());
    cout << "Exiting inner block." << endl;
  }
  // Undefined behavior: use after free.
  // p->Print();
  cout << "Exiting outer block." << endl;
}
