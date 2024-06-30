#include "fibonacci.hh"

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace fibonacci;

int main(int argc, char *argv[]) {
  if (2 != argc) {
    std::cerr << "Please provide a positive seed number." << std::endl;
    exit(EXIT_SUCCESS);
  }
  for (const char c : std::string(argv[1])) {
    if (!isdigit(c)) {
      std::cerr << "Illegal seed " << argv[1] << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  uint64_t seed = strtoul(argv[1], nullptr, 10);
  if (errno) {
    std::cerr << "Illegal seed " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }
  const std::pair<int, rlim_t> stack_max = get_max_stack_depth();
  if (stack_max.first) {
    std::cerr << "getrlimit() failed with  " << strerror(stack_max.first)
              << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << calc_sum(seed, stack_max.second) << std::endl;
  exit(EXIT_SUCCESS);
}
