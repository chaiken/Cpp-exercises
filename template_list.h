#ifndef TEMPLATE_LIST_H
#define TEMPLATE_LIST_H

#include <list>

namespace template_list {

void FillList(::std::list<long int> *elemlist);
void FillTrivialList(::std::list<int> *elemlist);

} // namespace template_list
#include "template_list_impl.h"

#endif
