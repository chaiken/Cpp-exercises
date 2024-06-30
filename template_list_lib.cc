#include "template_list.hh"

#include "complex.hh"

using namespace std;

namespace template_list {
namespace {
long int GetRand() { return (random() % (ELEMNUM - 1)); }
} // namespace

void FillList(list<long int> &elemlist) {
  for (int i = 0; i < ELEMNUM; i++) {
    elemlist.emplace_back(GetRand());
  }
}

void FillTrivialList(list<int> &elemlist) {
  for (int i = 0; i < ELEMNUM; i++) {
    elemlist.emplace_back(i);
  }
}

void FillComplexListTrivial(list<complex::Complex> &elemlist) {
  for (int i = 0; i < SMALLNUM; i++) {
    complex::Complex elem(i % 7, i % 7);
    elemlist.emplace_back(elem);
  }
}

} // namespace template_list
