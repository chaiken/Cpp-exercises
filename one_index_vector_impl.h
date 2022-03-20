#include <cassert>

namespace one_index_vector {
template <typename T> OneIndexVector<T>::OneIndexVector(std::vector<T> &&vec) {
  ::std::cout << "local move constructor" << ::std::endl;
  std::swap(*this, vec);
}

template <typename T>
OneIndexVector<T> &OneIndexVector<T>::operator=(std::vector<T> &&vec) {
  std::cout << "local move assignment" << std::endl;
  std::swap(*this, vec);
  return *this;
}

} // namespace one_index_vector
