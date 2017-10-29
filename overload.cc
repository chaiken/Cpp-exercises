#include <complex>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

complex<double> avg_arr(const vector<complex<double>> a) {
  complex<double> sum(0.0, 0.0), elem;
  // Get a non-const vector we can iterate.
  vector<complex<double>> b = a;
  for (vector<complex<double>>::iterator it = b.begin(); it != b.end(); it++) {
    sum += *it;
  }
  // Doesn't work : return (sum/b.size()).
  // complex /= is defined, not /.
  sum /= b.size();
  return (sum);
}

double avg_arr(const vector<double> a) {
  double sum = 0.0;
  // Get a non-const vector we can iterate.
  vector<double> b = a;
  for (vector<double>::iterator it = b.begin(); it != b.end(); it++) {
    sum += *it;
  }
  return (sum / a.size());
}

int main(void) {
  const vector<double> doublearr = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
  cout << avg_arr(doublearr) << endl;
  vector<complex<double>> complexarr;
  for (int i = 0; i < doublearr.size(); i++) {
    complexarr.push_back(complex<double>(doublearr[i], doublearr[i]));
  }
  cout << avg_arr(complexarr) << endl;

  exit(EXIT_SUCCESS);
}
