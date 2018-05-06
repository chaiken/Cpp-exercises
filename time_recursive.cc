#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

void calcr_sequence(long long operand, long long *result) {
  //  cout << operand << "\t" << *result << endl;
  if (operand > 1)
    (*result) *= operand;
  else
    return;
  calcr_sequence(--operand, result);
}

void calc_sequence(long long operand, long long *result) {
  while (operand >= 1) {
    //    cout << operand << "\t" << *result << endl;
    (*result) *= operand;
    operand--;
  }
}

int main(void) {
  long long ans = 1;
  high_resolution_clock::time_point start = high_resolution_clock::now();
  calcr_sequence(20, &ans);
  high_resolution_clock::time_point stop = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(stop - start).count();
  cout << "10 recursive iterations: " << ans << " took " << duration
       << " nanooseconds." << endl;

  ans = 1;
  start = high_resolution_clock::now();
  calc_sequence(20, &ans);
  stop = high_resolution_clock::now();
  duration = duration_cast<nanoseconds>(stop - start).count();
  cout << "10 non-recursive iterations: " << ans << " took " << duration
       << " nanooseconds." << endl;
  exit(0);
}

///////////////////////////////////////////////////////////////////////////////
//	[alison@bonnet Pohl (master)]$ ./time_recursive 		     //
//	10 recursive iterations: 2432902008176640000 took 615 nanooseconds.  //
//	10 non-recursive iterations: 2432902008176640000 took 508 nanooseconds.//
///////////////////////////////////////////////////////////////////////////////
