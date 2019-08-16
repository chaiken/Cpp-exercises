#include <array>
#include <iostream>

namespace rotate_array {

template <typename T, unsigned long N>
void rotate_array(::std::array<T, N> *arr) {
  // https://stackoverflow.com/questions/20866892/c-iterator-with-template
  // error: need ‘typename’ before ‘std::array<T, N>::iterator’ because
  // ‘std::array<T, N>’ is a dependent scope
  // ::std::array<T, N>::iterator it;
  /* clang-format off
  it++ here is a No-op: why?  *it = arr[0] and doesn't change.
  unsigned long i = 1;
  typename ::std::array<T, N>::iterator it;
  for (it = arr->begin(); i < N; ++it, i++) {
    (*arr)[i] = *it;
  }
  The following causes stack overflow since the condition is never satisfied:
  for (unsigned long i=1; it != arr->end(); it++, i++) {
  The typename statement creates a type which is a point to arr[0], not an iterator.

  Another approach with same problem:
  auto it = ::std::begin(*arr);
  for (; i < N; ++it, i++) {
    (*arr)[i] = *it;
  }
  */
  // clang-format on
  T temp = (*arr)[N - 1];
  for (long unsigned i = N - 1; i > 0; i--) {
    (*arr)[i] = (*arr)[i - 1];
  }
  (*arr)[0] = temp;
}

} // namespace rotate_array
