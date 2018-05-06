#include <iostream>
#include <cstdlib>

using namespace std;

#define ITERS 10000

int main(void) {
  int consecutive = 0, current_run = 0;
  double heads = 0.0;
  for (int i=ITERS; i > 0; i--) {
    //    if (rand() > (RAND_MAX/2)) {
    if (drand48() > 0.5) {
      heads += 1.0;
      current_run++;
      consecutive = (current_run > consecutive) ? current_run : consecutive;
    }
  }
  cout << "Fractions HEADs is " << (heads/ITERS) << endl;
  cout << "Consecutive HEADs is " << consecutive << endl;
  exit(EXIT_SUCCESS);
}
