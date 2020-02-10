#ifndef ONE_INDEX_VECTOR_H
#define ONE_INDEX_VECTOR_H

#include <cassert>
#include <climits>

#include <iostream>
#include <vector>

namespace one_index_vector {

template <typename T> class OneIndexVector : public ::std::vector<T> {
public:
  // Storage allocation is performed by the base class, so none is needed here.
  OneIndexVector() : std::vector<T>() {}
  OneIndexVector(size_t n) : std::vector<T>(n) {}
  OneIndexVector(size_t s, T val) : std::vector<T>(s, val) {}
  // Causes GCC overload resolution to fail.
  // OneIndexVector(::std::vector<T> vec) : std::vector<T>(vec) {}
  template <typename ForwardIterator>
  OneIndexVector(ForwardIterator first, ForwardIterator last)
      : std::vector<T>(first, last) {}

  OneIndexVector(::std::vector<T> &&vec);
  OneIndexVector(const ::std::vector<T> &vec) = delete;
  OneIndexVector &operator=(std::vector<T> &&vec);
  ~OneIndexVector() {}

  // ULONG_MAX is what std::vector::size() returns for a default-constructed
  // vector.
  bool empty() const {
    return ((0 == std::vector<T>::size()) ||
            (ULONG_MAX == std::vector<T>::size()));
  }
  // Only the member functions that know about indices need to be overridden.
  T &at(size_t n) {
    if ((n < 1u) || (n > 1 + std::vector<T>::size())) {
      std::cerr << "Index " << n << " out of range." << std::endl;
      assert_perror(EINVAL);
    }
    return std::vector<T>::at(n - 1);
  }
  const T &at(size_t n) const {
    if ((n < 1u) || (n > 1 + std::vector<T>::size())) {
      std::cerr << "Index " << n << " out of range." << std::endl;
      assert_perror(EINVAL);
    }
    return std::vector<T>::at(n - 1);
  }
  T &operator[](size_t n) {
    if ((n < 1u) || (n > 1 + std::vector<T>::size())) {
      std::cerr << "Index " << n << " out of range." << std::endl;
      assert_perror(EINVAL);
    }
    return std::vector<T>::operator[](n - 1);
  }
  const T &operator[](size_t n) const {
    if ((n < 1u) || (n > 1 + std::vector<T>::size())) {
      std::cerr << "Index " << n << " out of range." << std::endl;
      assert_perror(EINVAL);
    }
    return std::vector<T>::operator[](n - 1);
  }
};

} // namespace one_index_vector

#include "one_index_vector_impl.h"

#endif
