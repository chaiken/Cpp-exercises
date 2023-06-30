#include <cstdint>
#include <sys/resource.h>
#include <utility>

namespace fibonacci {

constexpr uint16_t SUCCESS = 0U;
constexpr uint16_t FAILURE = 1U;

// Employ the getrlimit() syscall to find maximum configured stack utilization.
std::pair<int, rlim_t> get_max_stack_depth();

// Provide the Fibonacci sum for the given seed value as long as stack space
// does not exceed stack_max.
uint64_t calc_sum(uint64_t seed, const rlim_t stack_max);

} // namespace fibonacci
