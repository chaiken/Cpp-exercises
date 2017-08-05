#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int get_op(ifstream &ifile) {
  char instring[32];
  int op = 0;

  // use of get() instead of getline() puts terminating
  // char into returned string and causes strtoul() to fail.
  // With get(), if the terminating character is ' ', then the second
  // string read will have only ' '.
  ifile.getline(instring, 32, ' ');
  if (!strlen(instring)) {
    exit(EXIT_SUCCESS);
  }
  //  op = strtoul(instring, nullptr, 10);
  op = atoi(instring);
  if ((errno == ERANGE) || (op == 0)) {
    cerr << "Operand out of range: " << op << endl;
    exit(EXIT_FAILURE);
  }
  return op;
}

int gcd(int a, int b) {
  if ((a * b) == 0) {
    cerr << "Can't process zero." << endl;
    exit(EXIT_FAILURE);
  }
  int divisor = (a < b) ? a : b;
  int dividend = (a < b) ? b : a;
  int remainder = divisor;

  while (remainder != 0) {
    remainder = dividend % divisor;
    dividend = divisor;
    divisor = remainder;
  }
  return dividend;
}

int main(int argc, char **argv) {
  // std::ofstream ostrm("Test.b", std::ios::binary);
  // double d = 3.14;
  // ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // binary output
  // ostrm << 123 << "abc" << '\n';                      // text output
  // Without the following line, no output.
  // Can't have two ofstreams open at once?
  // ostrm.close();

  if (argc != 3) {
    cerr << "Please specify filenames for input and output" << endl;
    exit(EXIT_FAILURE);
  }
  ifstream ifile(argv[1]);
  if (!ifile) {
    cerr << "Couldn't open requested file:" << argv[1] << endl;
    exit(EXIT_FAILURE);
  }
  ofstream ofile(argv[2]);
  if (!ofile) {
    cerr << "Couldn't open requested file:" << argv[2] << endl;
    exit(EXIT_FAILURE);
  }
  int num1 = 0, num2 = 0;
  while ((num1 = get_op(ifile)) && (num2 = get_op(ifile))) {
    int ans = gcd(num1, num2);
    ofile << "gcd of ";
    ofile << num1;
    ofile << ", ";
    ofile << num2;
    ofile << " is ";
    // no output is produced with following line:
    // ofile << ans << '\n';
    // Without an endl, the output is never flushed.
    // ofile.flush() does not rectify the problem.
    ofile << ans << endl;
    ofile << "file position is " << ofile.tellp() << endl;
  }
  ifile.close();
  ofile.close();
}
