#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <sstream>

using namespace std;

bool get_coeffs(ifstream& ifile, double arr[]) {
  char instring[32];
  int ctr = 0;

  ifile.getline(instring, 32, '\n');
  string line(instring), fragment;
  if (line.length() == 0 || line[0] == '#')
    return false;
  istringstream iss(line);
  while ((getline(iss, fragment, ' ')) && (ctr < 3)) {
    if (fragment.length() == 0) continue;
    arr[ctr++] = atof(fragment.c_str());
  }
  if (ctr != 3) return false;
  return true;
}

int main(int argc, char **argv)
{
  if (argc != 3) {
    cerr << "Usage: quadratic <input file> <output file>" << endl;
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

  double coeffs[3], sqrt_disc, root1, root2;
  while (get_coeffs(ifile, coeffs)) {
    cout << "coeffs: " << coeffs[0] << '\t' << coeffs[1] << '\t' << coeffs[2] << endl;
    ofile << "coeffs: " << coeffs[0] << '\t' << coeffs[1] << '\t' << coeffs[2] << endl;
    root1 = root2 = 0;
    double discriminant = (coeffs[1] * coeffs[1]) - (4*coeffs[0]*coeffs[2]);
    if ((discriminant > 0) && (sqrt_disc = sqrt(discriminant))) {
      root1 = (sqrt_disc - coeffs[1])/(2* coeffs[0]);
      root2 = (-1*sqrt_disc - coeffs[1])/(2* coeffs[0]);
    }
    else if (discriminant == 0) {
      root1 = -1*coeffs[1]/(2*coeffs[0]);
      root2 = root1;
    } else {
      cout << "complex roots" << endl << endl;
      ofile << "complex roots" << endl << endl;
      continue;
    }
    if ((root1 != 0) && (root2 != 0)) {
      ofile << "roots: " << root1 << '\t'<< root2 << endl << endl;
      cout << "roots: " << '\t' << root1 << '\t'<< root2 << endl << endl;
    }
  }
  cout << endl;
  ofile << endl;
  exit(EXIT_SUCCESS);
}
