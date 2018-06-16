/* Compute a table of cubes, passing in ints as doubles. */

#include <cstdlib>
#include <iostream>

using namespace std;

constexpr int MAX = 20;

double cube(double input) { return (input * input * input); }

int main(int argc, char **argv) {
  for (int i = 0; i < MAX; i++) {
    printf("i: %d  cube: %d\n", i, cube(i));
    cout << "i: " << i << " cube: " << cube(i) << endl << endl;
  }
  return (EXIT_SUCCESS);
}
