// There's no way to directly dynamically allocate an array, so we must
// dynamically allocate a vector and then access it as an array.

#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

constexpr int SIZE = 1024;

int main(int argc, char *argv[]) {
  unsigned ub = 0, lb = 0;
  cout << "Lower bound of array: ";
  cin >> lb;
  cout << "Upper bound of array: ";
  cin >> ub;
  assert(ub - lb > 0u);
  vector<int> vec;
  for (int i = 0; i < SIZE; i++) {
    vec.push_back(i);
  }

  // https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array-in-c
  int *arr = &vec[lb];
  // Taking elements lb through ub.
  for (int i = 0; i < (ub - lb); i++) {
    cout << "i: " << i << " arr[i]: " << arr[i] << endl;
  }

  exit(EXIT_SUCCESS);
}
