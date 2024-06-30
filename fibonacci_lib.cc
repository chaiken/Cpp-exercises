#include "fibonacci.hh"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace fibonacci {

/* from include/linux/kernel.hh: */
#define _RET_IP_ (unsigned long)__builtin_return_address(0)
#define _THIS_IP_                                                              \
  ({                                                                           \
    __label__ __here;                                                          \
  __here:                                                                      \
    (unsigned long)&&__here;                                                   \
  })

std::pair<int, rlim_t> get_max_stack_depth() {
  struct rlimit rlim {};
  // getrlimit() returns -1 on error, but also sets errno.
  getrlimit(RLIMIT_STACK, &rlim);
  return std::pair{errno, rlim.rlim_cur};
}

uint64_t calc_sum(uint64_t seed, const rlim_t stack_max) {
  static uint64_t stack_depth = 0U;
  static uint64_t stack_size_estimate = 100U;
  // Presumably there is a better way to estimate stack size.
  if (1U == stack_depth) [[unlikely]] {
    stack_size_estimate = _RET_IP_ - _THIS_IP_;
    std::cout << "stack_size_estimate " << stack_size_estimate << std::endl;
    if (0U == stack_size_estimate) {
      throw std::runtime_error("Stack size estimate failed.");
    }
  }
  if (1U < seed) {
    // Stop before stack overflow.
    if (stack_depth >= (stack_max / stack_size_estimate)) {
      throw std::runtime_error("Stack overflow.");
    }
    stack_depth++;
    uint64_t sum = calc_sum(seed - 1U, stack_max);
    return sum + seed;
  } else [[unlikely]] {
    // Deepest level of recursion is reached.
    return seed;
  }
}

} // namespace fibonacci
