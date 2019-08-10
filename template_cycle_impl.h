#include "template_cycle.h"

#include <utility>

namespace tcycle {

// For simple types, simply copy the contents.
// For complex types, we should move the contents.
template <typename T> void cycle(T &a, T &b, T &c) {
  T temp = a;
  a = b;
  b = c;
  c = temp;
}

template <typename T> void move_cycle(T &a, T &b, T &c) {
  T temp(::std::move(a));
  a = ::std::move(b);
  b = ::std::move(c);
  c = ::std::move(temp);
}

} // namespace tcycle
