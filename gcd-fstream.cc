#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

unsigned long get_op(ifstream &ifile) {
  char instring[32];
  unsigned long op = 0;

  // use of get() instead of getline() puts terminating
  // into returned string and causes strtoul() to fail.
  // With get(), if the terminating character is ' ', then the second
  // string read will have only ' '.
  ifile.getline(instring, 32, ' ');
  if (!strlen(instring)) {
    exit(EXIT_SUCCESS);
  }
  op = strtoul(instring, nullptr, 10);
  // atoi() is considered deprecated, will be replaced
  // by strtoi().
  // op = atoi(instring);
  if ((errno == ERANGE) || (errno == EINVAL) || (op == 0)) {
    cerr << "Operand out of range: " << op << endl;
    exit(EXIT_FAILURE);
  }
  return op;
}

unsigned long gcd(unsigned long a, unsigned long b) {
  if ((a * b) == 0) {
    cerr << "Can't process zero." << endl;
    exit(EXIT_FAILURE);
  }
  unsigned long divisor = (a < b) ? a : b;
  unsigned long dividend = (a < b) ? b : a;
  unsigned long remainder = divisor;

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
  unsigned long num1 = 0, num2 = 0;
  while ((num1 = get_op(ifile)) && (num2 = get_op(ifile))) {
    unsigned long ans = gcd(num1, num2);
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
///////////////////////////////////////////////////////////////////////////////
//									     //
//	[alison@bonnet Pohl (master)]$ ./gcd-fstream foo bar		     //
//	[alison@bonnet Pohl (master)]$ echo $?				     //
//	0								     //
//	[alison@bonnet Pohl (master)]$ cat foo				     //
//	100 35 11 99 256 1000 3 1449 31a 3b				     //
//	[alison@bonnet Pohl (master)]$ cat bar				     //
//	gcd of 100, 35 is 5						     //
//	file position is 20						     //
//	gcd of 11, 99 is 11						     //
//	file position is 60						     //
//	gcd of 256, 1000 is 8						     //
//	file position is 102						     //
//	gcd of 3, 1449 is 3						     //
//	file position is 143						     //
//	gcd of 31, 3 is 1						     //
//	file position is 182						     //
///////////////////////////////////////////////////////////////////////////////
