#include <iostream>
#include <cstdlib>
#include <climits>

using namespace std;

int main(void) {
  int max=INT_MIN, min=INT_MAX, ctr;
  long ave=0L;
  string instring;

  cout << "How many? " << endl;
  cin >> instring;
  ctr = atoi(instring.c_str());

  for (int i=0; i < ctr; i++) {
    int num = rand();
    cout << hex << "0x" << num << '\t';
    max = (num > max) ? num : max;
    min = (num < min) ? num : min;
    ave += num;
  }
  cout << endl;
  if (ctr > 0) {
    cout << hex << "max: 0x" << max <<  " min: 0x" << min << " ave: 0x" << (ave/ctr) << endl;
  }
  exit(EXIT_SUCCESS);
}
