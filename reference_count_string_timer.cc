#include "reference_count_string.h"

#include <cstdlib>
#include <ctime>

#include <iomanip>
#include <iostream>

using namespace std;
using namespace reference_counted_string;

constexpr int32_t REPS = 1e7;

int main() {
  time_t now, done;
  struct timespec start, finish;
  double timediff;
  int count = 0;

  CountedString tester(
      "Now is the time for all good men to come to the aid of their party");

  time(&now);
  {
    const int success = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    // assert_perror(errno);
    assert(success != -1);
  }

  for (int i = 0; i < REPS; i++) {
    count += (tester(i, i + 3) == "the");
  }

  {
    const int success = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &finish);
    // assert_perror(errno);
    assert(success != -1);
  }
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &finish);
  time(&done);
  timediff =
      (finish.tv_sec - start.tv_sec) + 1e-9 * (finish.tv_nsec - start.tv_nsec);
  cout << "Comparison done in " << difftime(done, now) << " s." << endl;
  cout << setprecision(5) << "Comparison done in " << timediff << " s." << endl;
}

/*
Explicitly calling the CountedString Ctor in substring operator().
$ ./reference_count_string_timer
Comparison done in 13 s.
Comparison done in 12.44 s.
$ ./reference_count_string_timer
Comparison done in 13 s.
Comparison done in 12.724 s.
 */
