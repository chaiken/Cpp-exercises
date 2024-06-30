#ifndef ONE_INDEX_VECTOR_H
#define ONE_INDEX_VECTOR_H

#include <climits>

#include <iostream>
#include <vector>

namespace one_index_vector {

template <typename T> struct OneIndexVector : public ::std::vector<T> {
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

  // ULONG_MAX is what std::vector::size() returns for a default-constructed
  // vector.
  bool empty() const {
    return ((0 == std::vector<T>::size()) ||
            (ULONG_MAX == std::vector<T>::size()));
  }

  // Return an error indication or the expected value in a pair.
  std::pair<bool, T &> make_result(bool b, T &res, size_t idx = 0) {
    if (!b) {
      std::cerr << "Index " << idx << " out of range." << std::endl;
      return std::pair<bool, T &>(false, res);
    }
    return std::pair<bool, T &>(true, res);
  }
  // Only the member functions that know about indices need to be overridden.
  // std:vector's element access functions work for indices 0 to size()-1.
  std::pair<bool, T &> at(size_t n) {
    if ((n < 1u) || (n > std::vector<T>::size())) {
      return make_result(false, *(std::vector<T>::end()), n);
    }
    return make_result(true, std::vector<T>::at(n - 1));
  }
  // https://en.cppreference.com/w/cpp/language/override
  // clang-format off
  /*
   * In a member function declaration or definition, override ensures that the
   * function is virtual and is overriding a virtual function from a base class.
   *
   *  ‘const T& one_index_vector::OneIndexVector<T>::at(size_t) const [with T = int; size_t = long unsigned int]’ marked ‘override’, but does not override
   *   const T &at(size_t n) const override {
   *
   * Override fails since the STL functions are not marked virtual.
   */
  // clang-format on

  std::pair<bool, const T &> at(size_t n) const {
    if ((n < 1u) || (n > std::vector<T>::size())) {
      return make_result(false, *(std::vector<T>::end()), n);
    }
    return make_result(true, std::vector<T>::at(n - 1));
  }
  std::pair<bool, T &> operator[](size_t n) {
    if ((n < 1u) || (n > std::vector<T>::size())) {
      return make_result(false, *(std::vector<T>::end()), n);
    }
    return make_result(true, std::vector<T>::operator[](n - 1));
  }
  std::pair<bool, const T &> operator[](size_t n) const {
    if ((n < 1u) || (n > std::vector<T>::size())) {
      return make_result(false, *(std::vector<T>::end()), n);
    }
    return make_result(true, std::vector<T>::operator[](n - 1));
  }
};

} // namespace one_index_vector

#include "one_index_vector_impl.hh"

#endif
