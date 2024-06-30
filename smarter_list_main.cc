#include "smarter_list.hh"

#include <cassert>
#include <cstdlib>

#include <iostream>

using namespace smarter_list;
using namespace std;

int main(void) {
  SmarterList sl({"A", "B", "C", "D", "E", "F"});
  cout << "main: third element: " << (sl)[0].next->next->name << endl;
  cout << "main: third element: " << (sl)[2].name << endl;
  assert(nullptr != sl.begin());
  cout << sl;
  exit(EXIT_SUCCESS);
}
