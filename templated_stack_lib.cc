// Any functions that do not refer explicitly to the template parameter can live
// here.

#include "templated_stack.h"

#include <iostream>
#include <string>

using namespace std;

namespace templated_stack {

// Version in text does not compile due to MVP.
void reverse(char *strarr[], int n) {
  // https://en.wikipedia.org/wiki/Most_vexing_parse
  // The following suffers from this compiler error:
  // error: request for member ‘push’ in ‘templated_stack::stk’, which is of
  // non-class type
  // ‘templated_stack::TemplatedStack<std::__cxx11::basic_string<char> >()’
  // TemplatedStack<::std::string> stk();
  // New default constructor: TemplatedStack<char *> stk({}, 0);
  // for (int i = 0; i < n; i++) { stk.push(strarr[i]); }
  // This constructor is the only way I see to initialize a stack of length n.
  TemplatedStack<char *> stk(strarr, n);
  assert(!stk.empty());
  assert(stk.full());
  for (int i = 0; i < n; i++) {
    strarr[i] = stk.pop();
  }
}

} // namespace templated_stack
