#include <utility>

// Effective Modern C++, p. 16
// "Although functions cannot declare parameters that are truly arrays, they can
// declare parameters that are references to arrays."
// Return size of an array as a compile-time constant.  (The array parameter has
// no name because we only care about the number of elements it contains.)
template <typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept {
  return N;
}
