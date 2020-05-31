// [alison@bonnet Pohl (master)]$ ./dbl_vector_time
// DoubleVector sum done in 0 s.
// DoubleVector sum done in monotonic -965.581 ms.     <<<< !
// Simple double vector sum done in 0 s.
// Simple sum done in monotonic 20.0368 ms.
// [alison@bonnet Pohl (master)]$ ./dbl_vector_time
// DoubleVector sum done in 0 s.
// DoubleVector sum done in monotonic 34.3522 ms.
// Simple double vector sum done in 0 s.
// Simple sum done in monotonic 20.0302 ms.

#include "dbl_vector.h"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace dbl_vect;

constexpr int elnum = 1000000;

int main(void) {

  time_t now, done;
  struct timespec start, finish;

  double input[elnum], sum1 = 0, sum2 = 0, timediff;
  int i = 0;
  while (i < elnum) {
    input[i] = 10.0 * i;
    i++;
  }
  DoubleVector fancy(static_cast<const double *>(input), elnum);
  i = 0;
  time(&now);
  clock_gettime(CLOCK_MONOTONIC, &start);
  while (i <= fancy.ub()) {
    sum1 += fancy.Element(i);
    i++;
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time(&done);
  timediff = finish.tv_nsec - start.tv_nsec;
  cout << "DoubleVector sum done in " << difftime(done, now) << " s." << endl;
  cout << "DoubleVector sum done in monotonic " << (timediff / 1e6) << " ms."
       << endl;

  i = 0;
  now = time(NULL);
  clock_gettime(CLOCK_MONOTONIC, &start);
  while (i <= fancy.ub()) {
    sum2 += input[i];
    i++;
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time(&done);
  timediff = finish.tv_nsec - start.tv_nsec;
  cout << "Simple double vector sum done in " << difftime(done, now) << " s."
       << endl;
  cout << "Simple sum done in monotonic " << (timediff / 1e6) << " ms." << endl;

  assert(sum1 == sum2);

  exit(EXIT_SUCCESS);
}
