#ifndef TEMPLATE_ROTATE_H
#define TEMPLATE_ROTATE_H

#include <array>

namespace rotate_array {

template <typename T, unsigned long N>
void rotate_array(::std::array<T, N> *arr);

}

#include "template_rotate_impl.h"

#endif
