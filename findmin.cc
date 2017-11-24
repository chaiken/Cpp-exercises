#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

constexpr double PI = 3.14159;

double domain_arg(double x) {
  int mod = (x / (2 * PI));
  return (x - (mod * (2 * PI)));
}

double fcn1(double x) { return cos(x); }

// Only trailing arguments can have defaults.
double findmin(double fcn(double), double &xmin, double x0 = -20.0,
               double x1 = 0.0, double incr = 0.01) {
  // make sure loop will terminate
  assert((x1 - x0) / incr > 0.0);

  double min = INT_MAX, exx;
  for (exx = x0; exx < x1; exx += incr) {
    double angle = domain_arg(exx);
    if (fcn(angle) < min) {
      min = fcn(angle);
      xmin = angle;
    }
  }
  return min;
}

int main(void) {
  // Passing &minx as a function parameter leads to compiler error:
  // invalid initialization of non-const reference of type ‘double&’ from an
  // rvalue of type ‘double*’
  // Trying to cast a const pointer (double * const minx) to a ref also fails.
  double minx = -100;
  double &xref = minx;
  // Doesn't compile:
  // cout << "min is " << findmin(fcn1, -20.0, 20.0, 0.01, &minx) << " at " <<
  // xref
  // Note that in contrast to C, the value of the reference is changed
  // by the function call.
  cout << "min is " << findmin(fcn1, xref, -20.0, 5.0, 0.1) << " at " << xref
       << endl;
  cout << "min is " << findmin(fcn1, xref, 40.0, 45.0, 0.01) << " at " << xref
       << endl;
  cout << "min is " << findmin(fcn1, xref) << " at " << xref << endl;
  exit(EXIT_SUCCESS);
}
