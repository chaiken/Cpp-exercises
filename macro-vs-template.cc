#include "macro-vs-template.hh"
#include "complex.hh"

#include <cmath>
#include <iostream>

using namespace complex;

#define CUBE(X) ((X) * (X) * (X))

namespace macro_testing {
template <int> int cube(int x) { return (x * x * x); }
constexpr double cubeme(double x) { return x * x * x; }
// macro-vs-template.cc: In function ‘constexpr complex::Complex
// macro_testing::cubeme(const complex::Complex&)’: macro-vs-template.cc:14:19:
// error: invalid return type ‘complex::Complex’ of ‘constexpr’ function
// ‘constexpr complex::Complex macro_testing::cubeme(const complex::Complex&)’
// constexpr Complex cubeme(const Complex &x) { return x * x * x; }
// In file included from macro-vs-template.cc:2:
// complex.hh:20:7: note: ‘complex::Complex’ is not literal because:
// class Complex {
// complex.hh:20:7: note:   ‘complex::Complex’ is not an aggregate,
// does not have a trivial default constructor, and has no ‘constexpr’
// constructor that is not a copy or move constructor

// https://en.cppreference.com/w/cpp/named_req/LiteralType
// Literal types are the types of constexpr variables and they can be
// constructed, manipulated, and returned from constexpr functions.
constexpr Complex cubeme(const Complex &x) { return x * x * x; }

// macro-vs-template.cc:14:10: error: ‘double’ is not a valid type for a
// template non-type parameter
// https://stackoverflow.com/questions/2183087/why-cant-i-use-float-value-as-a-template-parameter
// template<double> double cube(double x) {
//  return (x*x*x);
//}

void pretty_print_result(const std::string &eval_type, const Complex &result) {
  std::cout << eval_type;
  complex::operator<<(std::cout, result);
  std::cout << std::endl;
}

} // namespace macro_testing

using namespace macro_testing;

int main(void) {
  std::cout << "macro: " << CUBE(sqrt(27.0)) << std::endl;
  std::cout << "template: " << cube(sqrt(27)) << std::endl;
  std::cout << "constexpr: " << cubeme(sqrt(27.0)) << std::endl << std::endl;

  Complex cn(27.0, 13.5);
  std::pair<Complex, bool> res = complex::sqrt(cn);
  if (!res.second) {
    std::cerr << "Bad sqrt" << std::endl;
    exit(-EINVAL);
  }
  pretty_print_result("macro: ", CUBE(res.first));
  pretty_print_result("template: ", cube(res.first));
  pretty_print_result("constexpr: ", cubeme(res.first));
  exit(0);
}
