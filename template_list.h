#ifndef TEMPLATE_LIST_H
#define TEMPLATE_LIST_H

#include "complex.h"

#include <list>

namespace template_list {

void FillList(std::list<long int> &elemlist);
void FillTrivialList(std::list<int> &elemlist);
void FillComplexListTrivial(std::list<complex::Complex> &elemlist);

} // namespace template_list
#include "template_list_impl.h"

#endif
