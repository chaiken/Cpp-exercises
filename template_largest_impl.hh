#include <cassert>
#include <climits>

#include <iostream>

namespace largest {

// Approach copied from _A Tour of C++, 2nd Edition_, 6.3.2, p. 86.
template <typename T> int FindSecondLargest(const T &acont) {
  int largest = INT_MIN, secondlargest = INT_MIN;
  for (const auto &x : acont) {
    // The value x is greater than both.
    if (x > largest) {
      if (largest > secondlargest) {
        secondlargest = largest;
      }
      largest = x;
    } else {
      // The value x is between the two values.
      // Never executed for steadily increasing sequences.
      if ((x > secondlargest) && (x < largest)) {
        secondlargest = x;
      }
    }
  }
  assert(secondlargest <= largest);
  return secondlargest;
}

} // namespace largest
