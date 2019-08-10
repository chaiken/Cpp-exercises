#ifndef TEMPLATE_CYCLE_H
#define TEMPLATE_CYCLE_H

namespace tcycle {

template <typename T> void cycle(T &a, T &b, T &c);
template <typename T> void move_cycle(T &a, T &b, T &c);

} // namespace tcycle

#include "template_cycle_impl.h"

#endif
