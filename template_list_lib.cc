#include "template_list.h"

namespace template_list {
namespace {
long int GetRand() { return (random() % (ELEMNUM - 1)); }
} // namespace

void FillList(::std::list<long int> *elemlist) {
  for (int i = 0; i < ELEMNUM; i++) {
    elemlist->emplace_back(GetRand());
  }
}

void FillTrivialList(::std::list<int> *elemlist) {
  for (int i = 0; i < ELEMNUM; i++) {
    elemlist->emplace_back(i);
  }
}

} // namespace template_list
