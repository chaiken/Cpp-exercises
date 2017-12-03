#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

constexpr double PI = 3.14159;
constexpr int NUMPTS = 1000;
constexpr int COLUMNS = 80;

double domain_arg(double x) {
  int mod = (x / (2 * PI));
  return (x - (mod * (2 * PI)));
}

double fcn1(double x) { return cos(x); }
double fcn2(double x) { return (cos(x) * cos(x) * sin(x)); }

// Only trailing arguments can have defaults.
double findmin(double fcn(double), double &xmin, double x0 = -20.0,
               double x1 = 0.0, double incr = 0.01) {
  int count = (x1 - x0) / incr;
  // Make sure function will terminate.
  assert(count > 0);

  double min = INT_MAX, exx = x0;
  int i = 0;
  while (i++ < count) {
    double angle = domain_arg(exx);
    if (fcn(angle) < min) {
      min = fcn(angle);
      xmin = angle;
    }
    exx += incr;
  }
  return min;
}

// Only trailing arguments can have defaults.
double findmax(double fcn(double), double &xmax, double x0 = -2.0 * PI,
               double x1 = 2.0 * PI, double incr = 0.01 * PI) {
  int count = (x1 - x0) / incr;
  // Make sure function will terminate.
  assert(count > 0);

  double max = INT_MIN, exx = x0;
  int i = 0;
  while (i++ < count) {
    double angle = domain_arg(exx);
    if (fcn(angle) > max) {
      max = fcn(angle);
      xmax = angle;
    }
    exx += incr;
  }
  return max;
}

void plotval(const double val, const double min, const double max) {

  char output[COLUMNS];
  memset(output, '\0', sizeof(output));

  // These assertions fail for == case.
  // assert(val >= min);
  // assert(val <= max);
  // https://stackoverflow.com/questions/18971533/c-comparison-of-two-double-values-not-working-properly
  double resolution = (max - min) / COLUMNS;
  int numchars = round((val - min) / resolution);
  if ((numchars < 0) || (numchars > COLUMNS)) {
    cout << "Illegal numchars " << numchars << " for val " << val << endl;
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < numchars; i++) {
    output[i] = '*';
  }
  cout << output << endl;
}

void plot(double fcn(double), double x0 = -2 * PI, double x1 = 2 * PI,
          double incr = 0.01 * PI) {
  int count = (x1 - x0) / incr;
  // Make sure function will terminate.
  assert(count > 0);

  // Passing &minx as a function parameter leads to compiler error:
  // invalid initialization of non-const reference of type ‘double&’ from an
  // rvalue of type ‘double*’
  // Trying to cast a const pointer (double * const minx) to a ref also fails.
  double minx = -100.0;
  double &xref = minx;

  // Doesn't compile:
  // cout << "min is " << findmin(fcn1, -20.0, 20.0, 0.01, &minx) << " at " <<
  // xref
  // Note that in contrast to C, the value of the reference is changed
  // by the function call.
  double min = findmin(fcn1, xref);
  cout << endl << "min is " << min << " at " << xref << endl;
  double max = findmax(fcn1, xref);
  cout << "max is " << max << " at " << xref << endl << endl;
  assert(max > min);

  int i = 0;
  double exx = x0;
  vector<double> vec;
  while (i++ < count) {
    double arg = domain_arg(exx);
    vec.push_back(fcn(arg));
    exx += incr;
  }
  for (vector<double>::iterator it = vec.begin(); it != vec.end(); it++) {
    plotval(*it, min, max);
  }
}

int main(void) {

  plot(fcn1);
  plot(fcn2, -40.0, -45.0, -0.005);

  exit(EXIT_SUCCESS);
}
