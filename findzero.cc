#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

double fcn1(double val) {
  return (-3.0 * pow(val, 3.0) + 4 * pow(val, 2.0) - 2 * val + 6);
}

double plot(vector<double> y, double fcn(double), double x0, double x1,
            double incr) {
  // Make sure the loop is going to terminate.
  assert(incr != 0.0);
  assert(((x1 - x0) / incr) > 0.0);
}

int main(void) { exit(EXIT_SUCCESS); }
