#include "template_integrate.hh"

#include <cassert>
#include <cmath>

#include "gtest/gtest.h"

using namespace std;

namespace integration {
namespace testing {

double Reciprocal(double param) {
  assert(param != 0.0);
  //  cout << "Reciprocal called with " << param << endl;
  return 1.0 / param;
}

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

  vector<double> negative_interval(fine);
  CubeIt<double> negative_fn(*(negative_interval.begin()), -1.0/fine);
  generate(negative_interval.begin(), negative_interval.end(), negative_fn);
  double negative_result = do_integrate(negative_interval);
  // x^3 has odd parity.
  ASSERT_EQ(fine_result, -1.0*negative_result);
}

TEST(NumericalIntegrationTest, PassInFunctionTest) {
  int coarse = 10, fine = 1e5;

  // https://stackoverflow.com/questions/877523/error-request-for-member-in-which-is-of-non-class-type
  // template_integrate_lib_test.cc:45:6: error: request for member ‘SetFn’ in
  // ‘integration::testing::fw’, which is of non-class type
  // ‘integration::FunctionWrapper<double>()’ FunctionWrapper<double> fw() You
  // get the error because compiler thinks of Foo foo2() as of function
  // declaration with name 'foo2' and the return type 'Foo'.
  FunctionWrapper<double> fw;
  fw.SetFn(&Reciprocal);

  vector<double> coarse_interval(coarse);
  call_fill(coarse_interval.begin(), coarse_interval.end(), fw);
  double coarse_result = do_integrate(coarse_interval);
  double coarse_error = abs(coarse_result - log(1.0));
  cout << "Coarse-interval error is " << coarse_error << endl;

  vector<double> fine_interval(fine);
  call_fill(fine_interval.begin(), fine_interval.end(), fw);
  double fine_result = do_integrate(fine_interval);
  double fine_error = abs(fine_result - log(1.0));
  cout << "Fine-interval error is " << fine_error << endl;
  ASSERT_GT(coarse_error, fine_error);
}

TEST(NumericalIntegrationTest, LambdaTest) {
  int coarse = 10, fine = 1e5;

  // https://stackoverflow.com/questions/877523/error-request-for-member-in-which-is-of-non-class-type
  // template_integrate_lib_test.cc:45:6: error: request for member ‘SetFn’ in
  // ‘integration::testing::fw’, which is of non-class type
  // ‘integration::FunctionWrapper<double>()’ FunctionWrapper<double> fw() You
  // get the error because compiler thinks of Foo foo2() as of function
  // declaration with name 'foo2' and the return type 'Foo'.
  // FunctionWrapper<double> fw;
  // template_integrate_lib_test.cc:78:40: error: cannot convert
  // ‘integration::testing::NumericalIntegrationTest_LambdaTest_Test::TestBody()::<lambda(double)>’
  // to ‘double (*)(double)’ 78 |   fw.SetFn([&](double x){return 1.0/x;});
  // Cannot take address of the lambda as it is an rvalue.
  // fw.SetFn([&](double x){return 1.0/x;});
  // template_integrate_lib_test.cc:82:16: error: cannot convert
  // ‘std::_Bind_helper<false, double (*)(double), const
  // std::_Placeholder<1>&>::type’ {aka ‘std::_Bind<double
  // (*(std::_Placeholder<1>))(double)>’} to ‘double (*)(double)’ 82 |
  // fw.SetFn(bind(&Reciprocal, placeholders::_1));
  //  fw.SetFn(bind(Reciprocal, placeholders::_1));
  //
  // Lambdas are R-values, and as such cannot be converted to function pointers
  // with &. template_integrate_lib_test.cc:85:39: error: cannot convert
  // ‘integration::testing::Numerical
  // IntegrationTest_LambdaTest_Test::TestBody()::<lambda(double)>’ to ‘double
  // (*)(double)’ 85 |  fw.SetFn([&](double x){return 1.0/x;});
  // fw.SetFn([&](double x){return 1.0/x;});

  vector<double> coarse_interval(coarse);
  FillWithLambda(coarse_interval.begin(), coarse_interval.end(), 0.0,
                 M_PI / coarse, ([](double x) { return sin(x); }));
  double coarse_result = do_integrate(coarse_interval);
  // Integral of sine is -cosine.  -(cos(pi) - cos(0)) = -(-1 - 1) = 2.
  // Multiply by pi because numerical integration depends on the size of the
  // interval, which here runs from 0 to pi, not zero to 1.
  double coarse_error = abs(M_PI * coarse_result - 2.0);
  cout << "Coarse-interval error is " << coarse_error << endl;

  vector<double> fine_interval(fine);
  FillWithLambda(fine_interval.begin(), fine_interval.end(), 0.0, M_PI / fine,
                 ([](double x) { return sin(x); }));
  double fine_result = do_integrate(fine_interval);
  double fine_error = abs(M_PI * fine_result - 2.0);
  cout << "Fine-interval error is " << fine_error << endl;
  ASSERT_GT(coarse_error, fine_error);
}

TEST(NumericalIntegrationTest, BindTest) {
  int coarse = 10, fine = 1e5;
  vector<double> coarse_interval(coarse);
  FillWithLambda(coarse_interval.begin(), coarse_interval.end(), 1.0,
                 1.0 / coarse, bind(&Reciprocal, placeholders::_1));
  double coarse_result = do_integrate(coarse_interval);
  // Integral of sine is -cosine.  -(cos(pi) - cos(0)) = -(-1 - 1) = 2.
  // Multiply by pi because numerical integration depends on the size of the
  // interval, which here runs from 0 to pi, not zero to 1.
  double coarse_error = abs(coarse_result - (log(2.0) - log(1.0)));
  cout << "Coarse-interval error is " << coarse_error << endl;

  vector<double> fine_interval(fine);
  FillWithLambda(fine_interval.begin(), fine_interval.end(), 1.0, 1.0 / fine,
                 bind(&Reciprocal, placeholders::_1));
  double fine_result = do_integrate(fine_interval);
  double fine_error = abs(fine_result - (log(2.0) - log(1.0)));
  cout << "Fine-interval error is " << fine_error << endl;
  ASSERT_GT(coarse_error, fine_error);
}

} // namespace testing
} // namespace integration
