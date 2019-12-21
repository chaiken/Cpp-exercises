#include "template_integrate.h"

#include <cmath>

#include "gtest/gtest.h"

using namespace std;

namespace integration {
namespace testing {

TEST(NumericalIntegrationTest, CubeTest) {
  int coarse = 10, fine = 1e5;
  vector<double> coarse_interval(coarse);
  // http://www.cplusplus.com/reference/algorithm/generate/
  // "Generator function that is called **with no arguments** and returns some
  // value of a type convertible to those pointed by the iterators. This can
  // either be a function pointer or a function object."
  CubeIt<double> coarse_fn(*(coarse_interval.begin()), 1.0 / coarse);
  generate(coarse_interval.begin(), coarse_interval.end(), coarse_fn);
  double coarse_result = do_integrate(coarse_interval);
  double coarse_error = abs(coarse_result - (1.0 / 4));
  cout << "Coarse-interval error is " << coarse_error << endl;

  vector<double> fine_interval(fine);
  CubeIt<double> fine_fn(*(fine_interval.begin()), 1.0 / fine);
  generate(fine_interval.begin(), fine_interval.end(), fine_fn);
  double fine_result = do_integrate(fine_interval);
  double fine_error = abs(fine_result - (1.0 / 4));
  cout << "Fine-interval error is " << fine_error << endl;

  ASSERT_GT(coarse_error, fine_error);
}

} // namespace testing
} // namespace integration
