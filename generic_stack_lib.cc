#include "generic_stack.hh"

#include <ostream>

using namespace std;

namespace generic_stack {

ostream &operator<<(ostream &out, const GenericStack &astack) {
  GenericStack copy(astack);
  while (!copy.empty()) {
    out << copy.DoublePop();
    out << ", ";
  }
  out << endl;
  return out;
}

// warning: pointer of type ‘void *’ used in arithmetic [-Wpointer-arith]
// Clearly these functions need to be templated, but that's the next chapter.
/* void GenericStack::Push(void *datum) {
  if ((!datum) || full()) {
    return;
  }
  // error: ‘void*’ is not a pointer-to-object type
  arr_[top_++] = *datum;
}

void *GenericStack::Pop() {
  if (empty()) {
    return nullptr;
  }
  // error: ‘void*’ is not a pointer-to-object type
  return &(arr_[top_--]);
}
*/

void GenericStack::DoublePush(double datum) {
  if (full()) {
    return;
  }
  arr_[top_++] = static_cast<void *>(&datum);
}

double GenericStack::DoublePop() {
  double val = static_cast<double>(*arr_[top_]);
  top_--;
  return val;
}

void Reverse(double data[], int size) {
  GenericStack astack(data, size);
  int i = 0;
  while (!astack.full()) {
    astack.DoublePush(static_cast<void *>(&data[i]));
    i++;
  }
  while (!astack.empty()) {
    data[i] = astack.DoublePop();
  }
}

} // namespace generic_stack
