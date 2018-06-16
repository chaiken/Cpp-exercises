#include <cerrno>
#include <cmath>
#include <complex>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

bool get_coeffs(ifstream &ifile, double arr[]) {
  char instring[32];
  int ctr = 0;

  ifile.getline(instring, 32, '\n');
  string line(instring), fragment;
  if (line.length() == 0 || line[0] == '#')
    return false;
  // __istream_type & getline (char_type *__s, streamsize __n, char_type
  // __delim)
  // String extraction.
  istringstream iss(line);
  while ((getline(iss, fragment, ' ')) && (ctr < 3)) {
    if (fragment.length() == 0)
      continue;
    arr[ctr++] = atof(fragment.c_str());
  }
  if (ctr != 3)
    return false;
  return true;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
    exit(EXIT_FAILURE);
  }
  ifstream ifile(argv[1]);
  if (!ifile) {
    cerr << "Cannot open input file: " << argv[1] << endl;
    exit(EXIT_FAILURE);
  }
  ofstream ofile(argv[2]);
  if (!ofile) {
    cerr << "Cannot open output file: " << argv[2] << endl;
    exit(EXIT_FAILURE);
  }

  double coeffs[3], realroot1, realroot2, sqrt_disc;
  while (get_coeffs(ifile, coeffs)) {
    cout << "coeffs: " << coeffs[0] << '\t' << coeffs[1] << '\t' << coeffs[2]
         << endl;
    ofile << "coeffs: " << coeffs[0] << '\t' << coeffs[1] << '\t' << coeffs[2]
          << endl;
    double discriminant = (coeffs[1] * coeffs[1]) - (4 * coeffs[0] * coeffs[2]),
           denom = 1 / (2 * coeffs[0]);
    if ((discriminant > 0) && (sqrt_disc = sqrt(discriminant))) {
      realroot1 = (sqrt_disc - coeffs[1]) * denom;
      realroot2 = (-1 * sqrt_disc - coeffs[1]) * denom;
    } else if (discriminant == 0) {
      realroot1 = -1 * coeffs[1] / (2 * coeffs[0]);
      realroot2 = realroot1;
    } else {
      sqrt_disc = sqrt(-1 * discriminant);
      complex<double> comproot1(-coeffs[1] * denom, -sqrt_disc * denom);
      complex<double> comproot2(-coeffs[1] * denom, sqrt_disc * denom);
      ofile << "roots: " << comproot1 << '\t' << comproot2 << endl << endl;
      cout << "roots: " << '\t' << comproot1 << '\t' << comproot2 << endl
           << endl;
    }
    if (discriminant >= 0) {
      ofile << "roots: " << realroot1 << '\t' << realroot2 << endl << endl;
      cout << "roots: " << '\t' << realroot1 << '\t' << realroot2 << endl
           << endl;
    }
  }
  cout << endl;
  ofile << endl;
  exit(EXIT_SUCCESS);
}
